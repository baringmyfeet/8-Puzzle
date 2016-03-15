//
//  LinkedList.h
//  8-puzzle
//
//  Created by Jinwei Lin on 9/22/15.
//  Copyright (c) 2015 JL. All rights reserved.
//

class LinkedList{
    
private:
    Node* listHead;
    Node* dummyNode;
public:
    LinkedList(){
        dummyNode = new Node();
        listHead = dummyNode;
    }
    
    ~LinkedList(){}
    
    Node* get_listHead(){
        return listHead;
    }
    
    void set_listHead(Node* n){
        listHead = n;
    }
};
