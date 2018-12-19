//
//  oscReceiver.h
//  Beams_Controlls
//
//  Created by Eduard Frigola Bagué on 16/11/2018.
//

#ifndef oscReceiver_h
#define oscReceiver_h

#include "ofxOceanodeNodeModel.h"
#include "ofxOsc.h"

class oscReceiver : public ofxOceanodeNodeModel {
public:
    oscReceiver() : ofxOceanodeNodeModel("Osc Receiver"){
        receiver = nullptr;
    };
    ~oscReceiver(){
        delete receiver;
    };
    
    void setup() override;
    void update(ofEventArgs &a) override;
    
private:
    void setupReceiver(){
        if(receiver != nullptr){
            delete receiver;
        }
        receiver = new ofxOscReceiver();
        receiver->setup(ofToInt(port));
    }
    
    ofEventListeners listeners;
    
    int movHeadsInputs;
    
    ofParameter<string> port;
    ofParameter<int> size;
//    vector<ofParameter<vector<float>>> pans;
//    vector<ofParameter<vector<float>>> tilts;
//    vector<ofParameter<vector<float>>> intensities;
//    vector<ofParameter<vector<float>>> colors;
//
//    vector<vector<float>> pansStore;
//    vector<vector<float>> tiltsStore;
//    vector<vector<float>> intensitiesStore;
//    vector<vector<float>> colorsStore;
    ofParameter<vector<float>> x;
    ofParameter<vector<float>> y;
    ofParameter<vector<float>> bool1;
    ofParameter<vector<float>> bool2;
    ofParameter<vector<float>> bool3;
    vector<float> xStore;
    vector<float> yStore;
    vector<float> bool1Store;
    vector<float> bool2Store;
    vector<float> bool3Store;
    
    ofxOscReceiver* receiver;
};

#endif /* oscReceiver_h */
