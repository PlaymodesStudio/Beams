//
//  movingheadController.hpp
//  Espills_controller
//
//  Created by Eduard Frigola Bagué on 02/07/2018.
//

#ifndef movingheadController_h
#define movingheadController_h

#include "ofxOceanodeNodeModelExternalWindow.h"

class movingheadController : public ofxOceanodeNodeModelExternalWindow{
public:
    movingheadController();
    ~movingheadController(){
         if(numIdentifier != -1){
             saveCalibration();
         }
    };
    
    void setup() override{
        if(numIdentifier != -1){
            loadCalibration();
        }
    }
    
    void update(ofEventArgs &a) override;
    
    void presetRecallBeforeSettingParameters(ofJson &json) override{
        for(auto &i : intensity) i = {0};
    };
    
private:
    
    void saveCalibration();
    void loadCalibration();
    
    void drawInExternalWindow(ofEventArgs &e) override;
    void windowResized(ofResizeEventArgs &a) override;
    void keyPressed(ofKeyEventArgs &a) override;
    void keyReleased(ofKeyEventArgs &a) override;
    
    void mousePressed(ofMouseEventArgs &a) override;
    void mouseReleased(ofMouseEventArgs &a) override;
    void mouseDragged(ofMouseEventArgs &a) override;
    
    template<typename T>
    float getValueAtIndex(const vector<T> &v, int index){
        if(v.size() == 1 || v.size() <= index){
            return v[0];
        }
        else{
            return v[index];
        }
    }
    
    ofEventListeners dropdownListeners;
    
    int numGroups;
    vector<ofParameter<int>>    size;
    vector<ofParameter<vector<float>>> pan;
    vector<ofParameter<vector<float>>> tilt;
    vector<ofParameter<vector<float>>> intensity;
    vector<ofParameter<vector<int>>> colorwheel;
    vector<ofParameter<int>> colorDropdown;
    vector<ofParameter<float>> strobe;
    vector<ofParameter<float>> gobo;
    vector<ofParameter<float>> frost;
    ofParameter<float> masterFader;
    ofParameter<vector<float>> dmxOutput;
    ofParameter<vector<float>> panOutput;
    ofParameter<vector<float>> tiltOutput;
    ofParameter<vector<float>> colorOutput;
    
    vector<float> minPan;
    vector<float> maxPan;
    vector<float> minTilt;
    vector<float> maxTilt;
    
    
    float originalValue;
    vector<ofRectangle> points;
    int indexClicked;
    bool isHorizontal;
    glm::vec2 initialClicPos;
    
    int panRange;
    int tiltRange;
};

#endif /* movingheadController_h */
