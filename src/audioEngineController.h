//
//  audioEngineController.h
//  MIRABCN_Generator
//
//  Created by Eduard Frigola on 26/07/2017.
//
//

#ifndef audioEngineController_h
#define audioEngineController_h

#include "ofxOceanodeNodeModel.h"
#include "ofxOsc.h"

class audioEngineController : public ofxOceanodeNodeModel {
public:
    audioEngineController();
    ~audioEngineController(){};
    
private:
    void oscHostAndPortListener(string &s);
    
    void presetNumSender(int &i);
    
    ofEventListeners listeners;
        
    ofParameter<string> oscHost;
    ofParameter<string> oscPort;
    ofParameter<int>    presetNum;
    vector<ofParameter<vector<float>>> params;
    
    ofxOscSender oscSender;
};

#endif /* audioEngineController_h */
