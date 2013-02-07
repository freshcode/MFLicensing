MFMathLib
=========

A mathematical library providing basic math operations on data types from 8-bits to 1024-bits
with overflow/underflow tracking.

Standard C operations such as multiply and divide do not properly track overflow, underflow and
remainders. As an example, multiplying two unsigned char values of 100 together will produce a
result larger than could be stored in a resulting unsigned char without any warning at runtime.

All functions defined in this library provide all the parameters and returned values necessary
to ensure mathematical accuracy for all supported data types.



Licensing
=========
Public Domain

by Freshcode, Cutting edge Mac, iPhone & iPad software development.
http://madefresh.ca/

Originally created by Dave Poirier on 2013-02-01.

Compatibility
=============
C compiler with support for 32-bit unsigned integers

Performance
===========
This library has _NOT_ yet been optimized for performance.  While its development is in its
early stage priority has been put into ensuring mathematical accuracy rather than execution speed.
All performance improvement contributions are welcome.

Dependencies
============
For the library files mfmathlib.c/.h:
-  none

For the test framework:
-  standard OSX environment
-  Accelerate framework
-  'bc' command line tool.

Limitations
===========
The library currently only support unsigned integer operations
