//
//  mflicensing.h
//  MFLicensing
//  https://github.com/freshcode/MFLicensing
//
//  Public Domain
//  By Freshcode, Cutting edge Mac, iPhone & iPad software development. http://madefresh.ca/
//  Created by Dave Poirier on 2013-02-01.
//

#include "mflicensing.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

static unsigned char defaultEncodingCharacters[] = "ACDEFGHJKLMNPQRSTUVWXYZ2345679";

void randomize128UsingIntSeed( mfU128 *x, unsigned int seed );
void randomize256UsingSeed( mfU256 *x, unsigned short int seed[3] );

typedef struct {
    unsigned int encoding_base;
    unsigned int bits_in_key;
    unsigned char *codec_characters;
    unsigned char *bits_ordering;
} mfLicensingCodecParams;

void mfLicensingInitializeDefaultVector( mfLicensingVector *vector )
{
    vector->coded_chars = defaultEncodingCharacters;
    vector->key_length = 25;
    vector->index_bits = 25;
    vector->scrambling_seed[0] = 0x17B6;
    vector->scrambling_seed[1] = 0x69D0;
    vector->scrambling_seed[2] = 0x22D3;
    vector->salt_seed[0] = 0xE7DF;
    vector->salt_seed[1] = 0x7514;
    vector->salt_seed[2] = 0x45B4;
    vector->private_key = 0;
}
int mfLicensingSetPrivateKey( mfLicensingVector *vector, const mfLicensingPrivateKey *key )
{
    if((key->data.h128.h64.h32.h16.h8 == 0) ||
       ((key->data.l128.l64.l32.l16.l8 & 0x01) == 0) ) {
        vector->private_key = 0;
        return -1;
    }
    vector->private_key = key;
    return 0;
}
int mfLicensingSetEncodingCharacters( mfLicensingVector *vector, const unsigned char *characters )
{
    // check how many characters are provided
    unsigned int char_count = 0;
    vector->coded_chars = 0;
    while( characters[char_count] != 0 ) {
        char_count++;
        if( char_count >= 100 ) {
            // too many characters in string, assume an invalid pointer
            return -1;
        }
    }

    // make sure there are no duplicated characters
    unsigned int char_i = 0;
    while( char_i < char_count ) {
        unsigned char c = characters[char_i];
        unsigned int char_j = char_i + 1;
        while( char_j < char_count ) {
            if( c == characters[char_j] ) {
                // duplicated character
                return -1;
            }
            char_j++;
        }
    }
    vector->coded_chars = characters;
    return 0;
}

int mfLicensingSetKeyLength( mfLicensingVector *vector, unsigned char characters )
{
    vector->key_length = characters;
    return 0;
}
int mfLicensingSetKeyIndexLength( mfLicensingVector *vector, unsigned char bits )
{
    if( bits > 32 ) {
        return -1;
    }
    vector->index_bits = bits;
    return 0;
}
int mfLicensingSetScramblingSeed( mfLicensingVector *vector, unsigned short int seed[3])
{
    vector->scrambling_seed[0] = seed[0];
    vector->scrambling_seed[1] = seed[1];
    vector->scrambling_seed[2] = seed[2];
    return 0;
}
int mfLicensingSetSaltSeed( mfLicensingVector *vector, unsigned short int seed[3] )
{
    vector->salt_seed[0] = seed[0];
    vector->salt_seed[1] = seed[1];
    vector->salt_seed[2] = seed[2];
    return 0;
}

int mfLicensingInitializePrivateKeyFromPrime( mfLicensingPrivateKey *key, const unsigned char *decimalRepresentation )
{
    mfU256 base, x, o;
    mfZero256(&base);
    mfZero256(&x);
    base.l128.l64.l32.l16.l8 = 10;

    mfZero256(&key->data);
    while( *decimalRepresentation != 0 ) {
        unsigned char c;
        c = *decimalRepresentation;
        decimalRepresentation = &decimalRepresentation[1];
        
        if( c >= '0' && c <= '9' ) {
            x.l128.l64.l32.l16.l8 = c - '0';
            mfMultiplyU256(&key->data, &base, &key->data, &o);
            if( mfAddU256(&key->data, &x, &key->data) == 1 ) {
                return -EOVERFLOW;
            }
        } else {
            return -EINVAL;
        }
    }
    // Perform 2 validations
    // 1.) Ensure at least some bits are defined in the upper 8 bits of the key
    // 2.) Make sure the value is odd
    //
    // note: other tests could be performed but really the validity of the prime should be left
    // to the person using the library.
    if( (key->data.h128.h64.h32.h16.h8 == 0) || ((key->data.l128.l64.l32.l16.l8 & 0x01) == 0) ) {
        return -EINVAL;
    }
    return 0;
}

