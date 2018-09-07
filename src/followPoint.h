//
//  followPoint.h
//  Beams_Controlls
//
//  Created by Eduard Frigola on 07/09/2018.
//

#ifndef followPoint_h
#define followPoint_h

#include "ofxOceanodeNodeModelExternalWindow.h"

class followPoint : public ofxOceanodeNodeModelExternalWindow{
public:
    followPoint() : ofxOceanodeNodeModelExternalWindow("Follow Point"){};
    ~followPoint(){};
    
    void setup() override;
    void update(ofEventArgs &a) override;
    
private:
    
    void drawInExternalWindow(ofEventArgs &a) override;
    void showExternalWindow(bool &b);
    
    void keyPressed(ofKeyEventArgs &a) override;
    
    ofParameter<vector<float>> pointsX;
    ofParameter<vector<float>> pointsY;
    ofParameter<vector<float>> pointsZ;
    
    ofParameter<vector<float>> panOut1;
    ofParameter<vector<float>> tiltOut1;
    ofParameter<vector<float>> panOut2;
    ofParameter<vector<float>> tiltOut2;
    
    vector<glm::vec3> points;
    ofEasyCam cam;
};

#endif /* followPoint_h */
