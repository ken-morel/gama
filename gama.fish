#!/usr/bin/fish
set -x GAMA_DIR "$HOME/gama/assets"
if test "$argv[1]" != create
    cd test
    go run .. $argv[1..]
else
    go run . $argv[1..]
end
