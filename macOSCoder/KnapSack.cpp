//
//  KnapSack.cpp
//  macOSCoder
//
//  Copyright © 2016 Andy Warner.
//

#include "KnapSack.hpp"
#include <iostream>
using namespace std;

enum AlgoType
{
    AT_RECURSE,
    AT_PERMUTE_UNIQUE,
    AT_PERMUTE_DUPLICATE
};

#if 0
//////////////////////////// QuickPerm non-recursive ////////////////////////////
#define N    4   // number of elements to permute. N > 2

void display(unsigned int *a, unsigned int j, unsigned int i)
{
    for(unsigned int x = 0; x < N; x++)
    {
        cout << a[x] << " ";
    }
    cout << "   swapped(" << j << ", " << i << ")" << endl;
}

void QuickPerm(void)
{
    unsigned int a[N], p[N+1];
    unsigned int i, j, tmp; // Upper Index i; Lower Index j
    
    for (i = 0; i < N; i++)   // initialize arrays
    {
        a[i] = i + 1;
        p[i] = i;
    }
    
    p[N] = N; // p[N] > 0 controls iteration and the index boundary for i
    display(a, 0, 0);   // display initial array a[]
    
    i = 1;   // setup first swap points to be 1 and 0 respectively (i & j)
    while (i < N)
    {
        p[i]--;             // decrease index "weight" for i by one
        j = i % 2 * p[i];   // IF i is odd then j = p[i] otherwise j = 0
        tmp = a[j];         // swap(a[j], a[i])
        a[j] = a[i];
        a[i] = tmp;
        display(a, j, i); // display target array a[]
        i = 1;              // reset index i to 1 (assumed)
        while (!p[i])
        {
            p[i] = i;        // reset p[i] zero value
            i++;             // set new index value for i (increase by one)
        }
    }
}
#endif

/*
 * Knapsack problem: maximize the value of boxes in knacksack given a maximum weight constraint.
 * Determine solution when only 1 of each color box can be used.
 * Then determine solution when multiple of each color box can be used.
const char* boxColors[] = {
    "orange", "grey", "blue", "yellow", "green"
};
*/

int netWorth(const vector<int>& knapSack, const vector<Box>& boxes)
{
    int worth = 0;
    for (int idx = 0; idx < knapSack.size(); ++idx)
    {
        int xx = knapSack[idx];
        worth += boxes[xx].value;
    }
    return worth;
}

int netWeight(const vector<int>& knapSack, const vector<Box>& boxes)
{
    int weight = 0;
    for (int idx = 0; idx < knapSack.size(); ++idx)
    {
        int xx = knapSack[idx];
        weight += boxes[xx].weight;
    }
    return weight;
}

void swapItems(int* i1, int* i2)
{
    int tmp = *i1;
    *i1 = *i2;
    *i2 = tmp;
}

enum Method
{
    METHOD_EXAMPLE,
    METHOD_RECURSIVE_FILL,
    METHOD_PERMUTE_ONE_EACH,
    METHOD_PERMUTE_DUPS
};
static const char* methods[] = {
    "KnapSack-example.txt",
    "Use recursive fill",
    "Use permute, one each",
    "Use permute, duplicates allowed"
};

KnapSack::KnapSack()
: CodeJamPerformer("KnapSack")
, algorithm_(0)
{
    methods_.push_back(methods[METHOD_EXAMPLE]);
    methods_.push_back(methods[METHOD_RECURSIVE_FILL]);
    methods_.push_back(methods[METHOD_PERMUTE_ONE_EACH]);
    methods_.push_back(methods[METHOD_PERMUTE_DUPS]);
}

KnapSack::~KnapSack()
{
    
}

void KnapSack::printKnapsack(bool withValues)
{
    log_ << "Knapsack: ";
    if (contents_.empty())
    {
        log_ << "(empty)";
    }
    else
    {
        for (int idx = 0; idx < contents_.size(); ++idx)
        {
            int kx = contents_[idx];
            Box& box = boxes_[kx];
            log_ << " " << kx;
            if (withValues)
            {
                log_ << "(" << box.weight << "," << box.value << ")";
            }
        }
    }
    log_ << endl;
    //log_ << "\nNet worth = " << netWorth(contents_, boxes_) << endl;
}

void KnapSack::printSolution(int solution, int dups)
{
    log_ << "Knapsack with " << dups << " dups: ";
    for (int idx = 0; idx < dups * boxes_.size(); ++idx)
    {
        if (solution & (1 << idx))
        {
            log_ << " " << idx / dups;
        }
    }
    log_ << endl;
}

