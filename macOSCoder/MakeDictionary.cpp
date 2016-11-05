//
//  MakeDictionary.cpp
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

#include <fstream>
#include <iostream>
#include "Dictionary.hpp"
#include "MakeDictionary.hpp"
using namespace std;


MakeDictionary::MakeDictionary()
: dictionary_(new Dictionary(log_))
{
    methods_.push_back("add");
    methods_.push_back("balance");
    methods_.push_back("clear");
    methods_.push_back("get depth");
    methods_.push_back("load");
    methods_.push_back("lookup");
    methods_.push_back("print");
    methods_.push_back("print tree");
}

MakeDictionary::~MakeDictionary()
{
    delete dictionary_;
}

Dictionary* MakeDictionary::getDictionary() const
{
    return dictionary_;
}

int MakeDictionary::perform(const std::string& method, const StringList& arguments)
{
    log_.clear();
    log_.str("");
    log_ << "\n\n ====================== MakeDictionary::" << method << " =====================" << endl;
    
    if (method == "add")
    {
        for (int idx = 0; idx < arguments.size(); ++idx)
        {
            if (!dictionary_->add(arguments[idx]))
            {
                log_ << "Error adding " << arguments[idx] << " to dictionary." << endl;
            }
        }
    }
    else if (method == "balance")
    {
        unsigned int treeSize  = dictionary_->getCount();
        unsigned int treeDepth = dictionary_->getDepth(true);
        log_ << "Before balance: " << treeDepth << "/" << treeSize << " (depth/size)" << endl;
        
        int ret = dictionary_->balance();
        
        treeSize  = dictionary_->getCount();
        treeDepth = dictionary_->getDepth(true);
        log_ << "After balance: " << treeDepth << "/" << treeSize << "  return code: " << ret << endl;
    }
    else if (method == "clear")
    {
        delete dictionary_;
        dictionary_ = new Dictionary(log_);
    }
    else if (method == "load")
    {
        ifstream dict(filePath_ + arguments[0]);
        log_ << "Open dictionary file " << (dict.is_open() ? "succeded" : "failed") << endl;
        if (dict.is_open())
        {
            unsigned int numAdded = 0;
            while (!dict.eof() && numAdded < 2000000)
            {
                if (numAdded && numAdded % 10000 == 0)
                {
                    cout << "Compress at " << numAdded << endl;
                    dictionary_->balance();
                }
                string item;
                dict >> item;
                if (dictionary_->addSimple(item))
                {
                    ++numAdded;
                }
                else
                {
                    log_ << "Error adding \"" << item << "\" to dictionary." << endl;
                }
            }
            dict.close();
            log_ << "Added " << numAdded << " items to dictionary." << endl;
            dictionary_->balance();
        }
    }
    else if (method == "lookup")
    {
        for (int idx = 0; idx < arguments.size(); ++idx)
        {
            log_ << "Word " << arguments[idx]
            << (dictionary_->lookup(arguments[idx]) ? " was found" : " was not found") << endl;
        }
    }
    else if (method == "get depth") {
        int depth = dictionary_->getDepth(true);
        log_ << "Depth of tree is " << depth << endl;
    }
    else if (method == "print")
    {
        log_ << "Contents of dictionary:" << endl;
        dictionary_->print(false);
    }
    else if (method == "print tree")
    {
        log_ << "Dictionary Tree:" << endl;
        dictionary_->print(true);
    }
    else
    {
        log_ << "Error: unknown method \"" << method << "\"" << endl;
    }
    log_ << " ========================   DONE   ===================" << endl;
    
    return 0;
}
