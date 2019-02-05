//
//  movingheadController.cpp
//  Espills_controller
//
//  Created by Eduard Frigola Bagué on 02/07/2018.
//

#include "movingheadController.h"

movingheadController::movingheadController() : ofxOceanodeNodeModelExternalWindow("Movinghead Controller"){
    numHeads = 12;
    
    parameters->add(intensity.set("Intensity", {0}, {0}, {1}));
    parameters->add(invertPan.set("Invert Pan", false));
    parameters->add(pan.set("Pan", {0}, {0}, {1}));
    parameters->add(tilt.set("Tilt", {0}, {0}, {1}));
    //{"White", "Dark Red", "Blue", "Green", "Yellow", "Light Green", "Pink", "Turquoise", "Cyan", "Orange", "Rose", "UV", "CTO", "CTB"}
    parameters->add(createDropdownAbstractParameter("Color", {"White", "Congo", "Cobalt Blue", "Pink", "Aquamarine", "CTB", "Cyan", "Dark Red", "Light Orznge", "Orange", "Tan", "Pale Rose", "Magenta", "Grass Green", "Lavander", "2500k", "Dark Green", "Light Green", "Yellow"}, colorDropdown));
    parameters->add(colorwheel.set("Color", {0}, {0}, {colorDropdown.getMax()}));
    addParameterToGroupAndInfo(gobo.set("Gobo", 0, 0, 1)).isSavePreset = false;
    addParameterToGroupAndInfo(focus.set("Focus", 0, 0, 1)).isSavePreset = false;
    addParameterToGroupAndInfo(zoom.set("Zoom", 0, 0, 1)).isSavePreset = false;
    addParameterToGroupAndInfo(frost.set("Frost", 0, 0, 1)).isSavePreset = false;
    parameters->add(masterFader.set("Master Fader", 1, 0, 1));
    addParameterToGroupAndInfo(lampOn.set("Lamp On", false)).isSavePreset = false;
    addParameterToGroupAndInfo(lampOff.set("Lamp Off", false)).isSavePreset = false;
    addOutputParameterToGroupAndInfo(output.set("Output", {0}, {0}, {1}));
    
    dropdownListener = colorDropdown.newListener([this](int &i){
        colorwheel = vector<int>(1, i);
    });
    
    indexClicked = -1;
    points.resize(numHeads);
    isHorizontal = true;
}

void movingheadController::loadCalibration(){
//    ofJson json = ofLoadJson("MovingHeadCalibration_" + ofToString(numIdentifier) + ".json");
//    if(!json.empty()){
//        minPan = json["MinPan"].get<vector<float>>();
//        maxPan = json["MaxPan"].get<vector<float>>();
//        minTilt = json["MinTilt"].get<vector<float>>();
//        maxTilt = json["MaxTilt"].get<vector<float>>();
//    }else{
        minPan = vector<float>(numHeads, 0.5);
        maxPan = vector<float>(numHeads, 0.1667);
        minTilt = vector<float>(numHeads, 1);
        maxTilt = vector<float>(numHeads, 0.5);
//    }
}

void movingheadController::saveCalibration(){
//    ofJson json;
//    json["MinPan"] = minPan;
//    json["MaxPan"] = maxPan;
//    json["MinTilt"] = minTilt;
//    json["MaxTilt"] = maxTilt;
//    ofSavePrettyJson("MovingHeadCalibration_" + ofToString(numIdentifier) + ".json", json);
}

