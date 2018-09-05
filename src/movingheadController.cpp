//
//  movingheadController.cpp
//  Espills_controller
//
//  Created by Eduard Frigola Bagué on 02/07/2018.
//

#include "movingheadController.h"

movingheadController::movingheadController() : ofxOceanodeNodeModel("Movinghead Controller"){
    parameters->add(intensity.set("Intensity", {0}, {0}, {1}));
    parameters->add(pan.set("Pan", {0}, {0}, {1}));
    parameters->add(tilt.set("Tilt", {0}, {0}, {1}));
    parameters->add(colorwheel.set("Color", {0}, {0}, {1}));
    parameters->add(strobe.set("Strobe", 0, 0, 1));
    parameters->add(gobo.set("Gobo", 0, 0, 1));
    parameters->add(frost.set("Frost", 0, 0, 1));
    parameters->add(focus.set("Focus", 0, 0, 1));
    addOutputParameterToGroupAndInfo(output.set("Output", {0}, {0}, {1}));
}

void movingheadController::update(ofEventArgs &a){
    vector<float> tempOutput;
    int maxSize = max(pan.get().size(), tilt.get().size());
    tempOutput.resize(16*32, 0);
    
    for(int i = 0; i < 32; i++){
        int index = i*16;
        //color wheel
        tempOutput[index] = getValueAtIndex(colorwheel.get(), index);
        //strobe
        tempOutput[index+1] = strobe;
        //dimmer
        tempOutput[index+2] = getValueAtIndex(intensity.get(), index);
        //gobo
        tempOutput[index+3] = gobo;
        //frost
        tempOutput[index+7] = frost;
        //focus
        tempOutput[index+8] = focus;
        //pan
        float panAtIndex = getValueAtIndex(pan.get(), index);
        tempOutput[index+9] = panAtIndex;
        tempOutput[index+10] = panAtIndex*255 - int(panAtIndex*255);
        //tilt
        float tiltAtIndex = getValueAtIndex(tilt.get(), index);
        tempOutput[index+11] = tiltAtIndex;
        tempOutput[index+12] = tiltAtIndex*255 - int(tiltAtIndex*255);
        //lamp
        tempOutput[index+15] = 0;
    }
    
    output = tempOutput;
}
