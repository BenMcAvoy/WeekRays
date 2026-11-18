# WeekRays

WeekRays is a C++ ray-tracing study based on the *Ray Tracing in One Weekend* approach. It implements rays, spheres, materials and camera sampling, then distributes the image across worker threads and tiles.

## Build

Open `WeekRays.sln` in Visual Studio and build the x64 configuration with a recent MSVC toolchain that supports the C++23 library features used by the project.

## Status

This is a learning renderer with a small hard-coded scene. It is not a general-purpose rendering engine.
