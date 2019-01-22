//
//  movingheadController.cpp
//  Espills_controller
//
//  Created by Eduard Frigola Bagué on 02/07/2018.
//

#include "movingheadController.h"

movingheadController::movingheadController() : ofxOceanodeNodeModelExternalWindow("Movinghead Controller"){
    numGroups = 4;
    size.resize(numGroups);
    intensity.resize(numGroups);
    pan.resize(numGroups);
    tilt.resize(numGroups);
    colorDropdown.resize(numGroups);
    colorwheel.resize(numGroups);
    zoom.resize(numGroups);
    gobo.resize(numGroups);
    frost.resize(numGroups);
    focus.resize(numGroups);
    for(int i = 0 ; i < numGroups; i++){
        ofParameter<char> label;
        parameters->add(label.set("Group " + ofToString(i) + "      ----------------------------------", ' '));
        parameters->add(size[i].set("Size " + ofToString(i), {1}, {1}, {500}));
        parameters->add(intensity[i].set("Intensity " + ofToString(i), {0}, {0}, {1}));
        parameters->add(pan[i].set("Pan " + ofToString(i), {0}, {-180}, {180}));
        parameters->add(tilt[i].set("Tilt " + ofToString(i), {0}, {-180}, {180}));
        parameters->add(createDropdownAbstractParameter("Color " + ofToString(i), {"White", "Deep Red", "Deep Blue", "Yellow", "Green", "Magenta", "Azure", "Red", "Dark Green", "Amber", "Blue", "Orange", "CTO", "UV filter"}, colorDropdown[i]));
        parameters->add(colorwheel[i].set("Color " + ofToString(i), {0}, {0}, {colorDropdown[i].getMax()}));
        addOutputParameterToGroupAndInfo(gobo[i].set("Gobo " + ofToString(i), 0, 0, 1));
        addOutputParameterToGroupAndInfo(zoom[i].set("Zoom " + ofToString(i), 0, 0, 1));
        addOutputParameterToGroupAndInfo(focus[i].set("Focus " + ofToString(i), 0, 0, 1));
        addOutputParameterToGroupAndInfo(frost[i].set("Frost " + ofToString(i), 0, 0, 1));
        
        dropdownListeners.push(colorDropdown[i].newListener([this, i](int &ind){
            colorwheel[i] = vector<int>(1, ind);
        }));
        sizeListeners.push(size[i].newListener([this](int &s){
            totalSize = 0;
            for(auto s : size) totalSize += s;
            loadCalibration();
            recalculateSliders();
        }));
    }
    ofParameter<char> label;
    parameters->add(label.set("Shared", ' '));
    parameters->add(masterFader.set("Master Fader", 1, 0, 1));
    addOutputParameterToGroupAndInfo(lampOn.set("LampOn", false));
    addOutputParameterToGroupAndInfo(dmxOutput.set("Dmx Output", {0}, {0}, {1}));
    addOutputParameterToGroupAndInfo(panOutput.set("Pan Output", {0}, {-180}, {180}));
    addOutputParameterToGroupAndInfo(tiltOutput.set("Tilt Output", {0}, {-180}, {180}));
    addOutputParameterToGroupAndInfo(colorOutput.set("Color Output", {0}, {0}, {1}));
    
    lampOn = false;
    
    indexClicked = -1;
    isHorizontal = true;
    
    totalSize = 4;
    panRange = 540;
    tiltRange = 253.125;
}

void movingheadController::loadCalibration(){
    bool loaded = false;
    ofJson json = ofLoadJson("MovingHeadCalibration_" + ofToString(numIdentifier) + ".json");
    if(!json.empty()){
        if(json["size"].get<int>() == totalSize){
            panOffset = json["panOffset"].get<vector<float>>();
            tiltOffset = json["tiltOffset"].get<vector<float>>();
            loaded = true;
        }
    }if(!loaded){
        panOffset = vector<float>(totalSize, 0);
        tiltOffset = vector<float>(totalSize, 0);
    }
}

void movingheadController::saveCalibration(){
    ofJson json;
    json["size"] = totalSize;
    json["panOffset"] = panOffset;
    json["tiltOffset"] = tiltOffset;
    ofSavePrettyJson("MovingHeadCalibration_" + ofToString(numIdentifier) + ".json", json);
}

