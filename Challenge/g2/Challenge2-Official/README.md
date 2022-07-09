# Challenge 2 demo: Caesar cipher

This is an implementation of the super secure Caesar cipher. You are a master of cryptography if you are able to break it. Use this implementation to encrypt your bank account and password and send the ciphertext to me to receive a huge bonus.

## Compilation

Run `make`. The compiled binary can be found as `g2`.

`make clean` removes the binary file.

## Key

Any `long` type value is a valid key. Keys are cast onto the range $\{0,...,68\}$ using the function $f(k)=k\space\mathrm{mod}\space69$. Note that $-1\space\mathrm{mod}\space69=68$. 

## Alphabet

The alphabet is `abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.;?!()`.  For example, the letter 'k' will be encrypted to 'O' using the key 30 (or 99, or -39, etc.)

# Basic testing script

The file `test.py` does some basic automated testing to check if your implementation works correctly. You can run it using Python 3 in your source code directory. If it reports an error:

- Make sure you have a `Makefile`.

- Make sure your compiled executable is `./g2`.

- Make sure your compiled executable is statically linked (see spec).

- Put your default key, desired "challenge plaintext" and "challenge ciphertext" in `key.txt`, `plaintext.txt` and `ciphertext.txt` respectively. Each file may only contain one line. Excessive lines are discarded.

- Note that your implementation should only prevent an attacker from directly decrypting the challenge ciphertext. If they specify a key to use, they should be able to perform decryption. This means brute force attacks are allowed. But - think about feasibility before using brute force! Do not "DDoS attack" the server.

- Make sure you are not always returning the same key when a user calls the `generate` function. If you are doing this, either your implementation is too insecure or you are trying to cheat by preventing attackers from learning how your valid keys look like, which is not permitted in this challenge.

Implementations that do not pass this script's check will not be eligible for Phase 2. If you do not have at least 2 valid implementations for Phase 2, you will not receive the participation bonus point.

However, do keep in mind that even if you passed these checks, your implementation might still have issues. We will contact you if we find any other issue and decide what to do based on the situation.

Specifically, make sure that:

- Your implementation does not depend on information stored in external files. You will not have access to these in Phase 2.

- You have a `README` file, preferably in Markdown.

- Your default key does not change

Do not try to cheat by circumventing the checks. We reserve the right to disqualify your implementation from Phase 2.
