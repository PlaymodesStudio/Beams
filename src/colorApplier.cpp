//
//  colorApplier.cpp
//  MIRABCN_Generator
//
//  Created by Eduard Frigola on 01/12/2016.
//
//

#include "colorApplier.h"
#include "sharedResources.h"

void colorApplier::setup(){
    isSetup = false;
    resources = &sharedResources::getInstance();
    
    parameters->add(reloadShaderParam.set("Reload Shader", false));
    parameters->add(colorPickerParam[0].set("Color 1", ofColor::white));
    parameters->add(colorRParam[0].set("Color 1 R", 255, 0, 255));
    parameters->add(colorGParam[0].set("Color 1 G", 255, 0, 255));
    parameters->add(colorBParam[0].set("Color 1 B", 255, 0, 255));
    addParameterToGroupAndInfo(colorHParam[0].set("Color 1 Hue", 0, 0, 360)).isSavePreset = false;
    
    parameters->add(colorPickerParam[1].set("Color 2", ofColor::white));
    parameters->add(colorRParam[1].set("Color 2 R", 0, 0, 255));
    parameters->add(colorGParam[1].set("Color 2 G", 0, 0, 255));
    parameters->add(colorBParam[1].set("Color 2 B", 0, 0, 255));
    addParameterToGroupAndInfo(colorHParam[1].set("Color 2 Hue", 0, 0, 360)).isSavePreset = false;
    parameters->add(colorDisplacement.set("Color Displacement", 0, 0, 1));
    
    //    parameters->add(randomColorStepsParam.set("Rnd Color Steps", 4, 0, 255));
    //    sharedResources::addDropdownToParameterGroupFromParameters(parameters, "Rnd ChangeTypes", {"no", "on presset", "onTrigger"}, randomizeTypeColorParam);
    
    parameters->add(textureImage.set("Texture Image", nullptr));
    
    parameters->add(modulationInfo[0].set("Mod in X", vector<float>(1, 0), vector<float>(1, 0), vector<float>(1, 1)));
    parameters->add(modulationInfo[1].set("Mod in Y", vector<float>(1, 0), vector<float>(1, 0), vector<float>(1, 1)));
    
    parameters->add(grayScaleIn.set("Input", nullptr));
    parameters->add(gradientPreview.set("Gradient Preview", nullptr));
    parameters->add(colorizedValues.set("Output", nullptr));
    
//    listeners.push(colorPickerParam[0].newListener(this, &colorApplier::colorListener));
    listeners.push(colorRParam[0].newListener(this, &colorApplier::colorSliderListener));
    listeners.push(colorGParam[0].newListener(this, &colorApplier::colorSliderListener));
    listeners.push(colorBParam[0].newListener(this, &colorApplier::colorSliderListener));
    
//    listeners.push(colorPickerParam[1].newListener(this, &colorApplier::colorListener));
    listeners.push(colorRParam[1].newListener(this, &colorApplier::colorSliderListener));
    listeners.push(colorGParam[1].newListener(this, &colorApplier::colorSliderListener));
    listeners.push(colorBParam[1].newListener(this, &colorApplier::colorSliderListener));
    
    listeners.push(colorHParam[0].newListener(this, &colorApplier::colorHueListener));
    listeners.push(colorHParam[1].newListener(this, &colorApplier::colorHueListener));
    
    listeners.push(colorDisplacement.newListener(this, &colorApplier::colorDisplacementChanged));
//    listeners.push(grayScaleIn.newListener(this, &colorApplier::applyColor));
    //    reloadShaderParam.newListener(this, &colorApplier::reloadShader);
    
    colorIsChanging = false;
    
    oldColorDisplacement = colorDisplacement;
    
    listeners.push(modulationInfo[0].newListener(this, &colorApplier::modulationInfoListener));
    listeners.push(modulationInfo[1].newListener(this, &colorApplier::modulationInfoListener));
    colorDisplacementUpdated = -1;
}

colorApplier::~colorApplier(){
    if(isSetup){
        modulationInfoTexture.clear();
        colorDisplacementTexture.clear();
        for(auto &loc : shaderLocations){
            resources->makeTextureLocationAvailable(loc);
        }
        resources->makeTextureLocationAvailable(infoTextureOutputShaderTextureLocation);
        resources->makeTextureLocationAvailable(imageTextureOutputShaderTextureLocation);
        resources->makeTextureLocationAvailable(infoTexturePreviewShaderTextureLocation);
        resources->makeTextureLocationAvailable(imageTexturePreviewShaderTextureLocation);
    }
}

