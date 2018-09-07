//
//  colorGradient.h
//  Beams_Controlls
//
//  Created by Eduard Frigola on 07/09/2018.
//

#ifndef colorGradient_h
#define colorGradient_h

#include "ofxOceanodeNodeModel.h"

class colorGradient : public ofxOceanodeNodeModel{
public:
    colorGradient();
    ~colorGradient(){};
    
    void setup() override;
    
private:
    ofEventListener listener;
    
    ofParameter<float> r0;
    ofParameter<float> g0;
    ofParameter<float> b0;
    
    ofParameter<float> r1;
    ofParameter<float> g1;
    ofParameter<float> b1;
    
    ofParameter<vector<float>> indexs;
    
    ofParameter<vector<float>> rOut;
    ofParameter<vector<float>> gOut;
    ofParameter<vector<float>> bOut;
};

#endif /* colorGradient_h */
