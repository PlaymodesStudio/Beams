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
    
    ofEventListener dropdownListener;
    
    //ofParameter<string> filename;
    ofParameter<bool> invertPan;
    ofParameter<vector<float>> pan;
    ofParameter<vector<float>> tilt;
    ofParameter<vector<float>> intensity;
    ofParameter<vector<int>> colorwheel;
    ofParameter<int> colorDropdown;
    ofParameter<float> strobe;
    ofParameter<float> gobo;
    ofParameter<float> frost;
    ofParameter<float> masterFader;
    ofParameter<vector<float>> output;
    
    vector<float> minPan;
    vector<float> maxPan;
    vector<float> minTilt;
    vector<float> maxTilt;
    
    
    float originalValue;
    vector<ofRectangle> points;
    int indexClicked;
    bool isHorizontal;
    glm::vec2 initialClicPos;
};

#endif /* movingheadController_h */
