//
//  envelopeGenerator.cpp
//  MIRABCN_Generator
//
//  Created by Eduard Frigola on 12/07/2017.
//
//

#include "envelopeGenerator.h"

envelopeGenerator::envelopeGenerator() : ofxOceanodeNodeModel("Envelope Generator"){
    easeStringFuncs = {"EASE_LINEAR", " EASE_IN_QUAD", " EASE_OUT_QUAD", " EASE_IN_OUT_QUAD", " EASE_IN_CUBIC", " EASE_OUT_CUBIC", " EASE_IN_OUT_CUBIC", " EASE_IN_QUART", " EASE_OUT_QUART", " EASE_IN_OUT_QUART", " EASE_IN_QUINT", " EASE_OUT_QUINT", " EASE_IN_OUT_QUINT", " EASE_IN_SINE", " EASE_OUT_SINE", " EASE_IN_OUT_SINE", " EASE_IN_EXPO", " EASE_OUT_EXPO", " EASE_IN_OUT_EXPO", " EASE_IN_CIRC", " EASE_OUT_CIRC", "EASE_IN_OUT_CIRC"};
    
    parameters->add(phasor.set("Phasor In", 0, 0, 1));
    parameters->add(gateIn.set("Gate In", {0}, {0}, {1}));
    parameters->add(attack.set("Attack", 0, 0, 1));
    parameters->add(decay.set("Decay", 0, 0, 1));
    parameters->add(sustain.set("Sustain", 1, 0, 1));
    parameters->add(release.set("Release", 0, 0, 1));

    parameters->add(createDropdownAbstractParameter("Attack Ease", easeStringFuncs, attackCurve));
    parameters->add(createDropdownAbstractParameter("Decay Ease", easeStringFuncs, decayCurve));
    parameters->add(createDropdownAbstractParameter("Release Ease", easeStringFuncs, releaseCurve));
    parameters->add(output.set("Output", {0}, {0}, {1}));
    oldGateIn = {0};
    
    outputComputeVec = {0};
    
    listener  = phasor.newListener(this, &envelopeGenerator::phasorListener);
    
    lastGateInSize = -1;
}

void envelopeGenerator::phasorListener(float &f){
    int inputSize = gateIn.get().size();
    if(inputSize != lastGateInSize){
        output = vector<float>(inputSize, 0);
        lastInput = vector<float>(inputSize, 0);
        lastChangedValue = vector<float>(inputSize, 0);
        phasorValueOnValueChange = vector<float>(inputSize, f);
        lastPhase = vector<float>(inputSize, 0);
        reachedMax = vector<bool>(inputSize, false);
        envelopeStage = vector<int>(inputSize, envelopeEnd);
        lastGateInSize = inputSize;
    }else{
        for(int i = 0; i < inputSize; i++){
            if(lastInput[i] == 0 && gateIn.get()[i] != 0){
                if(attack == 0){
                    if(decay == 0){
                        envelopeStage[i] = envelopeSustain;
                    }else{
                        envelopeStage[i] = envelopeDecay;
                    }
                }else{
                    envelopeStage[i] = envelopeAttack;
                }
                phasorValueOnValueChange[i] = f;
                reachedMax[i] = false;
                lastPhase[i] = 0;
                lastChangedValue[i] = lastInput[i];
            }else if(lastInput[i] != 0 && gateIn.get()[i] == 0){
                if(release == 0){
                    envelopeStage[i] = envelopeEnd;
                }else{
                    envelopeStage[i] = envelopeRelease;
                }
                phasorValueOnValueChange[i] = f;
                reachedMax[i] = false;
                lastPhase[i] = 0;
                lastChangedValue[i] = lastInput[i];
            }
        }
        vector<float> tempOutput = gateIn.get();
        for(int i = 0; i < inputSize; i++){
            float phase = f - phasorValueOnValueChange[i];
            if(phase < 0) phase = 1+phase;
            if(phase < lastPhase[i]) reachedMax[i] = true;
            else lastPhase[i] = phase;
            
            if(envelopeStage[i] == envelopeAttack){
                if(phase > attack || reachedMax[i] == true){
                    phasorValueOnValueChange[i] = f;
                    if(decay == 0){
                        envelopeStage[i] = envelopeSustain;
                    }else{
                        envelopeStage[i] = envelopeDecay;
                    }
                    reachedMax[i] = false;
                    lastPhase[i] = 0;
                    phase = 0;
                }else{
                    tempOutput[i] = ofxeasing::map(phase, 0, attack, 0, gateIn.get()[i], easingFromString(easeStringFuncs[attackCurve]));
                }
            }
            if(envelopeStage[i] == envelopeDecay){
                if(phase > decay || reachedMax[i] == true){
                    phasorValueOnValueChange[i] = f;
                    envelopeStage[i] = envelopeSustain;
                    reachedMax[i] = false;
                    lastPhase[i] = 0;
                    phase = 0;
                }else{
                    tempOutput[i] = ofxeasing::map(phase, 0, decay, gateIn.get()[i], sustain * gateIn.get()[i], easingFromString(easeStringFuncs[decayCurve]));
                }
            }
            if(envelopeStage[i] == envelopeSustain){
                tempOutput[i] = gateIn.get()[i] * sustain;
            }
            if(envelopeStage[i] == envelopeRelease){
                if(phase > release || reachedMax[i] == true){
                    phasorValueOnValueChange[i] = f;
                    envelopeStage[i] = envelopeEnd;
                    reachedMax[i] = false;
                    lastPhase[i] = 0;
                    phase = 0;
                }else{
                    tempOutput[i] = ofxeasing::map(phase, 0, release, sustain * lastChangedValue[i], 0, easingFromString(easeStringFuncs[releaseCurve]));
                }
            }
            if(envelopeStage[i] == envelopeEnd){
                tempOutput[i] = 0;
            }
        }
        lastInput = gateIn;
        output = tempOutput;
    }
}