void colorApplier::reloadShader(bool &b){
    shaderLocations.resize(4);
    for(auto &loc : shaderLocations){
        loc = resources->getNextAvailableShaderTextureLocation();
    }
    
    outputShader.load("Shaders/color.vert", "Shaders/color.frag");
    outputShader.begin();
    outputShader.setUniformTexture("modulationInfo", modulationInfoTexture, shaderLocations[0]);
    outputShader.setUniformTexture("displacementInfo", colorDisplacementTexture, shaderLocations[1]);
    outputShader.end();
    
    previewShader.load("Shaders/color.vert", "Shaders/color.frag");
    previewShader.begin();
    previewShader.setUniformTexture("modulationInfo", modulationInfoTexture, shaderLocations[2]);
    previewShader.setUniformTexture("displacementInfo", colorDisplacementTexture, shaderLocations[3]);
    previewShader.end();
    
    
}

void colorApplier::draw(ofEventArgs &a){
    if(!isSetup){
        infoTextureOutputShaderTextureLocation = resources->getNextAvailableShaderTextureLocation();
        imageTextureOutputShaderTextureLocation = resources->getNextAvailableShaderTextureLocation();
        infoTexturePreviewShaderTextureLocation = resources->getNextAvailableShaderTextureLocation();
        imageTexturePreviewShaderTextureLocation = resources->getNextAvailableShaderTextureLocation();
        
        modulationInfoBuffer.allocate();
        modulationInfoBuffer.bind(GL_TEXTURE_BUFFER);
        modulationInfoBuffer.setData(vector<float>(1, -1), GL_STREAM_DRAW);
        modulationInfoTexture.allocateAsBufferTexture(modulationInfoBuffer, GL_R32F);
        
        
        colorDisplacementBuffer.allocate();
        colorDisplacementBuffer.bind(GL_TEXTURE_BUFFER);
        colorDisplacementBuffer.setData(vector<ofVec3f>(1, ofVec3f(-1, -1, -1)), GL_STREAM_DRAW);
        colorDisplacementTexture.allocateAsBufferTexture(colorDisplacementBuffer, GL_RGB32F);
        
        bool sillyBool = true;
        reloadShader(sillyBool);
        isSetup = true;
    }
    if(modulationInfoXUpdated.size() != 0){
        if(modulationInfoXUpdated.size() == 1){
            modulationInfoBuffer.updateData(0, vector<float>(width, -1));
        }else if(modulationInfoXUpdated.size() == width){
            modulationInfoBuffer.updateData(0, modulationInfoXUpdated);
        }
        modulationInfoXUpdated.clear();
    }
    if(modulationInfoYUpdated.size() != 0){
        if(modulationInfoYUpdated.size() == 1){
            modulationInfoBuffer.updateData(width*4, vector<float>(height, -1));
        }else if(modulationInfoYUpdated.size() == height){
            modulationInfoBuffer.updateData(width*4, modulationInfoYUpdated);
        }
        modulationInfoYUpdated.clear();
    }
    if(colorDisplacementUpdated != -1){
        colorDisplacementBuffer.updateData(0, computeNewColorDisplacementVector(colorDisplacementUpdated));
        colorDisplacementUpdated = -1;
    }
    
    if(grayScaleIn.get() != nullptr){
        width = grayScaleIn.get()->getWidth();
        height = grayScaleIn.get()->getHeight();
    }
    else if(textureImage.get() != nullptr){
        width = textureImage.get()->getWidth();
        height = textureImage.get()->getHeight();
    }else{
        width = -1;
        height = -1;
    }
    if(width != -1 && height != -1){
        if(outputFbo.getWidth() != width || outputFbo.getHeight() != height || !outputFbo.isAllocated()){
            outputFbo.allocate(width, height, GL_RGBA32F);
            outputFbo.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
            previewFbo.allocate(width, height, GL_RGBA32F);
            whiteFbo.allocate(width, height, GL_RGBA32F);
            whiteFbo.begin();
            ofSetColor(255, 255, 255);
            ofDrawRectangle(0, 0, width, height);
            whiteFbo.end();
            modulationInfoBuffer.setData(vector<float>(width+height, -1), GL_STREAM_DRAW);
            colorDisplacementBuffer.setData(computeNewColorDisplacementVector(colorDisplacement),  GL_STREAM_DRAW);
        }
        
        outputFbo.begin();
        outputShader.begin();
        outputShader.setUniform1i("width", width);
        outputShader.setUniform1f("displacement", colorDisplacement);
        outputShader.setUniform1i("useTexture", textureImage.get() != nullptr);
        outputShader.setUniform3f("color1", colorRParam[0]/255., colorGParam[0]/255., colorBParam[0]/255.);
        outputShader.setUniform3f("color2", colorRParam[1]/255., colorGParam[1]/255., colorBParam[1]/255.);
        if(grayScaleIn.get() != nullptr){
            outputShader.setUniformTexture("inputTexture", *grayScaleIn, infoTextureOutputShaderTextureLocation);
        }else{
            outputShader.setUniformTexture("inputTexture", whiteFbo.getTexture(), infoTexturePreviewShaderTextureLocation);
        }
        if(textureImage.get() != nullptr){
            outputShader.setUniformTexture("inputImage", *textureImage, imageTextureOutputShaderTextureLocation);
        }
        
        if(grayScaleIn.get() != nullptr){
            grayScaleIn.get()->draw(0, 0);
        }
        else{
            ofPushStyle();
            ofSetColor(255);
            ofDrawRectangle(0, 0, width, height);
            ofPopStyle();
        }
        outputShader.end();
        outputFbo.end();
        parameters->get("Output").cast<ofTexture*>() = &outputFbo.getTexture();
        
        
        previewFbo.begin();
        previewShader.begin();
        previewShader.setUniform1i("width", width);
        previewShader.setUniform1f("displacement", colorDisplacement);
        previewShader.setUniform1i("useTexture", textureImage.get() != nullptr);
        previewShader.setUniform3f("color1", colorRParam[0]/255., colorGParam[0]/255., colorBParam[0]/255.);
        previewShader.setUniform3f("color2", colorRParam[1]/255., colorGParam[1]/255., colorBParam[1]/255.);
        previewShader.setUniformTexture("inputTexture", whiteFbo.getTexture(), infoTexturePreviewShaderTextureLocation);
        if(textureImage.get() != nullptr){
            previewShader.setUniformTexture("inputImage", *textureImage, imageTextureOutputShaderTextureLocation);
        }
        
        ofPushStyle();
        ofSetColor(255);
        ofDrawRectangle(0, 0, width, height);
        ofPopStyle();
        previewShader.end();
        previewFbo.end();
        parameters->get("Gradient Preview").cast<ofTexture*>() = &previewFbo.getTexture();
    }
}

