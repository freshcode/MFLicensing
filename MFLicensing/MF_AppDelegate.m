//
//  MF_AppDelegate.m
//  MFLicensing
//  https://github.com/freshcode/MFLicensing
//
//  Public Domain
//  By Freshcode, Cutting edge Mac, iPhone & iPad software development. http://madefresh.ca/
//  Created by Dave Poirier on 2013-02-01.
//

#import "MF_AppDelegate.h"
#import "mfmathlib.h"
#import "mflicensing.h"
#import "md5.h"
#include <stdlib.h>

@implementation MF_AppDelegate

static mfLicensingVector licensingVector;
static mfLicensingPrivateKey licensingPrivateKey;

static unsigned char samplePrivateKey[] = "104879082971311758664630764208593364096202589226484812035848152338939626324659";

-(BOOL)initializeLicensingLib
{
    mfLicensingInitializeDefaultVector(&licensingVector);
    NSString *privateKey = [_licensePrivateKey stringValue];
    mfLicensingInitializePrivateKeyFromPrime(&licensingPrivateKey, (const unsigned char *)[privateKey UTF8String]);
    mfLicensingSetPrivateKey(&licensingVector, &licensingPrivateKey);

    unsigned short scramble_seed[3];
    scramble_seed[0] = [[_licenseScrambleSeed1 stringValue] intValue] & 0xFFFF;
    scramble_seed[1] = [[_licenseScrambleSeed2 stringValue] intValue] & 0xFFFF;
    scramble_seed[2] = [[_licenseScrambleSeed3 stringValue] intValue] & 0xFFFF;
    mfLicensingSetScramblingSeed(&licensingVector, scramble_seed);

    unsigned short salt_seed[3];
    salt_seed[0] = [[_licenseSaltSeed1 stringValue] intValue] & 0xFFFF;
    salt_seed[1] = [[_licenseSaltSeed2 stringValue] intValue] & 0xFFFF;
    salt_seed[2] = [[_licenseSaltSeed3 stringValue] intValue] & 0xFFFF;
    mfLicensingSetSaltSeed(&licensingVector, salt_seed);

    mfLicensingSetKeyLength(&licensingVector, [[_licenseKeyLength stringValue] intValue]);
    mfLicensingSetKeyIndexLength(&licensingVector, [[_licenseIndexBits stringValue] intValue]);
    mfLicensingSetEncodingCharacters(&licensingVector, (const unsigned char *)[[_licenseEncodingCharacters stringValue] UTF8String]);

    return TRUE;
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    mfLicensingPrivateKey licensingKey;
    mfLicensingVector licensingVector;
    
    mfLicensingInitializeDefaultVector(&licensingVector);
    mfLicensingInitializePrivateKeyFromPrime(&licensingKey, samplePrivateKey);
    
    NSLog(@"Private Key set to: %@", [self stringFromU256:&licensingKey.data]);
    
    sranddev();
    unsigned int scramblingseed1 = lrand48() % 65525;
    unsigned int scramblingseed2 = lrand48() % 65525;
    unsigned int scramblingseed3 = lrand48() % 65525;
    unsigned int saltseed1 = lrand48() % 65525;
    unsigned int saltseed2 = lrand48() % 65525;
    unsigned int saltseed3 = lrand48() % 65525;

    [_digestInput setStringValue:@"Freshcode"];
    [_digestOutput setStringValue:@"141830477712288583012728279049707186775"];
    [_generatorDigest setStringValue:@"141830477712288583012728279049707186775"];
    [_generatorIndex setStringValue:@"1"];
    [_validatorDigest setStringValue:@"141830477712288583012728279049707186775"];
    [_licenseEncodingCharacters setStringValue:@"ACDEFGHJKLMNPQRSTUVWXYZ2345679"];
    [_licensePrivateKey setStringValue:[NSString stringWithFormat:@"%s", samplePrivateKey]];
    [_licenseSaltSeed1 setStringValue:[NSString stringWithFormat:@"%u", saltseed1]];
    [_licenseSaltSeed2 setStringValue:[NSString stringWithFormat:@"%u", saltseed2]];
    [_licenseSaltSeed3 setStringValue:[NSString stringWithFormat:@"%u", saltseed3]];
    [_licenseScrambleSeed1 setStringValue:[NSString stringWithFormat:@"%u", scramblingseed1]];
    [_licenseScrambleSeed2 setStringValue:[NSString stringWithFormat:@"%u", scramblingseed2]];
    [_licenseScrambleSeed3 setStringValue:[NSString stringWithFormat:@"%u", scramblingseed3]];
    [_licenseKeyLength setStringValue:@"25"];
    [_licenseIndexBits setStringValue:@"32"];
    
    [_generatorIndex becomeFirstResponder];
}

