# VE475 Homework 5 Exercise 4

This folder contains source code for Ex.4, written in C.

In this exercise, RSA encryption and decryption is implemented.

## Requirements

`gmp >= 6.2.0`

`gcc >= 9.4.0`

`make >= 4.2.1`

## Compilation

```shell
make
```

## Usage

First generate one RSA key with a security level
```c
RSA_keys keys = rsaGenerate(80);
```

Then encrypt the plain text
```c
uint8_t* text = (uint8_t*)"EL PSY CONGROO";
size_t length = strlen((char*)text);
Buffer plain_text = {text, length};

Buffer c = rsaEncrypt(plain_text, *keys.public_key);
```

And to decrypt use
```c
Buffer m = rsaDecrypt(c, *keys.private_key);
```

To see a full demo, run
```shell
./ex4
```

## Structure

```
.
├── inc
│    ├── // header files
│    └── rsa.h
├── obj
│    ├── // object files
├── src
│    ├── // source code
│    ├── rsa.c
│    └── main.c
├── Makefile
├── README.md
└── ex4 // Executable
```