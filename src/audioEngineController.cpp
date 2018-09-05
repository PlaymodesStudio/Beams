//
//  audioEngineController.cpp
//  MIRABCN_Generator
//
//  Created by Eduard Frigola on 26/07/2017.
//
//

#include "audioEngineController.h"

audioEngineController::audioEngineController() : ofxOceanodeNodeModel("Audio Controller"){
    addParameterToGroupAndInfo(oscHost.set("Osc Host", "localhost")).isSavePreset = false;
    addParameterToGroupAndInfo(oscPort.set("Osc Port", "11511")).isSavePreset = false;
    parameters->add(presetNum.set("Preset Num", 0, 0, 100));
    //params.resize(7);
    for(int i = 0; i < 7; i++){
        params.push_back(ofParameter<vector<float>>());
        parameters->add(params.back().set("param_" + ofToString(i), {0}, {0}, {1}));
        listeners.push(params.back().newListener([this, i](vector<float> &vf){
            ofxOscMessage message;
            message.setAddress("audio/" + ofToString(i));
            for(auto f : vf){
                message.addFloatArg(f);
            }
            oscSender.sendMessage(message);
        }));
    }
    oscSender.setup(oscHost, ofToInt(oscPort));
    
    listeners.push(presetNum.newListener(this, &audioEngineController::presetNumSender));
    
    listeners.push(oscHost.newListener(this, &audioEngineController::oscHostAndPortListener));
    listeners.push(oscPort.newListener(this, &audioEngineController::oscHostAndPortListener));
}

void audioEngineController::oscHostAndPortListener(string &s){
    oscSender.setup(oscHost, ofToInt(oscPort));
}

void audioEngineController::presetNumSender(int &i){
    ofxOscMessage message;
    message.setAddress("audio/preset");
    message.addIntArg(i);
    oscSender.sendMessage(message);
}

