#ifndef GAMA_API_H
#define GAMA_API_H

// --- Core Lifecycle & System ---
extern int gapi_init();
extern void gapi_log(const char* message);
extern void gapi_yield();
extern void gapi_set_title(const char* title);

// --- Canvas Control ---
extern void gapi_canvas_set_size(int width, int height);
extern void gapi_canvas_clear_rect(double x, double y, double w, double h);

// --- Styling ---
extern void gapi_canvas_set_fill_color(int r, int g, int b, int a);
extern void gapi_canvas_set_stroke_color(int r, int g, int b, int a);
extern void gapi_canvas_set_line_width(double width);

// --- Basic Shapes ---
extern void gapi_canvas_fill_rect(double x, double y, double w, double h);
extern void gapi_canvas_stroke_rect(double x, double y, double w, double h);

// --- Paths ---
extern void gapi_canvas_path_begin();
extern void gapi_canvas_path_move_to(double x, double y);
extern void gapi_canvas_path_line_to(double x, double y);
extern void gapi_canvas_path_arc(double x, double y, double radius, double start_angle, double end_angle, int counter_clockwise);
extern void gapi_canvas_path_close();
extern void gapi_canvas_path_fill();
extern void gapi_canvas_path_stroke();

// --- Transformations ---
extern void gapi_canvas_transform_save();
extern void gapi_canvas_transform_restore();
extern void gapi_canvas_transform_translate(double x, double y);
extern void gapi_canvas_transform_rotate(double angle_rad);
extern void gapi_canvas_transform_scale(double x, double y);
extern void gapi_canvas_transform_reset();

// --- Text ---
extern void gapi_canvas_set_font(const char* font);
extern void gapi_canvas_set_text_align(const char* align); // "left", "right", "center", "start", "end"
extern void gapi_canvas_fill_text(const char* text, double x, double y);
extern void gapi_canvas_stroke_text(const char* text, double x, double y);

#endif // GAMA_API_H
