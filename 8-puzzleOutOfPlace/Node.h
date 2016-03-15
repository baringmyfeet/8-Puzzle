//
//  Node.h
//  8-puzzle
//
//  Created by Jinwei Lin on 9/21/15.
//  Copyright (c) 2015 JL. All rights reserved.
//

#include <iostream>
using namespace std;

class Node{
    
    private:
    string puzzle;
    int gCost;
    int fCost;
    Node* next;
    Node* parent;
    
    public:
    Node(){
        puzzle = "";
        gCost = 0;
        fCost = 0;
        next = NULL;
        
    } // dummy node
    
    Node(string p, int g, int f, Node* pa){
        puzzle = p;
        gCost = g;
        fCost = f;
        next = NULL;
        parent = pa;
        
    } // for nodes in open/closed list
    
    Node(string p){
        puzzle = p;
        next = NULL;
    }
    
    void setPuzzle(string p){
        puzzle = p;
    }
    
    string getPuzzle(){
        return puzzle;
    }
    
    void setGCost(int g){
        gCost = g;
    }
    
    int getGCost(){
        return gCost;
    }

    
    void setFCost(int f){
        fCost = f;
    }
    
    int getFCost(){
        return fCost;
    }
    
    void setNext(Node* n){
        next = n;
    }
    
    Node* getNext(){
        return next;
    }
    
    void setParent(Node* p){
        parent = p;
    }
    
    Node* getParent(){
        return parent;
    }
};
