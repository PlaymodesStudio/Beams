//
//  valueEaser.cpp
//  Espills_controller
//
//  Created by Eduard Frigola Bagué on 05/07/2018.
//

#include "valueEaser.h"

valueEaser::valueEaser() : ofxOceanodeNodeModel("Value Easer"){
    parameters->add(phasor.set("Phasor In", {0}, {0}, {1}));
    parameters->add(input.set("Input", {0}, {0}, {1}));
    parameters->add(output.set("Output", {0}, {0}, {1}));
    
    listener = phasor.newListener([this](vector<float> &vf){
        if(input.get().size() != lastInput.size() || input.get().size() != phasorValueOnValueChange.size()){
            output = input;
            lastInput = input;
            lastChangedValue = input;
            phasorValueOnValueChange = (vf.size() == 1) ? vector<float>(input.get().size(), vf[0]) : vf;
            lastPhase = vector<float>(input.get().size(), 0);
            reachedMax = vector<bool>(input.get().size(), false);
        }else{
            int inputSize = input.get().size();
            for(int i = 0; i < inputSize; i++){
                if(lastInput[i] != input.get()[i]){
                    phasorValueOnValueChange[i] = (vf.size() < inputSize) ? vf[0] : vf[i];
                    reachedMax[i] = false;
                    lastPhase[i] = 0;
                    lastChangedValue[i] = output.get()[i];
                }
            }
            vector<float> tempOutput = input.get();
            for(int i = 0; i < inputSize; i++){
                float phase = ((vf.size() < inputSize) ? vf[0] : vf[i]) - phasorValueOnValueChange[i];
                if(phase < 0) phase = 1+phase;
                if(phase < lastPhase[i]) reachedMax[i] = true;
                else lastPhase[i] = phase;
                if(!reachedMax[i]){
                    tempOutput[i] = ofxeasing::map(phase, 0, 1, lastChangedValue[i], input.get()[i], ofxeasing::quad::easeInOut);
                }else{
                    tempOutput[i] = input.get()[i];
                }
            }
            lastInput = input;
            output = tempOutput;
            //ofLog() << "LastChangedVal = " << lastChangedValue[0] << "  LastInput = " << lastInput[0] << "  Input = " << input.get()[0];
        }
    });
}
