//
//  movingheadController.cpp
//  Espills_controller
//
//  Created by Eduard Frigola Bagué on 02/07/2018.
//

#include "movingheadController.h"

movingheadController::movingheadController() : ofxOceanodeNodeModelExternalWindow("Movinghead Controller"){
    parameters->add(intensity.set("Intensity", {0}, {0}, {1}));
    parameters->add(invertPan.set("Invert Pan", false));
    parameters->add(pan.set("Pan", {0}, {0}, {1}));
    parameters->add(tilt.set("Tilt", {0}, {0}, {1}));
    parameters->add(colorwheel.set("Color", {0}, {0}, {1}));
    parameters->add(strobe.set("Strobe", 0, 0, 1));
    parameters->add(gobo.set("Gobo", 0, 0, 1));
    parameters->add(frost.set("Frost", 0, 0, 1));
    parameters->add(focus.set("Focus", 0, 0, 1));
    addOutputParameterToGroupAndInfo(output.set("Output", {0}, {0}, {1}));
    
    indexClicked = -1;
    points.resize(32);
    isHorizontal = true;
}

void movingheadController::loadCalibration(){
    ofJson json = ofLoadJson("MovingHeadCalibration_" + ofToString(numIdentifier) + ".json");
    if(!json.empty()){
        minPan = json["MinPan"].get<vector<float>>();
        maxPan = json["MaxPan"].get<vector<float>>();
        minTilt = json["MinTilt"].get<vector<float>>();
        maxTilt = json["MaxTilt"].get<vector<float>>();
    }else{
        minPan = vector<float>(32, 0);
        maxPan = vector<float>(32, 1);
        minTilt = vector<float>(32, 0);
        maxTilt = vector<float>(32, .5);
    }
}

void movingheadController::saveCalibration(){
    ofJson json;
    json["MinPan"] = minPan;
    json["MaxPan"] = maxPan;
    json["MinTilt"] = minTilt;
    json["MaxTilt"] = maxTilt;
    ofSavePrettyJson("MovingHeadCalibration_" + ofToString(numIdentifier) + ".json", json);
}

void movingheadController::update(ofEventArgs &a){
    vector<float> tempOutput;
    int maxSize = max(pan.get().size(), tilt.get().size());
    tempOutput.resize(16*32, 0);
    
    for(int i = 0; i < 32; i++){
        int index = i*16;
        //color wheel
        tempOutput[index] = getValueAtIndex(colorwheel.get(), i);
        //strobe
        tempOutput[index+1] = strobe;
        //dimmer
        tempOutput[index+2] = getValueAtIndex(intensity.get(), i);
        //gobo
        tempOutput[index+3] = gobo;
        //frost
        tempOutput[index+7] = frost;
        //focus
        tempOutput[index+8] = focus;
        //pan
        float panAtIndex = getValueAtIndex(pan.get(), i);
        if(invertPan) panAtIndex = 1 - panAtIndex;
        tempOutput[index+9] = panAtIndex;
        tempOutput[index+10] = panAtIndex*255 - int(panAtIndex*255);
        //tilt
        float tiltAtIndex = getValueAtIndex(tilt.get(), i);
        tempOutput[index+11] = tiltAtIndex;
        tempOutput[index+12] = tiltAtIndex*255 - int(tiltAtIndex*255);
        //lamp
        tempOutput[index+15] = 0;
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

