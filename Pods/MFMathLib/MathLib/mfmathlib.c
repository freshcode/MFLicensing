//
//  mfmathlib.c
//  MFMathLib
//  https://github.com/freshcode/MFMathLib
//
//  Public Domain
//  By Freshcode, Cutting edge Mac, iPhone & iPad software development. http://madefresh.ca/
//  Created by Dave Poirier on 2013-02-01.
//
//  WARNING:
//  --------
//  There are tons of optimizations that could be done, but in this first
//  iteration we concentrate on safe behaviour.
//
//  Compatibility Notice
//  --------------------
//  unsigned int must be at least 32-bits.
//
//  TODO:
//  -----
//  All signed operations

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "mfmathlib.h"

#pragma mark - Extend
void mfUintExtX( unsigned int s, mfU8 *d, unsigned int bytes)
{
    mfZeroX(d, bytes);
    d[0] = (unsigned char)(s & 0xFF);
    if( bytes > 1 ) {
        d[1] = (unsigned char)((s >> 8) & 0xFF);
        if( bytes > 2 ) {
            d[2] = (unsigned char)((s >> 16) & 0xFF);
            if( bytes > 3 ) {
                d[3] = (unsigned char)((s >> 24) & 0xFF);
            }
        }
    }
}

#pragma mark - Copy
// Copy value of s into d
void mfCopyX( const mfU8 *s, mfU8 *d, unsigned int bytes )
{
    while( bytes-- ) {
        d[bytes] = s[bytes];
    }
}
void mfCopy8( const mfU8 *s, mfU8 *d ) { *d = *s; }
void mfCopy16( const mfU16 *s, mfU16 *d ) { mfCopyX( s->b, d->b, sizeof(mfU16)); }
void mfCopy32( const mfU32 *s, mfU32 *d ) { mfCopyX( s->b, d->b, sizeof(mfU32)); }
void mfCopy64( const mfU64 *s, mfU64 *d ) { mfCopyX( s->b, d->b, sizeof(mfU64)); }
void mfCopy128( const mfU128 *s, mfU128 *d ) { mfCopyX( s->b, d->b, sizeof(mfU128)); }
void mfCopy256( const mfU256 *s, mfU256 *d ) { mfCopyX( s->b, d->b, sizeof(mfU256)); }
void mfCopy512( const mfU512 *s, mfU512 *d ) { mfCopyX( s->b, d->b, sizeof(mfU512)); }
void mfCopy1024( const mfU1024 *s, mfU1024 *d ) { mfCopyX( s->b, d->b, sizeof(mfU1024)); }

#pragma mark - Zero
// Set (d) to 0.
// return value:
// 0 = zeroing completed
void mfZeroX( mfU8 *d, unsigned int bytes )
{
    while( bytes-- ) {
        d[bytes] = 0;
    }
}
void mfZero8( mfU8 *d ) { *d = 0; }
void mfZero16( mfU16 *d ) { mfZeroX( d->b, sizeof(mfU16)); }
void mfZero32( mfU32 *d ) { mfZeroX( d->b, sizeof(mfU32)); }
void mfZero64( mfU64 *d ) { mfZeroX( d->b, sizeof(mfU64)); }
void mfZero128( mfU128 *d ) { mfZeroX( d->b, sizeof(mfU128)); }
void mfZero256( mfU256 *d ) { mfZeroX( d->b, sizeof(mfU256)); }
void mfZero512( mfU512 *d ) { mfZeroX( d->b, sizeof(mfU512)); }
void mfZero1024( mfU1024 *d ) { mfZeroX( d->b, sizeof(mfU1024)); }

