//
//  main.cpp
//  8-puzzle
//
//  Created by Jinwei Lin on 9/20/15.
//  Copyright (c) 2015 JL. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <time.h>
#include "Node.h"
#include "LinkedList.h"
using namespace std;

// represent 3*3 puzzle board in string, with left to right,up to down order, use 0 to represent empty tile

string easy,medium,hard,worst; // corresponding to 4 start state files
string goalState = "123804765";
string eightPuzzle; // initial state
LinkedList* openList;
LinkedList* closedList;
LinkedList* path;

// add new states to open list, at the same time perform insertion sort, lowest value of f cost
//is the first node in the list
void stateToOpen(string state, int gCost, int fCost, Node* parent){
    Node* newNode = new Node(state,gCost,fCost,parent);
    Node* walker = openList->get_listHead();
    while (walker->getNext() != NULL && newNode->getFCost() > walker->getNext()->getFCost()) {
        walker = walker->getNext();
    }
    newNode->setNext(walker->getNext());
    walker->setNext(newNode);
}

// print number of nodes expanded
void printClose(){
    Node* walker = closedList->get_listHead()->getNext();
    int numNodeExpaned=0;
    while (walker != NULL) {
        numNodeExpaned++;
        walker = walker->getNext();
    }
    cout<<"Number of nodes expanded: "<<numNodeExpaned<<endl;
}

void stateToClosed(string state, int gCost, int fCost, Node* parent){
    Node* newNode = new Node(state,gCost,fCost,parent);
    Node* walker = closedList->get_listHead();
    while (walker->getNext() != NULL) {
        walker = walker->getNext();
    }
    newNode->setNext(walker->getNext());
    walker->setNext(newNode);
}

// remove a node from open list
void removeFromOpen(){
    Node* firstNode = openList->get_listHead()->getNext();
    openList->get_listHead()->setNext(firstNode->getNext());
    //delete firstNode;
}

// compute the number of tiles out of place
int misHeuristic(string state){
    int misPlace=0; // mismatch tile counter
    for (int i=0; i<=goalState.length(); i++) {
        if (goalState[i] != state[i] && i != 4) {
            misPlace++;
        }
    }
    return misPlace;
}

// check if the new state genetated has expanded already
bool isInClosed(string state){
    int misPlace=0;
    string tempState="";
    Node* walker = closedList->get_listHead()->getNext();
    while (walker != NULL) {
        misPlace=0;
        tempState = walker->getPuzzle();
        for (int i=0; i<=state.length(); i++) {
            if (state[i] != tempState[i]) {
                misPlace++;
            }
        }
        if (misPlace == 0) {
            return true;
        }
        walker = walker->getNext();
    }
    return false;
    
}

// check if the new state is scheduled to be expanded
bool isInOpen(string state){
    int misPlace=0;
    string tempState="";
    Node* walker = openList->get_listHead()->getNext();
    while (walker != NULL) {
        misPlace=0;
        tempState = walker->getPuzzle();
        for (int i=0; i<=state.length(); i++) {
            if (state[i] != tempState[i]) {
                misPlace++;
            }
        }
        if (misPlace == 0) {
            return true;
        }
        walker = walker->getNext();
    }
    return false;
    
}

void printPath(){
    cout << "Sequence of moves from start state to goal state: "<<endl;
    Node* walker = path->get_listHead()->getNext();
    int numOfMove=0;
    string curState="";
    while (walker != NULL) {
        curState = walker->getPuzzle();
        for (int i=0; i<=curState.length(); i++) {
            if (i == 3 || i==6) {
                cout<<endl;
            }
            cout<<curState[i]<<" ";
        }
        walker = walker->getNext();
        cout<<endl<<endl;
        numOfMove++;
    }
    cout<<"Number of move is "<<numOfMove-1<<endl;
}

void addToPath(string state){
    Node* newNode = new Node(state);
    Node* walker = path->get_listHead();
  
    newNode->setNext(walker->getNext());
    walker->setNext(newNode);
}

void computePath(Node* parent){
    Node* curNode = parent;
    while (curNode != NULL) {
        addToPath(curNode->getPuzzle());
        curNode = curNode->getParent();
    }
}

