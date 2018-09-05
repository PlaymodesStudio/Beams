//
//  midiGateIn.h
//  MIRABCN_Generator
//
//  Created by Eduard Frigola on 14/07/2017.
//
//

#ifndef midiGateIn_h
#define midiGateIn_h

#include "ofxOceanodeNodeModel.h"
#include "ofxMidi.h"

class midiGateIn : public ofxOceanodeNodeModel, ofxMidiListener {
public:
    midiGateIn();
    ~midiGateIn(){};
    
    void update(ofEventArgs &e) override;
    
private:
    void newMidiMessage(ofxMidiMessage& eventArgs);
    void midiDeviceListener(int &device);
    void noteRangeChanged(int &note);
    
    ofEventListeners listeners;
    
    ofParameter<int>    midiDevice;
    ofParameter<int>    midiChannel;
    ofParameter<int>    noteOnStart;
    ofParameter<int>    noteOnEnd;
    
    ofParameter<vector<float>> output;
    
    ofxMidiIn   midiIn;
    vector<float>   outputStore;
    ofMutex mutex;
};

#endif /* midiGateIn_h */
