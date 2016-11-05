//
//  CPlusPlusBridge.h
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

#ifndef CPlusPlusBridge_h
#define CPlusPlusBridge_h

#import <Foundation/Foundation.h>

@interface CPlusPlusBridge : NSObject

- (NSString *)callRoutine:(NSString *)routine fromClass:(NSString *)className withArguments:(NSArray<NSString *> *)arguments;

- (NSArray<NSString *> *)getClasses;

- (NSArray<NSString *> *)getMethodsForClass:(NSString *)className;

- (void)setup;

@end

#endif /* CPlusPlusBridge_h */
