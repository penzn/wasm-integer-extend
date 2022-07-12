# Wasm integer extend

A few different ways, most notably SIMD. Examples explore extending 8 bit
integers to 32 bits.

- [micro.c](micro.c) - scalar vs vectorizer vs hand-written intrinsics
- [repro.c](repro.c) - reproducer of misvectorization of 8-bit to 32-bit extend
- [micro_runner.js](micro_runner.js) - JS wrapper to run `micro.c` in developer
  shell

To build, run `make`. To execute microbenchmark run it in a JS developer shell.

