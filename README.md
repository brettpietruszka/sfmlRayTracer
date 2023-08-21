
# SFML RAYTRACER
This is a completely CPU based simplistic raytracer written in C++ using the SFML Library. It follows along with [Computer Graphics From Scratch](https://gabrielgambetta.com/computer-graphics-from-scratch/) by Gabriel Gambetta.

## Setup
1. Must have [SFML](https://www.sfml-dev.org) installed to computer. The folder `SFML-2.5.1`contains example compiled binaries.
2. Use makefile to compile. You might need to change the location of the lib directories if they are different on your computer. It attempts to create a build directory, so if this folder is in a location on your computer where you don't have permission to create a new directory.
3. Run executable `./build/sfml-raytracer.out` on macOS or `./build/sfml-raytracer.exe` on windows/msys

## Usage
1. Use the keys wasd and qe to move around the scene. 
2. Use the arrow keys to rotate the camera 

Note: on macOS you must have given the OS permission to use the keyboard via `System Preferences -> Security and Privacy -> Input Monitoring` for whatever terminal you are running the program from or for the executable if you are running it directly.