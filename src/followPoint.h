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
    
    glm::vec2 calculateAlfa(glm::vec3 position, glm::vec3 lookAT, bool orientMode);
    
    ofParameter<vector<float>> pointsX;
    ofParameter<vector<float>> pointsY;
    ofParameter<vector<float>> pointsZ;
    ofParameter<bool> sortPoints;
    ofParameter<bool> randomizePoints;
    
    ofParameter<vector<float>> panL;
    ofParameter<vector<float>> tiltL;
    ofParameter<vector<float>> panR;
    ofParameter<vector<float>> tiltR;
    ofParameter<vector<int>> distribution;
    
    vector<int> randomDist;
    ofEventListeners listeners;
    
    vector<glm::vec3> points;
    ofEasyCam cam;
    
    vector<ofNode> movingHeadsL;
    vector<ofNode> movingHeadsR;
    ofRectangle limits;
    
    int lastSize;
    bool lastRandomizePoints;
};

#endif /* followPoint_h */
