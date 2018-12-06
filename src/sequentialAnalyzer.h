//
//  sequentialAnalyzer.h
//  Beams_Controlls
//
//  Created by Eduard Frigola Bagué on 30/11/2018.
//

#ifndef sequentialAnalyzer_h
#define sequentialAnalyzer_h

#include "ofxOceanodeNodeModel.h"

class sequentialAnalyzer : public ofxOceanodeNodeModel{
public:
    sequentialAnalyzer() : ofxOceanodeNodeModel("Sequential Analyzer"){};
    ~sequentialAnalyzer(){};
    
    void setup() override{
        parameters->add(input.set("Input", {0}, {0}, {1}));
        parameters->add(outputSize.set("Output Size", 1, 1, 100));
        parameters->add(floatOutput.set("Float Output", 0, 0, 1));
        parameters->add(pulseOutput.set("Pulse Output", {0}, {0}, {1}));
        
        currentPosition = 0;
        
        listener = input.newListener([this](vector<float> &vf){
            bool inputTriggered = false;
            if(vf.size() != lastInput.size()) lastInput = vf;
            else{
                for(int i = 0; i < vf.size(); i++){
                    if(vf[i] != 0 && lastInput[i] == 0){
                        inputTriggered = true;
                        break;
                    }
                }
                lastInput = vf;
            }
            if(inputTriggered){
                currentPosition++;
                currentPosition %= outputSize;
                floatOutput = (float)currentPosition/(float)outputSize;
                vector<float> tempPulseOutput(outputSize, 0);
                tempPulseOutput[currentPosition] = 1;
                pulseOutput = tempPulseOutput;
            }
        });
    }
    
private:
    ofParameter<vector<float>> input;
    ofParameter<int> outputSize;
    ofParameter<float> floatOutput;
    ofParameter<vector<float>> pulseOutput;
    
    int currentPosition;
    vector<float> lastInput;
    
    ofEventListener listener;
};

#endif /* sequentialAnalyzer_h */
