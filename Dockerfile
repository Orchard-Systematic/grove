FROM ubuntu:24.04 AS builder

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential cmake git ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src
COPY . .

RUN cmake -B build \
    -DCMAKE_BUILD_TYPE=Release \
    -DGROVE_BUILD_TESTS=OFF \
    -DGROVE_ENABLE_ONNX=OFF \
    && cmake --build build --parallel $(nproc) \
    && cmake --install build --prefix /opt/grove

# ── Runtime image ────────────────────────────────────────────────────
FROM ubuntu:24.04 AS runtime

RUN apt-get update && apt-get install -y --no-install-recommends \
    libstdc++6 \
    && rm -rf /var/lib/apt/lists/*

COPY --from=builder /opt/grove /opt/grove

ENV LD_LIBRARY_PATH=/opt/grove/lib
