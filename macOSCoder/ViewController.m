//
//  ViewController.m
//  macOSCoder
//
//  Copyright © 2016 Andy Warner. All rights reserved.
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
//    Unless required by applicable law or agreed to in writing, software distributed
//    under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
//    CONDITIONS OF ANY KIND, either express or implied. See the License for the
//    specific language governing permissions and limitations under the License.
//

#import "CPlusPlusBridge.h"
#import "ViewController.h"


@interface ViewController ()

@property (weak) IBOutlet NSTextField *uiArgc;
@property (weak) IBOutlet NSTextField *uiArgv;
@property (weak) IBOutlet NSPopUpButton *uiMethods;
@property (weak) IBOutlet NSPopUpButton *uiClasses;

@property (nonatomic) NSArray<NSString *> *arguments;
@property (unsafe_unretained) IBOutlet NSTextView *console;

@property CPlusPlusBridge *cppBridge;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    // Do any additional setup after loading the view.
    [self.uiArgv setDelegate:self];
    [self.uiClasses removeAllItems];
    [self.uiMethods removeAllItems];
    
    self.cppBridge = [[CPlusPlusBridge alloc] init];
    [self.cppBridge setup];
    NSArray<NSString *> *arrClasses = [self.cppBridge getClasses];
    [self.uiClasses addItemsWithTitles:arrClasses];
    
    [self selectClass:_uiMethods];
}

- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];
    
    // Update the view, if already loaded.
}

- (IBAction)selectClass:(NSPopUpButton *)sender {
    NSString *className = [self.uiClasses titleOfSelectedItem];
    NSArray<NSString *> *arrMethods = [self.cppBridge getMethodsForClass:className];
    NSLog(@"Got %lu methods for class %@", [arrMethods count], className);
    
    [self.uiMethods removeAllItems];
    [self.uiMethods addItemsWithTitles:arrMethods];
}

- (IBAction)doRun:(NSButton *)sender {
    NSString *className = [self.uiClasses titleOfSelectedItem];
    NSString *method = [self.uiMethods titleOfSelectedItem];
    NSLog(@"doRun called with class %@ method %@", className, method);
    NSString *result = [self.cppBridge callRoutine:method fromClass:className withArguments:self.arguments];
    [self write:result];
    NSLog(@"Class %@ Method %@ finished", className, method);
}

- (IBAction)doCancel:(NSButton *)sender {
    NSLog(@"doCancel called");
    [[NSApplication sharedApplication] terminate:self];
}

- (void)controlTextDidBeginEditing:(NSNotification *)obj {
    
}

- (void)controlTextDidChange:(NSNotification *)obj {
    NSString *argv = [[self uiArgv] stringValue];
    //NSLog(@"Edit argv %@", argv);
    self.arguments = [argv componentsSeparatedByString:@" "];
    NSUInteger argCount = [self.arguments count];
    [[self uiArgc] setIntegerValue:argCount];
}

- (void)controlTextDidEndEditing:(NSNotification *)obj {
    //NSLog(@"End edit argv");
}

- (void)write:(NSString *)message {
    NSAttributedString *attrStr = [[NSAttributedString alloc] initWithString:(message ? message : @"<nil>")];
    [[self.console textStorage] appendAttributedString:attrStr];
    [[self.console textStorage] setFont:[NSFont fontWithName:@"Menlo Regular" size:11.0]];
    [self.console scrollToEndOfDocument:nil];
}

@end
