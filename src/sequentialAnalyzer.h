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
        parameters->add(threshold.set("Threshold", {0.5}, {0}, {1}));
        parameters->add(outputSize.set("Output Size", 1, 1, 100));
        parameters->add(floatOutput.set("Float Output", 0, 0, 1));
        parameters->add(pulseOutput.set("Pulse Output", {0}, {0}, {1}));
        
        currentPosition = 0;
        
        listeners.push(outputSize.newListener([this](int &i){
            currentPosition = 0;
            floatOutput = 0;
            vector<float> tempOut(i, 0);
            tempOut[0] = 1;
            pulseOutput = tempOut;
        }));
        
        listeners.push(input.newListener([this](vector<float> &vf){
            bool inputTriggered = false;
            if(vf.size() != lastInput.size()) lastInput = vector<float>(vf.size(), 0);
            
            for(int i = 0; i < vf.size(); i++){
                if(vf[i] >= threshold && lastInput[i] < threshold){
                    inputTriggered = true;
                    break;
                }
            }
            lastInput = vf;
            
            if(inputTriggered){
                currentPosition++;
                currentPosition %= outputSize;
                floatOutput = (float)currentPosition/(float)outputSize;
                vector<float> tempPulseOutput(outputSize, 0);
                tempPulseOutput[currentPosition] = 1;
                pulseOutput = tempPulseOutput;
            }
        }));
    }
    
private:
    ofParameter<vector<float>> input;
    ofParameter<float> threshold;
    ofParameter<int> outputSize;
    ofParameter<int> gap;
    ofParameter<float> floatOutput;
    ofParameter<vector<float>> pulseOutput;
    
    int currentPosition;
    vector<float> lastInput;
    
    ofEventListeners listeners;
};

#endif /* sequentialAnalyzer_h */
