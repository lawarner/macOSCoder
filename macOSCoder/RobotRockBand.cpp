//
//  RobotRockBand.cpp
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

#include <algorithm>
#include <iostream>
#include <map>
#include "RobotRockBand.hpp"
using namespace std;

int bandSelected[4];

/*
 # test cases
 Input
 2
 2 3
 0 0
 2 0
 0 0
 0 1
 2 0
 1 10
 1 10
 1 10
 1 10
 
 Output
 Case #1: 4
 Case #2: 8
 
 In sample case #1, in order to get a combined bitwise XOR of 3, the robot chosen from the second list must
 be 2, and the robot chosen from the fourth list must be 1. For the first and third lists, either of the 
 two 0 robots can be chosen, so there are 2 * 2 = 4 possible bands that meet the criteria. Note that even 
 though all of these bands are of the form (0, 2, 0, 1), they are considered different because the 
 selections from the lists were different.
 */

RobotRockBand::RobotRockBand()
: CodeJamPerformer("RobotRockBand")
, kay_(0)
, count_(0)
{
    methods_.push_back("RRB-example.txt");
    methods_.push_back("RRB-small.txt");
    methods_.push_back("RRB-large.txt");
    methods_.push_back("simple");
}

RobotRockBand::~RobotRockBand()
{

}

int RobotRockBand::perform(const std::string& method, const StringList& arguments)
{
    log_.clear();
    log_.str("");
    log_ << "\n\n====================== " << name_ << "::" << method << " =====================" << endl;

    int retval = -1;
    if (method == "simple")
    {
        int num = permute();
        log_ << "# RRB Perms = " << num << endl;
    }
    else
    {
        cout << "Pass to base class, method: " << method << endl;
        retval = CodeJamPerformer::perform(method, arguments);
    }
    log_ << "========================   DONE   ===================" << endl;
    
    return retval;
}

int RobotRockBand::cjPerform(const std::string& method, const std::vector<IntList>& lines)
{
    long long int retval;
    kay_ = lines[0][1];
    count_ = static_cast<int>(lines[1].size() / 4);
    for (int i = 0; i < 4; ++i) dups_[i].clear();
    for (int i = 0, offset = 0; i < 4; ++i, offset += count_)
    {
        ABCD[i] = IntList(lines[1].begin() + offset, lines[1].begin() + offset + count_);
#if 0
        sort(ABCD[i].begin(), ABCD[i].end());
        // count dups before removing them
        IntList::iterator it = ABCD[i].begin();
        int last = ABCD[i][0];
        int lastCount = 0;
        for (int idx = 0; idx < ABCD[i].size(); ++idx)
        {
            if (last == ABCD[i][idx])
            {
                ++lastCount;
            }
            else
            {
                dups_[i].push_back(lastCount);
                last = ABCD[i][idx];
                lastCount = 1;
            }
        }
        dups_[i].push_back(lastCount);
        it = unique(ABCD[i].begin(), ABCD[i].end());
        ABCD[i].resize(distance(ABCD[i].begin(), it));
#endif
    }
    displayLists();

    retval = permute();
    log_ << "# RRB Perms = " << retval << endl;
    return (int)retval;
}

void RobotRockBand::cjSetup(std::ifstream& ins, std::vector<IntList>& lines)
{
    // NumRobots K
    // 4 lines with NumRobots values
    IntList numAndK;
    int numItems;
    int scratch;
    ins >> numItems;
    ins >> scratch;
    numAndK.push_back(numItems);
    numAndK.push_back(scratch);
    IntList items;
    for (int idx = 0; idx < 4 * numItems; ++idx)
    {
        ins >> scratch;
        items.push_back(scratch);
    }

    log_ << "K = " << numAndK.back() << " # items = " << numItems << endl;
    lines.push_back(numAndK);
    lines.push_back(items);
}

void RobotRockBand::display()
{
    log_ << "Band: ";
    for (int i = 0; i < 4; ++i)
    {
        log_ << band_[i] << "  ";
    }
    // log_ << " Xor = " << xorValue();
    log_ << endl;
}