#pragma mark - Add
// Add a1 to a2, store result in d
// d can be the same address as a1 or a2.
// return value:
// 0 = addition completed
// 1 = addition completed with overflow
int mfAddUX( const mfU8 *a1, const mfU8 *a2, mfU8 *d, unsigned int bytes)
{
    unsigned int t = 0;
    unsigned int i = 0;
    unsigned int a1_u, a2_u;
    while( i < bytes ) {
        a1_u = a1[i] & 0xFF;
        a2_u = a2[i] & 0xFF;
        t = a1_u + a2_u + (t & 0x01);
        d[i] = (unsigned char)(t & 0xFF);
        t = (t >> 8) & 0x01;
        i++;
    }
    return t == 0 ? 0 : 1;
}
int mfAddU8( const mfU8 *a1, const mfU8 *a2, mfU8 *d)
{
    unsigned int x = *a1 + *a2;
    *d = (unsigned char)(x & 0xFF);
    return (x >> 8) & 0x01;
}
int mfAddU16( const mfU16 *a1, const mfU16 *a2, mfU16 *d) { return mfAddUX( a1->b, a2->b, d->b, sizeof(mfU16)); }
int mfAddU32( const mfU32 *a1, const mfU32 *a2, mfU32 *d) { return mfAddUX( a1->b, a2->b, d->b, sizeof(mfU32)); }
int mfAddU64( const mfU64 *a1, const mfU64 *a2, mfU64 *d) { return mfAddUX( a1->b, a2->b, d->b, sizeof(mfU64)); }
int mfAddU128( const mfU128 *a1, const mfU128 *a2, mfU128 *d) { return mfAddUX( a1->b, a2->b, d->b, sizeof(mfU128)); }
int mfAddU256( const mfU256 *a1, const mfU256 *a2, mfU256 *d) { return mfAddUX( a1->b, a2->b, d->b, sizeof(mfU256)); }
int mfAddU512( const mfU512 *a1, const mfU512 *a2, mfU512 *d) { return mfAddUX( a1->b, a2->b, d->b, sizeof(mfU512)); }
int mfAddU1024( const mfU1024 *a1, const mfU1024 *a2, mfU1024 *d) { return mfAddUX( a1->b, a2->b, d->b, sizeof(mfU1024)); }

#pragma mark - Substract

// Substract s2 from s1, store result in d
// return value:
// 0 = substraction completed
// 1 = substraction completed with underflow (s2 was bigger than s1)
int mfSubstractUX( const mfU8 *s1, const mfU8 *s2, mfU8 *d, unsigned int bytes)
{
    mfU8 *left_over = (mfU8 *)malloc(bytes);
    mfCopyX(s1, left_over, bytes);

    int underflow = 0;
    int perma_underflow = 0;
    int i = 0;
    while( i < bytes ) {
        mfU8 sb = s2[i];
        mfU8 lb = left_over[i];
        if( sb > lb ) {
            lb = lb - sb;
            int j = i + 1;
            underflow = 1;
            while( ((underflow == 1) && (j < bytes)) ) {
                if( left_over[j] > 0 ) {
                    left_over[j]--;
                    underflow = 0;
                } else {
                    left_over[j]--;
                    j++;
                }
            }
            if( underflow == 1 ) {
                perma_underflow = 1;
            }
        } else {
            lb = lb - sb;
        }
        left_over[i] = lb;
        i++;
    }
    mfCopyX(left_over, d, bytes);
    free(left_over);
    
    return perma_underflow;
}
int mfSubstractU8( const mfU8 *s1, const mfU8 *s2, mfU8 *d) { return mfSubstractUX(s1, s2, d, sizeof(mfU8)); }
int mfSubstractU16( const mfU16 *s1, const mfU16 *s2, mfU16 *d) { return mfSubstractUX(s1->b, s2->b, d->b, sizeof(mfU16)); }
int mfSubstractU32( const mfU32 *s1, const mfU32 *s2, mfU32 *d) { return mfSubstractUX(s1->b, s2->b, d->b, sizeof(mfU32)); }
int mfSubstractU64( const mfU64 *s1, const mfU64 *s2, mfU64 *d) { return mfSubstractUX(s1->b, s2->b, d->b, sizeof(mfU64)); }
int mfSubstractU128( const mfU128 *s1, const mfU128 *s2, mfU128 *d) { return mfSubstractUX(s1->b, s2->b, d->b, sizeof(mfU128)); }
int mfSubstractU256( const mfU256 *s1, const mfU256 *s2, mfU256 *d) { return mfSubstractUX(s1->b, s2->b, d->b, sizeof(mfU256)); }
int mfSubstractU512( const mfU512 *s1, const mfU512 *s2, mfU512 *d) { return mfSubstractUX(s1->b, s2->b, d->b, sizeof(mfU512)); }
int mfSubstractU1024( const mfU1024 *s1, const mfU1024 *s2, mfU1024 *d) { return mfSubstractUX(s1->b, s2->b, d->b, sizeof(mfU1024)); }

