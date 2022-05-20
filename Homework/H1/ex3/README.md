# VE475 Homework 1 Exercise 3

This folder contains source code for Ex.3, written in C++.

In this exercise, extended Euclidean algorithm is implemented, and a short main demo program is written to compare the implementation to the built-in `gcd()` function of [GMP](https://gmplib.org/). 

## Requirements

`gmp >= 6.2.0`

`gcc >= 9.4.0`

`make >= 4.2.1`

## Compilation

```shell
make
```

## Run

```shell
./ex3
```

## Structure

```
.
├── inc
│    ├── // header files
│    └── euclid.h
├── obj
│    ├── // object files
├── src
│    ├── // source code
│    ├── euclid.cpp
│    └── ex3.cpp
├── Makefile
└── README.md
```