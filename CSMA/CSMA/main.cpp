//
//  main.cpp
//  CSMA
//
//  Created by Abhinav KR on 5/1/14.
//  Copyright (c) 2014 Abhinav KR. All rights reserved.
//

#include <iostream>
#include <GLUT/glut.h>
#include "unistd.h"
#include "csma.h"

#define DELAY_IN_SECONDS 1
#define STATE_RECT_WIDTH 20 //Width of each rectangle
#define STATE_RECT_HEIGHT 40 //Height of each rectangle
#define GAP_BETWEEN_RECTS 15
#define STATE_ARRAY_X 60 //The X-Coordinate where the ractangles start

typedef struct  {
    GLfloat red;
    GLfloat blue;
    GLfloat green;
}Color;

void display(void);
void reshape(int, int);
void drawRectangle (int x, int y, int w, int h, Color *color);
void drawRectanglesForVector ( vector<NodeState> &nodeStates, int y );
void getColorForNodeState ( Color *color, NodeState nodeState);
void drawText ( int x, int y, Color *color, string text);

int main(int argc, char** argv) {
    
    glutInit(&argc, argv);
    glutInitWindowSize(512, 512);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutCreateWindow("CSMA");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glClearColor(0,0,0,1);
    glutMainLoop();
    return 0;
}

void display() {
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    CSMA *csma = new CSMA();
    
    Color rectColor;
    
    getColorForNodeState( &rectColor, eSensing);

    vector<NodeState> node1;
    vector<NodeState> node2;
    vector<NodeState> node3;
    vector<NodeState> node4;
    node1.push_back(eIdle);
    node2.push_back(eIdle);
    node3.push_back(eIdle);
    node4.push_back(eIdle);

    vector<vector<NodeState>> nodes;
    nodes.push_back(node1);
    nodes.push_back(node2);
    nodes.push_back(node3);
    nodes.push_back(node4);
    
    int count  = 0 ;
    
    while ( DELAY_IN_SECONDS ) {
        
        /* Instead of User input, I am simulating the input values here.  Feel free to send data to any node at any point of time*/
        if ( count ++ == 3) {
            csma->sendDataThroughNode(2, 1);
        }
        if (count == 5) {
            csma->sendDataThroughNode(3, 1);
        }
        if ( count == 2) {
            csma->sendDataThroughNode(0, 1);
        }
        
        csma->updateNodesStateAndCopyToVector(nodes);
        
        drawRectanglesForVector(nodes[0], 300);
        
        glRasterPos2i(STATE_ARRAY_X - 50, 350+20);

        drawRectanglesForVector(nodes[1], 350);

        drawRectanglesForVector(nodes[2], 400);

        drawRectanglesForVector(nodes[3], 450);
        
        Color color;
        getColorForNodeState(&color, eCollision);
        
        string text1 = "Node 0";
        drawText(STATE_ARRAY_X - 50, 320, &color, text1);
        text1 = "Node 1";
        drawText(STATE_ARRAY_X - 50, 370, &color, text1);
        text1 = "Node 2";
        drawText(STATE_ARRAY_X - 50, 420, &color, text1);
        text1 = "Node 3";
        drawText(STATE_ARRAY_X - 50, 470, &color, text1);
        
        if ( count == 10)
            break;

        sleep(1);
    }
    
    glutSwapBuffers();
}

void reshape(int width, int height) {

    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,width,0,height);
    glMatrixMode(GL_MODELVIEW);
}

void drawText ( int x, int y, Color *color, string text) {
    
    glPushMatrix();
    glLoadIdentity();
    glColor3f(color->red,color->blue,color->green);
    glRasterPos2i(x, y);
    string s = text;
    void * font = GLUT_BITMAP_9_BY_15;
    for (string::iterator i = s.begin(); i != s.end(); ++i)
    {
        char c = *i;
        glColor3d(1.0, 0.0, 0.0);
        glutBitmapCharacter(font, c);
    }
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void drawRectangle (int x, int y, int w, int h, Color *color) {
    
    glColor3f(color->red, color->green, color->blue);
    glBegin(GL_TRIANGLES);
    
    glVertex2f(x, y);
    glVertex2f(x+w, y);
    glVertex2f(x, y+h);
    
    glBegin(GL_TRIANGLES);
    
    glVertex2f(x+w, y+h);
    glVertex2f(x+w, y);
    glVertex2f(x, y+h);
    
    glEnd();
}

void drawRectanglesForVector ( vector<NodeState> &nodeStates, int y ) {
    
    for ( int i = 0; i < nodeStates.size(); i++ ) {
        Color color;
        getColorForNodeState(&color, nodeStates[i]);
        drawRectangle(STATE_ARRAY_X + ((1+i)*(STATE_RECT_WIDTH + GAP_BETWEEN_RECTS)) , y, STATE_RECT_WIDTH, STATE_RECT_HEIGHT, &color);
    }
}

void getColorForNodeState ( Color* color, NodeState nodeState) {
    
    switch (nodeState) {
        case eIdle:
        {
            //Grey
            color->red = 0.7f;
            color->blue = 0.7f;
            color->green = 0.7f;
            
            break;
        }
        case eSensing:
        {
            //White
            color->red = 1.0f;
            color->blue = 1.0f;
            color->green = 1.0f;
            
            break;
        }
        case eBackOff:
        {
            //Blue
            color->red = 0.0f;
            color->blue = 1.0f;
            color->green = 0.0f;
            
            break;
        }
        case eAckReceived:
        {
            //Light Yellow
            color->red = 0.88f;
            color->blue = 0.61f;
            color->green = 0.92f;
            
            break;
        }
        case eCollision:
        {
            //Red
            color->red = 1.0f;
            color->blue = 0.0f;
            color->green = 0.0f;
            
            break;
        }
        case eSendingData:
        {
            //Green
            color->red = 0.0f;
            color->blue = 0.0f;
            color->green = 1.0f;
            
            break;
        }
        case eWaitingForAck:
        {
            //Purple
            color->red = 1.00f;
            color->blue = 1.00f;
            color->green = 0.00f;
            
            break;
        }
            
        default:
            break;
    }
    
}

