# Grove

Quantitative numerics library for commodity futures trading. Part of the [Orchard Systematic](https://github.com/orchard-systematic) platform.

## Modules

| Module | Description |
| --- | --- |
| **pricing** | Black-76 options on futures, cost-of-carry fair value, calendar spread / roll yield analysis |
| **curves** | Forward curve bootstrap, natural cubic spline interpolation, seasonal decomposition |
| **signals** | Fast EMA/SMA/DEMA, rolling z-score, 1D Kalman filter |
| **ml** | ONNX Runtime inference wrapper (optional, compile with `-DGROVE_ENABLE_ONNX=ON`) |

## Build

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel $(nproc)
```

## Test

```bash
cmake -B build -DGROVE_BUILD_TESTS=ON
cmake --build build --parallel $(nproc)
cd build && ctest --output-on-failure
```

## Docker

```bash
docker build -t grove .
```

## Integration

### CMake FetchContent

```cmake
include(FetchContent)
FetchContent_Declare(
  grove
  GIT_REPOSITORY https://github.com/orchard-systematic/grove.git
  GIT_TAG main
)
FetchContent_MakeAvailable(grove)
target_link_libraries(your_target PRIVATE grove)
```

### Python (via nanobind in strategy-engine)

The `strategy-engine` repo provides Python bindings to grove via nanobind. See that repo for usage.

## License

[MIT](LICENSE)
