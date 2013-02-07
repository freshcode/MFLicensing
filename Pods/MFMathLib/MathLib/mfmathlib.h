//
//  mfmathlib.h
//  MFMathLib
//  https://github.com/freshcode/MFMathLib
//
//
//  A mathematical library providing basic math operations on data types from 8-bits to 1024-bits
//  with overflow/underflow tracking.
//
//  Standard C operations such as multiply and divide do not properly track overflow, underflow and
//  remainders. As an example, multiplying two unsigned char values of 100 together will produce a
//  result larger than could be stored in a resulting unsigned char without any warning at runtime.
//
//  All functions defined in this library provide all the parameters and returned values necessary
//  to ensure mathematical accuracy for all supported data types.
//
//  Licensing
//  ---------
//  Public Domain
//  By Freshcode, Cutting edge Mac, iPhone & iPad software development. http://madefresh.ca/
//  Created by Dave Poirier on 2013-02-01.
//
//  Compatibility Notice
//  --------------------
//  unsigned int must be at least 32-bits.
//
//  Performance
//  -----------
//  This library has _NOT_ yet been optimized for performance.  While its development is in its
//  early stage priority has been put into ensuring mathematical accuracy rather than execution speed.
//  All performance improvement contributions are welcome.
//
//  Dependencies
//  ------------
//  None

#ifndef MathLib_mfmathlib_h
#define MathLib_mfmathlib_h

typedef unsigned char mfU8;
typedef union { struct { mfU8 l8, h8; }; unsigned char b[2]; } mfU16;
typedef union { struct { mfU16 l16, h16; }; unsigned char b[4]; } mfU32;
typedef union { struct { mfU32 l32, h32; }; unsigned char b[8]; mfU32 b32[2]; } mfU64;
typedef union { struct { mfU64 l64, h64; }; unsigned char b[16]; mfU32 b32[4]; } mfU128;
typedef union { struct { mfU128 l128, h128; }; unsigned char b[32]; mfU32 b32[8]; } mfU256;
typedef union { struct { mfU256 l256, h256; }; unsigned char b[64]; mfU32 b32[16]; } mfU512;
typedef union { struct { mfU512 l512, h512; }; unsigned char b[128]; mfU32 b32[32]; } mfU1024;

typedef enum {
    mfCompareEqual = 0,
    mfCompareGreater = 1,
    mfCompareSmaller = 2,
} mfComparisonResult;

void mfUintExtX( unsigned int s, mfU8 *d, unsigned int bytes);

// Copies the source operand (s) to destination operand (d)
// return value:
// 0 = copy completed
void mfCopyX( const mfU8 *s, mfU8 *d, unsigned int bytes);
void mfCopy8( const mfU8 *s, mfU8 *d );
void mfCopy16( const mfU16 *s, mfU16 *d );
void mfCopy32( const mfU32 *s, mfU32 *d );
void mfCopy64( const mfU64 *s, mfU64 *d );
void mfCopy128( const mfU128 *s, mfU128 *d );
void mfCopy256( const mfU256 *s, mfU256 *d );
void mfCopy512( const mfU512 *s, mfU512 *d );
void mfCopy1024( const mfU1024 *s, mfU1024 *d );

// Set (d) to 0.
// return value:
// 0 = zeroing completed
void mfZeroX( mfU8 *d, unsigned int bytes );
void mfZero8( mfU8 *d );
void mfZero16( mfU16 *d );
void mfZero32( mfU32 *d );
void mfZero64( mfU64 *d );
void mfZero128( mfU128 *d );
void mfZero256( mfU256 *d );
void mfZero512( mfU512 *d );
void mfZero1024( mfU1024 *d );

// Checks whether e == 0
// 0 = no
// 1 = yes
unsigned int mfIsZeroX( const mfU8 *e, unsigned int bytes );
unsigned int mfIsZero8( const mfU8 *e );
unsigned int mfIsZero16( const mfU16 *e );
unsigned int mfIsZero32( const mfU32 *e );
unsigned int mfIsZero64( const mfU64 *e );
unsigned int mfIsZero128( const mfU128 *e );
unsigned int mfIsZero256( const mfU256 *e );
unsigned int mfIsZero512( const mfU512 *e );
unsigned int mfIsZero1024( const mfU1024 *e );


// Compare e to r
// return value:
// mfCompareEqual: e == r
// mfCompareGreater e > r
// mfCompareSmaller e < r
mfComparisonResult mfCompareUX( mfU8 *e, mfU8 *r, unsigned int bytes );
mfComparisonResult mfCompareU8( mfU8 *e, mfU8 *r );
mfComparisonResult mfCompareU16( mfU16 *e, mfU16 *r );
mfComparisonResult mfCompareU32( mfU32 *e, mfU32 *r );
mfComparisonResult mfCompareU64( mfU64 *e, mfU64 *r );
mfComparisonResult mfCompareU128( mfU128 *e, mfU128 *r );
mfComparisonResult mfCompareU256( mfU256 *e, mfU256 *r );
mfComparisonResult mfCompareU512( mfU512 *e, mfU512 *r );
mfComparisonResult mfCompareU1024( mfU1024 *e, mfU1024 *r );

