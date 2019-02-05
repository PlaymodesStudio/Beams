//
//  manualOscillatorBank.h
//  MIRABCN_Generator
//
//  Created by Eduard Frigola on 30/08/2017.
//
//

#ifndef manualOscillatorBank_h
#define manualOscillatorBank_h

#include "baseIndexer.h"

class manualOscillatorBank: public baseIndexer{
public:
    manualOscillatorBank();
    ~manualOscillatorBank(){};
    
    void presetRecallBeforeSettingParameters(ofJson &json) override;
    
private:
    void computeValues(float &f);
    
    ofEventListener phasorInEvent;
    ofEventListener manualInEvent;
    
    ofParameter<float>  manualInput;
    deque<float>        buffer;
    deque<pair<float, float>>    indexedBuffer;
    int bufferOverflow;
    float               bufferIndex;
    ofParameter<float>  phasorIn;
    float               oldPhasor;
    ofParameter<float>  damping;
    ofParameter<float>  dampingPow;
    ofParameter<vector<float>>  output;
    
    bool isFirstInput;
};

#endif /* manualOscillatorBank_h */