void RobotRockBand::displayLists()
{
    char memName = 'A';
    log_ << "Lists: ";
    for (int mem = 0; mem < 4; ++mem)
    {
        log_ << static_cast<char>(memName + mem) << "=";
        for (int idx = 0; idx < ABCD[mem].size(); ++idx) log_ << ABCD[mem][idx] << " ";
    }
    log_ << endl << "Dups: ";
    for (int mem = 0; mem < 4; ++mem)
    {
        log_ << static_cast<char>(memName + mem) << "=";
        for (int idx = 0; idx < dups_[mem].size(); ++idx) log_ << dups_[mem][idx] << " ";
    }
    log_ << endl;
}

bool RobotRockBand::increment(IntList::iterator* indices)
{
    // start at right and increment. if already max, set to 0 and move left.
    for (int ix = 3; ix >= 0; --ix)
    {
        if (++indices[ix] == ABCD[ix].end())
        {
            indices[ix] = ABCD[ix].begin();
        }
        else
        {
            return true;
        }
    }
    return false;
}

/*
 * A=0 1 2 3 B=1 2 3 : 01 02 03 11 12 13 21 22 23 31 32 33
   Work with AB and CD xor's: For each unique xor(AB), run with all unique xor(CD)
 K = 101 = D=011  xor=K=110 | K=A^B if K=10, B=01 then A=11
 New algo: K = A ^ B ^ C ^ D   Pick D
 
 Shortcut bitwise combos: If K=0100  (bit = 1 if odd number of 1's else 0)
 
 */
#if 1
long long int RobotRockBand::permute()
{
    long long int result = 0;
    map<int, long long int> mm;
    for (int i = 0; i < ABCD[2].size(); ++i)
        for (int j = 0; j < ABCD[3].size(); ++j)
            mm[ABCD[2][i] ^ ABCD[3][j]]++;

    for (int i = 0; i < ABCD[0].size(); ++i)
        for (int j = 0; j < ABCD[1].size(); ++j)
        {
            map<int, long long int>::iterator it;
            it = mm.find(ABCD[0][i] ^ ABCD[1][j] ^ kay_);
            if (it != mm.end()) result += it->second;
        }

    return result;
}
#else
int RobotRockBand::recursePerm(int KK, int curr)
{
    int matches = 0;
    if (curr == 0)
    {
        IntList::iterator it = find(ABCD[0].begin(), ABCD[0].end(), KK);
        if (it != ABCD[0].end())
        {
            band_[0] = *it;
            int ix = distance(ABCD[0].begin(), it);
            matches = dups_[0][ix];
            for (int i = 1; i < 4; ++i)
            {
                // find band_[i] in ABCD[i]
                it = find(ABCD[i].begin(), ABCD[i].end(), band_[i]);
                ix = distance(ABCD[i].begin(), it);
                matches *= dups_[i][ix];
            }
            //display();
        }
    }
    else
    {
        for (int idx = 0; idx < ABCD[curr].size(); ++idx)
        {
            band_[curr] = ABCD[curr][idx];
            matches += recursePerm(KK ^ band_[curr], curr - 1);
        }
    }
    return matches;
}

int RobotRockBand::permute()
{
    for (int i = 0; i < 4; ++i) band_[i] = 0;
    int matches = recursePerm(kay_, 3);
    return matches;
}
// #else
int RobotRockBand::permute()
{
    IntList::iterator iters[4];
    for (int idx = 0; idx < 4; ++idx) iters[idx] = ABCD[idx].begin();
    int matches = 0;
    do
    {
        for (int i = 0; i < 4; ++i) band_[i] = *iters[i];
        // display();
        if (kay_ == xorValue())
        {
            int factor = 1;
            for (int i = 0; i < 4; ++i)
            {
                int ix = distance(ABCD[i].begin(), iters[i]);
                factor *= dups_[i][ix];
            }
            matches += factor;
        }
        
    }
    while (increment(iters));
    return matches;
}
#endif

int RobotRockBand::xorValue() const
{
    int ret = band_[0];
    for (int i = 1; i < 4; ++i) ret ^= band_[i];
    return ret;
}
