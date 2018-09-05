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
        tempOutput[index] = colorwheel->at(0);
        //strobe
        tempOutput[index+1] = strobe;
        //dimmer
        if(intensity.get().size() > i){
            tempOutput[index+2] = intensity.get()[i];
        }
        //gobo
        tempOutput[index+3] = gobo;
        //frost
        tempOutput[index+7] = frost;
        //focus
        tempOutput[index+8] = focus;
        if(pan.get().size() > i){
            tempOutput[index+9] = pan.get()[i];
            tempOutput[index+10] = pan.get()[i]*255 - int(pan.get()[i]*255);
        }
        if(tilt.get().size() > i){
            tempOutput[index+11] = tilt.get()[i];
            tempOutput[index+12] = tilt.get()[i]*255 - int(tilt.get()[i]*255);
        }
        //lamp
        tempOutput[index+15] = 0;
    }
    
    output = tempOutput;
}