int mfLicensingInitializeCodecParams(mfLicensingCodecParams *codec_params, mfLicensingVector *vector)
{
    mfU256 encoded_base;
    mfU256 max_key;
    mfU256 ignored;

    if( vector->private_key == 0 || vector->coded_chars == 0 ) {
        return 0;
    }

    // Compute key encoding base
    unsigned char encoding_chars = 0;
    const unsigned char *c;
    c = vector->coded_chars;
    while( c[encoding_chars] != 0 ) {
        encoding_chars++;
        if( encoding_chars > 128 ) {
            // there really shouldn't be that many possible characters in a key code, something went wrong.
            return 0;
        }
    }
    if( encoding_chars > 100 ) {
        // too many encoding characters, pointer must be invalid
        return 0;
    }
    codec_params->encoding_base = encoding_chars;



    // Compute total key length in bits
    // Step 1: find out the largest value that could be created using the key length and encoding chars
    mfZero256(&encoded_base);
    encoded_base.l128.l64.l32.l16.l8 = encoding_chars;
    mfZero256(&max_key);
    max_key.l128.l64.l32.l16.l8 = 1;
    unsigned char encoded_key_length = vector->key_length;
    if( encoded_key_length == 0 ) {
        // cannot generate a key of 0 length..
        return 0;
    }
    while( encoded_key_length-- ) {
        mfMultiplyU256(&max_key, &encoded_base, &max_key, &ignored);
        // while we will not use the data in "ignored", we should check for overflow
        if( mfIsZero256(&ignored) == 0 && encoded_key_length > 0 ) {
            // the requested key would contain more than 256-bit of data, unsupported.
            return 0;
        }
    }
    // Step 2: find out how many bits of data can be reliably encoded
    unsigned char binary_key_length = 0;
    while( mfIsZero256(&max_key) == 0 ) {
        binary_key_length++;
        mfShiftRight256By1(&max_key);
    }
    binary_key_length--;
    codec_params->bits_in_key = binary_key_length;



    // Determine encoding characters weight and bits ordering
    // Step 1: scramble the encoding characters
    seed48(vector->scrambling_seed);
    unsigned char *scrambled_encoding_char = malloc(encoding_chars);
    if( scrambled_encoding_char == 0 ) return 0; // failed malloc
    unsigned char scrambled_chars = 0;
    while( scrambled_chars < encoding_chars ) {
        scrambled_encoding_char[scrambled_chars++] = 0;
    }
    scrambled_chars = 0;
    while( scrambled_chars < encoding_chars ) {
        unsigned int scrambled_char = lrand48() % encoding_chars;
        if( scrambled_encoding_char[scrambled_char] == 0 ) {
            scrambled_encoding_char[scrambled_char] = vector->coded_chars[scrambled_chars];
            scrambled_chars++;
        }
    }
    codec_params->codec_characters = scrambled_encoding_char;



    // Step 2: scramble the bits ordering
    unsigned char *bits = malloc(binary_key_length);
    if( bits == 0 ) { free(codec_params->codec_characters); return 0; }

    unsigned char *bit_indexes = malloc(binary_key_length);
    if( bit_indexes == 0 ) { free(bits); free(codec_params->codec_characters); return 0; }

    for( int i=0; i < binary_key_length; i++ ) { bits[i] = 0; bit_indexes[i] = 0; }
    unsigned char scrambled_bits = 0;
    while (scrambled_bits < binary_key_length) {
        unsigned int rnd_i = lrand48() % binary_key_length;
        if( bits[rnd_i] == 0 ) {
            // that bit index is still free
            bit_indexes[scrambled_bits] = rnd_i;
            bits[rnd_i] = 1;
            scrambled_bits++;
        }
    }
    free(bits);
    codec_params->bits_ordering = bit_indexes;



    // Can the index be stored entirely with at least some validator bits?
    if( vector->index_bits >= codec_params->bits_in_key) {
        free(codec_params->bits_ordering);
        free(codec_params->codec_characters);
        return 0;
    }

    return 1;
}

