#pragma once
//
//  RobotRockBand.hpp
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
#include <vector>


class RobotRockBand : public CodeJamPerformer
{
public:
    RobotRockBand();
    virtual ~RobotRockBand();
    
    int perform(const std::string& method, const StringList& arguments);
    int cjPerform(const std::string& method, const std::vector<IntList>& lines);
    void cjSetup(std::ifstream& ins, std::vector<IntList>& lines);
    
private:
    void display();
    void displayLists();
    bool increment(IntList::iterator* indices);
    long long int  permute();
    int  recursePerm(int KK, int currSize);
    int  xorValue() const;

private:
    int kay_;
    int count_;
    // robots that are auditioning for the 4 parts: A,B,C,D
    IntList ABCD[4];
    // For each position, indicate the number of robots with the same number
    IntList dups_[4];
    // Current band line up
    int band_[4];
};