-(IBAction)generateDigestHash:(id)sender
{
    MD5_CTX md5ctx;
    MD5_Init(&md5ctx);
    
    union {
        unsigned char c[16];
        mfU128 u128;
    } md5hash;

    NSString *stringToBeHashed = [_digestInput stringValue];
    NSData *dataToBeHashed = [stringToBeHashed dataUsingEncoding:NSUTF8StringEncoding];
    MD5_Update(&md5ctx, (void *)[dataToBeHashed bytes], [dataToBeHashed length]);
    MD5_Final(md5hash.c, &md5ctx);

    NSString *hash = [self decimalValueFromU128:&md5hash.u128];
    [_digestOutput setStringValue:hash];
}

-(IBAction)validateLicenseKey:(id)sender
{
    if( [self initializeLicensingLib] ) {
        mfLicensingDigest digest;
        if( [self setU128:&digest.md5hash fromDecimalString:[_validatorDigest stringValue]] ) {
            if( mfLicensingValidateLicense(&licensingVector, &digest, (const unsigned char *)[[_validatorLicenseKey stringValue] UTF8String]) == 1 ) {
                NSAlert *alert = [NSAlert alertWithMessageText:@"Success" defaultButton:@"OK" alternateButton:nil otherButton:nil informativeTextWithFormat:@"The license key specified is valid."];
                [alert runModal];
            } else {
                NSAlert *alert = [NSAlert alertWithMessageText:@"Failure" defaultButton:@"OK" alternateButton:nil otherButton:nil informativeTextWithFormat:@"The license key specified failed validation."];
                [alert runModal];
            }
        } else {
            NSAlert *alert = [NSAlert alertWithMessageText:@"Failure" defaultButton:@"OK" alternateButton:nil otherButton:nil informativeTextWithFormat:@"Invalid digits in digest."];
            [alert runModal];
        }
    }
}

-(IBAction)generateLicenseKey:(id)sender
{
    if( [self initializeLicensingLib] ) {
        mfLicensingDigest digest;
        if( [self setU128:&digest.md5hash fromDecimalString:[_generatorDigest stringValue]] ) {
            unsigned char *license = mfLicensingGenerateLicense(&licensingVector, &digest, [[_generatorIndex stringValue] intValue]);
            if( license != 0 ) {
                NSString *licenseKey = [NSString stringWithFormat:@"%s", license];
                [_generatorLicenseKey setStringValue:licenseKey];
            } else {
                [_generatorLicenseKey setStringValue:@"An error occured"];
            }
        } else {
            [_generatorLicenseKey setStringValue:@"Invalid digits in digest"];
        }
    }
}

-(IBAction)generatePrivateKey:(id)sender
{
    // Generate random 256-bit digit
    mfU256 data;
    srandomdev();
    data.l128.l64.l32.l16.l8 = random() & 0xFF;
    data.l128.l64.l32.l16.h8 = random() & 0xFF;
    data.l128.l64.l32.h16.l8 = random() & 0xFF;
    data.l128.l64.l32.h16.h8 = random() & 0xFF;
    data.l128.l64.h32.l16.l8 = random() & 0xFF;
    data.l128.l64.h32.l16.h8 = random() & 0xFF;
    data.l128.l64.h32.h16.l8 = random() & 0xFF;
    data.l128.l64.h32.h16.h8 = random() & 0xFF;
    data.l128.h64.l32.l16.l8 = random() & 0xFF;
    data.l128.h64.l32.l16.h8 = random() & 0xFF;
    data.l128.h64.l32.h16.l8 = random() & 0xFF;
    data.l128.h64.l32.h16.h8 = random() & 0xFF;
    data.l128.h64.h32.l16.l8 = random() & 0xFF;
    data.l128.h64.h32.l16.h8 = random() & 0xFF;
    data.l128.h64.h32.h16.l8 = random() & 0xFF;
    data.l128.h64.h32.h16.h8 = random() & 0xFF;
    data.h128.l64.l32.l16.l8 = random() & 0xFF;
    data.h128.l64.l32.l16.h8 = random() & 0xFF;
    data.h128.l64.l32.h16.l8 = random() & 0xFF;
    data.h128.l64.l32.h16.h8 = random() & 0xFF;
    data.h128.l64.h32.l16.l8 = random() & 0xFF;
    data.h128.l64.h32.l16.h8 = random() & 0xFF;
    data.h128.l64.h32.h16.l8 = random() & 0xFF;
    data.h128.l64.h32.h16.h8 = random() & 0xFF;
    data.h128.h64.l32.l16.l8 = random() & 0xFF;
    data.h128.h64.l32.l16.h8 = random() & 0xFF;
    data.h128.h64.l32.h16.l8 = random() & 0xFF;
    data.h128.h64.l32.h16.h8 = random() & 0xFF;
    data.h128.h64.h32.l16.l8 = random() & 0xFF;
    data.h128.h64.h32.l16.h8 = random() & 0xFF;
    data.h128.h64.h32.h16.l8 = random() & 0xFF;
    data.h128.h64.h32.h16.h8 = random() & 0xF;
    while( data.h128.h64.h32.h16.h8 == 0 ) {
        data.h128.h64.h32.h16.h8 = random() & 0xF;
    }

    // Get a decimal representation so we can send query to Wolfram Alpha
    NSString *dataHex =[self stringFromU256:&data];
    NSLog(@"random data initialized to %@", dataHex);
    NSString *dataDec = [self bcWithOp:[NSString stringWithFormat:@"obase=A; %@", dataHex]];

    // Build a Wolfram Alpha query to return the prime closest to the number we generated
    NSString *wolframQuery = [NSString stringWithFormat:@"http://www.wolframalpha.com/input/?i=prime+closest+to+%@", dataDec];

    // Send out the request and let the user record the value
    [[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:wolframQuery]];
}

