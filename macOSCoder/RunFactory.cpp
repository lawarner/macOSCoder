//
//  RunFactory.cpp
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

#include <iostream>
#include <map>

#include "RunFactory.hpp"
#include "Performer.hpp"
using namespace std;

static RunFactory* _instance = 0;


class RunFactoryImpl
{
public:
    //
    std::map<std::string, Performer*> performers;
    StringList methods;
    std::string output;
    std::string filePath;
    
    StringList empty;   // yeah, i'll refactor later
};


RunFactory::RunFactory()
: impl_(*new RunFactoryImpl)
{
    
}

RunFactory::~RunFactory()
{
    delete &impl_;
}

RunFactory* RunFactory::instance()
{
    if (!_instance)
    {
        _instance = new RunFactory;
    }
    return _instance;
}

void RunFactory::loadDictionary()
{
    
}

void RunFactory::unloadDictionary()
{
    
}

bool RunFactory::addPerformer(const std::string& method, Performer* performer)
{
    RunFactory* myself = instance();
    myself->impl_.methods.push_back(method);
    myself->impl_.performers[method] = performer;
    
    return true;
}

const StringList& RunFactory::getClasses()
{
    return instance()->impl_.methods;
}

const StringList& RunFactory::getClassMethods(const std::string& className)
{
    Performer* performer = instance()->impl_.performers[className];
    return performer ? performer->getMethods() : instance()->impl_.empty;
}

const std::string& RunFactory::getOutput()
{
    return instance()->impl_.output;
}

int RunFactory::runMethod(const std::string& className, const std::string& method, const StringList& arguments)
{
    cout << "Going to run " << className << "::" << method << " with " << arguments.size() << " arguments." << endl;
    Performer* performer = instance()->impl_.performers[className];
    if (performer)
    {
        instance()->impl_.output.erase();
        performer->setDirectory(instance()->impl_.filePath);
        int retval = performer->perform(method, arguments);
        performer->getOutput(instance()->impl_.output);
        std::cout << "Factory " << (instance()->impl_.output.empty() ? "did not get" : "got") << " output" << endl;
        return retval;
    }
    
    std::cout << "Error: method not found" << std::endl;
    return -321;
}

void RunFactory::setDirectory(const std::string& filePath)
{
    instance()->impl_.filePath = filePath;
}
