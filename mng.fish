#!/usr/bin/fish

function package -a mode
    switch "$mode"
        case windows

            echo "Compiling NSIS installer..."
            # Use the correct command: makensis
            makensis nsis/setup.nsi
            if test $status -eq 0
                echo "NSIS installer created successfully!"
            else
                echo "NSIS compilation failed."
                exit 1
            end
        case linux
            echo "Linux packaging not supported yet"
            exit 2
        case "*"
            echo "No or invalid package mode specified"

    end
end
function build -a mode
    switch "$mode"
        case windows
            set -gx GOOS windows
            set -gx GOARCH amd64

            echo "Building Go application for Windows..."
            go build -o bin/gama.exe .
        case linux
            echo "buillding for linux"
            go build -o bin/gama .

        case "*"
            echo "No or invalid build mode specified"
            exit 2
    end
    if test $status -ne 0
        echo "Go build failed. Aborting."
        exit 1
    end

end
switch "$argv[1]"
    case build
        build "$argv[2]"
    case package
        package "$argv[2]"
    case run
        go run .
    case ""
        echo "No command specified"
    case "*"
        echo "Invalid command: $argv[1]"
end
