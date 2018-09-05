//
//  pantiltEditor.cpp
//  Espills_controller
//
//  Created by Eduard Frigola Bagué on 30/07/2018.
//

#include "pantiltEditor.h"


void panTiltEditor::setup(){
    for(int i = 0; i < 32; i++){
        pans.emplace_back();
        parameters->add(pans.back().set("Pan " + ofToString(i), .5, 0, 1));
        listeners.push(pans.back().newListener([this](float &f){
            computePans();
        }));
        
        tilts.emplace_back();
        parameters->add(tilts.back().set("Tilt " + ofToString(i), .5, 0, 1));
        listeners.push(tilts.back().newListener([this](float &f){
            computeTilts();
        }));
    }
    addOutputParameterToGroupAndInfo(panOutput.set("Pan Output", {.5}, {0}, {1}));
    addOutputParameterToGroupAndInfo(tiltOutput.set("Tilt Output", {.5}, {0}, {1}));
    
    listeners.push(panOutput.newListener([this](vector<float> &vf){
        for(auto &f : pans){
            f = vf[0];
        }
        computePans();
    }));
    
    listeners.push(tiltOutput.newListener([this](vector<float> &vf){
        for(auto &f : tilts){
            f = vf[0];
        }
        computeTilts();
    }));
    
    computePans();
    computeTilts();
}

void panTiltEditor::computePans(){
    vector<float> tempOutput;
    tempOutput.resize(pans.size());
    for(int i = 0; i < pans.size(); i++){
        tempOutput[i] = pans[i];
    }
    panOutput = tempOutput;
}

void panTiltEditor::computeTilts(){
    vector<float> tempOutput;
    tempOutput.resize(tilts.size());
    for(int i = 0; i < tilts.size(); i++){
        tempOutput[i] = tilts[i];
    }
    tiltOutput = tempOutput;
}
