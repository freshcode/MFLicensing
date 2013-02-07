MFLicensing
===========

Software License Code Generation and Validation Library


Licensing
=========

Public Domain

by Freshcode, Cutting edge Mac, iPhone & iPad software development. http://madefresh.ca/

Originally created by Dave Poirier on 2013-02-01.


Compatibility
=============

C compiler with support for 32-bit unsigned integers.


Dependencies
============

For the library files mflicensing.c/.h:
- MFMathLib (https://github.com/freshcode/MFMathLib)

For the test application:
- Standard OSX environment
- 'bc' command line utility
- CocoaPods (http://cocoapods.org)


Introduction
============

This library provides functions to generate and validate unformatted software license keys.  It offers
flexibility in the characters used for the encoding, how many characters are used, how many keys can
be generated.

The information provided to generate the key, other than the index, is one way hashed into a "validator"
which is then partially included in the final key.  The original information used to generate this validator
is not retrievable from the key.

The keys generated can be validated by a software with knowledge of the original parameters used to generate
the key, and may include a user provided component (provided as a 128-bit digest to the library).


Limitations
===========

This library does not provide any functions for the storage, retrieval or manipulation of licensing data other than
providing a set of functions to generate and validate license keys.

While care was taken to engineer a robust key generation algorithm, the security of the licensing scheme will be
useless if final product can be hacked or reversed engineer.  Any hacker getting hold of your private key and the 
license vector used in your application will be able to generate their own keys.


Overview
========

The library uses a number of parameters to generate the keys, including:
- 256-bit prime number (your private key)
- Custom seed for the scrambler random number generator
- Custom seed for the salt random number generator
- Custom set of encoding characters, scrambled differently for each key
- Custom generated key length
- Custom number of validator/index bits in the key

Without knowledge of any of these, generating a key or validating it is near impossible (bruteforce attack not
withstanding).

Furthermore, during each key generation, the following must be provided:
- 128-bit digest
- 1-bit to 32-bit index

The digest can be used to provide a MD5 hash of the licensee name, or of the serial number of the machine allowed to
license the software.  For COTS software, the digest could be set to a product code or some other custom identifer
common to all keys.


How it works
============

Key Generation
--------------
1. All the information required is assembled into a "licensing vector"
2. The licensing vector, along with the digest and the index are passed to the license generation function
3. The scrambler random number generator is initialized with the scrambling seed
4. The encoding characters are scrambled
5. The bits positions in the final key are scrambled for both the index and validator
6. The index is used to initialize a random number generator and a block of 128-bit of random data is created
7. The 128-bit digest is multiplied to the previous block generated to get a 256-bit intermediate block
8. The salt sees are used to initialize a random number generator and a block of 256-bit of random data is created
9. The two 256-bit blocks are multiplied together to obtain a 512-bit block
10. The 512-bit block is divided by the 256-bit prime number to obtain a unique 256-bit remainder, referred to as validator
11. An array of "bits" is filled in the scrambled order with the bits of the index and as many bits of the validator as can be fitted
12. These bits are reassembled into a 256-bit "binary key"
13. The binary key is successively divided by the number of encoding characters provided, encoding on each iteration character of the final key

Key Validation
--------------
1. All the known information required is assembled into a "licensing vector"
2. The licensing vector, along with a digest and a license key are passed to the license validator function
3. The scrambler random number generator is initialized with the scrambling seed
4. The encoding characters are scrambled
5. The bits positions in the binary key are scrambled
6. The license key is processed, one character at a time
7. Each character's binary value is matched to the scrambled encoding characters, then successively multiplied into a "binary key"
8. The binary key is split into an array of "bits"
9. The index originally used to create the key is retrieved from the scrambled bit positions
10. A new license key is generated from the extracted index, the provided digest and the known parameters
11. The new license key is compared to the provided license key



