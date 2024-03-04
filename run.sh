#!/bin/bash
# Build the cmake project
cmake -S . -B build
cmake --build build

# Check if the build was successful
if [ ! -d "build" ]; then
    echo "Build directory not found. Build failed."
    exit 1
fi

# Run the program for each json file in the specified filepath
for jsonfile in "assets"/*.json; do
    if [ -f "$jsonfile" ]; then
        echo "Processing $jsonfile..."
        build/raytracer "$jsonfile"
    else
        echo "No JSON files found in $FILEPATH."
        break
    fi
done
