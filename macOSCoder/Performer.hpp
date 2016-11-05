#pragma once
//
//  Performer.hpp
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
#include <sstream>
#include "RunFactory.hpp"


class Performer
{
public:
    // virtual Performer* createInstance() = 0;
    
    virtual const StringList& getMethods() const = 0;
    virtual int perform(const std::string& method, const StringList& arguments) = 0;
    virtual void getOutput(std::string& output) = 0;
    
    virtual void setDirectory(const std::string& filePath) = 0;
};

class BasePerformer : public Performer
{
public:
    BasePerformer(const std::string name = std::string());
    virtual ~BasePerformer();
    
    virtual const StringList& getMethods() const;
    virtual void getOutput(std::string& output);
    virtual void setDirectory(const std::string& filePath);
    
protected:
    std::string name_;
    StringList methods_;
    std::ostringstream log_;
    std::string filePath_;
};

class CodeJamPerformer : public BasePerformer
{
public:
    CodeJamPerformer(const std::string name = std::string());
    virtual ~CodeJamPerformer();
    
    virtual int perform(const std::string& fileName, const StringList& arguments);
    
    /** Called once per test case with the lines of the file for that case */
    virtual int cjPerform(const std::string& method, const std::vector<IntList>& lines);
    
    /** Called before cjPerform to setup its "lines" parameter */
    virtual void cjSetup(std::ifstream& ins, std::vector<IntList>& lines) = 0;
    
protected:
    std::string cjOutput_;
};
