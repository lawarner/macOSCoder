#pragma once
//
//  Dictionary.hpp
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

#include <ostream>
#include <string>

// Forward reference
class Tree;


class Dictionary
{
public:
    Dictionary(std::ostream& logger);
    virtual ~Dictionary();
    
    bool add(const std::string& name);
    bool addSimple(const std::string& name);
    int balance();
    
    unsigned int getCount() const;
    unsigned int getDepth(bool recalculate = false);
    
    bool lookup(const std::string& name) const;
    void print(bool asTree = true) const;
    
private:
    std::ostream& log_;
    Tree* tree_;
    unsigned int count_;
    unsigned int depth_;
};