void movingheadController::update(ofEventArgs &a){
    int numChannels = 24;
    vector<float> tempOutput;
    int maxSize = max(pan.get().size(), tilt.get().size());
    tempOutput.resize(numChannels*numHeads, 0);
    
    for(int i = 0; i < numHeads; i++){
        int index = i*numChannels;
        //pan
        float panAtIndex = getValueAtIndex(pan.get(), i);
        if(invertPan) panAtIndex = 1 - panAtIndex;
        panAtIndex = ofMap(panAtIndex, 0., 1., minPan[i], maxPan[i]);
        tempOutput[index] = panAtIndex;
        tempOutput[index+1] = panAtIndex*255 - int(panAtIndex*255);
        
        //tilt
        float tiltAtIndex = getValueAtIndex(tilt.get(), i);
        tiltAtIndex = ofMap(tiltAtIndex, 0., 1., minTilt[i], maxTilt[i]);
        tempOutput[index+2] = tiltAtIndex;
        tempOutput[index+3] = tiltAtIndex*255 - int(tiltAtIndex*255);
        
        //PantiltSpeed
        tempOutput[index+4] = 0;
        
        //Shutter
        tempOutput[index+5] = 1;
        
        //dimmer
        float dimmerAtIndex = getValueAtIndex(intensity.get(), i) * masterFader;
        tempOutput[index+6] = dimmerAtIndex;
        
        //color wheel
        //tempOutput[index+4] = ofMap(getValueAtIndex(colorwheel.get(), i), colorwheel.getMin()[0], colorwheel.getMax()[0]-1, 0.0, 120.0/255.0, true);
        int colorwheelIndex = getValueAtIndex(colorwheel.get(), i);
        tempOutput[index+7] = colorwheelIndex < 7 ? ofMap(colorwheelIndex, 0, 6, 0, 60.0/255.0, true) : 0;
        tempOutput[index+8] = (colorwheelIndex >= 7 && colorwheelIndex < 13) ? ofMap(colorwheelIndex, 7, 12, 10.0/255.0, 60.0/255.0, true) : 0;
        tempOutput[index+9] = colorwheelIndex >= 13 ? ofMap(colorwheelIndex, 13, 19, 10.0/255.0, 60.0/255.0, true) : 0;
        
        //C, M, & Y
        tempOutput[index+10] = 0;
        tempOutput[index+11] = 0;
        tempOutput[index+12] = 0;
        
        //Gobo
        tempOutput[index+15] = ofMap(gobo, 0, 1, 0, 75.0/255.0);
        
        //Frost
        tempOutput[index+18] = frost;
        
        //Focus
        tempOutput[index+20] = focus;
        
        //zoom
        tempOutput[index+21] = zoom;
        
        if(lampOn)
            tempOutput[index+23] = 85.0f/255.0f;
        else if(lampOff){
            tempOutput[index+23] = 75.0f/255.0f;
//        }else if(reset){
//            tempOutput[index+15] = 10.0f/255.0f;
        }else{
            tempOutput[index+23] = 0;
        }
    }
    
    output = tempOutput;
}

void movingheadController::drawInExternalWindow(ofEventArgs &e){
    ofBackground(0);
    ofSetColor(255);
    for(int i = 0; i < points.size(); i++){
        auto &point = points[i];
        ofSetColor(255);
        ofDrawCircle(point.getCenter(), point.getWidth()/2);
        glm::vec3 numOffset = isHorizontal ? glm::vec3(-point.getWidth()/4, -point.getWidth()/2, 1) : glm::vec3(point.getWidth()/2, point.getWidth()/4, 1);
        ofDrawBitmapString(ofToString(i), point.getCenter() + numOffset);
        if(ofGetKeyPressed(OF_KEY_LEFT) || ofGetKeyPressed(OF_KEY_RIGHT)){
            ofPath path;
            path.setColor(ofColor::red);
            path.arc(point.getCenter(), point.getWidth()/1.5, point.getWidth()/1.5, 0, 270);
//            path.moveTo(point.getCenter());
            path.draw();
        }else if(ofGetKeyPressed(OF_KEY_UP) || ofGetKeyPressed(OF_KEY_DOWN)){
            ofPath path;
            path.setColor(ofColor::red);
            path.arc(point.getCenter(), point.getWidth()/1.5, point.getWidth()/1.5, ofMap(minTilt[i], 0., 1., 140, 400), ofMap(maxTilt[i], 0., 1., 140, 400));
            path.draw();
        }
    }
}