unsigned char* mfLicensingGenerateLicense( mfLicensingVector *vector, mfLicensingDigest *digest, unsigned int index )
{
//    fprintf(stderr, "generating license for index %u\n", index);
//    fprintf(stderr, "vector:\n\tcoded chars: %s\n\tindex bits: %u\n\tkey length: %u\n\tsalt: %u %u %u\n\tscrambling: %u %u %u\n",
//            vector->coded_chars,
//            vector->index_bits,
//            vector->key_length,
//            vector->salt_seed[0],
//            vector->salt_seed[1],
//            vector->salt_seed[2],
//            vector->scrambling_seed[0],
//            vector->scrambling_seed[1],
//            vector->scrambling_seed[2]);
//    fprintf(stderr, "private key:\n\t%02X%02X%02X%02X %02X%02X%02X%02X\n\t%02X%02X%02X%02X %02X%02X%02X%02X\n\t%02X%02X%02X%02X %02X%02X%02X%02X\n\t%02X%02X%02X%02X %02X%02X%02X%02X\n",
//            vector->private_key->data.b[0],
//            vector->private_key->data.b[1],
//            vector->private_key->data.b[2],
//            vector->private_key->data.b[3],
//            vector->private_key->data.b[4],
//            vector->private_key->data.b[5],
//            vector->private_key->data.b[6],
//            vector->private_key->data.b[7],
//            vector->private_key->data.b[8],
//            vector->private_key->data.b[9],
//            vector->private_key->data.b[10],
//            vector->private_key->data.b[11],
//            vector->private_key->data.b[12],
//            vector->private_key->data.b[13],
//            vector->private_key->data.b[14],
//            vector->private_key->data.b[15],
//            vector->private_key->data.b[16],
//            vector->private_key->data.b[17],
//            vector->private_key->data.b[18],
//            vector->private_key->data.b[19],
//            vector->private_key->data.b[20],
//            vector->private_key->data.b[21],
//            vector->private_key->data.b[22],
//            vector->private_key->data.b[23],
//            vector->private_key->data.b[24],
//            vector->private_key->data.b[25],
//            vector->private_key->data.b[26],
//            vector->private_key->data.b[27],
//            vector->private_key->data.b[28],
//            vector->private_key->data.b[29],
//            vector->private_key->data.b[30],
//            vector->private_key->data.b[31]);
//    fprintf(stderr, "digest:\n\t%02X%02X%02X%02X %02X%02X%02X%02X\n\t%02X%02X%02X%02X %02X%02X%02X%02X\n",
//            digest->md5hash.b[0],
//            digest->md5hash.b[1],
//            digest->md5hash.b[2],
//            digest->md5hash.b[3],
//            digest->md5hash.b[4],
//            digest->md5hash.b[5],
//            digest->md5hash.b[6],
//            digest->md5hash.b[7],
//            digest->md5hash.b[8],
//            digest->md5hash.b[9],
//            digest->md5hash.b[10],
//            digest->md5hash.b[11],
//            digest->md5hash.b[12],
//            digest->md5hash.b[13],
//            digest->md5hash.b[14],
//            digest->md5hash.b[15] );

    mfU256 validator; mfZero256(&validator);
    mfU256 binary_key; mfZero256(&binary_key);
    unsigned char *bits = 0;
    unsigned char *encoded_key = 0;

    // Retrieve codec parameters
    mfLicensingCodecParams codec_params;
    if( mfLicensingInitializeCodecParams(&codec_params, vector) == 0 )
    {
        // some codec parameters couldn't be validated
        return 0;
    }
    // Pre-allocate all needed memory blocks
    bits = malloc(codec_params.bits_in_key);
    encoded_key = malloc(vector->key_length+1); // +1 for null terminator
    if( bits == 0 || encoded_key == 0 ) {
        if( bits != 0 ) free(bits);
        if( encoded_key != 0 ) free( encoded_key );
        free( codec_params.bits_ordering );
        free( codec_params.codec_characters );
        return 0;
    }

    // Compute the validator bits for the index and digest
    {
        mfU256 salt;
        mfU128 index_block;
        mfU256 mixed_block;
        mfU512 pivot;
        mfU256 ignored;
        
        // Compute 128-bit representation of the index and 256-bit salt
        randomize128UsingIntSeed(&index_block, index);
        randomize256UsingSeed(&salt, vector->salt_seed);
        
        // Multiply the 128-bit index representation by the digest, produces 256-bit result
        mfMultiplyU128(&digest->md5hash, &index_block, &mixed_block.l128, &mixed_block.h128);
        
        // Multiply previous result with 256-bit salt, produces 512-bit result
        mfMultiplyU256(&salt, &mixed_block, &pivot.l256, &pivot.h256);
        
        // Take most significant 256-bits of previous result, divide by the private key
        mfDivideU256(&pivot.h256, &vector->private_key->data, &ignored, &validator);
        // Remainder is the "validator" for the key
    }

    // Compute the binary representation of the key
    {
        // Store the index bits
        unsigned char index_bits = vector->index_bits;
        unsigned int bit_i = 0;
        unsigned int rnd_i;
        
        while( bit_i < index_bits ) {
            rnd_i = codec_params.bits_ordering[bit_i];
            bits[rnd_i] = index & 0x01;
            index = index >> 1;
            bit_i ++;
        }
        // Ensure the index specified did fit entirely
        if( index == 0 ) {
            
            // Store as many validator bits as key will allow
            while( bit_i < codec_params.bits_in_key ) {
                rnd_i = codec_params.bits_ordering[bit_i];
                bits[rnd_i] = validator.l128.l64.l32.l16.l8 & 0x01;
                mfShiftRight256By1(&validator);
                bit_i ++;
            }
            
            // Create a flat binary representation of the scrambled bits
            mfZero256(&binary_key);
            bit_i = 0;
            while( bit_i < codec_params.bits_in_key ) {
                mfShiftLeft256By1(&binary_key);
                binary_key.l128.l64.l32.l16.l8 = binary_key.l128.l64.l32.l16.l8 | bits[bit_i];
                bit_i ++;
            }
        }
        // make sure the binary key contains data
        if( mfIsZero256(&binary_key) == 1 ) {
            free( bits );
            free( encoded_key );
            free( codec_params.bits_ordering );
            free( codec_params.codec_characters );
            return 0;
        }
    }

    // Encode the binary key using the encoding characters
    // binary_key contains the scrambled bits
    {
        mfU256 left_to_encode;
        mfU256 remainder;
        mfU256 encoded_base;

        mfZero256(&encoded_base);
        encoded_base.l128.l64.l32.l16.l8 = codec_params.encoding_base;

        // Successively divide the binary key by the encoding base to get the encoded character indexes
        unsigned int coded_key_i = 0;
        while( coded_key_i < vector->key_length ) {
            mfDivideU256(&binary_key, &encoded_base, &left_to_encode, &remainder);
            encoded_key[coded_key_i] = codec_params.codec_characters[remainder.l128.l64.l32.l16.l8];
            mfCopy256(&left_to_encode, &binary_key);
            coded_key_i++;
        }
        encoded_key[coded_key_i] = 0; // null terminator for the string

        // binary_key should be 0
        if( mfIsZero256(&binary_key) == 0 ) {
            // something went wrong...
            free( bits );
            free( encoded_key );
            free( codec_params.bits_ordering );
            free( codec_params.codec_characters );
            return 0;
        }
    }

    // All done, clean up and return
    free( bits );
    free( codec_params.bits_ordering );
    free( codec_params.codec_characters );
    return encoded_key;
}

