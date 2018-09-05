//
//  pantiltEditor.h
//  Espills_controller
//
//  Created by Eduard Frigola Bagué on 30/07/2018.
//

#ifndef pantiltEditor_h
#define pantiltEditor_h


#include "ofxOceanodeNodeModelLocalPreset.h"

class panTiltEditor : public ofxOceanodeNodeModelLocalPreset{
 public:
    panTiltEditor() : ofxOceanodeNodeModelLocalPreset("Pan Tilt Editor"){};
    ~panTiltEditor(){};
    
    void setup() override;
    
    void presetHasLoaded() override{
        panOutput = panOutput;
        tiltOutput = tiltOutput;
    }
    
private:
    void computePans();
    void computeTilts();
    
    ofEventListeners listeners;
    
    vector<ofParameter<float>> pans;
    vector<ofParameter<float>> tilts;
    ofParameter<vector<float>> panOutput;
    ofParameter<vector<float>> tiltOutput;
};

#endif /* pantiltEditor_h */
