#!/usr/bin/fish

# Set environment variables for cross-compiling to Windows
set -gx GOOS windows
set -gx GOARCH amd64

echo "Building Go application for Windows..."
go build -o bin/gama.exe .

# Check if the build was successful before proceeding
if test $status -ne 0
    echo "Go build failed. Aborting."
    exit 1
end

echo "Compiling NSIS installer..."
# Use the correct command: makensis
makensis nsis/setup.nsi

if test $status -eq 0
    echo "NSIS installer created successfully!"
else
    echo "NSIS compilation failed."
    exit 1
end
