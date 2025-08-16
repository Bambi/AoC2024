default:
    @just --list

# Clean build artifacts
clean:
    rm -rf build

setup:
    cmake -B build -G Ninja

build:
    cmake --build build -j

run:
    pytest

alias compile := build

