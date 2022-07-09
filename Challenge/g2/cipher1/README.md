# INTRODUCTION
This program is used to encrypt or decrypt messages. The alrorithm is based on RC6, 
defined in 

Rivest, Ronald L. et al. "The RC6TM block cipher." *First advanced encryption standard (AES) conference*. 1998

(https://people.csail.mit.edu/rivest/pubs/RRSY98.pdf)
# BUILD
Enter the directory of `g2.c` in terminal and use `make` command to compile the program. 

# USE
1. After building, an executable file `g2` should exist. 
2. The program receives four arguments `--generate`, `--decrypt`, `--encrypt` and `--key`. 
    `--generate` argument is used to generate a valid key, should would be a 
    1024-bit long number expressed in HEX form. 
    That is to say, running this would give a 2048-letter long string to 
    the stantdard output stream. 
    Therefore, `./g2 --generate > key.txt` command is recommended to store 
    the key into `key.txt` file. 

    To encrypt message, the command `./g2 --encrypt "theMessageToEncrypt"` can be used. 
    The message should contains only `a-zA-Z0-8,.;?!()`, or the program won't work 
    correctly. 
    Besides, with this command, message will be encrypted with a default key. 
    To use other keys, you can refer to the introduction of `--key` argument. 
    If input is valid, the program will output the ciphertext to the standard 
    output stream. 
    
    As for decryption, you should use command `./g2 --decrypt "theMessageToDecrypt"`. 
    Similar to encryption, without `--key`, it will decrypt ciphertext using the 
    default key. 
    Different from plaintext, ciphertexts are limited. They are supposed to be 
    HEX form of numbers without `0x` or space. Even if the form is correct, they 
    may have no valid corresponding plaintext, which will trigger an output of 
    `This ciphertext has no corresponding plaintext!`. 

    For encryption and decryption, `--key` can be used to specify the key to use, 
    which is followed by the name of the file containing a valid key. 
    For example, `./g2 --encrypt "hello" --key key.txt`. 

    This program can only do one task at once. Therfore, using any two or more of 
    `--decrypt`, `--encrypt` and `--generate` causes inproper functioning. 