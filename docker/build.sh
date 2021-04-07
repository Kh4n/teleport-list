apk add --no-cache cmake clang g++
mkdir -p /teleport-list/build-docker
clang++ -O3 /teleport-list/Benchmark.cpp -o /teleport-list/build/benchmark --include-directory /teleport-list/include