#pragma mark - Shift Right
void mfShift128Right32( mfU128 *x )
{
    x->l64.l32 = x->l64.h32;
    x->l64.h32 = x->h64.l32;
    x->h64.l32 = x->h64.h32;
    mfZero32(&x->h64.h32);
}
void mfShiftRightXBy1( unsigned char *x, unsigned int bytes ) {
    while( bytes > 1 ) {
        x[0] = ((x[0] >> 1) & 0x7F) | ((x[1] << 7) & 0x80);
        x = &x[1];
        bytes --;
    }
    x[0] = x[0] >> 1;
}
void mfShiftRight8By1( mfU8 *x) { *x = *x >> 1; }
void mfShiftRight16By1( mfU16 *x) { mfShiftRightXBy1( x->b, sizeof(mfU16)); }
void mfShiftRight32By1( mfU32 *x) { mfShiftRightXBy1( x->b, sizeof(mfU32)); }
void mfShiftRight64By1( mfU64 *x) { mfShiftRightXBy1( x->b, sizeof(mfU64)); }
void mfShiftRight128By1( mfU128 *x ) { mfShiftRightXBy1(x->b, sizeof(mfU128)); }
void mfShiftRight256By1( mfU256 *x ) { mfShiftRightXBy1(x->b, sizeof(mfU256)); }
void mfShiftRight512By1( mfU512 *x ) { mfShiftRightXBy1(x->b, sizeof(mfU512)); }
void mfShiftRight1024By1( mfU1024 *x ) { mfShiftRightXBy1(x->b, sizeof(mfU1024)); }

#pragma mark - Shift Left
void mfShiftLeftXBy1( unsigned char *x, unsigned int bytes ) {
    while( bytes > 1 ) {
        bytes--;
        x[bytes] = ((x[bytes] << 1) & 0xFE) | ((x[bytes-1] >> 7) & 0x01);
    }
    x[0] = x[0] << 1;
}
void mfShiftLeft8By1( mfU8 *x) { *x = *x << 1; }
void mfShiftLeft16By1( mfU16 *x) { mfShiftLeftXBy1( x->b, sizeof(mfU16) ); }
void mfShiftLeft32By1( mfU32 *x) { mfShiftLeftXBy1( x->b, sizeof(mfU32) ); }
void mfShiftLeft64By1( mfU64 *x) { mfShiftLeftXBy1( x->b, sizeof(mfU64) ); }
void mfShiftLeft128By1( mfU128 *x) { mfShiftLeftXBy1( x->b, sizeof(mfU128) ); }
void mfShiftLeft256By1( mfU256 *x) { mfShiftLeftXBy1( x->b, sizeof(mfU256) ); }
void mfShiftLeft512By1( mfU512 *x) { mfShiftLeftXBy1( x->b, sizeof(mfU512) ); }
void mfShiftLeft1024By1( mfU1024 *x) { mfShiftLeftXBy1( x->b, sizeof(mfU1024) ); }


#pragma mark - Compare
mfComparisonResult mfCompareUX( mfU8 *e, mfU8 *r, unsigned int bytes )
{
    mfU8 eb, rb;
    while( bytes-- ) {
        eb = e[bytes];
        rb = r[bytes];
        if( eb > rb ) return mfCompareGreater;
        if( eb < rb ) return mfCompareSmaller;
    }
    return mfCompareEqual;
}
mfComparisonResult mfCompareU8( mfU8 *e, mfU8 *r ) {
    if( *e > *r ) return mfCompareGreater;
    if( *e < *r ) return mfCompareSmaller;
    return mfCompareEqual;
}
mfComparisonResult mfCompareU16( mfU16 *e, mfU16 *r ) { return mfCompareUX(e->b, r->b, sizeof(mfU16)); }
mfComparisonResult mfCompareU32( mfU32 *e, mfU32 *r ) { return mfCompareUX(e->b, r->b, sizeof(mfU32)); }
mfComparisonResult mfCompareU64( mfU64 *e, mfU64 *r ) { return mfCompareUX(e->b, r->b, sizeof(mfU64)); }
mfComparisonResult mfCompareU128( mfU128 *e, mfU128 *r ) { return mfCompareUX(e->b, r->b, sizeof(mfU128)); }
mfComparisonResult mfCompareU256( mfU256 *e, mfU256 *r ) { return mfCompareUX(e->b, r->b, sizeof(mfU256)); }
mfComparisonResult mfCompareU512( mfU512 *e, mfU512 *r ) { return mfCompareUX(e->b, r->b, sizeof(mfU512)); }
mfComparisonResult mfCompareU1024( mfU1024 *e, mfU1024 *r ) { return mfCompareUX(e->b, r->b, sizeof(mfU1024)); }