int mfLicensingValidateLicense( mfLicensingVector *vector, mfLicensingDigest *digest, const unsigned char *license)
{
    mfU256 binary_key; mfZero256(&binary_key);
    unsigned int index = 0;
    unsigned char *bits = 0;
    unsigned char *expected_license = 0;

    // Retrieve codec parameters
    mfLicensingCodecParams codec_params;
    if( mfLicensingInitializeCodecParams(&codec_params, vector) == 0 )
    {
        // some codec parameters couldn't be validated
        return 0;
    }
    // Pre-allocate required memory blocks
    bits = malloc( codec_params.bits_in_key );
    if( bits == 0 ) {
        free( codec_params.bits_ordering );
        free( codec_params.codec_characters );
        return 0;
    }

    // Compute the binary equivalent for the license
    {
        // Find the end of the license key, process it in reverse order
        unsigned int coded_key_i = 0;
        while( license[coded_key_i] != 0 ) {
            if( coded_key_i > 100 ) {
                // the license key should not be that long, most likely invalid pointer or data
                break;
            }
            coded_key_i++;
        }
        if( coded_key_i == vector->key_length ) {
            mfU256 encoded_base; mfZero256(&encoded_base);
            mfU256 extended_weight; mfZero256(&extended_weight);
            
            encoded_base.l128.l64.l32.l16.l8 = codec_params.encoding_base;
            
            while( coded_key_i-- ) {
                // retrieve next character to decode
                unsigned char coded_char = license[coded_key_i];
                // find the weight of the character
                unsigned char weight = 0;
                while( weight < codec_params.encoding_base ) {
                    if( coded_char == codec_params.codec_characters[weight] ) {
                        break;  // character match, weight identified
                    }
                    weight++;
                }
                if( weight < codec_params.encoding_base ) {
                    extended_weight.l128.l64.l32.l16.l8 = weight;

                    mfU256 temp, overflow;
                    mfMultiplyU256(&binary_key, &encoded_base, &temp, &overflow);
                    if((mfIsZero256(&overflow) == 0) ||
                       (mfAddU256(&temp, &extended_weight, &binary_key) == 1) ) {
                        // resulting binary key is larger than we can support.
                        mfZero256(&binary_key);
                        break;
                    }                    
                } else {
                    // Invalid character detected
                    mfZero256(&binary_key);
                    break;
                }
            }
        }
        if( mfIsZero256(&binary_key) ) {
            free( codec_params.bits_ordering );
            free( codec_params.codec_characters );
            return 0;
        }
    }
    
    // Explode the binary key into bits
    {
        unsigned int bit_i = codec_params.bits_in_key;
        while( bit_i-- ) {
            bits[ bit_i ] = binary_key.l128.l64.l32.l16.l8 & 0x01;
            mfShiftRight256By1(&binary_key);
        }
    }

    // Retrieve the index from the exploded bits
    {
        unsigned int bit_i = vector->index_bits;
        unsigned int rnd_i;
        while( bit_i-- ) {
            rnd_i = codec_params.bits_ordering[ bit_i ];
            index = (index << 1) | bits[rnd_i];
        }
    }

    // Generate what would be the expected license for the given digest and the index decoded
    {
        expected_license = mfLicensingGenerateLicense(vector, digest, index);
    }

    // Free up allocated resources except the expected license
    free( bits );
    free( codec_params.bits_ordering );
    free( codec_params.codec_characters );
    if( expected_license == 0 ) {
        return 0;
    }

    // Compare the two strings
    {
        unsigned int coded_key_i = vector->key_length;
        while( coded_key_i-- ) {
            if( expected_license[coded_key_i] != license[coded_key_i] ) {
                free( expected_license );
                return 0;
            }
        }
    }
    // License is matching the expected value
    free( expected_license );

    return 1;
}

