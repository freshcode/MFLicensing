//
//  mflicensing.h
//  MFLicensing
//  https://github.com/freshcode/MFLicensing
//
//
//  Software License Code Generation and Validation Library.
//
//  This library was specifically designed to be extremely flexible in the format of the
//  software license keys generated while providing a robust key generation algorithm.
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
//  While not extensively optimized for speed, the routines should be fairly fast.  Most
//  of the optimizations would have to be performed on the dependent libraries.
//
//  Dependencies
//  ------------
//  MFMathLib @ http://github.com/freshcode/MFMathLib
//  (also available as a CocoaPods @ http://cocoapods.org )



#ifndef MFLicensing_mflicensing_h
#define MFLicensing_mflicensing_h

#include "mfmathlib.h"

typedef struct {
    mfU256 data;
} mfLicensingPrivateKey;

typedef struct {
    mfU128 md5hash;
} mfLicensingDigest;

typedef struct {
    const mfLicensingPrivateKey *privateKey;
    const unsigned char *coded_chars;
    unsigned short int scrambling_seed[3];
    unsigned short int salt_seed[3];
    unsigned char key_length;
    unsigned char index_bits;
} mfLicensingVector;

void mfLicensingInitializeDefaultVector( mfLicensingVector *vector );
void mfLicensingSetPrivateKey( mfLicensingVector *vector, const mfLicensingPrivateKey *key );
void mfLicensingSetEncodingCharacters( mfLicensingVector *vector, const unsigned char *characters );
void mfLicensingSetKeyLength( mfLicensingVector *vector, unsigned char characters );
void mfLicensingSetKeyIndexLength( mfLicensingVector *vector, unsigned char bits );
void mfLicensingSetScramblingSeed( mfLicensingVector *vector, unsigned short int seed[3]);
void mfLicensingSetSaltSeed( mfLicensingVector *vector, unsigned short int seed[3]);

int mfLicensingInitializePrivateKeyFromPrime( mfLicensingPrivateKey *key, const unsigned char *decimalRepresentation );

unsigned char* mfLicensingGenerateLicense( mfLicensingVector *vector, mfLicensingDigest *digest, unsigned int index);
int mfLicensingValidateLicense( mfLicensingVector *vector, mfLicensingDigest *digest, const unsigned char *license);

#endif
