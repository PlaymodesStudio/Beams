//
//  oscSender.h
//  Beams_Controlls
//
//  Created by Eduard Frigola Bagué on 14/11/2018.
//

#ifndef oscSender_h
#define oscSender_h

#include "ofxOceanodeNodeModel.h"
#include "ofxOsc.h"

class oscSender : public ofxOceanodeNodeModel{
public:
    oscSender() : ofxOceanodeNodeModel("Osc Sender"){
        sender = nullptr;
    };
    ~oscSender(){};
    
    void setup() override;
    void update(ofEventArgs &a) override;
private:
    void setupSender(){
        if(sender != nullptr){
            delete sender;
        }
        sender = new ofxOscSender();
        sender->setup(host, ofToInt(port));
    }
    
    ofEventListeners listeners;
    
    ofParameter<string> host;
    ofParameter<string> port;
    ofParameter<vector<float>> pan;
    ofParameter<vector<float>> tilt;
    vector<ofParameter<vector<float>>> colors;
    
    
    
    ofxOscSender* sender;
};

#endif /* oscSender_h */