void movingheadController::windowResized(ofResizeEventArgs &a){
    if(externalWindowRect.width >= externalWindowRect.height){
        isHorizontal = true;
        int center = externalWindowRect.height/2;
        float step = float(externalWindowRect.width) / 32.0;
        float size = min(externalWindowRect.height / 2, externalWindowRect.width/(32*4));
        for(int i = 0; i < 32; i++){
            int pos = (step*i) + step/2;
            points[i].setFromCenter(pos, center, size*2, size*2);
        }
    }else{
        isHorizontal = false;
        int center = externalWindowRect.width/2;
        float step = externalWindowRect.height / 32.0;
        float size = min(externalWindowRect.width / 2, externalWindowRect.height/(32*4));
        for(int i = 0; i < 32; i++){
            int pos = (step*i) + step/2;
            points[i].setFromCenter(center, pos, size*2, size*2);
        }
    }
}

void movingheadController::keyPressed(ofKeyEventArgs &a){

}

void movingheadController::keyReleased(ofKeyEventArgs &a){

}

void movingheadController::mousePressed(ofMouseEventArgs &a){
    if(ofGetKeyPressed()){
        for(int i = 0; i < points.size(); i++){
            if(points[i].inside(a)){
            indexClicked = i;
            break;
            }
        }
        if(ofGetKeyPressed(OF_KEY_LEFT)){
            originalValue = minPan[indexClicked];
        }else if(ofGetKeyPressed(OF_KEY_RIGHT)){
            originalValue = maxPan[indexClicked];
        }else if(ofGetKeyPressed(OF_KEY_DOWN)){
            originalValue = minTilt[indexClicked];
        }else if(ofGetKeyPressed(OF_KEY_UP)){
            originalValue = maxTilt[indexClicked];
        }
        initialClicPos = a;
    }
}

void movingheadController::mouseReleased(ofMouseEventArgs &a){
    indexClicked = -1;
}

void movingheadController::mouseDragged(ofMouseEventArgs &a){
    if(indexClicked != -1){
        glm::vec2 amountMoved = a - initialClicPos;
        if(isHorizontal){
            float modVal = -amountMoved.y/(externalWindowRect.height/2);
            if(ofGetKeyPressed(OF_KEY_SHIFT)) modVal /= 2;
            if(ofGetKeyPressed(OF_KEY_LEFT)){
                minPan[indexClicked] = originalValue + modVal;
            }else if(ofGetKeyPressed(OF_KEY_RIGHT)){
                maxPan[indexClicked] = originalValue + modVal;
            }else if(ofGetKeyPressed(OF_KEY_DOWN)){
                minTilt[indexClicked] = originalValue + modVal;
            }else if(ofGetKeyPressed(OF_KEY_UP)){
                maxTilt[indexClicked] = originalValue + modVal;
            }
            ofLog() <<"Index: " << indexClicked << " Amount: "<< ofClamp(-amountMoved.y/(externalWindowRect.height/2), -1, 1) ;
        }else{
            float modVal = -amountMoved.x/(externalWindowRect.width/2);
            if(ofGetKeyPressed(OF_KEY_SHIFT)) modVal /= 2;
            if(ofGetKeyPressed(OF_KEY_LEFT)){
                minPan[indexClicked] = originalValue + modVal;
            }else if(ofGetKeyPressed(OF_KEY_RIGHT)){
                maxPan[indexClicked] = originalValue + modVal;
            }else if(ofGetKeyPressed(OF_KEY_DOWN)){
                minTilt[indexClicked] = originalValue + modVal;
            }else if(ofGetKeyPressed(OF_KEY_UP)){
                maxTilt[indexClicked] = originalValue + modVal;
            }
            ofLog() <<"Index: " << indexClicked << " Amount: "<< ofClamp(amountMoved.x /(externalWindowRect.width/2), -1, 1);
        }
    }
}

