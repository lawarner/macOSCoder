#pragma once
//
//  NotSoRandom.hpp
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

#include "Performer.hpp"


class NotSoRandom : public CodeJamPerformer
{
public:
    typedef unsigned int DataType;

    NotSoRandom();
    virtual ~NotSoRandom();
    
    int perform(const std::string& method, const StringList& arguments);
    int cjPerform(const std::string& method, const std::vector<IntList>& lines);
    void cjSetup(std::ifstream& ins, std::vector<IntList>& lines);

private:
    double doOpers(DataType val, DataType seq);
    double doOpersBrute(DataType val, DataType seq);
    int setValues(DataType val, DataType* values, double* probs);
    int setValuesSimple(DataType val, DataType* values);

private:
    DataType N, X, K;
    double A, B, C;
};
