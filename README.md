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


