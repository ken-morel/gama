#!/usr/bin/fish
set -x GAMA_DIR "$HOME/gama/assets"
if test "$argv[1]" != create
    cd test
end
go run . $argv[1..]
