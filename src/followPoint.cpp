//
//  followPoint.c
//  Beams_Controlls
//
//  Created by Eduard Frigola on 07/09/2018.
//

#include "followPoint.h"

void followPoint::setup(){
    parameters->add(pointsX.set("Points X", {.5}, {0}, {1}));
    parameters->add(pointsY.set("Points Y", {.5}, {0}, {1}));
    parameters->add(pointsZ.set("Points Z", {.5}, {0}, {1}));
}

void followPoint::update(ofEventArgs &a){
    int maxSize = max(max(pointsX->size(), pointsY->size()), pointsZ->size());
    
    auto getValForIndex = [this](const vector<float> &vf, int index)->float{
        if(vf.size() == 1 || vf.size() <= index){
            return vf[0];
        }
        else{
            return vf[index];
        }
    };
    
    points.resize(maxSize);
    for(int i = 0; i < points.size(); i++){
        points[i] = glm::vec3(getValForIndex(pointsX.get(), i), getValForIndex(pointsY.get(), i), getValForIndex(pointsZ.get(), i));
    }
}

void followPoint::drawInExternalWindow(ofEventArgs &a){
    cam.begin();
    ofSetColor(255, 0, 0, 127);
    for(int i = 0; i < points.size(); i++){
        ofFill();
        ofDrawSphere((points[i] * glm::vec3(2) - glm::vec3(1)) * glm::vec3(ofGetWindowSize(), 4000), 20);
    }
    cam.end();
    
    ofRectangle vp = ofGetCurrentViewport();
    float r = std::min<float>(vp.width, vp.height) * 0.5f;
    float x = vp.width * 0.5f;
    float y = vp.height * 0.5f;
    
    ofPushStyle();
    ofSetLineWidth(3);
    ofSetColor(255, 255, 0);
    ofNoFill();
    glDepthMask(false);
    ofDrawCircle(x, y, r);
    glDepthMask(true);
    ofPopStyle();
}

void followPoint::showExternalWindow(bool &b){
    ofxOceanodeNodeModelExternalWindow::showExternalWindow(b);
    if(b){
        cam.setEvents(externalWindow->events());
        cam.enableMouseInput();
    }else{
        cam.disableMouseInput();
    }
}

void followPoint::keyPressed(ofKeyEventArgs &a){
    switch(a.key) {
        case ' ':
            cam.getOrtho() ? cam.disableOrtho() : cam.enableOrtho();
            break;
        case 'C':
        case 'c':
            cam.getMouseInputEnabled() ? cam.disableMouseInput() : cam.enableMouseInput();
            break;
        case 'F':
        case 'f':
            ofToggleFullscreen();
            break;
        case 'H':
        case 'h':
            //bHelpText ^=true;
            break;
        case 'I':
        case 'i':
            cam.getInertiaEnabled() ? cam.disableInertia() : cam.enableInertia();
            break;
        case 'Y':
        case 'y':
            cam.setRelativeYAxis(!cam.getRelativeYAxis());
            break;
    }
}