// Add a1 to a2, store result in d
// return value:
// 0 = addition completed
// 1 = addition completed with overflow
int mfAddUX( const mfU8 *a1, const mfU8 *a2, mfU8 *d, unsigned int bytes);
int mfAddU8( const mfU8 *a1, const mfU8 *a2, mfU8 *d);
int mfAddU16( const mfU16 *a1, const mfU16 *a2, mfU16 *d);
int mfAddU32( const mfU32 *a1, const mfU32 *a2, mfU32 *d);
int mfAddU64( const mfU64 *a1, const mfU64 *a2, mfU64 *d);
int mfAddU128( const mfU128 *a1, const mfU128 *a2, mfU128 *d);
int mfAddU256( const mfU256 *a1, const mfU256 *a2, mfU256 *d);
int mfAddU512( const mfU512 *a1, const mfU512 *a2, mfU512 *d);
int mfAddU1024( const mfU1024 *a1, const mfU1024 *a2, mfU1024 *d);

// Substract s2 from s1, store result in d
// return value:
// 0 = substraction completed
// 1 = substraction completed with underflow (s2 was bigger than s1)
int mfSubstractUX( const mfU8 *s1, const mfU8 *s2, mfU8 *d, unsigned int bytes);
int mfSubstractU8( const mfU8 *s1, const mfU8 *s2, mfU8 *d);
int mfSubstractU16( const mfU16 *s1, const mfU16 *s2, mfU16 *d);
int mfSubstractU32( const mfU32 *s1, const mfU32 *s2, mfU32 *d);
int mfSubstractU64( const mfU64 *s1, const mfU64 *s2, mfU64 *d);
int mfSubstractU128( const mfU128 *s1, const mfU128 *s2, mfU128 *d);
int mfSubstractU256( const mfU256 *s1, const mfU256 *s2, mfU256 *d);
int mfSubstractU512( const mfU512 *s1, const mfU512 *s2, mfU512 *d);
int mfSubstractU1024( const mfU1024 *s1, const mfU1024 *s2, mfU1024 *d);

// Multiply s1 with s2, store result in d with overflow in o
// return value:
void mfMultiplyUX( const mfU8 *s1, const mfU8 *s2, mfU8 *d, mfU8 *o, unsigned int bytes);
void mfMultiplyU8( const mfU8 *s1, const mfU8 *s2, mfU8 *d, mfU8 *o);
void mfMultiplyU16( const mfU16 *s1, const mfU16 *s2, mfU16 *d, mfU16 *o);
void mfMultiplyU32( const mfU32 *s1, const mfU32 *s2, mfU32 *d, mfU32 *o);
void mfMultiplyU64( const mfU64 *s1, const mfU64 *s2, mfU64 *d, mfU64 *o);
void mfMultiplyU128( const mfU128 *s1, const mfU128 *s2, mfU128 *d, mfU128 *o);
void mfMultiplyU256( const mfU256 *s1, const mfU256 *s2, mfU256 *d, mfU256 *o);
void mfMultiplyU512( const mfU512 *s1, const mfU512 *s2, mfU512 *d, mfU512 *o);
void mfMultiplyU1024( const mfU1024 *s1, const mfU1024 *s2, mfU1024 *d, mfU1024 *o);

// Divide n by d, quotient stored in q with remainder in r
// return value:
// 0 = division completed
// -1 = error, division by 0
int mfDivideUX( const mfU8 *n, const mfU8 *d, mfU8 *q, mfU8 *r, unsigned int bytes);
int mfDivideU8( const mfU8 *n, const mfU8 *d, mfU8 *q, mfU8 *r);
int mfDivideU16( const mfU16 *n, const mfU16 *d, mfU16 *q, mfU16 *r);
int mfDivideU32( const mfU32 *n, const mfU32 *d, mfU32 *q, mfU32 *r);
int mfDivideU64( const mfU64 *n, const mfU64 *d, mfU64 *q, mfU64 *r);
int mfDivideU128( const mfU128 *n, const mfU128 *d, mfU128 *q, mfU128 *r);
int mfDivideU256( const mfU256 *n, const mfU256 *d, mfU256 *q, mfU256 *r);
int mfDivideU512( const mfU512 *n, const mfU512 *d, mfU512 *q, mfU512 *r);
int mfDivideU1024( const mfU1024 *n, const mfU1024 *d, mfU1024 *q, mfU1024 *r);

// Shift bits to the right
void mfShift128Right32( mfU128 *x );    // by 32-bits

// Shift bits to the right by 1 bit
void mfShiftRightXBy1( mfU8 *x, unsigned int bytes);
void mfShiftRight8By1( mfU8 *x);
void mfShiftRight16By1( mfU16 *x);
void mfShiftRight32By1( mfU32 *x);
void mfShiftRight64By1( mfU64 *x);
void mfShiftRight128By1( mfU128 *x);
void mfShiftRight256By1( mfU256 *x);
void mfShiftRight512By1( mfU512 *x);
void mfShiftRight1024By1( mfU1024 *x);

// shift bits to the left by 1 bit
void mfShiftLeftXBy1( mfU8 *x, unsigned int bytes);
void mfShiftLeft8By1( mfU8 *x);
void mfShiftLeft16By1( mfU16 *x);
void mfShiftLeft32By1( mfU32 *x);
void mfShiftLeft64By1( mfU64 *x);
void mfShiftLeft128By1( mfU128 *x);
void mfShiftLeft256By1( mfU256 *x);
void mfShiftLeft512By1( mfU512 *x);
void mfShiftLeft1024By1( mfU1024 *x);

#endif
