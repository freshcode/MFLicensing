//
//  MF_AppDelegate.h
//  MFLicensing
//  https://github.com/freshcode/MFLicensing
//
//  Public Domain
//  By Freshcode, Cutting edge Mac, iPhone & iPad software development. http://madefresh.ca/
//  Created by Dave Poirier on 2013-02-01.
//

#import <Cocoa/Cocoa.h>

@interface MF_AppDelegate : NSObject <NSApplicationDelegate>

@property (assign) IBOutlet NSWindow *window;
@property (assign) IBOutlet NSTextField *digestInput;
@property (assign) IBOutlet NSTextField *digestOutput;
@property (assign) IBOutlet NSTextField *licenseScrambleSeed1;
@property (assign) IBOutlet NSTextField *licenseScrambleSeed2;
@property (assign) IBOutlet NSTextField *licenseScrambleSeed3;
@property (assign) IBOutlet NSTextField *licenseSaltSeed1;
@property (assign) IBOutlet NSTextField *licenseSaltSeed2;
@property (assign) IBOutlet NSTextField *licenseSaltSeed3;
@property (assign) IBOutlet NSTextField *licenseEncodingCharacters;
@property (assign) IBOutlet NSTextField *licensePrivateKey;
@property (assign) IBOutlet NSTextField *licenseKeyLength;
@property (assign) IBOutlet NSTextField *licenseIndexBits;
@property (assign) IBOutlet NSTextField *generatorDigest;
@property (assign) IBOutlet NSTextField *generatorIndex;
@property (assign) IBOutlet NSTextField *generatorLicenseKey;
@property (assign) IBOutlet NSTextField *validatorLicenseKey;
@property (assign) IBOutlet NSTextField *validatorDigest;

@end
