#pragma once
//
//  LazySpellingBee.hpp
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

#include "Performer.hpp"


class LazySpellingBee : public CodeJamPerformer
{
public:
    LazySpellingBee();
    virtual ~LazySpellingBee();
    
    int perform(const std::string& method, const StringList& arguments);
    int cjPerform(const std::string& method, const std::vector<IntList>& lines);
    void cjSetup(std::ifstream& ins, std::vector<IntList>& lines);
    
private:
    void doPermute3(std::string& output, const std::string& word);
    void doPermute(std::string& str, std::size_t curr);
    void doPermuteDups(std::string& output, const std::string& letters, size_t curr, size_t total);
    
    uint64_t correctAnswers_;
    std::string word_;
};
