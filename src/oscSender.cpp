//
//  oscSender.cpp
//  Beams_Controlls
//
//  Created by Eduard Frigola Bagué on 14/11/2018.
//

#include "oscSender.h"

void oscSender::setup(){
    addParameterToGroupAndInfo(host.set("Host", "127.0.0.1")).isSavePreset = false;
    addParameterToGroupAndInfo(port.set("Port", "55555")).isSavePreset = false;
    parameters->add(pan.set("Pan In", {0}, {-180}, {180}));
    parameters->add(tilt.set("Tilt In", {0}, {-180}, {180}));
    
    colors.resize(4);
    parameters->add(colors[0].set("Heads Color", {1}, {0}, {1}));
    
    parameters->add(colors[1].set("Wash Color", {1}, {0}, {1}));
    
    parameters->add(colors[2].set("ParL Color", {1}, {0}, {1}));
    parameters->add(colors[3].set("ParR Color", {1}, {0}, {1}));
    
    listeners.push(host.newListener([this](string &s){setupSender();}));
    listeners.push(port.newListener([this](string &s){setupSender();}));
    
    setupSender();
}

void oscSender::update(ofEventArgs &a){
    auto sendBanksStartingAtIndex = [this](vector<float> pan, vector<float> tilt, int index) -> int{
        for(int i = 0; i < pan.size(); i++){
            ofxOscMessage m;
            m.setAddress("/movingHead/" + ofToString(i + index) + "/pan");
            m.addFloatArg(pan[i]);
            sender->sendMessage(m, false);
        }
        for(int i = 0; i < tilt.size(); i++){
            ofxOscMessage m;
            m.setAddress("/movingHead/" + ofToString(i + index) + "/tilt");
            m.addFloatArg(tilt[i]);
            sender->sendMessage(m, false);
        }
    
        return max(pan.size(), tilt.size()) + index;
    };
    
    auto getValForIndex = [this](const vector<float> &vf, int index)->float{
        if(vf.size() == 1 || vf.size() <= index){
            return vf[0];
        }
        else{
            return vf[index];
        }
    };
    
    if(sender != nullptr){
        int lastIndex = 0;
        sendBanksStartingAtIndex(pan, tilt, lastIndex);
        
        int maxColors = colors[0]->size()/3;
        for(int i = 0; i < maxColors * 3; i += 3){
            ofxOscMessage m;
            m.setAddress("/movingHead/" + ofToString(i/3) + "/color");
            m.addFloatArg(getValForIndex(colors[0], i));
            m.addFloatArg(getValForIndex(colors[0], i+1));
            m.addFloatArg(getValForIndex(colors[0], i+2));
            sender->sendMessage(m, false);
        }
        
        maxColors = colors[1]->size()/3;
        for(int i = 0; i < maxColors*3; i += 3){
            ofxOscMessage m;
            m.setAddress("/wash/" + ofToString(i/3) + "/color");
            m.addFloatArg(getValForIndex(colors[1], i));
            m.addFloatArg(getValForIndex(colors[1], i+1));
            m.addFloatArg(getValForIndex(colors[1], i+2));
            sender->sendMessage(m, false);
        }
        
        maxColors = colors[2]->size()/3;
        for(int i = 0; i < maxColors*3; i += 3){
            ofxOscMessage m;
            m.setAddress("/parL/" + ofToString(i/3) + "/color");
            m.addFloatArg(getValForIndex(colors[2], i));
            m.addFloatArg(getValForIndex(colors[2], i+1));
            m.addFloatArg(getValForIndex(colors[2], i+2));
            sender->sendMessage(m, false);
        }
        
        maxColors = colors[3]->size()/3;
        for(int i = 0; i < maxColors*3; i += 3){
            ofxOscMessage m;
            m.setAddress("/parR/" + ofToString(i/3) + "/color");
            m.addFloatArg(getValForIndex(colors[3], i));
            m.addFloatArg(getValForIndex(colors[3], i+1));
            m.addFloatArg(getValForIndex(colors[3], i+2));
            sender->sendMessage(m, false);
        }
    }
}
