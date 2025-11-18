const std = @import("std");

const Allocator = std.mem.Allocator;
const Server = std.http.Server;
const Request = std.http.Request;
const Response = std.http.Response;
const fs = std.fs;
const ws = std.net.websocket;

const PORT = 8080;
const MAX_HTTP_CLIENTS = 100; // Max concurrent HTTP connections

// Global list of active WebSocket connections for hot-reloading
// Protected by a mutex since it's accessed by multiple threads.
var active_ws_connections_mutex = std.Thread.Mutex{};
var active_ws_connections = std.ArrayList(ws.WebSocket).init(std.heap.page_allocator);

pub fn runBuild(allocator: Allocator) !void {
    var server = Server.init(allocator, .{});
    defer server.deinit();

    const address = try std.net.Address.resolveIp("127.0.0.1", PORT);
    try server.listen(address);
    std.log.info("Server listening on http://127.0.0.1:{}", .{PORT});

    // Initialize the ArrayList for WebSocket connections. Deinit on exit.
    defer active_ws_connections.deinit();

    // Start the file watcher in a separate thread.
    // This thread will monitor files and trigger reloads via WebSockets.
    var watcher_thread = try std.Thread.spawn(null, startFileWatcher, .{});
    // Ensure the watcher thread is joined when runBuild exits.
    defer watcher_thread.join();

    // Create a thread pool for handling incoming HTTP requests.
    var http_thread_pool = std.Thread.Pool.init(.{
        .allocator = allocator,
        .max_threads = MAX_HTTP_CLIENTS,
    });
    defer http_thread_pool.deinit();

    // Main server loop: accept connections and dispatch to thread pool.
    while (true) {
        var conn = try server.accept();
        try http_thread_pool.spawn(handleConnection, .{ .connection = conn, .allocator = allocator });
    }
}

// Handles a single incoming TCP connection from the HTTP server.
fn handleConnection(connection: Server.Connection, allocator: Allocator) void {
    // Ensure the connection is closed when this function exits.
    defer connection.close();

    // Use an arena allocator for request-specific memory to easily clean up.
    var arena = std.heap.ArenaAllocator.init(allocator);
    defer arena.deinit();
    const a = arena.allocator();

    var request = Request.init(a);
    var response = connection.response(a);

    // Loop to handle multiple requests on a persistent HTTP/1.1 connection.
    while (connection.readRequest(&request) catch |err| {
        // Log errors reading request and exit this connection handler.
        std.log.warn("Failed to read request: {s}", .{@errorName(err)});
        return;
    }) {
        // Check if the request is for a WebSocket upgrade.
        if (std.mem.eql(u8, request.target, "/ws")) {
            // Attempt WebSocket handshake.
            if (ws.Server.handshake(&request, &response) catch |err| {
                std.log.err("WebSocket handshake failed: {s}", .{@errorName(err)});
                // If handshake fails, send an error response and close connection.
                response.status = .internal_server_error;
                response.transfer_encoding = .chunked;
                response.do() catch {};
                return;
            }) {
                // If handshake succeeds, send the HTTP 101 Switching Protocols response.
                response.do() catch |err| {
                    std.log.warn("Failed to send handshake response: {s}", .{@errorName(err)});
                    return;
                };

                // Initialize the WebSocket object using the underlying TCP stream.
                var websocket = ws.WebSocket.init(connection.stream);

                // Add the new WebSocket connection to our global list.
                active_ws_connections_mutex.lock();
                errdefer active_ws_connections_mutex.unlock();
                active_ws_connections.append(websocket) catch |err| {
                    std.log.err("Failed to add WebSocket connection: {s}", .{@errorName(err)});
                    websocket.deinit(); // Clean up the WebSocket if append fails.
                    return;
                };
                active_ws_connections_mutex.unlock();

                std.log.info("WebSocket client connected.", .{});

                // Keep the WebSocket alive. This thread will now exclusively handle
                // reading frames from this WebSocket. It will block here until
                // the client disconnects or an error occurs.
                var frame: ws.Frame = undefined;
                while (websocket.readFrame(&frame) catch |err| {
                    std.log.info("WebSocket client disconnected or error: {s}", .{@errorName(err)});
                    break; // Exit loop on error/disconnect.
                }) {
                    // If we receive a close frame, acknowledge and break.
                    if (frame.header.opcode == .close) {
                        break;
                    }
                    // Optionally handle other incoming WebSocket messages here if needed.
                    // For hot-reloading, we usually don't expect messages from the client.
                }

                // --- WebSocket Cleanup ---
                // Remove the disconnected WebSocket from our global list.
                active_ws_connections_mutex.lock();
                errdefer active_ws_connections_mutex.unlock();
                for (active_ws_connections.items) |*conn_ptr, i| {
                    // Compare the raw file descriptor handle to find the correct WebSocket.
                    if (conn_ptr.*.stream.handle.raw == websocket.stream.handle.raw) {
                        _ = active_ws_connections.swapRemove(i);
                        break;
                    }
                }
                active_ws_connections_mutex.unlock();
                websocket.deinit(); // Deinitialize the WebSocket resources.
                return; // This thread has finished handling the WebSocket connection.
            }
        }

        // If not a WebSocket upgrade, handle as a regular HTTP request.
        if (handleHttpRequest(a, &request, &response) catch |err| {
            std.log.err("Error handling HTTP request: {s}", .{@errorName(err)});
            response.status = .internal_server_error;
            response.transfer_encoding = .chunked;
            response.do() catch {};
            return;
        }) {
            response.do() catch |err| {
                std.log.warn("Failed to send response: {s}", .{@errorName(err)});
                return;
            };
        }
    }
}

