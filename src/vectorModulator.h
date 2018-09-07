//
//  vectorModulator.h
//  Espills_controller
//
//  Created by Eduard Frigola Bagué on 30/08/2018.
//

#ifndef vectorModulator_h
#define vectorModulator_h

#include "ofxOceanodeNodeModel.h"

class vectorModulator : public ofxOceanodeNodeModel{
public:
    vectorModulator() : ofxOceanodeNodeModel("Vector Modulator"){};
    ~vectorModulator(){};
    
    void setup() override{
        parameters->add(source.set("Source", {0}, {0}, {1}));
        parameters->add(modulator.set("Modulator", {0.5}, {0}, {1}));
        parameters->add(amount.set("Amount", {0}, {0}, {1}));
        parameters->add(output.set("Output", {0}, {0}, {1}));
        
        listeners.push(source.newListener(this, &vectorModulator::computeOutput));
        listeners.push(modulator.newListener(this, &vectorModulator::computeOutput));
    }
    
private:
    void computeOutput(vector<float> &vf){
        vector<float> tempOut(max(source->size(), modulator->size()));
        for (int i = 0; i < tempOut.size(); i++) {
            tempOut[i] = ofClamp(getParameterAtIndex(source.get(), i) + (getParameterAtIndex(modulator.get(), i)-0.5) * getParameterAtIndex(amount.get(), i), 0, 1);
        }
        output = tempOut;
    }
    
    float getParameterAtIndex(const vector<float> &vf, int index){
        if(vf.size() == 1 || vf.size() <= index){
            return vf[0];
        }
        else{
            return vf[index];
        }
    }
    
    ofEventListeners listeners;
    
    ofParameter<vector<float>> source;
    ofParameter<vector<float>> modulator;
    ofParameter<vector<float>> amount;
    ofParameter<vector<float>> output;
};

#endif /* vectorModulator_hpp */
