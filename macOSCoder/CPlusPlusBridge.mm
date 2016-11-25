//
//  CPlusPlusBridge.m
//  macOSCoder
//
//  Copyright © 2016 Andy Warner.
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
#import "KnapSack.hpp"
#import "LazySpellingBee.hpp"
#import "MakeDictionary.hpp"
#import "NotSoRandom.hpp"
#import "RobotRockBand.hpp"
#import "RunFactory.hpp"
#if 0
#import "MaximizeCredit.hpp"
#import "PermuteString.hpp"
#import "SortString.hpp"
#endif

@implementation CPlusPlusBridge

- (NSString *)callRoutine:(NSString *)routine fromClass:(NSString *)className withArguments:(NSArray<NSString *> *)arguments {
    NSUInteger classLen = [className length];
    NSUInteger routineLen = [routine length];
    NSString *retval;
    if (classLen == 0) {
        return retval;
    }
    char chClass[classLen + 1];
    char chRoutine[routineLen + 1];
    if (![className getCString:chClass maxLength:classLen + 1 encoding:NSASCIIStringEncoding]) {
        NSLog(@"Failed to convert class name to ascii");
        return retval;
    }
    if (![routine getCString:chRoutine maxLength:routineLen + 1 encoding:NSASCIIStringEncoding]) {
        NSLog(@"Failed to convert method name to ascii");
        return retval;
    }
    NSString *filePath = [[NSBundle mainBundle] pathForResource:@"words" ofType:@"txt"];
    filePath = [filePath stringByDeletingLastPathComponent];
//    filePath = [filePath substringToIndex:filePath.length - 10];
//    filePath = [filePath stringByAppendingPathComponent:@"data"];

    NSUInteger fpLen = [filePath length] + 2;
    char chFilePath[fpLen];
    [filePath getCString:chFilePath maxLength:fpLen encoding:NSASCIIStringEncoding];

    StringList args;
    for (NSString *argument in arguments) {
        NSUInteger argLen = [argument length] + 1;
        char chArg[argLen];
        [argument getCString:chArg maxLength:argLen encoding:NSASCIIStringEncoding];
        args.push_back(chArg);
    }
    
    RunFactory* factory = RunFactory::instance();
    factory->setDirectory(chFilePath);
    factory->runMethod(chClass, chRoutine, args);
    const std::string& methodOutStr = factory->getOutput();
    retval = [NSString stringWithCString:methodOutStr.c_str() encoding:NSASCIIStringEncoding];
    return retval;
}

- (NSArray<NSString *> *)getClasses {
    RunFactory* factory = RunFactory::instance();
    const StringList& methodsList = factory->getClasses();
    
    NSMutableArray<NSString *> *arrMethods = [[NSMutableArray alloc] initWithCapacity:methodsList.size()];
    for (int idx = 0; idx < methodsList.size(); ++idx) {
        NSString *strMethod = [NSString stringWithCString:methodsList[idx].c_str() encoding:NSASCIIStringEncoding];
        [arrMethods addObject:strMethod];
    }
    
    return arrMethods;
}

- (NSArray<NSString *> *)getMethodsForClass:(NSString *)className {
    NSUInteger classLen = [className length];
    if (classLen == 0) {
        return  nil;
    }
    char chClass[classLen + 1];
    if (![className getCString:chClass maxLength:classLen + 1 encoding:NSASCIIStringEncoding]) {
        NSLog(@"Failed to convert class name to ascii");
        return nil;
    }
    
    RunFactory* factory = RunFactory::instance();
    const StringList& methodsList = factory->getClassMethods(chClass);
    NSMutableArray<NSString *> *arrMethods = [[NSMutableArray alloc] initWithCapacity:methodsList.size()];
    for (int idx = 0; idx < methodsList.size(); ++idx) {
        NSString *strMethod = [NSString stringWithCString:methodsList[idx].c_str() encoding:NSASCIIStringEncoding];
        [arrMethods addObject:strMethod];
    }
    
    return arrMethods;
}

- (void)setup {
    RunFactory* factory = RunFactory::instance();

    factory->addPerformer("Knap Sack", new KnapSack);
    factory->addPerformer("Lazy Spelling Bee", new LazySpellingBee);
    factory->addPerformer("MakeDictionary", new MakeDictionary);
    factory->addPerformer("Not So Random", new NotSoRandom);
    factory->addPerformer("Robot Rock Band", new RobotRockBand);
#if 0
    factory->addPerformer("MaximizeCredit", new MaximizeCredit);
    MakeDictionary* makeDictionary = new MakeDictionary;
    factory->addPerformer("MakeDictionary", makeDictionary);
    factory->addPerformer("PermuteString", new PermuteString(makeDictionary->getDictionary()));
    factory->addPerformer("SortString", new SortString);
#endif
}

@end
