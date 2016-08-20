//
//  quadWarper.cpp
//  Affine Addon
//
//  Created by Neil van Vuuren on 18/08/2016.
//
//

#include "quadWarper.h"

quadWarper::quadWarper(){
    ofAddListener(ofEvents().mouseMoved, this, &quadWarper::mouseMoved);
    ofAddListener(ofEvents().mousePressed, this, &quadWarper::mousePressed);
    ofAddListener(ofEvents().mouseDragged, this, &quadWarper::mouseDragged);
    ofAddListener(ofEvents().keyPressed, this, &quadWarper::keyPressed);
    
    cornerHighlight = -1;
    cornerSelected =-1;
    
    b_debug=false;
    b_moveMultiplier=false;
}



quadWarper::~quadWarper(void){

}


void quadWarper::load(string filepath){

    video.load(filepath);

    ofPoint source[]={ofPoint(0,0),ofPoint(video.getWidth(),0),ofPoint(video.getWidth(),video.getHeight()),ofPoint(0,video.getHeight())};
    
    
//    ofPoint destination[]={ofPoint(0,0),ofPoint(video.getWidth(),0),ofPoint(video.getWidth(),video.getHeight()),ofPoint(0,video.getHeight())};

    ofPoint destination[]={ofPoint(0,0),ofPoint(ofGetWidth()/4,0),ofPoint(ofGetWidth()/4,ofGetHeight()/4),ofPoint(0,ofGetHeight()/4)};

    setDestination(destination);
    
}




void quadWarper::load(string filepath, ofPoint position[4]){
    
    video.load(filepath);
    
    ofPoint source[]={ofPoint(0,0),ofPoint(video.getWidth(),0),ofPoint(video.getWidth(),video.getHeight()),ofPoint(0,video.getHeight())};
    
    for (int i=0; i<4; ++i){
        destination[i] = position[i];
    }
    
    setDestination(destination);
    
}


void quadWarper::load(string filepath, int x, int y, int w, int h){
    
    video.load(filepath);
    
    ofPoint source[]={ofPoint(0,0),ofPoint(video.getWidth(),0),ofPoint(video.getWidth(),video.getHeight()),ofPoint(0,video.getHeight())};
    
    destination[0].x=x; //Top Left
    destination[0].y=y;
    
    destination[1].x=x+w; //Top Right
    destination[1].y=y;
    
    destination[2].x=x+w; //Bottom Right
    destination[2].y=y+h;
    
    destination[3].x=x; //Bottom Left
    destination[3].y=y+h;
    
    setDestination(destination);
    
}


void quadWarper::load(string filepath, int TLx, int TLy, int TRx, int TRy, int BRx, int BRy, int BLx, int BLy){
    
    video.load(filepath);
    
    ofPoint source[]={ofPoint(0,0),ofPoint(video.getWidth(),0),ofPoint(video.getWidth(),video.getHeight()),ofPoint(0,video.getHeight())};
    
    destination[0].x=TLx; //Top Left
    destination[0].y=TLy;
    
    destination[1].x=TRx; //Top Right
    destination[1].y=TRy;
    
    destination[2].x=BRx; //Bottom Right
    destination[2].y=BRy;
    
    destination[3].x=BLx; //Bottom Left
    destination[3].y=BLy;
    
    setDestination(destination);
    
}



void quadWarper::updateVideo(){
    video.update();
}

void quadWarper::play(){
    video.play();
}

void quadWarper::setDestination(ofPoint newDestination[4]){
    for (int i=0;i<4;++i){
        destination[i] = newDestination[i];
        
    }
    b_destinationSet = true;
}

ofPoint* quadWarper::getDestination(){
    return destination;
}

void quadWarper::setDebugMode(bool debug){
    b_debug = debug;
}

bool quadWarper::getDebugMode(){
    return b_debug;
}



void quadWarper::setMoveMultiplier(bool multiplier){
    b_moveMultiplier = multiplier;
}

bool quadWarper::getMoveMultiplier(){
    return b_moveMultiplier;
}


