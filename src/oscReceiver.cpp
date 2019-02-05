//
//  oscReceiver.cpp
//  Beams_Controlls
//
//  Created by Eduard Frigola Bagué on 16/11/2018.
//

#include "oscReceiver.h"

void oscReceiver::setup(){
    addParameterToGroupAndInfo(port.set("Port", "44444")).isSavePreset = false;
    
//    movHeadsInputs = 4;
//    sizes.resize(movHeadsInputs);
//    pans.resize(movHeadsInputs);
//    tilts.resize(movHeadsInputs);
//    intensities.resize(movHeadsInputs);
//    pansStore.resize(movHeadsInputs);
//    tiltsStore.resize(movHeadsInputs);
//    intensitiesStore.resize(movHeadsInputs);
//    
//    colors.resize(movHeadsInputs+9);
//    colorsStore.resize(movHeadsInputs+9);
//    
    parameters->add(size.set("Size", 1, 1, 100));
    parameters->add(x.set("X", {0}, {-20}, {20}));
    parameters->add(y.set("Z", {0}, {-50}, {50}));
    parameters->add(bool1.set("Bool 1", {0}, {0}, {1}));
    parameters->add(bool2.set("Bool 2", {0}, {0}, {1}));
    parameters->add(bool3.set("Bool 3", {0}, {0}, {1}));
//        parameters->add(intensities[i].set("Intensity " + ofToString(i), {0}, {0}, {1}));
//        parameters->add(colors[i].set("Heads Color " + ofToString(i), {0}, {0}, {100}));
//        listeners.push(sizes[i].newListener([this, i](int s){
//            pansStore[i].resize(s);
//            tiltsStore[i].resize(s);
//            intensitiesStore[i].resize(s);
//            colorsStore[i].resize(s);
//        }));
//    }
//    
//    parameters->add(colors[movHeadsInputs].set("Wash R", {1}, {0}, {1}));
//    parameters->add(colors[movHeadsInputs+1].set("Wash G", {1}, {0}, {1}));
//    parameters->add(colors[movHeadsInputs+2].set("Wash B", {1}, {0}, {1}));
//    
//    parameters->add(colors[movHeadsInputs+3].set("ParL R", {1}, {0}, {1}));
//    parameters->add(colors[movHeadsInputs+4].set("ParL G", {1}, {0}, {1}));
//    parameters->add(colors[movHeadsInputs+5].set("ParL B", {1}, {0}, {1}));
//    
//    parameters->add(colors[movHeadsInputs+6].set("ParR R", {1}, {0}, {1}));
//    parameters->add(colors[movHeadsInputs+7].set("ParR G", {1}, {0}, {1}));
//    parameters->add(colors[movHeadsInputs+8].set("ParR B", {1}, {0}, {1}));
//    
//    for(int i = 0; i < movHeadsInputs; i++){
//        pansStore[i] = pans[i];
//        tiltsStore[i] = tilts[i];
//        intensitiesStore[i] = intensities[i];
//    }
//    
//    for(int i = 0; i < colorsStore.size(); i++){
//        colorsStore[i] = colors[i];
//    }
    xStore = x;
    yStore = y;
    bool1Store = bool1;
    bool2Store = bool2;
    bool3Store = bool3;
    
    listeners.push(port.newListener([this](string &s){setupReceiver();}));
    listeners.push(size.newListener([this](int &i){
        xStore.resize(i, 0);
        yStore.resize(i, 0);
        bool1Store.resize(i, 0);
        bool2Store.resize(i, 0);
        bool3Store.resize(i, 0);
    }));
    
    setupReceiver();
}

void oscReceiver::update(ofEventArgs &a){
    while(receiver->hasWaitingMessages()){
        ofxOscMessage m;
        receiver->getNextMessage(m);
        
        vector<string> splitAddress = ofSplitString(m.getAddress(), "/");
        if(splitAddress[0].size() == 0) splitAddress.erase(splitAddress.begin());
        if(splitAddress.size() == 2){
            if(splitAddress[0] == "point"){
                int id = ofToInt(splitAddress[1])-1;
                if(id <= size){
                    xStore[id] = ofMap(m.getArgAsFloat(0), 1, 0, x.getMin()[0], x.getMax()[0]);
                    yStore[id] = ofMap(m.getArgAsFloat(1), 1, 0, y.getMin()[0], y.getMax()[0]);
                }
            }
        }else if(splitAddress.size() == 4){
            if(splitAddress[0] == "toggle"){
                int id = ofToInt(splitAddress[1])-1;
                if(id <= size){
                    switch (ofToInt(splitAddress[2])-1) {
                        case 0:
                            bool1Store[id] = m.getArgAsBool(0);
                            break;
                        case 1:
                            bool2Store[id] = m.getArgAsBool(0);
                            break;
                        case 2:
                            bool3Store[id] = m.getArgAsBool(0);
                            break;
                        default:
                            break;
                    }
                }
            }
            else if(splitAddress[0] == "config"){
                if(splitAddress[1] == "num"){
                    size = ofToInt(splitAddress[2]);
                }
                
            }
        }
    }
    x = xStore;
    y = yStore;
    bool1 = bool1Store;
    bool2 = bool2Store;
    bool3 = bool3Store;
}
