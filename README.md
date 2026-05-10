# Grove

Open-source educational resources and tools for quantitative finance.

Grove is an open-source quantitative numerics library intended for research, learning, and demonstration with commodity futures data. It is part of the Orchard Systematic educational resources and blog and is provided for instructional purposes only. Use at your own risk; see the license for terms.

## Modules

| Module      | Description                                                                                  |
| ----------- | -------------------------------------------------------------------------------------------- |
| **pricing** | Black-76 options on futures, cost-of-carry fair value, calendar spread / roll yield analysis |
| **curves**  | Forward curve bootstrap, natural cubic spline interpolation, seasonal decomposition          |
| **signals** | Fast EMA/SMA/DEMA, rolling z-score, 1D Kalman filter                                         |
| **ml**      | ONNX Runtime inference wrapper (optional, compile with `-DGROVE_ENABLE_ONNX=ON`)             |

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

## License

[MIT](LICENSE)
