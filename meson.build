project(
    'teleport-list',
    'cpp',
    version: '0.0.1',
    default_options: [
        'cpp_std=c++14',
        'warning_level=3'
    ]
)

teleport_list = include_directories(['include'])

executable(
    'teleport-list-benchmark',
    'Benchmark.cpp',
    include_directories: [teleport_list]
)
