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
