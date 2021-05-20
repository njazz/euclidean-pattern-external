Very simple euclidean pattern generator for Max and Pure Data

usage:

[make_pattern ones zeros ( 	

generates patterns that contains 0 and 1:
[make_pattern 3 4 ( -> 1 0 0 1 0 1 0

[make_fractions ones zeros (
generates time values (0..1) when [1] elements appear in sequence

[make_fractions 3 4 ( -> 0. 0.428571 0.714286

Alex Nadzharov, 2021
MIT


build:
./scripts/build-osx.sh from repository dir
