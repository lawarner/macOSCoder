#pragma once
//
//  KnapSack.hpp
//  macOSCoder
//
//  Created by Lynn Warner on 11/6/16.
//  Copyright © 2016 Andy Warner. All rights reserved.
//

#include <vector>
#include "Performer.hpp"


struct Box
{
    Box(int w, int v) : weight(w), value(v) {  }
    int weight;
    int value;
};

class KnapSack : public CodeJamPerformer
{
public:
    KnapSack();
    virtual ~KnapSack();

    int perform(const std::string& method, const StringList& arguments);
    int cjPerform(const std::string& method, const std::vector<IntList>& lines);
    void cjSetup(std::ifstream& ins, std::vector<IntList>& lines);

private:
    bool fillKnapSack(int maxWeight);
    void permuteItems(int maxItems);
    void permuteOneEach(int dups);
    void printKnapsack(bool withValues);
    void printSolution(int solution, int dups);
    
private:
    int algorithm_;
    int maxValue_;
    int maxWeight_;
    std::vector<Box> boxes_;
    std::vector<int> contents_;
    std::vector<int> saveContents_;

};
