# VE475 Homework 7 Exercise 5

This folder contains source code for Ex.5, written in C.

In this exercise, faster modular exponentiation is implemented.

## Requirements

`gmp >= 6.2.0`

`gcc >= 9.4.0`

`make >= 4.2.1`

## Compilation

```shell
make
```

## Usage

To run normal modular exponentiation, use
```c
void mod1(mpz_t m_0, mpz_t d_0, mpz_t m_1, mpz_t d_1, mpz_t n);
```

To run faster modular exponentiation, first calculate `m = m_0 * m_1` and use
```c
void mod2(mpz_t m_0, mpz_t d_0, mpz_t m_1, mpz_t d_1, mpz_t n, mpz_t m);
```

To see a demo on random 15360-bit numbers, run
```shell
./ex5
```

## Structure

```
.
├── obj
│    └── // Object files
├── Makefile
├── README.md
├── ex5 // Executable
└── modular.c // Source code
```