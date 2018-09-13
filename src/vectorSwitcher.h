//
//  vectorSwitcher.h
//  Beams_Controlls
//
//  Created by Eduard Frigola on 12/09/2018.
//

#ifndef vectorSwitcher_h
#define vectorSwitcher_h

#include "ofxOceanodeNodeModel.h"

class vectorSwitcher : public ofxOceanodeNodeModel{
public:
    vectorSwitcher() : ofxOceanodeNodeModel("Vector Switcher"){};
    ~vectorSwitcher(){};
    
    void setup() override{
        parameters->add(inputSelector.set("Input Selector", {0}, {0}, {2}));
        parameters->add(input0.set("Input 0", {0}, {0}, {1}));
        parameters->add(input1.set("Input 1", {0}, {0}, {1}));
        parameters->add(input2.set("Input 2", {0}, {0}, {1}));
        parameters->add(output.set("Output", {0}, {0}, {1}));

        listener = inputSelector.newListener([this](vector<int> &v){
            int size = max(max(input0->size(), input1->size()), max(input2->size(), v.size()));
            vector<float> tempOutput(size);
            for(int i = 0; i < v.size(); i++){
                switch (getValueForIndex(inputSelector.get(), i)){
                    case 0:
                        tempOutput[i] = getValueForIndex(input0.get(), i);
                        break;
                    case 1:
                        tempOutput[i] = getValueForIndex(input1.get(), i);
                        break;
                    case 2:
                        tempOutput[i] = getValueForIndex(input2.get(), i);
                        break;
                    default:
                        break;
                }
            }
            output = tempOutput;
        });
    }
    
    
private:
    
    template<typename T>
    T getValueForIndex(const vector<T> &vf, int i){
        if(vf.size() == 1 || vf.size() <= i){
            return vf[0];
        }
        else{
            return vf[i];
        }
    }

    ofEventListener listener;
    
    ofParameter<vector<int>> inputSelector;
    ofParameter<vector<float>> input0;
    ofParameter<vector<float>> input1;
    ofParameter<vector<float>> input2;
    ofParameter<vector<float>> output;
};


#endif /* vectorSwitcher_h */