void quadWarper::drawVideo(){

        ofPoint src[]={ofPoint(0,0),ofPoint(video.getWidth(),0),ofPoint(video.getWidth(),video.getHeight()),ofPoint(0,video.getHeight())};
        GLfloat matrix[16];
        findHomography(src,destination,matrix);
        glPushMatrix();
        glMultMatrixf(matrix);
        glPushMatrix();
        video.draw(0,0);
        if (b_debug){
            ofDrawBitmapString("IN DEBUG MODE",0,50);
            ofDrawBitmapString("MOVE CORNERS WITH MOUSE OR",0,100);
            ofDrawBitmapString("SELECT CORNER AND USE ARROW KEYS",0,150);
        }
        glPopMatrix();
        glPopMatrix();

    if (b_debug){
        for (int i=0;i<4;++i){
            
            //Show highlighted corner
            
            //Show selected corner
            if (i==cornerSelected){
                if (b_moveMultiplier)
                    ofSetColor(0,68,255);
                else
                    ofSetColor(0,191,255);
            }
            
            else if (i==cornerHighlight){
                ofSetColor(0,255,0);
            }
            else{
                ofSetColor(255,255,255);
            }
            
            
            ofDrawCircle(destination[i],4.0f);
            ofSetColor(255,255,255);
        }
    }

}


void quadWarper::findHomography(ofPoint src[4], ofPoint dst[4],float homography[16]){
    
    // arturo castro - 08/01/2010
    //
    // create the equation system to be solved
    //
    // from: Multiple View Geometry in Computer Vision 2ed
    //       Hartley R. and Zisserman A.
    //
    // x' = xH
    // where H is the homography: a 3 by 3 matrix
    // that transformed to inhomogeneous coordinates for each point
    // gives the following equations for each point:
    //
    // x' * (h31*x + h32*y + h33) = h11*x + h12*y + h13
    // y' * (h31*x + h32*y + h33) = h21*x + h22*y + h23
    //
    // as the homography is scale independent we can let h33 be 1 (indeed any of the terms)
    // so for 4 points we have 8 equations for 8 terms to solve: h11 - h32
    // after ordering the terms it gives the following matrix
    // that can be solved with gaussian elimination:
    
    float P[8][9]={
        {-src[0].x, -src[0].y, -1,   0,   0,  0, src[0].x*dst[0].x, src[0].y*dst[0].x, -dst[0].x }, // h11
        {  0,   0,  0, -src[0].x, -src[0].y, -1, src[0].x*dst[0].y, src[0].y*dst[0].y, -dst[0].y }, // h12
        
        {-src[1].x, -src[1].y, -1,   0,   0,  0, src[1].x*dst[1].x, src[1].y*dst[1].x, -dst[1].x }, // h13
        {  0,   0,  0, -src[1].x, -src[1].y, -1, src[1].x*dst[1].y, src[1].y*dst[1].y, -dst[1].y }, // h21
        
        {-src[2].x, -src[2].y, -1,   0,   0,  0, src[2].x*dst[2].x, src[2].y*dst[2].x, -dst[2].x }, // h22
        {  0,   0,  0, -src[2].x, -src[2].y, -1, src[2].x*dst[2].y, src[2].y*dst[2].y, -dst[2].y }, // h23
        
        {-src[3].x, -src[3].y, -1,   0,   0,  0, src[3].x*dst[3].x, src[3].y*dst[3].x, -dst[3].x }, // h31
        {  0,   0,  0, -src[3].x, -src[3].y, -1, src[3].x*dst[3].y, src[3].y*dst[3].y, -dst[3].y }, // h32
    };
    
    gaussian_elimination(&P[0][0],9);
    
    // gaussian elimination gives the results of the equation system
    // in the last column of the original matrix.
    // opengl needs the transposed 4x4 matrix:
    float aux_H[]={ P[0][8],P[3][8],0,P[6][8], // h11  h21 0 h31
        P[1][8],P[4][8],0,P[7][8], // h12  h22 0 h32
        0      ,      0,0,0,       // 0    0   0 0
        P[2][8],P[5][8],0,1};      // h13  h23 0 h33
    
    for(int i=0;i<16;i++) homography[i] = aux_H[i];
 
}

