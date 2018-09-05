//
//  vectorPresetEaser.h
//  Espills_controller
//
//  Created by Eduard Frigola Bagué on 05/07/2018.
//

#ifndef vectorPresetEaser_h
#define vectorPresetEaser_h

#include "ofxOceanodeNodeModel.h"
#include "ofxEasing.h"

class vectorPresetEaser : public ofxOceanodeNodeModel{
public:
    vectorPresetEaser();
    ~vectorPresetEaser(){};
    
    void update(ofEventArgs &a) override;
    void presetWillBeLoaded() override;
    void presetHasLoaded() override;
    
private:
    ofParameter<vector<float>> input;
    ofParameter<vector<float>> output;
    ofParameter<float> time;
    
    vector<float> lastInputStore;
    
    bool isTweening;
    float timeElapsed;
    float timeSincePreset;
    ofEventListener listener;
};

#endif /* vectorPresetEaser_h */
