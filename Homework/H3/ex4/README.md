# VE475 Homework 3 Exercise 4

This folder contains source code for Ex.4, written in C.

In this exercise, AES128 encryption and decryption is implemented.

## Requirements

`gcc >= 9.4.0`

`make >= 4.2.1`

## Compilation

```shell
make
```

## Usage

Key should be put into `./key` as a binary file. Plain text should be written in `./hoge`. The encrypted data will be both stored in `./encrypted` and displayed in command line. To decrypt, put the content into `./encrypted` as a binary file and the result will be both stored in `./decrypted` and displayed in command line.

To see a full demo of encryption and decryption, run
```shell
./ex4
```

Other options:

<table>
	<tr>
		<td>-h</td>
		<td>Show help message</td>
	</tr>
	<tr>
		<td>-a</td>
		<td>Encrypt ./hoge and show the decryption</td>
	</tr>
	<tr>
		<td>-e</td>
		<td>Encrypt ./hoge</td>
	</tr>
	<tr>
		<td>-d</td>
		<td>Decrypt ./encrypted</td>
	</tr>
</table>

## Structure

```
.
├── inc
│    ├── // header files
│    └── aes.h
├── obj
│    ├── // object files
├── src
│    ├── // source code
│    ├── aes.c
│    └── main.c
├── Makefile
├── README.md
├── decrypted
├── encrypted
├── ex4 // Executable
├── hoge // Plain text
├── key // Key
└── test.sh // Use AES from openssl 
```