void aStarMisplace(string inFileName){
    openList = new LinkedList();
    closedList = new LinkedList();
    path = new LinkedList();
    eightPuzzle = "";
    ifstream infile;
    infile.open(inFileName);
    string puzzleValue = "";
    
    while(!infile.eof()){
        infile >> puzzleValue;
        eightPuzzle = eightPuzzle + puzzleValue;
    }
    infile.close();
    int misPlace = 0,gValue=0;
    misPlace = misHeuristic(eightPuzzle); // initially f = 0 + h
    stateToOpen(eightPuzzle,gValue,misPlace, NULL); // put initial state into open list
    
    string curState=""; //current state
    int curGValue=0;
    int fValue=0;
    while (!(openList->get_listHead()->getNext() == NULL)) {
        int emptyTileIndex = 0;
        string nextState = "";
        Node* promisingNode = openList->get_listHead()->getNext(); // most promising node, with lowest f value
        curState = promisingNode->getPuzzle();
        curGValue = promisingNode->getGCost();
        fValue = promisingNode->getFCost();
        misPlace = misHeuristic(curState);
        
        if (misPlace==0) { // if node is the goal, we are done
            cout<<inFileName<<" input file "<<"Reach the goal state!"<<endl;
            computePath(promisingNode);
            printPath();
            break;
        }else{
            removeFromOpen();
            stateToClosed(curState,curGValue,fValue,promisingNode);
            for (int i=0; i<=curState.length(); i++) {
                if (curState[i] == '0') {
                    emptyTileIndex = i;
                    break;
                }
            } // get the index of the empty tile
            
            //right, tiles can move right if not in the right most column
            if (emptyTileIndex !=2 && emptyTileIndex != 5 && emptyTileIndex !=8) {
                nextState = curState.substr(0,emptyTileIndex)+curState[emptyTileIndex+1] + '0' + curState.substr(emptyTileIndex+2);
                misPlace = misHeuristic(nextState);
                gValue = curGValue + 1;
                fValue = gValue + misPlace;
                if (isInClosed(nextState)==false && isInOpen(nextState)==false) {
                    stateToOpen(nextState,gValue,fValue,promisingNode);
                }
                
            }
            
            //down, tiles can move down if not in the very bottom row
            if (emptyTileIndex !=6 && emptyTileIndex != 7 && emptyTileIndex !=8) {
                nextState = curState.substr(0,emptyTileIndex)+ curState.substr(emptyTileIndex+3,1) + curState.substr(emptyTileIndex+1,2) + '0' + curState.substr(emptyTileIndex+4);
                misPlace = misHeuristic(nextState);
                gValue = curGValue + 1;
                fValue = gValue + misPlace;
                if (isInClosed(nextState)==false && isInOpen(nextState)==false) {
                     stateToOpen(nextState,gValue,fValue,promisingNode);
                }
            }
            
            //left, tiles can move left if not in the left most column
            if (emptyTileIndex !=0 && emptyTileIndex != 3 && emptyTileIndex !=6) {
                nextState = curState.substr(0,emptyTileIndex-1)+'0'+curState[emptyTileIndex-1]+curState.substr(emptyTileIndex+1);
                misPlace = misHeuristic(nextState);
                gValue = curGValue + 1;
                fValue = gValue + misPlace;
                if (isInClosed(nextState)==false && isInOpen(nextState)==false) {
                    stateToOpen(nextState,gValue,fValue,promisingNode);
                }
            }
            
            //up, tiles can move up if not in the very top of row
            if (emptyTileIndex !=0 && emptyTileIndex != 1 && emptyTileIndex !=2) {
                nextState = curState.substr(0,emptyTileIndex-3)+'0'+curState.substr(emptyTileIndex-2,2)+curState[emptyTileIndex-3]+curState.substr(emptyTileIndex+1);
                misPlace = misHeuristic(nextState);
                gValue = curGValue + 1;
                fValue = gValue + misPlace;
                if (isInClosed(nextState)==false && isInOpen(nextState)==false) {
                    stateToOpen(nextState,gValue,fValue,promisingNode);
                }
            }
        }
        
    }
    printClose();
}

int main(int argc, const char * argv[]) {
    easy = argv[1];
    medium = argv[2];
    hard = argv[3];
    worst = argv[4];
    clock_t t;
    t = clock();
    double duration;
    aStarMisplace(easy);
    //aStarMisplace(medium);
    //aStarMisplace(hard);
    //aStarMisplace(worst);
    duration = (clock() - t)/(double) CLOCKS_PER_SEC;
    cout<<"Total time: "<<duration<<endl;
    
    delete openList;
    delete closedList;
    delete path;
    return 0;
}
