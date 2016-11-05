//
//  LazySpellingBee.cpp
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

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "LazySpellingBee.hpp"
using namespace std;


LazySpellingBee::LazySpellingBee()
: correctAnswers_(0)
{
    methods_.push_back("test one");
}

LazySpellingBee::~LazySpellingBee()
{
    
}

int LazySpellingBee::perform(const std::string& method, const StringList& arguments)
{
    log_.clear();
    log_.str("");
    log_ << "\n\n ====================== LazySpellingBee::" << method << " =====================" << endl;
    
    int retval = -1;
    if (method == "test one")
    {
        correctAnswers_ = 0;
        if (arguments.size() > 0)
        {
            word_ = arguments[0];
#if 1
            // 1=1, 2=4, 3=27, 4=, 5=108
            // except for first,last and dups, each position b (abc) has 3 possibilities.
            // p = 3 ** (n - 2) * 4
            // loop each character:
            //    count unique neighbors (1,2,3)
            //    p *= count
            correctAnswers_ = 1;
            size_t len = word_.length();
            for (size_t idx = 0; idx < len; ++idx)
            {
                size_t nn = 1;
                if (idx > 0 && word_[idx - 1] != word_[idx]) ++nn;
                if (idx < len - 1 && word_[idx] != word_[idx + 1]) ++nn;
                correctAnswers_ *= nn;
            }
#else
            correctAnswers_ = 1;
            if (word_.length() > 2)
            {
                size_t nn = word_.length() - 2;
                correctAnswers_ = pow(3, nn) * 4;
            }
            else if (word_.length() == 2)
            {
                correctAnswers_ = 4;
            }
            size_t wordLen = word_.length();
            string sorted(word_);
            sort(&sorted[0], &sorted[0] + wordLen);
            ostringstream oss;
            char last = 0;
            for (char ch : sorted)
            {
                if (last != ch)
                {
                    last = ch;
                    oss << ch;
                }
            }
            string letters(oss.str());
            log_ << "Sorted: " << sorted << " Letters: " << letters << endl;
            string output(word_.length(), '.');
            doPermuteDups(output, letters, 0, word_.length());
#endif
            log_ << "Word " << word_ << " has " << correctAnswers_ << " correct answers" << endl;
        }
    }
    else
    {
        cout << "Pass to base class, method: " << method << endl;
        retval = CodeJamPerformer::perform(method, arguments);
    }
    log_ << " ========================   DONE   ===================" << endl;
    
    return retval;
}

int LazySpellingBee::LazySpellingBee::cjPerform(const std::string& method, const std::vector<IntList>& lines)
{
    int retval = -1;
    return retval;
}

void LazySpellingBee::cjSetup(std::ifstream& ins, std::vector<IntList>& lines)
{
    
}

bool checkSpelling(const std::string& word, const std::string& spelling, size_t length)
{
    for (size_t idx = 0; idx < length; ++idx)
    {
        if (spelling[idx] == word[idx]) continue;
        if (idx > 0 && spelling[idx] == word[idx - 1]) continue;
        if (idx < word.length() - 1 && spelling[idx] == word[idx + 1]) continue;
        return false;
    }
    return true;
}

void LazySpellingBee::doPermute(std::string& str, std::size_t curr)
{
    // aa = aa; ab = aa, ab, ba, bb
    if (str.length() - curr > 1)
    {
        if (!checkSpelling(word_, str, word_.length())) return;
        
        doPermute(str, curr + 1);
        size_t maxSwap = curr + 2;
        if (maxSwap > str.length()) maxSwap = str.length();
        for (size_t idx = curr + 1; idx < maxSwap; ++idx)
        {
            char tmp = str[curr];
            str[curr] = str[idx];
            str[idx] = tmp;
            doPermute(str, curr + 1);
            tmp = str[curr];
            str[curr] = str[idx];
            str[idx] = tmp;
        }
    }
    else
    {
        bool isCorrect = checkSpelling(word_, str, word_.length());
        if (isCorrect) ++correctAnswers_;
        log_ << " + " << str << "  " << boolalpha << isCorrect << endl;
    }
}

bool iterJitter(int* jitt, size_t numJitts)
{
    for (size_t i = numJitts; i > 0; --i)
    {
        size_t idx = i - 1;
        if (jitt[idx] < (i == numJitts ? 0 : 1))
        {
            ++jitt[idx];
            return true;
        }
        else
        {
            jitt[idx] = -1;
        }
    }
    return false;
}
// This variant put letters from word into output with +/-1 jitter, copy each letter -1, 0, +1
// abcd: -,abcd,aacd,
void LazySpellingBee::doPermute3(string& output, const string& word)
{
    int jitt[output.length()];
    jitt[0] = 0;
    for (size_t idx = 1; idx < output.length(); ++idx)
    {
        jitt[idx] = -1;
    }
    do
    {
        bool skip = false;
        for (size_t idx = 0; idx < output.length(); ++idx)
        {
            if (jitt[idx] && word_[idx] == word_[idx + jitt[idx]])
            {
                skip = true;
                break;
            }
            output[idx] = word_[idx + jitt[idx]];
        }
        if (skip) continue;
        ++correctAnswers_;
        if (output.length() < 13)
        {
            log_ << " W+  " << output << "  Jitts: ";
            for (size_t ii = 0; ii < output.length(); ++ii) log_ << setw(2) << jitt[ii] << " ";
            log_ << endl;
        }
    } while (iterJitter(jitt, output.length()));
}
// Permute 'length' subset of str
/*
 aa:a,a; aabb:aa,ab,ba,bb
 abc: aaa,aab,aac,aba,abb,abc,aca,acb,acc,baa,bab,bac,bba,bbb,bbc,bca,bcb,bcc,caa,cab,cac,cba,cbb,cbc,cca,ccb,ccc
 */
void LazySpellingBee::doPermuteDups(string& output, const string& letters, size_t curr, size_t total)
{
    if (curr >= total)
    {
        bool isCorrect = checkSpelling(word_, output, word_.length());
        if (isCorrect) ++correctAnswers_;
        log_ << " + " << output << "  " << boolalpha << isCorrect << endl;
    }
    else
    {
        for (char ch : letters)
        {
            output[curr] = ch;
            if (checkSpelling(word_, output, curr + 1))
            {
                doPermuteDups(output, letters, curr + 1, total);
            }
        }
    }
}
#if 0
void LazySpellingBee::doPermuteDups(std::string& output, std::string& str, std::size_t curr)
{
    if (word_.length() - curr > 1)
    {
        if (!checkSpelling(word_, str)) return;
        
        doPermuteDups(output, str, curr + 1);
        size_t maxSwap = curr + 2;
        if (maxSwap > str.length()) maxSwap = str.length();
        for (size_t idx = curr + 1; idx < maxSwap; ++idx)
        {
            char tmp = str[curr];
            str[curr] = str[idx];
            str[idx] = tmp;
            doPermuteDups(output, str, curr + 1);
            tmp = str[curr];
            str[curr] = str[idx];
            str[idx] = tmp;
        }
    }
    else
    {
        bool isCorrect = checkSpelling(word_, str);
        if (isCorrect) ++correctAnswers_;
        log_ << " + " << str << "  " << boolalpha << isCorrect << endl;
    }
}
#endif