void quadWarper::gaussian_elimination(float *input, int n){
    // arturo castro - 08/01/2010
    //
    // ported to c from pseudocode in
    // [http://en.wikipedia.org/wiki/Gaussian-elimination](http://en.wikipedia.org/wiki/Gaussian-elimination)
    
    float * A = input;
    int i = 0;
    int j = 0;
    int m = n-1;
    while (i < m && j < n){
        // Find pivot in column j, starting in row i:
        int maxi = i;
        for(int k = i+1; k<m; k++){
            if(fabs(A[k*n+j]) > fabs(A[maxi*n+j])){
                maxi = k;
            }
        }
        if (A[maxi*n+j] != 0){
            //swap rows i and maxi, but do not change the value of i
            if(i!=maxi)
                for(int k=0;k<n;k++){
                    float aux = A[i*n+k];
                    A[i*n+k]=A[maxi*n+k];
                    A[maxi*n+k]=aux;
                }
            //Now A[i,j] will contain the old value of A[maxi,j].
            //divide each entry in row i by A[i,j]
            float A_ij=A[i*n+j];
            for(int k=0;k<n;k++){
                A[i*n+k]/=A_ij;
            }
            //Now A[i,j] will have the value 1.
            for(int u = i+1; u< m; u++){
                //subtract A[u,j] * row i from row u
                float A_uj = A[u*n+j];
                for(int k=0;k<n;k++){
                    A[u*n+k]-=A_uj*A[i*n+k];
                }
                //Now A[u,j] will be 0, since A[u,j] - A[i,j] * A[u,j] = A[u,j] - 1 * A[u,j] = 0.
            }
            
            i++;
        }
        j++;
    }
    
    //back substitution
    for(int i=m-2;i>=0;i--){
        for(int j=i+1;j<n-1;j++){
            A[i*n+m]-=A[i*n+j]*A[j*n+m];
            //A[i*n+j]=0;
        }
    }
}




void quadWarper::mouseMoved(ofMouseEventArgs& event){
    
    if (b_debug){
        cornerHighlight=-1;
        float errorthreshold=10.0f;
        for (int i=0; i<4;i++){
            if (ofDist(destination[i].x, destination[i].y,event.x, event.y)<10){
                cornerHighlight=i;
            }
            
        }
        
    }
}


void quadWarper::mousePressed(ofMouseEventArgs& event){
    if (b_debug){
        cornerSelected = -1;
        float errorthreshold=10.0f;
        for (int i=0; i<4;i++){
            if (ofDist(destination[i].x, destination[i].y,event.x, event.y)<10){
                cornerSelected=i;
            }
            
        }
        
    }
}



void quadWarper::mouseDragged(ofMouseEventArgs& event){
    if (b_debug){
        int cornermoved = -1;
        float errorthreshold=10.0f;
        for (int i=0; i<4;i++){
            if (ofDist(destination[cornerSelected].x, destination[cornerSelected].y,event.x, event.y)<1000){
                destination[cornerSelected].x = event.x;
                destination[cornerSelected].y = event.y;
            }
            
        }
    }
}


void quadWarper::keyPressed(ofKeyEventArgs& event){
    if (b_debug){
                    
        if (cornerSelected>=0){
            switch (event.key){
                    
                case OF_KEY_UP:
                    
                    if (b_moveMultiplier)
                        destination[cornerSelected].y -= 10;
                    else
                        destination[cornerSelected].y -= 1;
                    break;
                case OF_KEY_DOWN:
                    if (b_moveMultiplier)
                        destination[cornerSelected].y += 10;
                    else
                        destination[cornerSelected].y += 1;
                    break;
                case OF_KEY_LEFT:
                    if (b_moveMultiplier)
                        destination[cornerSelected].x -= 10;
                    else
                        destination[cornerSelected].x -= 1;
                    break;
                case OF_KEY_RIGHT:
                    if (b_moveMultiplier)
                        destination[cornerSelected].x += 10;
                    else
                        destination[cornerSelected].x += 1;
                    break;
            }
        }
    }
}



