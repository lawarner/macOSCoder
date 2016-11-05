#pragma once
//
//  RunFactory.hpp
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

#include <string>
#include <vector>

typedef std::vector<int> IntList;
typedef std::vector<std::string> StringList;

// Forward reference
class RunFactoryImpl;
class Performer;


class RunFactory
{
private:
    RunFactory();
    virtual ~RunFactory();

    void loadDictionary();
    void unloadDictionary();

public:
    static RunFactory* instance();

    static bool addPerformer(const std::string& method, Performer* performer);
    
    static const StringList& getClasses();
    static const StringList& getClassMethods(const std::string& className);
    static const std::string& getOutput();
    
    static bool isWord(const std::string& text);
    
    static int runMethod(const std::string& className, const std::string& method, const StringList& arguments);
    
    static void setDirectory(const std::string& filePath);
    
private:
    RunFactoryImpl& impl_;
};