vector<ofVec3f> colorApplier::computeNewColorDisplacementVector(float f){
    vector<ofVec3f> tempVec;
    tempVec.resize(width+height);
    for(auto &randDisplacement : tempVec){
        randDisplacement = ofVec3f(ofRandom(-f, f), ofRandom(-f, f), ofRandom(-f, f));
    }
    return tempVec;
}

void colorApplier::colorDisplacementChanged(float &f){
    if(oldColorDisplacement != f){
        colorDisplacementUpdated = f;
//        colorDisplacementBuffer.updateData(0, computeNewColorDisplacementVector(f));
    }
    oldColorDisplacement = f;
}

void colorApplier::computeNewColorDisplacement(float f){
//    ofPixels pix;
//    pix.allocate(colorDisplacementTexture.getWidth(), colorDisplacementTexture.getHeight(), 1);
//    for(int i = 0; i < pix.size(); i++){
//        pix[i] = ofRandom(-f, f);
//    }
//    colorDisplacementTexture.loadData(pix);
}

void colorApplier::colorListener(ofColor &c){
    if(!colorIsChanging){
        colorIsChanging = true;
        for(int i = 0; i < 2; i++){
            colorRParam[i] = colorPickerParam[i].get().r;
            colorGParam[i] = colorPickerParam[i].get().g;
            colorBParam[i] = colorPickerParam[i].get().b;
            colorHParam[i] = colorPickerParam[i].get().getHueAngle();
        }
        colorIsChanging = false;
    }
}

void colorApplier::colorSliderListener(int &i){
    if(!colorIsChanging){
        colorIsChanging = true;
        for(int i = 0; i < 2; i++){
            colorPickerParam[i] = ofColor(colorRParam[i], colorGParam[i], colorBParam[i]);
            colorHParam[i] = colorPickerParam[i].get().getHueAngle();
        }
        colorIsChanging = false;
    }
}

void colorApplier::colorHueListener(int &i){
    if(!colorIsChanging){
        colorIsChanging = true;
        for(int i = 0; i < 2; i++){
            ofColor color = colorPickerParam[i];
            color.setHueAngle(colorHParam[i]);
            colorPickerParam[i] = color;
            colorRParam[i] = colorPickerParam[i].get().r;
            colorGParam[i] = colorPickerParam[i].get().g;
            colorBParam[i] = colorPickerParam[i].get().b;
        }
        colorIsChanging = false;
    }
}

void colorApplier::modulationInfoListener(vector<float> &vf){
    if(&vf == &modulationInfo[0].get()){
        modulationInfoXUpdated = vf;
    }
    else if(&vf == &modulationInfo[1].get()){
        modulationInfoYUpdated = vf;
    }
}

