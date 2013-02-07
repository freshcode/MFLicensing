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

// 256-bit representation of the private key; large prime number
//--------------------------------------------------------------
// see the project website for procedure on how to generate a 256-bit prime
typedef struct {
    mfU256 data;
} mfLicensingPrivateKey;

// 128-bit license key digest; usually a MD5 hash
//-----------------------------------------------
typedef struct {
    mfU128 md5hash;
} mfLicensingDigest;

// Licensing Vector structure, containing the parameters for the key generation
//-----------------------------------------------------------------------------
// private_key: a pointer to a mfLicensingPrivateKey
// coded_char: a pointer to a null-terminated string of characters to use for encoding
// scrambling_seed: array of 3 16-bit numbers, used as seed for scrambling
// salt_seed: array of 3 16-bit numbers, used to generate intermediate multiplier
// key_length: number of characters contained in the final license key
// index_bits: number of bits to reserve in the final key for the key index
typedef struct {
    const mfLicensingPrivateKey *private_key;
    const unsigned char *coded_chars;
    unsigned short int scrambling_seed[3];
    unsigned short int salt_seed[3];
    unsigned char key_length;
    unsigned char index_bits;
} mfLicensingVector;

// mfLicensingInitializeDefaultVector
//-----------------------------------
// Set the default values for the specified licensing vector
//
// Completely optional if all the value sof mfLicensingVector are set manually
void mfLicensingInitializeDefaultVector( mfLicensingVector *vector );

// mfLicensingSetPrivateKey
//-------------------------
// Sets vector->private_key to the pointer specified
//
// This performs some minimal validation on the key.  The caller should ensure the key used
// is a 256-bit prime number.  The key specified will be rejected if the top 8 bits of the
// key are 0 or if the key is an even number.
int mfLicensingSetPrivateKey( mfLicensingVector *vector, const mfLicensingPrivateKey *key );

// mfLicensingSetEncodingCharacters
//---------------------------------
// Sets vector->coded_car to the pointer specified
//
// This performs some minimal validation on the encoding characters.  The encoding characters
// should not repeat and the entire string should contain fewer than 100 unique characters.
//
// There is no need to scramble the characters, as they will be scrambled automatically during
// key generation and key validation.
int mfLicensingSetEncodingCharacters( mfLicensingVector *vector, const unsigned char *characters );

// mfLicensingSetKeyLength
//------------------------
// Sets vector->key_length to the value specified
//
// The number of "characters" specified in this function will dictate how many characters will
// be included in the final generated key.  The caller should ensure this number is large enough
// to accomodate the number of index bits and sufficient validator bits to reasonably assert
// the validity of the user provided key during validation.
//
// See documentation on our website for more information.
int mfLicensingSetKeyLength( mfLicensingVector *vector, unsigned char characters );

// mfLicensingSetKeyIndexLength
//-----------------------------
// Sets vector->index_bits to the value specified
//
// This value indicates how many bits will be reserved in the final generated key to storing
// the index.  Incidentally, it also dictates how many keys can be generated for the vector
// and digest specified.
//
// This value should not exceed 32.
int mfLicensingSetKeyIndexLength( mfLicensingVector *vector, unsigned char bits );

// mfLicensingSetScramblingSeed
//-----------------------------
// Sets vector->scrambling_seed to the 3 16-bit values specified
//
// The scrambling seeds are used to set the initial starting value of the random number
// generator prior to scrambling the encoding characters and the bits of the generated key.
int mfLicensingSetScramblingSeed( mfLicensingVector *vector, unsigned short int seed[3]);

// mfLicensingSetSaltSeed
//-----------------------
// Sets vector->salt_seed to the 3 16-bit values specified
//
// The salt seeds are used to set the initial starting value of the random number
// generator prior to generating the intermediate multiplier.
//
// see documentation on our website or the code in mflicensing.c to for more information.
int mfLicensingSetSaltSeed( mfLicensingVector *vector, unsigned short int seed[3]);

// mfLicensingInitializePrivateKeyFromPrime
//-----------------------------------------
// Loads the decimal string into the mfLicensingPrivateKey specified.
//
// The private key should be a 256-bit prime number.  See our documentation on our website
// for tips on how to generate such a large prime number.
int mfLicensingInitializePrivateKeyFromPrime( mfLicensingPrivateKey *key, const unsigned char *decimalRepresentation );

// mfLicensingGenerateLicense
//---------------------------
// This function generates a license key based on the provided vector, digest and index.
//
// Returns 0 if an error occured (insufficient index bits, index too large, etc)
unsigned char* mfLicensingGenerateLicense( mfLicensingVector *vector, mfLicensingDigest *digest, unsigned int index);

// mfLicensingValidateLicense
//---------------------------
// This function validates a license key against a provided vector and digest.
//
// Returns 1 if the key is valid, 0 otherwise.
int mfLicensingValidateLicense( mfLicensingVector *vector, mfLicensingDigest *digest, const unsigned char *license);

#endif