void randomize128UsingIntSeed( mfU128 *x, unsigned int seed )
{
    srand48( seed );
    *((unsigned int *)&x->l64.l32) = (unsigned int)(lrand48() & 0xFFFFFFFF);
    *((unsigned int *)&x->l64.h32) = (unsigned int)(lrand48() & 0xFFFFFFFF);
    *((unsigned int *)&x->h64.l32) = (unsigned int)(lrand48() & 0xFFFFFFFF);
    *((unsigned int *)&x->h64.h32) = (unsigned int)(lrand48() & 0xFFFFFFFF);
}
void randomize256UsingSeed( mfU256 *x, unsigned short int seed[3] )
{
    seed48( seed );
    *((unsigned int *)&x->l128.l64.l32) = (unsigned int)(lrand48() & 0xFFFFFFFF);
    *((unsigned int *)&x->l128.l64.h32) = (unsigned int)(lrand48() & 0xFFFFFFFF);
    *((unsigned int *)&x->l128.h64.l32) = (unsigned int)(lrand48() & 0xFFFFFFFF);
    *((unsigned int *)&x->l128.h64.h32) = (unsigned int)(lrand48() & 0xFFFFFFFF);
    *((unsigned int *)&x->h128.l64.l32) = (unsigned int)(lrand48() & 0xFFFFFFFF);
    *((unsigned int *)&x->h128.l64.h32) = (unsigned int)(lrand48() & 0xFFFFFFFF);
    *((unsigned int *)&x->h128.h64.l32) = (unsigned int)(lrand48() & 0xFFFFFFFF);
    *((unsigned int *)&x->h128.h64.h32) = (unsigned int)(lrand48() & 0xFFFFFFFF);
}