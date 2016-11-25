//
//  NotSoRandom.cpp
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

#include <iomanip>
#include <map>
#include "NotSoRandom.hpp"
using namespace std;

static const int AND = 0;
static const int OR  = 1;
static const int XOR = 2;

NotSoRandom::DataType AA, BB, CC;
NotSoRandom::DataType KK;

NotSoRandom::NotSoRandom()
: CodeJamPerformer("NotSoRandom")
{
    methods_.push_back("NSR-example.txt");
    methods_.push_back("NSR-small.txt");
    methods_.push_back("NSR-large.txt");
}

NotSoRandom::~NotSoRandom()
{
    
}

double NotSoRandom::doOpers(DataType val, DataType seq)
{
    DataType values[3];
    double probs[3];
    int numVals = setValues(val, values, probs);
    double result = 0;
    if (seq > 1)
    {
        for (int idx = 0; idx < numVals; ++idx)
        {
            result += doOpers(values[idx], seq - 1) * probs[idx];
        }
    }
    else
    {
        for (int idx = 0; idx < numVals; ++idx)
        {
            result += values[idx] * probs[idx];
        }
    }
    return result;
}

double NotSoRandom::doOpersBrute(DataType val, DataType seq)
{
    DataType values[3];
    double result = (double)val;
    setValuesSimple(val, values);
    if (seq > 1)
    {
        double d0 = doOpers(values[0], seq - 1);
        double d1 = doOpers(values[1], seq - 1);
        double d2 = doOpers(values[2], seq - 1);
        result = d0 * A + d1 * B + d2 * C;
    }
    else
    {
        result = values[0] * A + values[1] * B + values[2] * C;
    }
    return result;
}

int NotSoRandom::setValues(DataType val, DataType* values, double* probs)
{
    values[AND] = val & K;
    values[OR]  = val | K;
    values[XOR] = val ^ K;
    probs[AND] = A;
    probs[OR]  = B;
    probs[XOR] = C;
    // log_ << "val=" << val << ", AND=" << values[AND] << " OR=" << values[OR] << ", XOR=" << values[XOR] << endl;
    int retval = 3;
    if (values[0] == values[2] || values[1] == values[2])
    {
        --retval;
        int idx = values[0] == values[2] ? 0 : 1;
        probs[idx] += probs[2];
        probs[2] = 0;
    }
    if (values[0] == values[1])
    {
        probs[0] += probs[1];
        if (retval == 3)
        {
            retval = 2;
            values[1] = values[2];
            probs[1]  = probs[2];
            values[2] = 0;
            probs[2] = 0;
        }
        else
        {
            retval = 1;
            probs[0] = 1;
        }
    }
    return retval;
}

int NotSoRandom::setValuesSimple(DataType val, DataType* values)
{
    values[AND] = val & K;
    values[OR]  = val | K;
    values[XOR] = val ^ K;
    // log_ << "val=" << val << ", AND=" << values[AND] << " OR=" << values[OR] << ", XOR=" << values[XOR] << endl;
    return 3;
}

int NotSoRandom::perform(const std::string& method, const StringList& arguments)
{
    int retval = -1;

    retval = CodeJamPerformer::perform(method, arguments);

    return retval;
}

/*
 v1, v2, v3 with probability p1, p2, p3
 each unique input value:
 */
class RNode
{
public:
    typedef NotSoRandom::DataType IntType;
    RNode(IntType v, IntType probAnd, IntType probOr, IntType probXor)
    : value(v)
    {
        probs[0] = probAnd;
        probs[1] = probOr;
        probs[2] = probXor;
        for (int i = 0; i < 3; ++i) children[i] = 0;
    }
    ~RNode()
    {
        for (int i = 0; i < 3; ++i)
        {
            if (children[i]) delete children[i];
        }
    }

    void setChildren(int num)
    {
        children[0] = new RNode(value & KK, AA, BB, CC);
        children[1] = new RNode(value | KK, AA, BB, CC);
        children[2] = new RNode(value ^ KK, AA, BB, CC);
        if (--num > 0)
        {
            children[0]->setChildren(num);
            children[1]->setChildren(num);
            children[2]->setChildren(num);
        }
    }

    string printTree() const
    {
        ostringstream oss;
        double avg = 0.0;;
        oss << "Children: ";
        for (int i = 0; i < 3; ++i)
        {
            if (children[i])
            {
                oss << children[i]->value << " prob(" << probs[i] << ")   ";
                avg += 0.01 * probs[i] * children[i]->value;
            }
            else
            {
                oss << "(null)   ";
            }
        }
        oss << "Average: " << avg << endl;
        return oss.str();
    }
public:
    IntType value;
    IntType probs[3];
    RNode* children[3];
};

int NotSoRandom::cjPerform(const std::string& method, const std::vector<IntList>& lines)
{
#if 1
    // try to handle the large data set (non-recursive)
    // build tree for each level N with values v1, v2, v3:
    //               N
    //     v1        v2        v3
    //  w1 w2 w3  w4 w5 w6  w7 w8 w9
    //
    RNode* tree = new RNode(X, AA, BB, CC);
    tree->setChildren(N);
    log_ << tree->printTree();
    delete tree;
#else
    map<int, int> vm;
    DataType input = X;
    for (int i = 0; i < N; ++i)
    {
        DataType values[3];
        setValuesSimple(input, values);
        vm[values[0]] += AA;
        vm[values[1]] += BB;
        vm[values[2]] += CC;
        
    }
    //double average = values[0] * A + values[1] * B + values[2] * C;
    ostringstream oss;
    oss << setprecision(15) << average;
    cjOutput_ = oss.str();
// #else
    double average = doOpers(X, N);
    ostringstream oss;
    oss << setprecision(15) << average;
    cjOutput_ = oss.str();
// #else
    DataType val = X;
    double average;
    for (int i = 0; i < N; ++i)
    {
        DataType iand = val & K;
        DataType ior  = val | K;
        DataType ixor = val ^ K;
        log_ << "val=" << val << ", AND=" << iand << " OR=" << ior << ", XOR=" << ixor << endl;
        
        average = iand * A + ior * B + ixor * C;
        log_ << "Average=" << average << endl;
        val = average;
    }
#endif
    return 0;
}

void NotSoRandom::cjSetup(std::ifstream& ins, std::vector<IntList>& lines)
{
    ins >> N;
    ins >> X;
    ins >> K;
    KK = K;
    ins >> AA;
    A = (double)AA / 100;
    ins >> BB;
    B = (double)BB / 100;
    ins >> CC;
    C = (double)CC / 100;
    log_ << "N=" << N << ", X=" << X << " K=" << K << ", A=" << A << ", B=" << B << ", C=" << C << endl;
}
