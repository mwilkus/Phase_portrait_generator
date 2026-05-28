# Phase Portrait Generator

A small C++ project that generates phase portraits for dynamical systems and renders them to a window or image. This repository contains the source, build configuration, and a compiled binary in `bin/`. Project uses libraries: SDL2 and parser Exprtk by Arash Partow.

## Requirements
- C++ compiler with C++17 support
- SDL2, SDL2_image, SDL2_ttf modules
- CMake
- Make

## Build
From the project root run:

```bash
cmake -B build
cmake --build build
```

Or (classic):

```bash
cmake -B build && cd build && make
```

## Run
After building, run the binary located in `bin/`:

```bash
cd ..
./bin/Phase_poitrat_generator
```

If the binary isn't present, ensure the build completed successfully and check `build/` for errors.

## Project layout (short)
- `src/` — implementation modules (control, cordinats, input_eq, numeric, parser, rendering)
- `include/` — only `config.h`
- `bin/` — compiled executable(s)
- `build/` — out-of-source build directory (generated)

## Usage
After launching the application, click on the appropriate fields and enter your equations.
Confirm the entered equations with Enter.
You can zoom in and out using the arrow keys.
You can restart the scale by clicking R.
You can click LMB to show a simulated trajectory from the clicked point.

## Caution!
Note that with large derivative values ​​the program will show approximate or completely wrong results due to numerical errors.