#pragma mark - Zero Test
unsigned int mfIsZeroX( const mfU8 *e, unsigned int bytes )
{
    mfU8 eb = 0;
    while( bytes-- ) {
        eb = eb | e[bytes];
    }
    return (eb == 0) ? 1 : 0;
}
unsigned int mfIsZero8( const mfU8 *e ) { return (*e == 0) ? 1 : 0; }
unsigned int mfIsZero16( const mfU16 *e ) { return mfIsZeroX( e->b, sizeof(mfU16) ); }
unsigned int mfIsZero32( const mfU32 *e ) { return mfIsZeroX( e->b, sizeof(mfU32) ); }
unsigned int mfIsZero64( const mfU64 *e ) { return mfIsZeroX( e->b, sizeof(mfU64) ); }
unsigned int mfIsZero128( const mfU128 *e ) { return mfIsZeroX( e->b, sizeof(mfU128) ); }
unsigned int mfIsZero256( const mfU256 *e ) { return mfIsZeroX( e->b, sizeof(mfU256) ); }
unsigned int mfIsZero512( const mfU512 *e ) { return mfIsZeroX( e->b, sizeof(mfU512) ); }
unsigned int mfIsZero1024( const mfU1024 *e ) { return mfIsZeroX( e->b, sizeof(mfU1024) ); }

#pragma mark - Bitwise or
void mforX( const mfU8 *s1, const mfU8 *s2, mfU8 *d, unsigned bytes )
{
    while( bytes-- ) {
        d[bytes] = s1[bytes] | s2[bytes];
    }
}
void mfor8( const mfU8 *s1, const mfU8 *s2, mfU8 *d ) { *d = *s1 | *s2; }
void mfor16( const mfU16 *s1, const mfU16 *s2, mfU16 *d ) { mforX(s1->b, s2->b, d->b, sizeof(mfU16)); }
void mfor32( const mfU32 *s1, const mfU32 *s2, mfU32 *d ) { mforX(s1->b, s2->b, d->b, sizeof(mfU32)); }
void mfor64( const mfU64 *s1, const mfU64 *s2, mfU64 *d ) { mforX(s1->b, s2->b, d->b, sizeof(mfU64)); }
void mfor128( const mfU128 *s1, const mfU128 *s2, mfU128 *d ) { mforX(s1->b, s2->b, d->b, sizeof(mfU128)); }
void mfor256( const mfU256 *s1, const mfU256 *s2, mfU256 *d ) { mforX(s1->b, s2->b, d->b, sizeof(mfU256)); }
void mfor512( const mfU512 *s1, const mfU512 *s2, mfU512 *d ) { mforX(s1->b, s2->b, d->b, sizeof(mfU512)); }
void mfor1024( const mfU1024 *s1, const mfU1024 *s2, mfU1024 *d ) { mforX(s1->b, s2->b, d->b, sizeof(mfU1024)); }


