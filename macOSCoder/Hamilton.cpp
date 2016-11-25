//
//  Hamilton.cpp
//  macOSCoder
//
//  Copyright © 2016 Andy Warner. All rights reserved.
//

#include "Hamilton.hpp"
#include <algorithm>
#include <iostream>
using namespace std;

static const int A = 1;
static const int B = 2;
static const int C = 3;
static const int D = 4;
static const int E = 5;
static const int F = 6;
//static const int G = 7;
//static const int H = 8;

unsigned int visited;
const unsigned int TotalVisit = 0x7e;   // Total visit == 0111 1110 (0xfe)
vector<Edge *> path;

bool tryPath(vector<Edge *> edges, bool isV1)
{
    Edge* edge1 = edges.back();
    int v1 = isV1 ? edge1->getV1() : edge1->getV2();
    unsigned int newVisit = 1 << v1;
    if (newVisit & visited)
    {
        visited = 0;
        path.clear();
        return false;
    }
    visited |= 1 << v1;
    path.push_back(edge1);
    
    //cout << edge1->getV1() << (isV1 ? "*->" : "->*") << edge1->getV2() << " ";
    if (visited == TotalVisit)
    {
        cout << "Path: ";
        for (int idx = 0; idx < path.size(); ++idx)
        {
            cout << path[idx]->getV1() << ":" << path[idx]->getV2() << "(" << path[idx]->getWeight() << ")  ";
        }
        cout << " YES" << endl;
        path.clear();
        return true;
    }
    edges.pop_back();
    bool foundPath = false;
    for (int idx = 0; idx < edges.size(); ++idx)
    {
        Edge *edge2 = edges[idx];
        if (v1 == edge2->getV1() || v1 == edge2->getV2())
        {
            if (idx != edges.size() - 1) Edge::swapEdges(edges[idx], edges.back());
            foundPath = tryPath(edges, v1 == edges.back()->getV2());
            if (idx != edges.size() - 1) Edge::swapEdges(edges[idx], edges.back());
        }
    }
    if (!foundPath)
    {
        //cout << "NO" << endl;
        visited = 0;
        path.clear();
    }
    return foundPath;
}

void Hamilton::simpleInit()
{
    // The solution to this is:  A E D C B F
    edges_.clear();
    edges_.push_back(new Edge(A, B, 9));
    edges_.push_back(new Edge(B, C, 5));
    edges_.push_back(new Edge(C, D, 2));
    edges_.push_back(new Edge(D, E, 1));
    edges_.push_back(new Edge(A, E, 3));
    edges_.push_back(new Edge(A, F, 5));
    edges_.push_back(new Edge(B, F, 4));
    edges_.push_back(new Edge(C, F, 8));
    edges_.push_back(new Edge(D, F, 7));
    edges_.push_back(new Edge(E, F, 5));
}

int Hamilton::perform(const std::string& method, const StringList& arguments)
{
    log_.clear();
    log_.str("");
    log_ << "\n ====================== Hamiltonian::" << method << " =====================" << endl;

    vector<Edge *> edges(edges_);
    for (int idx = 0; idx < edges.size(); ++idx)
    {
        //cout << "Edges " << idx << ": ";
        if (idx != edges.size() - 1) Edge::swapEdges(edges[idx], edges.back());
        
        visited = 0;
        path.clear();
        if (tryPath(edges, true))
        {
            cout << "Problem solved" << endl;
        }
        else
        {
            visited = 0;
            path.clear();
            if (tryPath(edges, false))
            {
                cout << "Problem solved" << endl;
            }
            else
            {
                cout << "No solution found" << endl;
            }
        }
        if (idx != edges.size() - 1) Edge::swapEdges(edges[idx], edges.back());
    }
    return false;
}
