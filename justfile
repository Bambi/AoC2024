default:
    @just --list

# Clean build artifacts
clean:
    rm -rf build

setup:
    cmake -B build

build:
    make -C build

run:
    pytest

alias compile := build

