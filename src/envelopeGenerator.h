//
//  envelopeGenerator.h
//  MIRABCN_Generator
//
//  Created by Eduard Frigola on 12/07/2017.
//
//

#ifndef envelopeGenerator_h
#define envelopeGenerator_h

#include "ofxOceanodeNodeModel.h"
#include "ofxEasing.h"

enum envelopeStages{
    envelopeAttack = 0,
    envelopeDecay = 1,
    envelopeSustain = 2,
    envelopeRelease = 3,
    envelopeEnd = 4
};

class envelopeGenerator : public ofxOceanodeNodeModel{
public:
    envelopeGenerator();
    ~envelopeGenerator(){};
    
    
private:
    void phasorListener(float &f);
    void gateInChanged(vector<float> &vf);
    void attackEnd(float *f);
    ofxeasing::function easingFromString(string easing);
    
    ofEventListener listener;
    
    ofParameter<float> phasor;
    ofParameter<float>  attack;
    ofParameter<float>  decay;
    ofParameter<float>  sustain;
    ofParameter<float>  release;
    
    ofParameter<int>  attackCurve;
    ofParameter<int>  decayCurve;
    ofParameter<int>  releaseCurve;
    
    ofParameter<vector<float>> gateIn;
    ofParameter<vector<float>> output;
    
    vector<float> lastInput;
    vector<float> lastChangedValue;
    vector<float> phasorValueOnValueChange;
    vector<float> lastPhase;
    vector<bool> reachedMax;
    vector<int> envelopeStage;
    
    vector<string> easeStringFuncs;
    
    vector<float>   oldGateIn;
    vector<float>   outputComputeVec;
    
    int lastGateInSize;
};

#endif /* envelopeGenerator_h */
