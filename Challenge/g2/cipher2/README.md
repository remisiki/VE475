# VE475 Challenge 2 Cipher 2

This is an implementation of twofish_128_ctr_sha256 cipher. More on [Twofish](https://www.schneier.com/wp-content/uploads/2016/02/paper-twofish-paper.pdf) and [SHA-256](https://csrc.nist.gov/csrc/media/publications/fips/180/2/archive/2002-08-01/documents/fips180-2.pdf).

## Requirements

`gcc >= 9.4.0`

`make >= 4.2.1`

## Compilation

```shell
make
```

## Usage

To generate a random key
```shell
./g2 --generate > key.txt
```

Key may be specified, and if not the default key will be used in both encryption and decryption
```shell
./g2 --encrypt "plaintext" [--key key.txt]
./g2 --decrypt "ciphertext" [--key key.txt]
```