#pragma mark - Multiply
// Multiply s1 with s2, store result in d with overflow in o
void mfMultiplyUX( const mfU8 *s1, const mfU8 *s2, mfU8 *d, mfU8 *o, unsigned int bytes)
{
    mfU8 *dt = (mfU8 *)malloc(bytes);
    mfU8 *ot = (mfU8 *)malloc(bytes);
    mfU8 *accumulator = (mfU8 *)malloc(bytes);
    mfU8 *temp = (mfU8 *)malloc(bytes);

    unsigned int lower_bound = 0;
    unsigned int upper_bound = 0;
    unsigned int a_i;
    
    mfZeroX(accumulator, bytes);

    while( upper_bound < bytes ) {
        unsigned int s1_i = lower_bound;
        unsigned int s2_i = upper_bound;
        unsigned int t = 0;
        while( s1_i <= upper_bound ) {
            t += s1[s1_i] * s2[s2_i];
            s1_i ++;
            s2_i --;
        }
        mfUintExtX(t, temp, bytes);
        mfAddUX( accumulator, temp, accumulator, bytes);
        a_i = 0;
        dt[upper_bound] = accumulator[a_i];
        while( ++a_i < bytes ) {
            accumulator[a_i -1] = accumulator[a_i];
        }
        upper_bound++;
    }
    upper_bound--;
    while( ++lower_bound < bytes ) {
        unsigned int s1_i = lower_bound;
        unsigned int s2_i = upper_bound;
        unsigned int t = 0;
        while( s1_i <= upper_bound ) {
            t += s1[s1_i] * s2[s2_i];
            s1_i ++;
            s2_i --;
        }
        mfUintExtX(t, temp, bytes);
        mfAddUX( accumulator, temp, accumulator, bytes);
        a_i = 0;
        ot[lower_bound-1] = accumulator[a_i];
        while( ++a_i < bytes ) {
            accumulator[a_i -1] = accumulator[a_i];
        }
    }
    ot[bytes-1] = accumulator[0];
    mfCopyX(dt, d, bytes);
    mfCopyX(ot, o, bytes);
    free( temp );
    free( accumulator );
    free( ot );
    free( dt );
    return;
}
void mfMultiplyU8( const mfU8 *s1, const mfU8 *s2, mfU8 *d, mfU8 *o)
{
    unsigned int x = *s1 * *s2;
    *d = (unsigned char)(x & 0xFF);
    *o = (unsigned char)(x >> 8);
}
void mfMultiplyU16( const mfU16 *s1, const mfU16 *s2, mfU16 *d, mfU16 *o) { mfMultiplyUX( s1->b, s2->b, d->b, o->b, sizeof(mfU16)); }
void mfMultiplyU32( const mfU32 *s1, const mfU32 *s2, mfU32 *d, mfU32 *o) { mfMultiplyUX( s1->b, s2->b, d->b, o->b, sizeof(mfU32)); }
void mfMultiplyU64( const mfU64 *s1, const mfU64 *s2, mfU64 *d, mfU64 *o) { mfMultiplyUX( s1->b, s2->b, d->b, o->b, sizeof(mfU64)); }
void mfMultiplyU128( const mfU128 *s1, const mfU128 *s2, mfU128 *d, mfU128 *o) { mfMultiplyUX( s1->b, s2->b, d->b, o->b, sizeof(mfU128)); }
void mfMultiplyU256( const mfU256 *s1, const mfU256 *s2, mfU256 *d, mfU256 *o) { mfMultiplyUX( s1->b, s2->b, d->b, o->b, sizeof(mfU256)); }
void mfMultiplyU512( const mfU512 *s1, const mfU512 *s2, mfU512 *d, mfU512 *o) { mfMultiplyUX( s1->b, s2->b, d->b, o->b, sizeof(mfU512)); }
void mfMultiplyU1024( const mfU1024 *s1, const mfU1024 *s2, mfU1024 *d, mfU1024 *o) { mfMultiplyUX( s1->b, s2->b, d->b, o->b, sizeof(mfU1024)); }

