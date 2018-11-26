//
//  movingheadController.cpp
//  Espills_controller
//
//  Created by Eduard Frigola Bagué on 02/07/2018.
//

#include "movingheadController.h"

movingheadController::movingheadController() : ofxOceanodeNodeModelExternalWindow("Movinghead Controller"){
    numGroups = 2;
    size.resize(numGroups);
    intensity.resize(numGroups);
    pan.resize(numGroups);
    tilt.resize(numGroups);
    colorDropdown.resize(numGroups);
    colorwheel.resize(numGroups);
    strobe.resize(numGroups);
    gobo.resize(numGroups);
    frost.resize(numGroups);
    for(int i = 0 ; i < numGroups; i++){
        ofParameter<char> label;
        parameters->add(label.set("Group " + ofToString(i) + "      ----------------------------------", ' '));
        parameters->add(size[i].set("Size " + ofToString(i), {1}, {1}, {500}));
        parameters->add(intensity[i].set("Intensity " + ofToString(i), {0}, {0}, {1}));
        parameters->add(pan[i].set("Pan " + ofToString(i), {0}, {-180}, {180}));
        parameters->add(tilt[i].set("Tilt " + ofToString(i), {0}, {-180}, {180}));
        parameters->add(createDropdownAbstractParameter("Color " + ofToString(i), {"White", "Red", "Orange", "Yellow", "Green-Blue", "Amber", "Blue Sky", "Green", "UV", "Light Red", "Magenta", "Dark Green", "Dark Yellow", "Blue"}, colorDropdown[i]));
        parameters->add(colorwheel[i].set("Color " + ofToString(i), {0}, {0}, {colorDropdown[i].getMax()}));
        parameters->add(strobe[i].set("Zoom " + ofToString(i), 0, 0, 1));
        parameters->add(gobo[i].set("Gobo " + ofToString(i), 0, 0, 1));
        parameters->add(frost[i].set("Focus " + ofToString(i), 0, 0, 1));
        
        dropdownListeners.push(colorDropdown[i].newListener([this, i](int &ind){
            colorwheel[i] = vector<int>(1, ind);
        }));
    }
    ofParameter<char> label;
    parameters->add(label.set("Shared", ' '));
    parameters->add(masterFader.set("Master Fader", 1, 0, 1));
    addOutputParameterToGroupAndInfo(dmxOutput.set("Dmx Output", {0}, {0}, {1}));
    addOutputParameterToGroupAndInfo(panOutput.set("Pan Output", {0}, {-180}, {180}));
    addOutputParameterToGroupAndInfo(tiltOutput.set("Tilt Output", {0}, {-180}, {180}));
    addOutputParameterToGroupAndInfo(colorOutput.set("Color Output", {0}, {0}, {1}));
    
    
    
    
    indexClicked = -1;
    points.resize(32);
    isHorizontal = true;
    
    panRange = 540;
    tiltRange = 270;
}

