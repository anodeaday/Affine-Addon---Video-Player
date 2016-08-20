
#include "ofApp.h"
#include "quadWarper.h"

void ofApp::setup(){
    
    
    if (settings.loadFile("settings.xml")){
        readFile();
    
    
    }
    else{
        quadVideo.load("video.mp4",100,100,250,250);
        quadVideo2.load("video.mp4",350,100,250,250);
    }
        quadVideo2.play();
        quadVideo.play();
    
    clickCount=0;
    
    fboFull.allocate(ofGetScreenWidth(),ofGetScreenHeight(),GL_RGBA,8);
}
                          
void ofApp::readFile(){
    
    //Video1
    string vid1 =settings.getValue("settings:Video1::vid1", "blah");
    int V1TLx = settings.getValue("settings:Video1:TopLeftx",100);
    int V1TLy = settings.getValue("settings:Video1:TopLefty",100);

    int V1TRx = settings.getValue("settings:Video1:TopRightx",200);
    int V1TRy = settings.getValue("settings:Video1:TopRighty",100);
    
    int V1BLx = settings.getValue("settings:Video1:BottomLeftx",100);
    int V1BLy = settings.getValue("settings:Video1:BottomLefty",200);
    
    int V1BRx = settings.getValue("settings:Video1:BottomRightx",200);
    int V1BRy = settings.getValue("settings:Video1:BottomRighty",200);
    
    
    //Video2
    string vid2 =settings.getValue("settings:Video2:vid2", "blah");
    
    int V2TLx = settings.getValue("settings:Video2:TopLeftx",100);
    int V2TLy = settings.getValue("settings:Video2:TopLefty",100);
    
    int V2TRx = settings.getValue("settings:Video2:TopRightx",200);
    int V2TRy = settings.getValue("settings:Video2:TopRighty",100);
    
    int V2BLx = settings.getValue("settings:Video2:BottomLeftx",100);
    int V2BLy = settings.getValue("settings:Video2:BottomLefty",200);
    
    int V2BRx = settings.getValue("settings:Video2:BottomRightx",200);
    int V2BRy = settings.getValue("settings:Video2:BottomRighty",200);

    
    
    
    quadVideo.load(vid1,V1TLx,V1TLy,V1TRx,V1TRy,V1BRx,V1BRy,V1BLx,V1BLy);
    quadVideo2.load(vid2,V2TLx,V2TLy,V2TRx,V2TRy,V2BRx,V2BRy,V2BLx,V2BLy);
//    quadVideo2.load(vid2,350,200,250,250);
    
    
}

void ofApp::exit(){
    cout << "Exiting..."<<endl;
    saveFile();
}

void ofApp::saveFile(){
    
    settings.clear();
    
    settings.setValue("settings:Video1:vid1", "video.mp4");
    
    ofPoint video1Points[4]; // Video1
    for (int i=0;i<4;i++){
        video1Points[i] =   quadVideo.getDestination()[i];
        cout << video1Points[i]<<endl;
    }
    
    
    settings.setValue("settings:Video1:TopLeftx",video1Points[0].x);
    settings.setValue("settings:Video1:TopLefty",video1Points[0].y);
    
    settings.setValue("settings:Video1:TopRightx",video1Points[1].x);
    settings.setValue("settings:Video1:TopRighty",video1Points[1].y);
    
    settings.setValue("settings:Video1:BottomLeftx",video1Points[3].x);
    settings.setValue("settings:Video1:BottomLefty",video1Points[3].y);
    
    settings.setValue("settings:Video1:BottomRightx",video1Points[2].x);
    settings.setValue("settings:Video1:BottomRighty",video1Points[2].y);
    
    
    settings.setValue("settings:Video2:vid2", "video.mp4");
    
    ofPoint video2Points[4];  // Video 2
    for (int i=0;i<4;i++){
        video2Points[i] =   quadVideo2.getDestination()[i];
        cout << video2Points[i]<<endl;
    }
    
    settings.setValue("settings:Video2:TopLeftx",video2Points[0].x);
    settings.setValue("settings:Video2:TopLefty",video2Points[0].y);
    
    settings.setValue("settings:Video2:TopRightx",video2Points[1].x);
    settings.setValue("settings:Video2:TopRighty",video2Points[1].y);
    
    settings.setValue("settings:Video2:BottomLeftx",video2Points[3].x);
    settings.setValue("settings:Video2:BottomLefty",video2Points[3].y);
    
    settings.setValue("settings:Video2:BottomRightx",video2Points[2].x);
    settings.setValue("settings:Video2:BottomRighty",video2Points[2].y);
    
    
    settings.saveFile("settings.xml");
    
}

                          
void ofApp::update(){
    
    
    quadVideo.updateVideo();
    quadVideo2.updateVideo();
    fboFull.begin();
    ofClear(0,0,0);
    ofBackground(ofColor(0,0,0));
    quadVideo.drawVideo();
    quadVideo2.drawVideo();
    fboFull.end();
}

void ofApp::draw(){
    fboFull.draw(0,0);
}

void ofApp::mousePressed(int x, int y, int button){
    
    
}

void ofApp::keyPressed(int key){
    if (key=='1'){
        if (quadVideo.getDebugMode())
            quadVideo.setDebugMode(false);
        else
            quadVideo.setDebugMode(true);
    }
    if (key=='2'){
        if (quadVideo2.getDebugMode())
            quadVideo2.setDebugMode(false);
        else
            quadVideo2.setDebugMode(true);
    }
    if (key==OF_KEY_SHIFT){
        if (quadVideo.getMoveMultiplier()){
            quadVideo.setMoveMultiplier(false);
            quadVideo2.setMoveMultiplier(false);
        }
        else{
            quadVideo.setMoveMultiplier(true);
            quadVideo2.setMoveMultiplier(true);
        }
    }
}