#pragma mark - Divide
// Divide n by d, quotient stored in q with remainder in r
// return value:
// 0 = division completed
// -1 = error, division by 0
int mfDivideUX( const mfU8 *n, const mfU8 *d, mfU8 *q, mfU8 *r, unsigned int bytes )
{
    if( mfIsZeroX(d, bytes)) return -1;

    mfU8 *left_over = (mfU8 *)malloc(bytes);
    mfU8 *shifted_quotient = (mfU8 *)malloc(bytes);
    mfU8 *bit = (mfU8 *)malloc(bytes);

    mfZeroX( bit, bytes );
    bit[0] = 1;

    mfZeroX(q, bytes);
    mfCopyX(n, left_over, bytes);
    mfCopyX(d, shifted_quotient, bytes);

    while( (shifted_quotient[bytes-1] & 0x80) == 0) {
        mfShiftLeftXBy1(shifted_quotient, bytes);
        mfShiftLeftXBy1(bit, bytes);
    }
    while( mfIsZeroX(bit, bytes) == 0 ) {
        if( mfCompareUX(left_over, shifted_quotient, bytes) != mfCompareSmaller ) {
            mfSubstractUX(left_over, shifted_quotient, left_over, bytes);
            mforX(q, bit, q, bytes);
        }
        mfShiftRightXBy1(shifted_quotient, bytes);
        mfShiftRightXBy1(bit, bytes);
    }
    mfCopyX(left_over, r, bytes);

    free( bit );
    free( shifted_quotient ) ;
    free( left_over );
    return 0;
}
int mfDivideU8( const mfU8 *n, const mfU8 *d, mfU8 *q, mfU8 *r)
{
    // Check for division by 0
    if( *d == 0 ) {
        return -1;
    }
    
    *q = *n / *d;
    *r = *n - *q;
    return 0;
}
int mfDivideU16( const mfU16 *n, const mfU16 *d, mfU16 *q, mfU16 *r)
{
    // Check for division by 0
    if( (d->l8 | d->h8) == 0 ) {
        return -1;
    }
    
    unsigned int n16 = ((unsigned int)(n->h8) << 8) + n->l8;
    unsigned int d16 = ((unsigned int)(d->h8) << 8) + d->l8;
    unsigned int q16 = n16 / d16;
    q->l8 = (unsigned char)(q16 & 0xFF);
    q->h8 = (unsigned char)(q16 >> 8);
    unsigned int r16 = n16 - q16;
    r->l8 = (unsigned char)(r16 & 0xFF);
    r->h8 = (unsigned char)(r16 >> 8);
    return 0;
}
int mfDivideU32( const mfU32 *n, const mfU32 *d, mfU32 *q, mfU32 *r)
{
    unsigned int d32 = (
                        ((unsigned int)(d->h16.h8) << 24) |
                        ((unsigned int)(d->h16.l8) << 16) |
                        ((unsigned int)(d->l16.h8) << 8) |
                        d->l16.l8);
    // Check for division by 0
    if( d32 == 0 ) return -1;
    
    unsigned int n32 = (
                        ((unsigned int)(n->h16.h8) << 24) |
                        ((unsigned int)(n->h16.l8) << 16) |
                        ((unsigned int)(n->l16.h8) << 8) |
                        n->l16.l8);
    unsigned int q32 = n32 / d32;
    q->l16.l8 = (unsigned char)(q32 & 0xFF);
    q->l16.h8 = (unsigned char)((q32 >> 8) & 0xFF);
    q->h16.l8 = (unsigned char)((q32 >> 16) & 0xFF);
    q->h16.h8 = (unsigned char)((q32 >> 24) & 0xFF);
    unsigned int r32 = n32 % d32;
    r->l16.l8 = (unsigned char)(r32 & 0xFF);
    r->l16.h8 = (unsigned char)((r32 >> 8) & 0xFF);
    r->h16.l8 = (unsigned char)((r32 >> 16) & 0xFF);
    r->h16.h8 = (unsigned char)((r32 >> 24) & 0xFF);
    return 0;
    
}
int mfDivideU64( const mfU64 *n, const mfU64 *d, mfU64 *q, mfU64 *r) { return mfDivideUX(n->b, d->b, q->b, r->b, sizeof(mfU64)); }
int mfDivideU128( const mfU128 *n, const mfU128 *d, mfU128 *q, mfU128 *r) { return mfDivideUX(n->b, d->b, q->b, r->b, sizeof(mfU128)); }
int mfDivideU256( const mfU256 *n, const mfU256 *d, mfU256 *q, mfU256 *r) { return mfDivideUX(n->b, d->b, q->b, r->b, sizeof(mfU256)); }
int mfDivideU512( const mfU512 *n, const mfU512 *d, mfU512 *q, mfU512 *r) { return mfDivideUX(n->b, d->b, q->b, r->b, sizeof(mfU512)); }
int mfDivideU1024( const mfU1024 *n, const mfU1024 *d, mfU1024 *q, mfU1024 *r) { return mfDivideUX(n->b, d->b, q->b, r->b, sizeof(mfU1024)); }