-(NSString *)bcWithOp:(NSString *)op
{
    NSString *bcPath = [[NSBundle mainBundle] pathForResource:@"bc" ofType:@"sh"];
    // keep only the last 64 digits, vU256Add does not support overflow
    NSString *bcResult = nil;
    @try {
        NSTask *bcTask = [[NSTask alloc] init];
        [bcTask setLaunchPath:bcPath];
        
        NSArray *bcArgs = [NSArray arrayWithObjects:op, nil];
        [bcTask setArguments:bcArgs];
        
        NSPipe *bcOutput = [NSPipe pipe];
        [bcTask setStandardOutput:bcOutput];
        NSFileHandle *bcOutputFile = [bcOutput fileHandleForReading];
        
        [bcTask launch];
        
        NSData *bcOutputData = [bcOutputFile readDataToEndOfFile];
        bcResult = [[[NSString alloc] initWithData:bcOutputData encoding:NSASCIIStringEncoding] stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
        bcResult = [bcResult stringByReplacingOccurrencesOfString:@"\\\n" withString:@""];
    }
    @catch (NSException *exception) {
        NSLog(@"WARNING: could not execute bc: %@", exception);
    }
    @finally {
        return bcResult;
    }
}

-(NSString *)stringFromU256:(mfU256 *)x
{
    return [NSString stringWithFormat:@"%@%@",
            [self stringFromU128:&x->h128],
            [self stringFromU128:&x->l128]];
}

-(NSString *)stringFromU128:(mfU128 *)x
{
    return [NSString stringWithFormat:@"%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
            x->h64.h32.h16.h8,
            x->h64.h32.h16.l8,
            x->h64.h32.l16.h8,
            x->h64.h32.l16.l8,
            x->h64.l32.h16.h8,
            x->h64.l32.h16.l8,
            x->h64.l32.l16.h8,
            x->h64.l32.l16.l8,
            x->l64.h32.h16.h8,
            x->l64.h32.h16.l8,
            x->l64.h32.l16.h8,
            x->l64.h32.l16.l8,
            x->l64.l32.h16.h8,
            x->l64.l32.h16.l8,
            x->l64.l32.l16.h8,
            x->l64.l32.l16.l8 ];
}

-(NSString *)decimalValueFromU128:(mfU128 *)x
{
    mfU128 base;
    mfU128 q,r,n;
    mfZero128(&base);
    base.l64.l32.l16.l8 = 10;

    mfCopy128(x, &n);

    NSString *digits = @"";
    while( mfIsZero128(&n) == 0 ) {
        mfDivideU128(&n, &base, &q, &r);
        unsigned char c = '0'+ r.l64.l32.l16.l8;
        digits = [NSString stringWithFormat:@"%c%@", c, digits];
        mfCopy128(&q, &n);
    }
    if( [digits length] == 0 ) { digits = @"0"; }
    return digits;
}

-(BOOL)setU128:(mfU128 *)x fromDecimalString:(NSString *)s
{
    mfU128 t;
    mfZero128(&t);
    mfU128 base;
    mfU128 o;
    mfZero128(&base);
    base.l64.l32.l16.l8 = 10;

    const char *digits = [s UTF8String];
    while( *digits != 0 ) {
        if( *digits >= '0' && *digits <= '9' ) {
            mfMultiplyU128(&t, &base, &t, &o);
            t.l64.l32.l16.l8 += (*digits - '0');
            digits = &digits[1];
            if( mfIsZero128(&o) == 0 ) {
                NSLog(@"Overflow during digit scan");
                return FALSE;
            }
        } else {
            NSLog(@"Invalid digit encountered in digit scan");
            return FALSE;
        }
    }
    mfCopy128(&t, x);
    return TRUE;
}




@end
