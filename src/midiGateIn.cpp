//
//  midiGateIn.cpp
//  MIRABCN_Generator
//
//  Created by Eduard Frigola on 14/07/2017.
//
//

#include "midiGateIn.h"

midiGateIn::midiGateIn() : ofxOceanodeNodeModel("Midi Note In"){
    midiIn = nullptr;
}

void midiGateIn::setup(){
    midiIn = new ofxMidiIn();
    
    vector<string> ports = {"None"};
    ports.resize(1+midiIn->getNumInPorts());
    for(int i = 0; i < midiIn->getNumInPorts(); i++){
        ports[i+1] = midiIn->getInPortList()[i];
    }
    parameters->add(createDropdownAbstractParameter("Midi Device", ports, midiDevice));
    parameters->add(midiChannel.set("Midi Channel", 0, 0, 16));
    parameters->add(noteOnStart.set("Note Begin", 0, 0, 127));
    parameters->add(noteOnEnd.set("Note End", 127, 0, 127));
    parameters->add(output.set("Output", {0}, {0}, {1}));
    
    outputStore.resize(noteOnEnd - noteOnStart + 1, 0);
    listeners.push(noteOnStart.newListener(this, &midiGateIn::noteRangeChanged));
    listeners.push(noteOnEnd.newListener(this, &midiGateIn::noteRangeChanged));
    listeners.push(midiDevice.newListener(this, &midiGateIn::midiDeviceListener));
}

void midiGateIn::update(ofEventArgs &e){
    if(mutex.try_lock()){
        parameters->get("Output").cast<vector<float>>() = outputStore;
        mutex.unlock();
    }
}

void midiGateIn::newMidiMessage(ofxMidiMessage &eventArgs){
    if(eventArgs.status == MIDI_NOTE_ON && (eventArgs.channel == midiChannel || midiChannel == 0)){
        if(eventArgs.pitch >= noteOnStart && eventArgs.pitch <= noteOnEnd){
            {
                mutex.lock();
                outputStore[eventArgs.pitch - noteOnStart] = (float)eventArgs.velocity/(float)127;
                mutex.unlock();
            }
        }
    }else if(eventArgs.status == MIDI_NOTE_OFF && (eventArgs.channel == midiChannel || midiChannel == 0)){
        if(eventArgs.pitch >= noteOnStart && eventArgs.pitch <= noteOnEnd){
            {
                mutex.lock();
                outputStore[eventArgs.pitch - noteOnStart] = 0;
                mutex.unlock();
            }
        }
    }
}

void midiGateIn::midiDeviceListener(int &device){
    outputStore = vector<float>(noteOnEnd - noteOnStart + 1, 0);
    midiIn->closePort();
    if(device > 0){
        midiIn->openPort(device-1);
        midiIn->addListener(this);
    }
}

void midiGateIn::noteRangeChanged(int &note){
    if(noteOnEnd < noteOnStart){
        noteOnStart = 0;
    }else{
        outputStore.resize(noteOnEnd - noteOnStart + 1, 0);
    }
}
