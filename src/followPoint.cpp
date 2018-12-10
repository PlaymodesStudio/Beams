//
//  followPoint.c
//  Beams_Controlls
//
//  Created by Eduard Frigola on 07/09/2018.
//

#include "followPoint.h"

void followPoint::setup(){
    parameters->add(pointsX.set("Points X", {0}, {-22.5}, {22.5}));
    parameters->add(pointsY.set("Points Y", {0}, {-10}, {100}));
    parameters->add(pointsZ.set("Points Z", {0}, {-50}, {50}));
    parameters->add(sortPoints.set("Sort Points", false));
    parameters->add(snakeSort.set("Snake Sort", true));
    
    parameters->add(panL.set("Pan L", {0}, {-180}, {180}));
    parameters->add(tiltL.set("Tilt L", {0}, {-180}, {180}));
    parameters->add(panR.set("Pan R", {0}, {-180}, {180}));
    parameters->add(tiltR.set("Tilt R", {0}, {-180}, {180}));
    
    movingHeadsL.resize(5);
    for(int i = 0; i < movingHeadsL.size(); i++){
        movingHeadsL[i].setPosition(-19.54567, 11.82634, -45 + (22.5*i));
        movingHeadsL[i].setScale(.1);
    }
    
    movingHeadsR.resize(5);
    for(int i = 0; i < movingHeadsL.size(); i++){
        movingHeadsR[i].setPosition(19.54567, 11.82634, -45 + (22.5*i));
        movingHeadsR[i].setScale(.1);
    }
    
    
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
        auto &point =  points[i];
        point = glm::vec3(getValForIndex(pointsX.get(), i), getValForIndex(pointsY.get(), i), getValForIndex(pointsZ.get(), i));
//        point.x = ofMap(point.x, 0, 1, -22.5, 22.5);
//        point.y = ofMap(point.y, 0, 1, 0, 100);
//        point.z = ofMap(point.z, 0, 1, -50, 50);
    }
    
    if(sortPoints){
        std::sort(points.begin(), points.end(), [](glm::vec3 p1, glm::vec3 p2){return p1.z > p2.z;});
    }
    
    glm::vec3 lookAtPos = points[0];
    vector<float> tempPans(5);
    vector<float> tempTilts(5);
    for(int i = 0; i < movingHeadsL.size(); i++){
        int pointPos = 0;
        if(points.size() < 5){
            pointPos = floor(i / (5.0 / points.size()));
        }else{
            pointPos = i;
        }
        
        //cout<<pointPos<<endl;
        glm::vec2 orientInfo = calculateAlfa(movingHeadsL[i].getPosition(), points[pointPos], 0);
        float tilt = orientInfo.x;
        float pan = orientInfo.y;
        //            if(pan < 0){
        //                tilt = - tilt;
        //            }
        //pan = pan+90;
        //            if(i == 0) ofLog() << "Pan:" << pan << " Tilt:" << tilt;
        tempPans[i] = -pan;
        tempTilts[i] = 90-tilt;
    }
    panL = tempPans;
    tiltL = tempTilts;
    for(int i = 0; i < movingHeadsR.size(); i++){
        int pointPos = 0;
        if(points.size() <= 5){
            pointPos = floor((i+0.5) / (5.0 / points.size()));
        }else{
            pointPos = (i+5) % points.size();
        }
        //cout<<pointPos<<endl;

        
        glm::vec2 orientInfo = calculateAlfa(movingHeadsR[i].getPosition(), points[pointPos], 0);
        float tilt = orientInfo.x;
        float pan = orientInfo.y;
        //            if(pan < 0){
        //                tilt = - tilt;
        //            }
        //pan = pan+90;
        tempPans[i] = pan-180;
        tempTilts[i] = tilt-90;
    }
    //cout<<"---------------------"<<endl;
    panR = tempPans;
    tiltR = tempTilts;
//        ofLog() << calculateAlfa(movingHeadsL[0].getPosition(), points[0], 0);

}

void followPoint::drawInExternalWindow(ofEventArgs &a){
    cam.begin();
    ofSetColor(255, 0, 0, 127);
    for(auto &head : movingHeadsL){
        head.draw();
    }
    for(auto &head : movingHeadsR){
        head.draw();
    }
//    auto polar = calculateAlfa(movingHeadsL[0].getPosition(), points[0], 0);
//    polar = glm::vec2(ofDegToRad(polar.x), ofDegToRad(polar.y));
////    ofLog() << polar.x << "---" << glm::distance(movingHeadsL[0].getPosition(), points[0]) * sin(polar.y);
//    glm::vec3 direction = glm::vec3(sin(polar.y) * cos(polar.x), sin(polar.y) * sin(polar.x), cos(polar.y));
////    glm::vec3 direction = glm::vec3(sin(polar.y), sin(polar.x), cos(90-polar.y));
//    ofDrawLine(movingHeadsL[0].getPosition(), movingHeadsL[0].getPosition() + 20 * direction);
//    movingHeadsL[0].draw();
    for(int i = 0; i < points.size(); i++){
        ofFill();
        ofDrawSphere(points[i], 1);
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

glm::vec2 followPoint::calculateAlfa(glm::vec3 position, glm::vec3 lookAT, bool orientMode){
    glm::vec2 result;
    glm::vec3 myVector;
    
    myVector = lookAT-position;
    
    float R1 = sqrt((myVector.x*myVector.x) + (myVector.z*myVector.z));
    float R2 = sqrt((R1*R1) + (myVector.y*myVector.y));
    
    if(orientMode==0)
    {
        result.y = -ofRadToDeg(asin((myVector.z/R1)));
        result.x = ofRadToDeg(asin(-myVector.y/R2));
    }
    else
    {
        result.y = ofRadToDeg(asin(myVector.z/R1));
        result.x = ofRadToDeg(asin(-myVector.y/R2));
    }
    
    
//    result.x = ofRadToDeg(atan(myVector.y/myVector.x));
//    result.y = ofRadToDeg(acos(myVector.z/(sqrt(pow(myVector.x, 2)+pow(myVector.y, 2)+pow(myVector.z, 2)))));
    
    return result;
}