void movingheadController::update(ofEventArgs &a){
    int nChannels = 24;
    
    vector<vector<float>> dmxInfo;
    vector<float> panInfo;
    vector<float> tiltInfo;
    vector<float> colorInfo;
    
    dmxInfo.resize(totalSize, vector<float>(nChannels, 0));
    panInfo.resize(totalSize, .5);
    tiltInfo.resize(totalSize, .5);
    colorInfo.resize(totalSize*3, .5); //3 color components;
    
    for(int i = 0; i < numGroups; i++){
        int accumulateSizes = 0;
        for(int k = 0; k < i; k++){
            accumulateSizes += size[k];
        }
        for(int j = 0; j < size[i]; j++){
            int index = j + accumulateSizes;
            
            //pan
            float panAtIndex = getValueAtIndex(pan[i].get(), j);
            panInfo[index] = panAtIndex;
            panAtIndex = ofMap(panAtIndex, -panRange/2 + panOffset[index], panRange/2 + panOffset[index], 0, 1, true);
            dmxInfo[index][0] = panAtIndex;
            dmxInfo[index][1] = panAtIndex*255 - int(panAtIndex*255);
            
            //tilt
            float tiltAtIndex = getValueAtIndex(tilt[i].get(), j);
            tiltInfo[index] = tiltAtIndex;
            tiltAtIndex = ofMap(tiltAtIndex, -tiltRange/2 + tiltOffset[index], tiltRange/2 + tiltOffset[index], 0, 1, true);
            dmxInfo[index][2] = tiltAtIndex;
            dmxInfo[index][3] = tiltAtIndex*255 - int(tiltAtIndex*255);
            
            //pan/tilt speed
            dmxInfo[index][4] = 1/255;
            
            dmxInfo[index][5] = lampOn ? 135.0f/255.0f : 0;
            
            //color wheel
            dmxInfo[index][6] = ofMap(getValueAtIndex(colorwheel[i].get(), j), colorwheel[i].getMin()[0], colorwheel[i].getMax()[0]-1, 128.0/255.0, 186/255.0, true);
            
            
            //gobo
            dmxInfo[index][9] = ofMap(gobo[i], 0, 1, 0, 87.0f/255.0f);
            
            //frost
            dmxInfo[index][15] = ofMap(frost[i], 0, 1, 0, 180.0f/255.0f);
            
            //zoom
            float zoomAtIndex = zoom[i];
            dmxInfo[index][16] = zoomAtIndex;
            dmxInfo[index][17] = zoomAtIndex*255 - int(zoomAtIndex*255);
            
            //Focus
            float focusAtIndex = focus[i];
            dmxInfo[index][18] = focusAtIndex;
            dmxInfo[index][19] = focusAtIndex*255 - int(focusAtIndex*255);

            
            //shutter
//            dmxInfo[index][21] = ofMap(zoom[i], 0, 1, 64.0f/255.0f, 96.0f/255.0f);
            dmxInfo[index][21] = 1;//lampOn ? 0 : 96.0f/255.0f;
            
            
            //dimmer
            float dimmerAtIndex = getValueAtIndex(intensity[i].get(), j) * masterFader;
            dmxInfo[index][22] = dimmerAtIndex;
            dmxInfo[index][23] = dimmerAtIndex*255.0f - int(dimmerAtIndex*255.0f);
            
            ofColor colorValue;
            switch (int(getValueAtIndex(colorwheel[i].get(), j))) {
                case 0:
                    colorValue = ofColor::white;
                    break;
                case 1:
                    colorValue = ofColor::darkRed;
                    break;
                case 2:
                    colorValue = ofColor::darkBlue;
                    break;
                case 3:
                    colorValue = ofColor::yellow;
                    break;
                case 4:
                    colorValue = ofColor::green;
                    break;
                case 5:
                    colorValue = ofColor::magenta;
                    break;
                case 6:
                    colorValue = ofColor::azure;
                    break;
                case 7:
                    colorValue = ofColor::red;
                    break;
                case 8:
                    colorValue = ofColor::darkGreen;
                    break;
                case 9:
                    colorValue = ofColor(231, 194, 81);
                    break;
                case 10:
                    colorValue = ofColor::blue;
                    break;
                case 11:
                    colorValue = ofColor::orange;
                    break;
                case 12:
                    colorValue = ofColor(246, 230, 199);
                    break;
                case 13:
                    colorValue = ofColor(46, 48, 89);
                    break;
                    
                    
                default:
                    colorValue = ofColor::white;
                    break;
            }
            colorInfo[index*3] = float(colorValue[0]) / 255.0 * dimmerAtIndex;
            colorInfo[(index*3)+1] = float(colorValue[1]) / 255.0 * dimmerAtIndex;
            colorInfo[(index*3)+2] = float(colorValue[2]) / 255.0 * dimmerAtIndex;
        }
    }
    
    vector<float> tempOutput(dmxInfo.size()*nChannels);
    for(int i = 0; i < dmxInfo.size()*nChannels; i++){
        tempOutput[i] = dmxInfo[i/nChannels][i%nChannels];
    }
    dmxOutput = tempOutput;
    panOutput = panInfo;
    tiltOutput = tiltInfo;
    colorOutput = colorInfo;
}

