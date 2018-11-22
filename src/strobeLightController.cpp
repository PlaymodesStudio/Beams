//
//  strobeLightController.cpp
//  Espills_controller
//
//  Created by Eduard Frigola on 14/08/2018.
//

#include "strobeLightController.h"

void strobeLightController::setup(){
    parameters->add(lightType.set("Light Type", 0, 0, 1));
    parameters->add(numElements.set("Num Elements", 4, 0, 1000));
    parameters->add(red.set("Red", {1}, {0}, {1}));
    parameters->add(green.set("Green", {1}, {0}, {1}));
    parameters->add(blue.set("Blue", {1}, {0}, {1}));
    parameters->add(saturate.set("Saturate", {0}, {0}, {1}));
    parameters->add(fader.set("Fader", {1}, {0}, {1}));
    parameters->add(strobeRate.set("Strobe Rate", {0}, {0}, {1}));
    parameters->add(strobeWidth.set("Pulse Width", {1}, {0}, {1}));
    parameters->add(masterFader.set("Master Fader", 1, 0, 1));
    parameters->add(dmxOutput.set("Dmx Output", {0}, {0}, {1}));
    parameters->add(colorOutput.set("Color Output", {0}, {0}, {1}));
}

void strobeLightController::update(ofEventArgs &e){
    auto getValueForPosition([&](ofParameter<vector<float>> &p, int pos) -> float{
        if(pos < p->size()){
            return p->at(pos);
        }
        return p->at(0);
    });
    
    int elementSize = 0;
    
    switch (lightType) {
        case 0:
            elementSize = 4;
            break;
        case 1:
            elementSize = 4;
            break;
            
        default:
            break;
    }
    
    vector<float> tempOutput;
    vector<float> tempColors;
    
    tempOutput.resize(numElements*elementSize);
    tempColors.resize(numElements*3);
    
    for(int i = 0; i < numElements; i++){
        float posSaturate = getValueForPosition(saturate, i);
        float posFader = getValueForPosition(fader, i);
        
        float red_ = ((getValueForPosition(red, i) * (1-posSaturate)) + (1 * posSaturate)) * posFader * masterFader;
        float green_ = ((getValueForPosition(green, i) * (1-posSaturate)) + (1 * posSaturate)) * posFader * masterFader;
        float blue_ = ((getValueForPosition(blue, i) * (1-posSaturate)) + (1 * posSaturate)) * posFader * masterFader;
        
        tempColors[(i*3)] = red_;
        tempColors[(i*3)+1] = green_;
        tempColors[(i*3)+2] = blue_;
        
        switch (lightType) {
            case 0:
                tempOutput[(i*elementSize)] = red_;
                tempOutput[(i*elementSize)+1] = green_;
                tempOutput[(i*elementSize)+2] = blue_;
                tempOutput[(i*elementSize)+3] = 0;
                break;
            case 1:
                tempOutput[(i*elementSize)] = red_;
                tempOutput[(i*elementSize)+1] = green_;
                tempOutput[(i*elementSize)+2] = blue_;
                tempOutput[(i*elementSize)+3] = 0;
                break;
                
            default:
                break;
        }
    }
    dmxOutput = tempOutput;
    colorOutput = tempColors;
}