void movingheadController::loadCalibration(){
//    ofJson json = ofLoadJson("MovingHeadCalibration_" + ofToString(numIdentifier) + ".json");
//    if(!json.empty()){
//        minPan = json["MinPan"].get<vector<float>>();
//        maxPan = json["MaxPan"].get<vector<float>>();
//        minTilt = json["MinTilt"].get<vector<float>>();
//        maxTilt = json["MaxTilt"].get<vector<float>>();
//    }else{
//        minPan = vector<float>(32, 0.5);
//        maxPan = vector<float>(32, 0.1667);
//        minTilt = vector<float>(32, 1);
//        maxTilt = vector<float>(32, 0.5);
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
    int nChannels = 16;
    
    vector<vector<float>> dmxInfo;
    vector<float> panInfo;
    vector<float> tiltInfo;
    vector<float> colorInfo;
    
    int totalNum = 0;
    for(auto s : size) totalNum += s;
    
    dmxInfo.resize(totalNum, vector<float>(nChannels, 0));
    panInfo.resize(totalNum, .5);
    tiltInfo.resize(totalNum, .5);
    colorInfo.resize(totalNum*3, .5); //3 color components;
    
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
            panAtIndex = ofMap(panAtIndex, -panRange/2, panRange/2, 0, 1, true);
            dmxInfo[index][0] = panAtIndex;
            dmxInfo[index][1] = panAtIndex*255 - int(panAtIndex*255);
            
            //tilt
            float tiltAtIndex = getValueAtIndex(tilt[i].get(), j);
            tiltInfo[index] = tiltAtIndex;
            tiltAtIndex = ofMap(tiltAtIndex, -tiltRange/2, tiltRange/2, 0, 1, true);
            dmxInfo[index][2] = tiltAtIndex;
            dmxInfo[index][3] = tiltAtIndex*255 - int(tiltAtIndex*255);
            
            //pan/tilt speed
            dmxInfo[index][4] = 0;
            
            dmxInfo[index][5] = 1; //Shutter open
            
            //dimmer
            float dimmerAtIndex = getValueAtIndex(intensity[i].get(), j) * masterFader;
            dmxInfo[index][6] = dimmerAtIndex;
            //dmxInfo[index][23] = dimmerAtIndex*255 - int(dimmerAtIndex*255);
            
            //color wheel
            dmxInfo[index][7] = ofMap(getValueAtIndex(colorwheel[i].get(), j), colorwheel[i].getMin()[0], colorwheel[i].getMax()[0]-1, 0, 27.0/255.0, true);
            
            
            //gobo
            dmxInfo[index][10] = ofMap(gobo[i], 0, 1, 0, 48.0/255.0);
            
            //frost
            dmxInfo[index][13] = ofMap(frost[i], 0, 1, 0, 1);
            
            //strobe
            dmxInfo[index][14] = ofMap(strobe[i], 0, 1, 0, 1);
            
            
            
            
            ofColor colorValue;
            switch (int(getValueAtIndex(colorwheel[i].get(), j))) {
                case 0:
                    colorValue = ofColor::white;
                    break;
                case 1:
                    colorValue = ofColor::red;
                    break;
                case 2:
                    colorValue = ofColor::orange;
                    break;
                case 3:
                    colorValue = ofColor::yellow;
                    break;
                case 4:
                    colorValue = ofColor::blueViolet;
                    break;
                case 5:
                    colorValue = ofColor(231, 194, 81);
                    break;
                case 6:
                    colorValue = ofColor::skyBlue;
                    break;
                case 7:
                    colorValue = ofColor::green;
                    break;
                case 8:
                    colorValue = ofColor(46, 48, 89);
                    break;
                case 9:
                    colorValue = ofColor::paleVioletRed;
                    break;
                case 10:
                    colorValue = ofColor::magenta;
                    break;
                case 11:
                    colorValue = ofColor::darkGreen;
                    break;
                case 12:
                    colorValue = ofColor::greenYellow;
                    break;
                case 13:
                    colorValue = ofColor::blue;
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
    for(int i = 0; i < dmxInfo.size()*16; i++){
        tempOutput[i] = dmxInfo[i/16][i%16];
    }
    dmxOutput = tempOutput;
    panOutput = panInfo;
    tiltOutput = tiltInfo;
    colorOutput = colorInfo;
}

void movingheadController::drawInExternalWindow(ofEventArgs &e){
//    ofBackground(0);
//    ofSetColor(255);
//    for(int i = 0; i < points.size(); i++){
//        auto &point = points[i];
//        ofSetColor(255);
//        ofDrawCircle(point.getCenter(), point.getWidth()/2);
//        glm::vec3 numOffset = isHorizontal ? glm::vec3(-point.getWidth()/4, -point.getWidth()/2, 1) : glm::vec3(point.getWidth()/2, point.getWidth()/4, 1);
//        ofDrawBitmapString(ofToString(i), point.getCenter() + numOffset);
//        if(ofGetKeyPressed(OF_KEY_LEFT) || ofGetKeyPressed(OF_KEY_RIGHT)){
//            ofPath path;
//            path.setColor(ofColor::red);
//            path.arc(point.getCenter(), point.getWidth()/1.5, point.getWidth()/1.5, 0, 270);
////            path.moveTo(point.getCenter());
//            path.draw();
//        }else if(ofGetKeyPressed(OF_KEY_UP) || ofGetKeyPressed(OF_KEY_DOWN)){
//            ofPath path;
//            path.setColor(ofColor::red);
//            path.arc(point.getCenter(), point.getWidth()/1.5, point.getWidth()/1.5, ofMap(minTilt[i], 0., 1., 140, 400), ofMap(maxTilt[i], 0., 1., 140, 400));
//            path.draw();
//        }
//    }
}

void movingheadController::windowResized(ofResizeEventArgs &a){
//    if(externalWindowRect.width >= externalWindowRect.height){
//        isHorizontal = true;
//        int center = externalWindowRect.height/2;
//        float step = float(externalWindowRect.width) / 32.0;
//        float size = min(externalWindowRect.height / 2, externalWindowRect.width/(32*4));
//        for(int i = 0; i < 32; i++){
//            int pos = (step*i) + step/2;
//            points[i].setFromCenter(pos, center, size*2, size*2);
//        }
//    }else{
//        isHorizontal = false;
//        int center = externalWindowRect.width/2;
//        float step = externalWindowRect.height / 32.0;
//        float size = min(externalWindowRect.width / 2, externalWindowRect.height/(32*4));
//        for(int i = 0; i < 32; i++){
//            int pos = (step*i) + step/2;
//            points[i].setFromCenter(center, pos, size*2, size*2);
//        }
//    }
}

void movingheadController::keyPressed(ofKeyEventArgs &a){

}

void movingheadController::keyReleased(ofKeyEventArgs &a){

}

void movingheadController::mousePressed(ofMouseEventArgs &a){
//    if(ofGetKeyPressed()){
//        for(int i = 0; i < points.size(); i++){
//            if(points[i].inside(a)){
//            indexClicked = i;
//            break;
//            }
//        }
//        if(ofGetKeyPressed(OF_KEY_LEFT)){
//            originalValue = minPan[indexClicked];
//        }else if(ofGetKeyPressed(OF_KEY_RIGHT)){
//            originalValue = maxPan[indexClicked];
//        }else if(ofGetKeyPressed(OF_KEY_DOWN)){
//            originalValue = minTilt[indexClicked];
//        }else if(ofGetKeyPressed(OF_KEY_UP)){
//            originalValue = maxTilt[indexClicked];
//        }
//        initialClicPos = a;
//    }
}

void movingheadController::mouseReleased(ofMouseEventArgs &a){
//    indexClicked = -1;
}

void movingheadController::mouseDragged(ofMouseEventArgs &a){
//    if(indexClicked != -1){
//        glm::vec2 amountMoved = a - initialClicPos;
//        if(isHorizontal){
//            float modVal = -amountMoved.y/(externalWindowRect.height/2);
//            if(ofGetKeyPressed(OF_KEY_SHIFT)) modVal /= 2;
//            if(ofGetKeyPressed(OF_KEY_LEFT)){
//                minPan[indexClicked] = originalValue + modVal;
//            }else if(ofGetKeyPressed(OF_KEY_RIGHT)){
//                maxPan[indexClicked] = originalValue + modVal;
//            }else if(ofGetKeyPressed(OF_KEY_DOWN)){
//                minTilt[indexClicked] = originalValue + modVal;
//            }else if(ofGetKeyPressed(OF_KEY_UP)){
//                maxTilt[indexClicked] = originalValue + modVal;
//            }
//            ofLog() <<"Index: " << indexClicked << " Amount: "<< ofClamp(-amountMoved.y/(externalWindowRect.height/2), -1, 1) ;
//        }else{
//            float modVal = -amountMoved.x/(externalWindowRect.width/2);
//            if(ofGetKeyPressed(OF_KEY_SHIFT)) modVal /= 2;
//            if(ofGetKeyPressed(OF_KEY_LEFT)){
//                minPan[indexClicked] = originalValue + modVal;
//            }else if(ofGetKeyPressed(OF_KEY_RIGHT)){
//                maxPan[indexClicked] = originalValue + modVal;
//            }else if(ofGetKeyPressed(OF_KEY_DOWN)){
//                minTilt[indexClicked] = originalValue + modVal;
//            }else if(ofGetKeyPressed(OF_KEY_UP)){
//                maxTilt[indexClicked] = originalValue + modVal;
//            }
//            ofLog() <<"Index: " << indexClicked << " Amount: "<< ofClamp(amountMoved.x /(externalWindowRect.width/2), -1, 1);
//        }
//    }
}

