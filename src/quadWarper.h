//
//  quadWarper.hpp
//  Affine Addon
//
//  Created by Neil van Vuuren on 18/08/2016.
//
//

#ifndef quadWarper_h
#define quadWarper_h

#include <stdio.h>
#include "ofMain.h"

class quadWarper{

private:
    ofVideoPlayer video;
        
    void findHomography(ofPoint src[4], ofPoint dst[4],float homography[16]);
    void gaussian_elimination(float *input, int n);
    
    int cornerHighlight,cornerSelected;
    
    bool b_debug,b_destinationSet,b_moveMultiplier;
    
    
public:
    quadWarper(); //Constructor
    ~quadWarper(); //Destructor

    
    bool b_setup;
    
    void mouseMoved(ofMouseEventArgs& event);
    void mousePressed(ofMouseEventArgs& event);
    void mouseDragged(ofMouseEventArgs& event);
    void keyPressed(ofKeyEventArgs& event);
    
    
    ofPoint source[4];
    ofPoint destination[4];
    
    void drawVideo();
    void setDebugMode(bool debug);
    bool getDebugMode();
    
    void setMoveMultiplier(bool multiplier);
    bool getMoveMultiplier();
    
    
    
    void load(string filepath);
    void load(string filepath, ofPoint position[4]);
    void load(string filepath, int x, int y, int w, int h);
    void load(string filepath, int TLx, int TLy, int TRx, int TRy, int BRx, int BRy, int BLx, int BLy);
    
    void setDestination(ofPoint newDestination[4]);
    ofPoint* getDestination();
    
    void updateVideo();
    void play();

    


    
};


#endif /* quadWarper_h */
