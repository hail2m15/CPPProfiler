# CPPProfiler
Lightweight C++ function and scope profiler that outputs profiling data in JSON format which can be viewed using chrome://tracing. Ideal for measuring performance hotspots in multi-threaded applications.

Following the tutorial VISUAL BENCHMARKING in C++ (how to measure performance visually)  https://www.youtube.com/watch?v=xlAH4dbMVnU&list=PLlrATfBNZ98dudnM48yfGUldqGD0S4FFb&index=81

# 🔍 PerProfiler - Lightweight C++ Function and Scope Profiler

A minimal and easy-to-integrate C++ function/scope profiler for measuring performance metrics like function duration and execution time across threads. Profiling data is written to a JSON file for visualization or further analysis.

## 🚀 Features

- Scoped-based profiling using RAII (`PerfTimer`)
- Function-level auto-profiling using `__FUNCSIG__`
- Thread-safe and multi-threaded capable
- Output in JSON format (compatible with tools like Chrome Trace Viewer)
- Console/debug mode logging (optional via macros)

## 📦 Setup

1. Include the `PerProfiler.h` and corresponding `.cpp` file in your project.
2. Define `PROFILING` as `1` to enable profiling globally.
3. Use the macros to mark profiling regions in your code.
