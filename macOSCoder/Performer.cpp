//
//  Performer.cpp
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

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "Performer.hpp"
using namespace std;


BasePerformer::BasePerformer(const std::string name)
: name_(name)
{
    
}

BasePerformer::~BasePerformer()
{
    
}

const StringList& BasePerformer::getMethods() const
{
    return methods_;
}

void BasePerformer::getOutput(std::string& output)
{
    output = log_.str();
}

void BasePerformer::setDirectory(const std::string& filePath)
{
    filePath_ = filePath;
}

CodeJamPerformer::CodeJamPerformer(const std::string name)
: BasePerformer(name)
{
    
}

CodeJamPerformer::~CodeJamPerformer()
{
    
}


int CodeJamPerformer::perform(const std::string& fileName, const StringList& arguments)
{
    log_.clear();
    log_.str("");
    log_ << "\n\n ====================== CodeJamPerform::" << name_ << " " << fileName << " =====================" << endl;
    
    ifstream ins(filePath_ + fileName);
    if (!ins.is_open())
    {
        log_ << "Could not open " << filePath_ + fileName << ", errno: " << errno << endl;
        return -1;
    }
    int retval = 0;
    size_t numTests = 0;
    ins >> numTests;
    if (numTests > 0 && numTests <= 10000)
    {
        log_.clear();
        log_.str("");
        log_ << "Going to read and run " << numTests << " test cases." << endl;
        for (size_t tcnum = 1; tcnum <= numTests; ++tcnum)
        {
            vector<IntList> testLines;
            cjOutput_ = "<undefined>";
            log_ << "\n====================== " << name_ << "::" << fileName << " =====================" << endl;
            cjSetup(ins, testLines);
            retval = cjPerform(fileName, testLines);
            log_ << "Case #" << tcnum << ": " << cjOutput_ << endl;
            log_ << "========================   DONE   ===================" << endl;
        }
    }
    else
    {
        log_ << "Error: " << numTests << " number of tests invalid" << endl;
        retval = -2;
    }
    
    ins.close();
    return retval;
}

int CodeJamPerformer::cjPerform(const std::string& method, const std::vector<IntList>& lines)
{
    log_ << "Base cjPerform " << method << " with " << lines.size() << " lines" << endl;
    return -1;
}
