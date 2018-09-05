//
//  movingheadController.hpp
//  Espills_controller
//
//  Created by Eduard Frigola Bagué on 02/07/2018.
//

#ifndef movingheadController_h
#define movingheadController_h

#include "ofxOceanodeNodeModel.h"

class movingheadController : public ofxOceanodeNodeModel{
public:
    movingheadController();
    ~movingheadController(){};
    
    void update(ofEventArgs &a) override;
    
private:
    
    float getValueAtIndex(const vector<float> &v, int index){
        if(v.size() == 1 || v.size() <= index){
            return v[0];
        }
        else{
            return v[index];
        }
    }
    
    //ofParameter<string> filename;
    ofParameter<vector<float>> pan;
    ofParameter<vector<float>> tilt;
    ofParameter<vector<float>> intensity;
    ofParameter<vector<float>> colorwheel;
    ofParameter<float> strobe;
    ofParameter<float> gobo;
    ofParameter<float> frost;
    ofParameter<float> focus;
    ofParameter<vector<float>> output;
};

#endif /* movingheadController_h */
