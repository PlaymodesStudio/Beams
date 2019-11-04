//
//  strobeLightController.cpp
//  Espills_controller
//
//  Created by Eduard Frigola on 14/08/2018.
//

#include "strobeLightController.h"
#define NUM_OUTPUTS 6

void strobeLightController::setup(){
//    parameters->add(lightType.set("Light Type", 0, 0, 1));
    parameters->add(numElements.set("Num Elements", 6, 1, 1000));
    parameters->add(red.set("Red", {1}, {0}, {1}));
    parameters->add(green.set("Green", {1}, {0}, {1}));
    parameters->add(blue.set("Blue", {1}, {0}, {1}));
    parameters->add(saturate.set("Saturate", {0}, {0}, {1}));
    parameters->add(fader.set("Fader", {1}, {0}, {1}));
    parameters->add(strobeRate.set("Strobe Rate", {0}, {0}, {1}));
    parameters->add(strobeWidth.set("Pulse Width", {1}, {0}, {1}));
    parameters->add(masterFader.set("Master Fader", 1, 0, 1));
    parameters->add(colorOutput.set("Color Output", {0}, {0}, {1}));
    dmxOutputs.resize(NUM_OUTPUTS);
    for(int i = 0; i < NUM_OUTPUTS; i++){
        parameters->add(dmxOutputs[i].set("Dmx Output " + ofToString(i), {0}, {0}, {1}));
    }
}

void strobeLightController::update(ofEventArgs &e){
    auto getValueForPosition([&](ofParameter<vector<float>> &p, int pos) -> float{
        if(pos < p->size()){
            return p->at(pos);
        }
        return p->at(0);
    });
    
    int elementSize = 12;
    int elementsPerUniverse = 6;
    int fixtureSize = 79;

//    switch (lightType) {
//        case 0:
//            elementSize = 4;
//            break;
//        case 1:
//            elementSize = 4;
//            break;
//
//        default:
//            break;
//    }
    
    
    
    vector<vector<float>> tempOutputs(NUM_OUTPUTS);
    vector<float> tempColors;
    
    for(int i = 0; i < NUM_OUTPUTS; i++){
        tempOutputs[i].reserve(elementsPerUniverse*fixtureSize);
    }
    
    tempColors.resize(numElements*3*12);
    vector<float> black = {0,0};
    vector<float> fixturePrefs = {masterFader.get(), 0, 0, 0, 0, 0, 0};
    if(fader->size() == elementSize*numElements){
        
    }else if(fader->size() == elementSize*numElements*3){
        for(int i = 0; i < numElements; i++){
            int univers = i/6;
            for(int j = 0; j < elementSize; j++){
                int index = (i*elementSize*3)+(j*3);
                vector<float> rgbw(fader->begin()+index, fader->begin()+index+4);
                rgbToRgbw(rgbw[0], rgbw[1], rgbw[2], rgbw[3], false);
                tempOutputs[univers].insert(tempOutputs[univers].end(), rgbw.begin(), rgbw.end());
                tempOutputs[univers].insert(tempOutputs[univers].end(), black.begin(), black.end());
            }
            
            tempOutputs[univers].insert(tempOutputs[univers].end(), fixturePrefs.begin(), fixturePrefs.end());
        }
        for(int i = 0; i < NUM_OUTPUTS; i++){
            dmxOutputs[i] = tempOutputs[i];
        }
    }
    
    
    
//    for(int i = 0; i < numElements; i++){
//        float posSaturate = getValueForPosition(saturate, i);
//        float posFader = getValueForPosition(fader, i);
//
//        float red_ = ((getValueForPosition(red, i) * (1-posSaturate)) + (1 * posSaturate)) * posFader * masterFader;
//        float green_ = ((getValueForPosition(green, i) * (1-posSaturate)) + (1 * posSaturate)) * posFader * masterFader;
//        float blue_ = ((getValueForPosition(blue, i) * (1-posSaturate)) + (1 * posSaturate)) * posFader * masterFader;
//
//        tempColors[(i*3)] = red_;
//        tempColors[(i*3)+1] = green_;
//        tempColors[(i*3)+2] = blue_;
//
////        switch (lightType) {
////            case 0:
//                tempOutput[(i*elementSize)] = red_;
//                tempOutput[(i*elementSize)+1] = green_;
//                tempOutput[(i*elementSize)+2] = blue_;
//                tempOutput[(i*elementSize)+3] = 0;
////                break;
////            case 1:
////                tempOutput[(i*elementSize)] = red_;
////                tempOutput[(i*elementSize)+1] = green_;
////                tempOutput[(i*elementSize)+2] = blue_;
////                tempOutput[(i*elementSize)+3] = 0;
////                break;
////
////            default:
////                break;
////        }
//    }
//    dmxOutput = tempOutput;
    colorOutput = tempColors;
}


void strobeLightController::rgbToRgbw(float &r, float &g, float &b, float &w, bool clamp){
    float tM = max(r, max(g, b));
    if(tM == 0){
        return;
    }
    
    //This section serves to figure out what the color with 100% hue is
    float multiplier = 1.0f/tM;
    float hR = r * multiplier;
    float hG = g * multiplier;
    float hB = b * multiplier;
    
    //This calculates the Whiteness (not strictly speaking Luminance) of the color
    float M = max(hR, max(hG, hB));
    float m = min(hR, min(hG, hB));
    float Luminance = ((M + m) / 2.0f - 0.5f) * (2.0f) / multiplier;
    
    if(clamp){
        if(r == g && r == b && g == b){
            Luminance = r;
        }else{
            Luminance = 0;
        }
    }
    
    //Calculate the output values
    w = Luminance;
    r -= Luminance;
    g -= Luminance;
    b -= Luminance;
    return;
}