// Handles serving static files and setting necessary headers for SharedArrayBuffer.
fn handleHttpRequest(allocator: Allocator, request: *Request, response: *Response) !bool {
    // Set critical headers for SharedArrayBuffer support.
    try response.headers.append("Cross-Origin-Opener-Policy", "same-origin");
    try response.headers.append("Cross-Origin-Embedder-Policy", "require-corp");

    const path = request.target;
    std.log.info("GET {s}", .{path});

    // Route requests to the appropriate file server.
    if (std.mem.eql(u8, path, "/") or std.mem.eql(u8, path, "/index.html")) {
        return serveFile(allocator, response, "buildtemplate/index.html", "text/html");
    } else if (std.mem.eql(u8, path, "/gama.js")) {
        return serveFile(allocator, response, "buildtemplate/gama.js", "application/javascript");
    } else if (std.mem.eql(u8, path, "/gama.worker.js")) {
        return serveFile(allocator, response, "buildtemplate/gama.worker.js", "application/javascript");
    } else if (std.mem.eql(u8, path, "/game.wasm")) {
        return serveFile(allocator, response, "game.wasm", "application/wasm");
    }

    // For any other path, return 404 Not Found.
    response.status = .not_found;
    return true;
}

// Utility function to read and serve a file.
fn serveFile(allocator: Allocator, response: *Response, file_path: []const u8, content_type: []const u8) !bool {
    var file = fs.cwd().openFile(file_path, .{}) catch |err| {
        std.log.warn("File not found: {s}", .{file_path});
        response.status = .not_found;
        return true;
    };
    defer file.close();

    const stat = try file.stat();
    const contents = try file.readToEndAlloc(allocator, stat.size);
    defer allocator.free(contents);
    
    response.status = .ok;
    try response.headers.append("Content-Type", content_type);
    response.content_length = contents.len;
    try response.writer().writeAll(contents);

    return true;
}

// --- Hot-Reloading File Watcher --- 

// List of files to monitor for changes.
const watched_files = &.{ "buildtemplate/index.html", "buildtemplate/gama.js", "buildtemplate/gama.worker.js", "game.wasm" };
const WATCH_INTERVAL_MS = 500; // Check for file changes every 500ms.

// Stores the last modification time (mtime_ns) for each watched file.
var last_mod_times: [watched_files.len]u64 = .{0} ** watched_files.len;

// Entry point for the file watcher thread.
fn startFileWatcher() void {
    // Initialize last_mod_times with current modification times.
    for (watched_files) |file_path, i| {
        if (fs.cwd().statFile(file_path)) |stat| {
            last_mod_times[i] = stat.mtime_ns;
        } else {
            std.log.warn("File watcher: Could not stat initial file {s}", .{file_path});
        }
    }

    // Main loop for the file watcher.
    while (true) {
        // Sleep for the defined interval before checking files again.
        std.time.sleep(WATCH_INTERVAL_MS * std.time.ns_per_ms);

        // Iterate through all watched files.
        for (watched_files) |file_path, i| {
            if (fs.cwd().statFile(file_path)) |stat| {
                // If a file's modification time has changed, notify clients.
                if (stat.mtime_ns > last_mod_times[i]) {
                    std.log.info("File changed: {s}. Notifying clients...", .{file_path});
                    last_mod_times[i] = stat.mtime_ns; // Update the stored time.
                    notifyReloadClients(); // Broadcast the reload message.
                    break; // Only need to notify once per change detection cycle.
                }
            } else {
                // Log a warning if a file cannot be stat'd (e.g., deleted or not yet created).
                std.log.warn("File watcher: Could not stat file {s}", .{file_path});
            }
        }
    }
}

// Sends a "reload" message to all currently connected WebSocket clients.
fn notifyReloadClients() void {
    // Lock the mutex before accessing the shared list of connections.
    active_ws_connections_mutex.lock();
    defer active_ws_connections_mutex.unlock();

    // Iterate through connections, sending the reload message.
    // Handle potential errors (e.g., disconnected clients) by removing them.
    var i: usize = 0;
    while (i < active_ws_connections.items.len) : (i += 1) {
        var ws_conn = &active_ws_connections.items[i];
        if (ws_conn.writeFrame(.text, "reload")) |err| {
            std.log.err("Failed to write to WebSocket client, removing: {s}", .{@errorName(err)});
            // Remove broken connection and adjust index.
            _ = active_ws_connections.swapRemove(i);
            ws_conn.deinit(); // Deinitialize the WebSocket.
            i -= 1; // Decrement index because an item was removed and the next item shifted.
        } else {
            ws_conn.flush() catch |err_flush| {
                std.log.err("Failed to flush WebSocket client, removing: {s}", .{@errorName(err_flush)});
                // Remove broken connection and adjust index.
                _ = active_ws_connections.swapRemove(i);
                ws_conn.deinit();
                i -= 1;
            };
        }
    }
}