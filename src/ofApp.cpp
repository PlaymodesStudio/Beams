#include "ofApp.h"
#include "vector_op_registrator.h"
#include "oscillatorTexture.h"
#include "waveScope.h"
#include "scriptModule.h"
#include "scriptModule.h"
#include "movingheadController.h"
#include "textureReader.h"
#include "artnetSender.h"
#include "vectorPresetEaser.h"
#include "valueEaser.h"
#include "midiGateIn.h"
#include "envelopeGenerator.h"
#include "audioEngineController.h"
#include "delta.h"
#include "pantiltEditor.h"
#include "manualOscillatorBank.h"
#include "strobeLightController.h"
#include "colorGradient.h"
#include "vectorModulator.h"
#include "followPoint.h"
#include "vectorSwitcher.h"
#include "oscSender.h"
#include "oscReceiver.h"
#include "sequentialAnalyzer.h"
#include "followIndexsToMovingHeads.h"
#include "senderManager.h"
#include "textureUnifier.h"
#include "texturePixelmap.h"
#include "vectorToTexture.h"
#include "colorApplier.h"

#include "ofxOceanodeBPMController.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetEscapeQuitsApp(false);
    ofDisableArbTex();
    ofSetVerticalSync(false);
    ofSetFrameRate(40);
    
    auto reg = make_shared<ofxOceanodeNodeRegistry>();
    auto treg = make_shared<ofxOceanodeTypesRegistry>();
    reg->registerModel<oscillatorTexture>("Texture");
    reg->registerModel<waveScope>("Texture");
    reg->registerModel<scriptModule>("Scripting");
    reg->registerModel<movingheadController>("MovingHeads");
    reg->registerModel<textureReader>("Texture");
    reg->registerModel<artnetSender>("ArtNet");
    reg->registerModel<vectorPresetEaser>("EASER");
    reg->registerModel<valueEaser>("EASER");
    reg->registerModel<midiGateIn>("Midi");
    reg->registerModel<envelopeGenerator>("Midi");
    reg->registerModel<audioEngineController>("OSC");
    reg->registerModel<delta>("OP");
    reg->registerModel<panTiltEditor>("MovingHeads");
    reg->registerModel<manualOscillatorBank>("Generators");
    reg->registerModel<strobeLightController>("PARS");
    reg->registerModel<colorGradient>("Color");
    reg->registerModel<vectorModulator>("MovingHeads");
    reg->registerModel<followPoint>("MovingHeads");
    reg->registerModel<vectorSwitcher>("Vector Operations");
    reg->registerModel<oscSender>("OSC");
    reg->registerModel<oscReceiver>("OSC");
    reg->registerModel<sequentialAnalyzer>("Modifiers");
    reg->registerModel<followIndexsToMovingHeads>("MovingHeads");
    reg->registerModel<senderManager>("Texture");
    reg->registerModel<textureUnifier>("Texture");
    reg->registerModel<texturePixelmap>("Texture");
    reg->registerModel<vectorToTexture>("Texture");
    reg->registerModel<colorApplier>("Texture");
    
    registerVectorOp(reg);
    treg->registerType<ofTexture*>();
    
    container = make_shared<ofxOceanodeContainer>(reg, treg);
    canvas.setContainer(container);
    canvas.setup();
    
    controls = make_unique<ofxOceanodeControls>(container);
    controls->get<ofxOceanodeBPMController>()->setBPM(120 );
    
    container->loadPersistent();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, 10);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(ofGetKeyPressed(OF_KEY_COMMAND)){
        if(key == 'p') container->savePersistent();
        else if(key == 'u') container->updatePersistent();
        else if(key == 'k') container->collapseGuis();
        else if(key == 'e') container->expandGuis();
        else if(key == 's') container->saveCurrentPreset();
        else if(key == 'r') container->resetPhase();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