ofxeasing::function envelopeGenerator::easingFromString(string easing){
    ofxeasing::Function easingFunc;
    ofxeasing::Type easingType;
//    {"EASE_LINEAR", " EASE_IN_QUAD", " EASE_OUT_QUAD", " EASE_IN_OUT_QUAD", " EASE_IN_CUBIC", " EASE_OUT_CUBIC", " EASE_IN_OUT_CUBIC", " EASE_IN_QUART", " EASE_OUT_QUART", " EASE_IN_OUT_QUART", " EASE_IN_QUINT", " EASE_OUT_QUINT", " EASE_IN_OUT_QUINT", " EASE_IN_SINE", " EASE_OUT_SINE", " EASE_IN_OUT_SINE", " EASE_IN_EXPO", " EASE_OUT_EXPO", " EASE_IN_OUT_EXPO", " EASE_IN_CIRC", " EASE_OUT_CIRC", "EASE_IN_OUT_CIRC"};
    //Type
    if(ofStringTimesInString(easing, "EASE_IN") == 1){
        easingType = ofxeasing::Type::In;
    }
    else if(ofStringTimesInString(easing, "EASE_IN_OUT") == 1){
        easingType = ofxeasing::Type::InOut;
    }
    else if(ofStringTimesInString(easing, "EASE_OUT") == 1){
        easingType = ofxeasing::Type::Out;
    }
    
    //Function
    if(ofStringTimesInString(easing, "LINEAR") == 1){
        return ofxeasing::linear::easeNone;
    }
    else if(ofStringTimesInString(easing, "QUAD") == 1){
        easingFunc = ofxeasing::Function::Quadratic;
    }
    else if(ofStringTimesInString(easing, "CUBIC") == 1){
        easingFunc = ofxeasing::Function::Cubic;
    }
    else if(ofStringTimesInString(easing, "QUART") == 1){
        easingFunc = ofxeasing::Function::Quartic;
    }
    else if(ofStringTimesInString(easing, "QUINT") == 1){
        easingFunc = ofxeasing::Function::Quintic;
    }
    else if(ofStringTimesInString(easing, "SINE") == 1){
        easingFunc = ofxeasing::Function::Sine;
    }
    else if(ofStringTimesInString(easing, "EXPO") == 1){
        easingFunc = ofxeasing::Function::Exponential;
    }
    else if(ofStringTimesInString(easing, "CIRC") == 1){
        easingFunc = ofxeasing::Function::Circular;
    }
    return ofxeasing::easing(easingFunc, easingType);
}

