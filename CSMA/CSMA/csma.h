//
//  csma.h
//  CSMA
//
//  Created by Abhinav KR on 5/2/14.
//  Copyright (c) 2014 Abhinav KR. All rights reserved.
//


#include <iostream>
#include <vector>

using namespace std;

typedef enum {
    eIdle=0,  //Grey
    eSendingData,  //Green
    eSensing, //White
    eBackOff, //Blue
    eCollision, //Red
    eWaitingForAck, //Magenta
    eAckReceived, //Yellow
}NodeState;

class CSMA {
    
    private :
    
    vector<vector<NodeState>> nodes;
    vector<int> backOffTimeForNodes;
    vector<int> ackWaitingTimeForNodes;
    int nodeNumberWhichSentData;
    int sizeOfDataWhichWasSent;
    int nodeOccupiedByChannel; //Assumption that there is only one channel
    
    void collisionCheck(vector<NodeState> &nodeStates);
    
    public :
    
    void sendDataThroughNode(int nodeNumber, int size);
    
    //This method should be called periodically
    void updateNodesStateAndCopyToVector(vector<vector<NodeState>> &nodes);
    
    CSMA () {
        nodeNumberWhichSentData = -1;
        nodeOccupiedByChannel = -1;        
    }
};