void movingheadController::drawInExternalWindow(ofEventArgs &e){
    ofBackground(0);
    ofSetColor(255);
    for(int i = 0; i < points.size(); i++){
        auto &point = points[i];
        ofSetColor(255);
        ofNoFill();
        ofDrawRectangle(point);
        glm::vec3 numOffset = isHorizontal ? glm::vec3(0, -point.getHeight()/1.9, 1) : glm::vec3(point.getWidth()/1.9, 0, 1);
        ofDrawBitmapString(ofToString(i), point.getCenter() + numOffset);
        if(ofGetKeyPressed('p')){
            ofDrawBitmapString(ofToString(panOffset[i]), point.getCenter());
        }else if(ofGetKeyPressed('t')){
            ofDrawBitmapString(ofToString(tiltOffset[i]), point.getCenter());
        }
        ofPushStyle();
        ofSetColor(255, 127);
        ofFill();
        ofRectangle sliderVal;
        float offset = 0;
        if(ofGetKeyPressed('p')){
            offset = -panOffset[i] * float(max(point.getWidth(), point.getHeight()))/2;
        }else if(ofGetKeyPressed('t')){
            offset = -tiltOffset[i] * float(max(point.getWidth(), point.getHeight()))/2;
        }
        if(isHorizontal)
            sliderVal.setFromCenter(point.getCenter().x, point.getCenter().y + offset, point.getWidth(), 10);
        else{
            sliderVal.setFromCenter(point.getCenter().x + offset, point.getCenter().y, 10, point.getHeight());
        }
        ofDrawRectangle(sliderVal);
        ofPopStyle();
    }
}

void movingheadController::recalculateSliders(){
    points.resize(totalSize);
    if(externalWindowRect.width >= externalWindowRect.height){
        isHorizontal = true;
        int center = externalWindowRect.height/2;
        float step = float(externalWindowRect.width) / float(totalSize);
        float height = externalWindowRect.height / 1.2;
        float width = externalWindowRect.width / (totalSize*2);
        for(int i = 0; i < totalSize; i++){
            int pos = (step*i) + step/2;
            points[i].setFromCenter(pos, center, width, height);
        }
    }else{
        isHorizontal = false;
        int center = externalWindowRect.width/2;
        float step = externalWindowRect.height / float(totalSize);
        float height = externalWindowRect.height/(totalSize*2);
        float width = externalWindowRect.width / 1.2;
        for(int i = 0; i < totalSize; i++){
            int pos = (step*i) + step/2;
            points[i].setFromCenter(center, pos, width, height);
        }
    }
}

void movingheadController::windowResized(ofResizeEventArgs &a){
    recalculateSliders();
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
        if(ofGetKeyPressed('p')){
            originalValue = panOffset[indexClicked];
        }else if(ofGetKeyPressed('t')){
            originalValue = tiltOffset[indexClicked];
        }
        initialClicPos = a;
    }
}

void movingheadController::mouseReleased(ofMouseEventArgs &a){
//    indexClicked = -1;
}

void movingheadController::mouseDragged(ofMouseEventArgs &a){
    if(indexClicked != -1){
        glm::vec2 amountMoved = a - initialClicPos;
        if(isHorizontal){
            float modVal = -amountMoved.y/(externalWindowRect.height/2);
            if(ofGetKeyPressed(OF_KEY_SHIFT)) modVal /= 2;
            if(ofGetKeyPressed('p')){
                panOffset[indexClicked] = originalValue + modVal;
            }else if(ofGetKeyPressed('t')){
                tiltOffset[indexClicked] = originalValue + modVal;
            }
            ofLog() <<"Index: " << indexClicked << " Amount: "<< ofClamp(-amountMoved.y/(externalWindowRect.height/2), -1, 1) ;
        }else{
            float modVal = -amountMoved.x/(externalWindowRect.width/2);
            if(ofGetKeyPressed(OF_KEY_SHIFT)) modVal /= 2;
            if(ofGetKeyPressed('p')){
                panOffset[indexClicked] = originalValue + modVal;
            }else if(ofGetKeyPressed('t')){
                tiltOffset[indexClicked] = originalValue + modVal;
            }
            ofLog() <<"Index: " << indexClicked << " Amount: "<< ofClamp(amountMoved.x /(externalWindowRect.width/2), -1, 1);
        }
    }
}

void movingheadController::showExternalWindow(bool &b){
    ofxOceanodeNodeModelExternalWindow::showExternalWindow(b);
    recalculateSliders();
}
