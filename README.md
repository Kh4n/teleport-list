For more information how this DS works, read my CS StackExchange question here: https://cs.stackexchange.com/questions/137076/does-this-data-structure-already-exist

This DS offers *amortized* `log(n)` for the following operations:
- Insert
- Find
- Remove

The backing storage is a linked list (or an unrolled linked list), and the data is always sorted.
This allows for a variety of other queries to be done in `log(n)` as well eg. range queries.

It was designed to be space efficient, simple, fast, and to support indexable operations.
For indexing, you can use either a segment tree or a Fenwick tree (I am currently experimenting with these options).
By adding indexable operations, this should be an excellent DS for a text editor.

Another advantage of this DS as far as text editing is concerned is that multiple people can edit different parts
of a document simultaneously with excellent performance (it is guaranteed `log(n/g)` where `g` is a gap size you can adjust)

Building should be as simple as (from root dir):
```shell
mkdir build
cd build
cmake ../ -DCMAKE_BUILD_TYPE=Release
```
The executable is named `benchmark` (or `benchmark.exe`)

or for Meson:
```shell
meson setup build_release --buildtype release
cd build_release
meson_compile
```
The executable is named `teleport-list-benchmark` (or `teleport-list-benchmark.exe`)

Alternatively, you can use Docker:
```shell
docker-compose -f docker/benchmark-compose.yml up
```

Here is the benchmark run on my computer:
```
Random inserts (std::set):
Time for 1000000 inserts: 775 ms
Time for 1000000 finds: 451 ms
Random inserts (TeleportList):
Time for 1000000 inserts: 1084 ms
Time for 1000000 finds: 318 ms
Random inserts (UnrolledTeleportList):
Time for 1000000 inserts: 307 ms
Time for 1000000 finds: 365 ms

Random inserts/finds (std::set):
Time for 1000000 inserts and finds: 768 ms
Random inserts/finds (TeleportList):
Time for 1000000 inserts and finds: 1116 ms
Random inserts/finds (UnrolledTeleportList):
Time for 1000000 inserts and finds: 573 ms

Sequential inserts (std::set):
Time for 1000000 inserts: 270 ms
Time for 1000000 finds: 76 ms
Sequential inserts (TeleportList):
Time for 1000000 inserts: 73 ms
Time for 1000000 finds: 67 ms
Sequential inserts (UnrolledTeleportList):
Time for 1000000 inserts: 44 ms
Time for 1000000 finds: 52 ms

Sequential inserts (reverse) (std::set):
Time for 1000000 inserts: 264 ms
Time for 1000000 finds: 62 ms
Sequential inserts (reverse) (TeleportList):
Time for 1000000 inserts: 63 ms
Time for 1000000 finds: 87 ms
Sequential inserts (reverse) (UnrolledTeleportList):
Time for 1000000 inserts: 29 ms
Time for 1000000 finds: 54 ms

Sequential inserts/finds (reverse) (std::set):
Time for 1000000 inserts and finds: 376 ms
Sequential inserts/finds (reverse) (TeleportList):
Time for 1000000 inserts and finds: 54 ms
Sequential inserts/finds (reverse) (UnrolledTeleportList):
Time for 1000000 inserts and finds: 34 ms

Random inserts (std::set):
Time for 1000000 inserts: 798 ms
Time for 1000000 finds: 483 ms
Time for 1000000 removes: 1079 ms
Random inserts (UnrolledTeleportList):
Time for 1000000 inserts: 360 ms
Time for 1000000 finds: 407 ms
Time for 1000000 removes: 358 ms
```