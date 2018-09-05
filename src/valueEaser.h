//
//  valueEaser.h
//  Espills_controller
//
//  Created by Eduard Frigola Bagué on 05/07/2018.
//

#ifndef valueEaser_h
#define valueEaser_h

#include "ofxOceanodeNodeModel.h"
#include "ofxEasing.h"

class valueEaser : public ofxOceanodeNodeModel{
public:
    valueEaser();
    ~valueEaser(){};
    
private:
    ofParameter<vector<float>> input;
    ofParameter<vector<float>> phasor;
    ofParameter<int> easingType;
    ofParameter<vector<float>> output;
    
    ofEventListener listener;
    vector<float> lastInput;
    vector<float> lastChangedValue;
    vector<float> phasorValueOnValueChange;
    vector<float> lastPhase;
    vector<bool> reachedMax;
};

#endif /* valueEaser_h */
