
#ifndef _OF_APP
#define _OF_APP


#include "ofMain.h"

#include "ofxXmlSettings.h"
#include "ofxVectorGraphics.h"
#include "quadWarper.h"


class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void mousePressed(int x, int y, int button);
    void keyPressed(int key);
    
    void readFile();
    void saveFile();
    
    ofPoint des[4];
    int clickCount;
    
    
    ofxXmlSettings settings;
    
    quadWarper quadVideo;
    quadWarper quadVideo2;
    
    ofFbo fboFull;
};  

#endif