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
    parameters->add(enableWhite.set("Enable White", false));
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
            elementSize = 18;
            break;
        case 1:
            elementSize = 5;
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
        float white_ = 0;
        
        tempColors[(i*3)] = red_;
        tempColors[(i*3)+1] = green_;
        tempColors[(i*3)+2] = blue_;
        
        switch (lightType) {
            case 0:
                if(enableWhite)
                    rgbToRgbw(red_, green_, blue_, white_, true);
                tempOutput[(i*elementSize)] = 1;
                tempOutput[(i*elementSize)+1] = 1;
                tempOutput[(i*elementSize)+2] = 0;
                tempOutput[(i*elementSize)+3] = 0;
                tempOutput[(i*elementSize)+4] = 0;
                tempOutput[(i*elementSize)+5] = 0;
                tempOutput[(i*elementSize)+6] = 0;
                tempOutput[(i*elementSize)+7] = 1;
                tempOutput[(i*elementSize)+8] = red_;
                tempOutput[(i*elementSize)+9] = red_*255 - int(red_*255);
                tempOutput[(i*elementSize)+10] = green_;
                tempOutput[(i*elementSize)+11] = green_*255 - int(green_*255);
                tempOutput[(i*elementSize)+12] = blue_;
                tempOutput[(i*elementSize)+13] = blue_*255 - int(blue_*255);
                tempOutput[(i*elementSize)+14] = white_; //white
                tempOutput[(i*elementSize)+15] = white_*255 - int(white_*255); //white fine
                tempOutput[(i*elementSize)+16] = 1;
                tempOutput[(i*elementSize)+17] = 1;
                break;
            case 1:
                if(enableWhite)
                    rgbToRgbw(red_, green_, blue_, white_, false);
                tempOutput[(i*elementSize)] = red_;
                tempOutput[(i*elementSize)+1] = green_;
                tempOutput[(i*elementSize)+2] = blue_;
                tempOutput[(i*elementSize)+3] = white_;
                tempOutput[(i*elementSize)+4] = 0;
                break;
                
            default:
                break;
        }
    }
    dmxOutput = tempOutput;
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
