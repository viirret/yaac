#!/bin/bash

# get the directory of the script
dir="$( cd "$( dirname "${bash_source[0]}" )" >/dev/null 2>&1 && pwd )"

# create the destination directory if it doesn't exist
mkdir -p "$HOME/.config/yaac"

# copy font from the assets directory to ~/.config/yaac/
cp "$dir/../assets/font.ttf"* "$HOME/.config/yaac/"

# copy the config file to ~/.config/yaac/
cp "$dir/../config" "$HOME/.config/yaac/"

# create the songs directory if it doesn't exist
mkdir -p "$HOME/.config/yaac/songs"

# copy song dir
cp "$dir/../songs/"* "$HOME/.config/yaac/songs/"

# create a README.md file in the songs directory
echo "Add your mp3 files here" > "$HOME/.config/yaac/songs/README.md"
