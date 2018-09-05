//
//  vectorPresetEaser.cpp
//  Espills_controller
//
//  Created by Eduard Frigola Bagué on 05/07/2018.
//

#include "vectorPresetEaser.h"

vectorPresetEaser::vectorPresetEaser() : ofxOceanodeNodeModel("Vector Preset Easer"){
    parameters->add(time.set("time", 0.5, 0, 5));
    addParameterToGroupAndInfo(input.set("Input", {0}, {0}, {1}));//.isSavePreset = false;
    addOutputParameterToGroupAndInfo(output.set("Output", {0}, {0}, {1}));
    
    listener = input.newListener([this](vector<float> &vf){
        if(!isTweening){
            output = vf;
        }
    });
}

void vectorPresetEaser::update(ofEventArgs &a){
    if(isTweening){
        timeElapsed = ofGetElapsedTimef() - timeSincePreset;
        if(timeElapsed >= time){
            isTweening = false;
            output = input;
            return;
        }
        if(lastInputStore.size() == input.get().size()){
            float val = ofxeasing::map(timeElapsed, 0, time, 0, 1, ofxeasing::linear::easeNone);
            vector<float> tempOut = lastInputStore;
            for(int i = 0; i < lastInputStore.size(); i++){
                tempOut[i] = (input.get()[i] * val) + (lastInputStore[i] * (1-val));
            }
            output = tempOut;
        }
        else{
            output = input;
        }
    }
}

void vectorPresetEaser::presetWillBeLoaded(){
    lastInputStore = output;
    isTweening = true;
}

void vectorPresetEaser::presetHasLoaded(){
    timeSincePreset = ofGetElapsedTimef();
}

