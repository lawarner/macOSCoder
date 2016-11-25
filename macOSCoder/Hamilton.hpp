#pragma once
//
//  Hamilton.hpp
//  macOSCoder
//
//  Copyright © 2016 Andy Warner. All rights reserved.
//

#include <vector>
#include "Performer.hpp"


// An edge connects 2 vertices and has a weight.
class Edge
{
public:
    Edge(int v1, int v2, int weight = 1)
    : v1_(v1)
    , v2_(v2)
    , weight_(weight)
    {
        
    }
    
    int getV1() const { return v1_; }
    int getV2() const { return v2_; }
    void getVertices(int &v1, int &v2) const
    {
        v1 = v1_;
        v2 = v2_;
    }
    int getWeight() const { return weight_; }
    
    static void swapEdges(Edge*& e1, Edge*& e2)
    {
        int tmp = e1->v1_;
        e1->v1_ = e2->v1_;
        e2->v1_ = tmp;
        tmp = e1->v2_;
        e1->v2_ = e2->v2_;
        e2->v2_ = tmp;
        tmp = e1->weight_;
        e1->weight_ = e2->weight_;
        e2->weight_ = tmp;
    }
    
private:
    int v1_;
    int v2_;
    int weight_;
};

class Hamilton : public BasePerformer
{
public:
    void simpleInit();
    int perform(const std::string& method, const StringList& arguments);
    
private:
    std::vector<Edge *> edges_;
};
