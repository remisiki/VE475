# VE475 Homework 6 Exercise 6

This folder contains source code for Ex.6, written in C.

In this exercise, Pollard's Rho algorithm is implemented to factorize numbers.

## Requirements

`gcc >= 9.4.0`

`make >= 4.2.1`

## Compilation

```shell
make
```

## Usage

To factorize number `n`, use
```c
void factorize(uint64_t n);
```

To try finding one factor of `n`, use
```c
uint64_t findFactor(uint64_t n, uint64_t (*f)(uint64_t, uint64_t));
```

To see a demo, run
```shell
./ex6
```

## Structure

```
.
├── obj
│    ├── // object files
├── Makefile
├── README.md
├── ex6 // Executable
└── pollard_rho.c // Source code
```