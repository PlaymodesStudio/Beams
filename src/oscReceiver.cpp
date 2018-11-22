//
//  oscReceiver.cpp
//  Beams_Controlls
//
//  Created by Eduard Frigola Bagué on 16/11/2018.
//

#include "oscReceiver.h"

void oscReceiver::setup(){
    addParameterToGroupAndInfo(port.set("Port", "44444")).isSavePreset = false;
    
    movHeadsInputs = 4;
    sizes.resize(movHeadsInputs);
    pans.resize(movHeadsInputs);
    tilts.resize(movHeadsInputs);
    intensities.resize(movHeadsInputs);
    pansStore.resize(movHeadsInputs);
    tiltsStore.resize(movHeadsInputs);
    intensitiesStore.resize(movHeadsInputs);
    
    colors.resize(movHeadsInputs+9);
    colorsStore.resize(movHeadsInputs+9);
    
    for(int i = 0; i < movHeadsInputs; i++){
        parameters->add(sizes[i].set("Size " + ofToString(i), 1, 1, 100));
        parameters->add(pans[i].set("Pan " + ofToString(i), {0}, {-180}, {180}));
        parameters->add(tilts[i].set("Tilt " + ofToString(i), {0}, {-180}, {180}));
        parameters->add(intensities[i].set("Intensity " + ofToString(i), {0}, {0}, {1}));
        parameters->add(colors[i].set("Heads Color " + ofToString(i), {0}, {0}, {100}));
        listeners.push(sizes[i].newListener([this, i](int s){
            pansStore[i].resize(s);
            tiltsStore[i].resize(s);
            intensitiesStore[i].resize(s);
            colorsStore[i].resize(s);
        }));
    }
    
    parameters->add(colors[movHeadsInputs].set("Wash R", {1}, {0}, {1}));
    parameters->add(colors[movHeadsInputs+1].set("Wash G", {1}, {0}, {1}));
    parameters->add(colors[movHeadsInputs+2].set("Wash B", {1}, {0}, {1}));
    
    parameters->add(colors[movHeadsInputs+3].set("ParL R", {1}, {0}, {1}));
    parameters->add(colors[movHeadsInputs+4].set("ParL G", {1}, {0}, {1}));
    parameters->add(colors[movHeadsInputs+5].set("ParL B", {1}, {0}, {1}));
    
    parameters->add(colors[movHeadsInputs+6].set("ParR R", {1}, {0}, {1}));
    parameters->add(colors[movHeadsInputs+7].set("ParR G", {1}, {0}, {1}));
    parameters->add(colors[movHeadsInputs+8].set("ParR B", {1}, {0}, {1}));
    
    for(int i = 0; i < movHeadsInputs; i++){
        pansStore[i] = pans[i];
        tiltsStore[i] = tilts[i];
        intensitiesStore[i] = intensities[i];
    }
    
    for(int i = 0; i < colorsStore.size(); i++){
        colorsStore[i] = colors[i];
    }
    
    listeners.push(port.newListener([this](string &s){setupReceiver();}));
    
    setupReceiver();
}

void oscReceiver::update(ofEventArgs &a){
    while(receiver->hasWaitingMessages()){
        ofxOscMessage m;
        receiver->getNextMessage(m);
        
        vector<string> splitAddress = ofSplitString(m.getAddress(), "/");
        if(splitAddress[0].size() == 0) splitAddress.erase(splitAddress.begin());
        if(splitAddress.size() == 3){
            int id = ofToInt(splitAddress[1])-1;
            if(splitAddress[0] == "movingHead"){
                int movGroup = 0;
                int accumulSizes = sizes[0];
                while(id >= accumulSizes){
                    movGroup++;
                    if(movGroup >= movHeadsInputs) return;
                    accumulSizes += sizes[movGroup];
                }
                int idInGroup = id-(accumulSizes-sizes[movGroup]);
                if(splitAddress[2] == "pan"){
                    pansStore[movGroup][idInGroup] = m.getArgAsFloat(0);
                }
                else if(splitAddress[2] == "tilt"){
                    tiltsStore[movGroup][idInGroup] = m.getArgAsFloat(0);
                }
                else if(splitAddress[2] == "intensity"){
                    intensitiesStore[movGroup][idInGroup] = m.getArgAsFloat(0);
                }
                else if(splitAddress[2] == "colorIndex"){
                    colorsStore[movGroup][idInGroup] = m.getArgAsFloat(0);
                }
            }
            else if(splitAddress[0] == "wash"){
                if(splitAddress[2] == "color"){
                    if(!(colorsStore[movHeadsInputs].size() > id)){
                        colorsStore[movHeadsInputs].resize(id);
                        colorsStore[movHeadsInputs+1].resize(id);
                        colorsStore[movHeadsInputs+2].resize(id);
                    }
                    colorsStore[movHeadsInputs][id] = m.getArgAsFloat(0);
                    colorsStore[movHeadsInputs + 1][id] = m.getArgAsFloat(1);
                    colorsStore[movHeadsInputs + 2][id] = m.getArgAsFloat(2);
                }
            }
            else if(splitAddress[0] == "panL"){
                if(splitAddress[2] == "color"){
                    if(!(colorsStore[movHeadsInputs + 3].size() > id)){
                        colorsStore[movHeadsInputs+3].resize(id);
                        colorsStore[movHeadsInputs+4].resize(id);
                        colorsStore[movHeadsInputs+5].resize(id);
                    }
                    colorsStore[movHeadsInputs + 3][id] = m.getArgAsFloat(0);
                    colorsStore[movHeadsInputs + 4][id] = m.getArgAsFloat(1);
                    colorsStore[movHeadsInputs + 5][id] = m.getArgAsFloat(2);
                }
            }
            else if(splitAddress[0] == "panR"){
                if(splitAddress[2] == "color"){
                    if(!(colorsStore[movHeadsInputs + 6].size() > id)){
                        colorsStore[movHeadsInputs+6].resize(id);
                        colorsStore[movHeadsInputs+7].resize(id);
                        colorsStore[movHeadsInputs+8].resize(id);
                    }
                    colorsStore[movHeadsInputs + 6][id] = m.getArgAsFloat(0);
                    colorsStore[movHeadsInputs + 7][id] = m.getArgAsFloat(1);
                    colorsStore[movHeadsInputs + 8][id] = m.getArgAsFloat(2);
                }
            }
        }
    }
    for(int i = 0; i < pans.size(); i++){
        pans[i] = pansStore[i];
        tilts[i] = tiltsStore[i];
        intensities[i] = intensitiesStore[i];
    }
    for(int i = 0; i < colorsStore.size(); i++){
        colors[i] = colorsStore[i];
    }
}