bool KnapSack::fillKnapSack(int maxWeight)
{
    bool retval = false;
    size_t savedSize = contents_.size();
    for (int idx = 0; idx < boxes_.size(); ++idx)
    {
        int remainder = maxWeight - boxes_[idx].weight;
        if (remainder < 0)
        {
            retval = true;
        }
        else
        {
            contents_.push_back(idx);
            if (remainder > 0) retval = fillKnapSack(remainder);
            int worth = netWorth(contents_, boxes_);
            if (worth > maxValue_)
            {
                maxValue_ = worth;
                saveContents_ = contents_;
            }
        }

        contents_.resize(savedSize);
    }
    return retval;
}

void KnapSack::permuteOneEach(int dups)
{
    int dupBoxesCount = dups * boxes_.size();
    int solution = 0;
    int boxBitMax = 1 << dupBoxesCount;
    maxValue_ = 0;
    for (int permBits = 1; permBits < boxBitMax; ++permBits)
    {
        int currValue = 0;
        int currWeight = 0;
        for (int idx = 0; idx < dupBoxesCount; ++idx)
        {
            if (permBits & (1 << idx))
            {
                currWeight += boxes_[idx / dups].weight;
                if (currWeight > maxWeight_)
                {
                    break;    // short cirtcuit if overweight
                }
                currValue += boxes_[idx / dups].value;
            }
        }
        if (currWeight <= maxWeight_ && currValue > maxValue_)
        {
            maxValue_ = currValue;
            solution = permBits;
            //cout << "New Maximum = " << dec << maxValue_ << ", bits 0x" << hex << permBits << endl;
        }
    }
    printSolution(solution, dups);
    log_ << "Net worth: " << maxValue_ << endl;
}

// Naive non-recursive permutation:
//    1 item: 0, 1, 2, 3, 4
//    2 items: 00, 01, ..., 04, 11, ..., 14, ..., 44
//    3 items: 000, 001
// Solution (from 0):  3 X 1, 3 X 3
void KnapSack::permuteItems(int maxItems)
{
    size_t maxIdx = boxes_.size() - 1;
    size_t currItem = 0;
    do
    {
        // printKnapsack(true);
        if (netWeight(contents_, boxes_) <= maxWeight_)
        {
            int worth = netWorth(contents_, boxes_);
            if (worth > maxValue_)
            {
                maxValue_ = worth;
                saveContents_ = contents_;
                log_ << "Net worth now: " << maxValue_ << endl;
            }
        }
        while (contents_[currItem] == maxIdx)
        {
            contents_[currItem++] = 0;
        }
        if (currItem == maxItems) break;
        ++contents_[currItem];
        currItem = 0;
    } while (currItem < maxItems);
}

int KnapSack::perform(const std::string &method, const StringList &arguments)
{
    if (method == methods[METHOD_RECURSIVE_FILL])
    {
        algorithm_ = AT_RECURSE;
    }
    else if (method == methods[METHOD_PERMUTE_ONE_EACH])
    {
        algorithm_ = AT_PERMUTE_UNIQUE;
    }
    else if (method == methods[METHOD_PERMUTE_DUPS])
    {
        algorithm_ = AT_PERMUTE_DUPLICATE;
    }
    else
    {
        return CodeJamPerformer::perform(method, arguments);
    }
    log_.clear();
    log_.str("");
    log_ << "Algorithm set to: " << method << endl;

    return 0;
}

int KnapSack::cjPerform(const std::string &method, const std::vector<IntList> &lines)
{
    switch (algorithm_)
    {
        case AT_RECURSE:
            fillKnapSack(maxWeight_);
            contents_ = saveContents_;
            printKnapsack(true);
            break;
        case AT_PERMUTE_UNIQUE:
            for (int idx = 1; idx < 6; ++idx)
            {
                log_ << "Permute allowing " << idx << " duplicates" << endl;
                permuteOneEach(idx);
            }
            break;
        case AT_PERMUTE_DUPLICATE:
            for (int idx = 1; idx < 8; ++idx)
            {
                log_ << "Permute and pick " << idx << " items" << endl;
                contents_.clear();
                while (contents_.size() < idx) contents_.push_back(0);
                permuteItems(idx);
            }
            contents_ = saveContents_;
            printKnapsack(true);
            break;
        default:
            break;
    }

    log_ << "Boxes: ";
    for (int idx = 0; idx < boxes_.size(); ++idx)
    {
        log_ << " " << idx << "(" << boxes_[idx].weight << "," << boxes_[idx].value << ")";
    }
    log_ << endl << "Max Weight: " << maxWeight_ << " Max Value: " << maxValue_ << endl;
    
    return 0;
}

void KnapSack::cjSetup(std::ifstream& ins, std::vector<IntList>& lines)
{
    boxes_.clear();
    contents_.clear();
    maxValue_ = 0;
    // first  line:  num_boxes max_weight
    // second line:  weight1 value1 ...
    int numBoxes;
    ins >> numBoxes;
    ins >> maxWeight_;
    for (int idx = 0; idx < numBoxes; ++idx)
    {
        int w, v;
        ins >> w >> v;
        boxes_.push_back(Box(w,v));
    }
}
