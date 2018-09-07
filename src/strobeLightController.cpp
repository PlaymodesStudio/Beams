//
//  strobeLightController.cpp
//  Espills_controller
//
//  Created by Eduard Frigola on 14/08/2018.
//

#include "strobeLightController.h"

void strobeLightController::setup(){
    parameters->add(numElements.set("Num Elements", 4, 0, 1000));
    parameters->add(red.set("Red", {1}, {0}, {1}));
    parameters->add(green.set("Green", {1}, {0}, {1}));
    parameters->add(blue.set("Blue", {1}, {0}, {1}));
    parameters->add(saturate.set("Saturate", {0}, {0}, {1}));
    parameters->add(fader.set("Fader", {1}, {0}, {1}));
    parameters->add(strobeRate.set("Strobe Rate", {0}, {0}, {1}));
    parameters->add(strobeWidth.set("Pulse Width", {1}, {0}, {1}));
    parameters->add(masterFader.set("Master Fader", 1, 0, 1));
    parameters->add(output.set("Output", {0}, {0}, {1}));
}

void strobeLightController::update(ofEventArgs &e){
    auto getValueForPosition([&](ofParameter<vector<float>> &p, int pos) -> float{
        if(pos < p->size()){
            return p->at(pos);
        }
        return p->at(0);
    });
    
    int elementSize = 6;
    vector<float> tempOutput;
    tempOutput.resize(numElements*elementSize);
    for(int i = 0; i < numElements; i++){
        float posSaturate = getValueForPosition(saturate, i);
        float posFader = getValueForPosition(fader, i);
        tempOutput[(i*elementSize)] = ((getValueForPosition(red, i) * (1-posSaturate)) + (1 * posSaturate)) * posFader * masterFader;
        tempOutput[(i*elementSize)+1] = ((getValueForPosition(green, i) * (1-posSaturate)) + (1 * posSaturate)) * posFader * masterFader;
        tempOutput[(i*elementSize)+2] = ((getValueForPosition(blue, i) * (1-posSaturate)) + (1 * posSaturate)) * posFader * masterFader;
        tempOutput[(i*elementSize)+3] = 0;
        tempOutput[(i*elementSize)+4] = 0;
        tempOutput[(i*elementSize)+5] = 0;
    }
    output = tempOutput;
}
