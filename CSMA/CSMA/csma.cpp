//
//  csma.cpp
//  CSMA
//
//  Created by Abhinav KR on 5/2/14.
//  Copyright (c) 2014 Abhinav KR. All rights reserved.
//

#include "csma.h"
#include "stdlib.h"

#define MAX_BACKOFF_TIME 4
#define MAX_ACK_TIME 4

#define MAX_ARRAY_SIZE 12

void CSMA::collisionCheck(vector<NodeState> &nodeStates) {
    
    bool oneNodeIsAlreadySendingData = false;
    bool collision = false;
    for ( vector<NodeState>::iterator it = nodeStates.begin(); it != nodeStates.end(); it++ ) {
        
        if ( *it == eSendingData ) {
            
            if ( !oneNodeIsAlreadySendingData ) {
                oneNodeIsAlreadySendingData = true;
            } else {
                *it = eCollision;
                collision = true;
                break;
            }
        }
    }
    
    if ( collision ) {
        for ( vector<NodeState>::iterator it = nodeStates.begin(); it != nodeStates.end(); it++ ) {
            if ( *it == eSendingData ) {
                *it = eCollision;
            }
        }
    }
}

void CSMA::sendDataThroughNode(int nodeNumber, int size) {
    
    //updating the member variables. These values will be used later on by the update method.
    nodeNumberWhichSentData = nodeNumber;
    sizeOfDataWhichWasSent = size;
}


void CSMA::updateNodesStateAndCopyToVector(vector<vector<NodeState>> &nodes) {
    
    vector<NodeState> currentStates;
    
    if (backOffTimeForNodes.size()==0) {
        for ( int i = 0; i < nodes.size(); i++ ) {
            backOffTimeForNodes.push_back(0);
        }
    }
    if (ackWaitingTimeForNodes.size()==0) {
        for ( int i = 0; i < nodes.size(); i++ ) {
            ackWaitingTimeForNodes.push_back(0);
        }
    }
    
    //Setting all states to idle for now
    for ( int it = 0; it <nodes.size(); it++) {
        currentStates.push_back(eIdle);
    }

    for ( int i = 0; i < nodes.size(); i++ ) {
        
        unsigned long nodeStateSize = nodes[i].size();

        NodeState latestNodeState;
        if ( nodeStateSize > 1) {
            latestNodeState = nodes[i][nodeStateSize-1];
        } else {
            latestNodeState = eIdle;
        }
        
        switch (latestNodeState) {
            
            case eIdle:
            {
                currentStates[i] = eIdle;
                break;
            }
                
            case eSensing:
            {
                bool needToBackOff = false;
                
                for ( int j = 0; j < nodes.size(); j++ ) {
                    if ( j!=i && nodes[j][nodes[j].size()-1] != eIdle ){
                        needToBackOff = true;
                        break;
                    }
                }
                
                if ( needToBackOff ) {
                    currentStates[i] = eBackOff;
                    backOffTimeForNodes[i] = rand() % (MAX_BACKOFF_TIME - 1);
                } else {
                    currentStates[i] = eSendingData;
                }
                
                break;
            }
        
            case eBackOff:
            {
                backOffTimeForNodes[i]--;
                if (backOffTimeForNodes[i] == 0) {
                    currentStates[i] = eSendingData;
                    collisionCheck(currentStates);
                } else {
                    currentStates[i] = eBackOff;
                }
                
                break;
            }
                
            case  eAckReceived:
            {
                currentStates[i] = eIdle;
                break;
            }
                
            case eCollision:
            {
                currentStates[i] = eIdle;
                break;
            }
                
            case eSendingData:
            {
                currentStates[i] = eWaitingForAck;
                ackWaitingTimeForNodes[i] = rand() % (MAX_ACK_TIME - 1);
                break;
            }
                
            case eWaitingForAck:
            {
                ackWaitingTimeForNodes[i]--;
                
                if ( ackWaitingTimeForNodes[i] == 0 ) {
                    currentStates[i] = eAckReceived;
                    nodeOccupiedByChannel = -1;
                } else {
                    currentStates[i] = eWaitingForAck;
                }

                break;
            }
                
            default:
                break;
        }
    }
  
    //Setting the user chosen node to sensing mode.
    if ( nodeNumberWhichSentData != -1) {
        nodeOccupiedByChannel = nodeNumberWhichSentData;
        nodeNumberWhichSentData = -1;
        currentStates[nodeOccupiedByChannel] = eSensing;
    }


    //Appending the new values to the earlier arrays.
    for ( int count = 0; count < nodes.size(); count++ ) {
        
        nodes[count].push_back(currentStates[count]);
        
        if ( nodes[count].size() >= MAX_ARRAY_SIZE ) {
            nodes[count].erase(nodes[count].begin());
        }
    }
    
}

