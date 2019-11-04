//
//  colorApplier.h
//  MIRABCN_Generator
//
//  Created by Eduard Frigola on 01/12/2016.
//
//

#ifndef colorApplier_h
#define colorApplier_h

#include "ofxOceanodeNodeModel.h"
#include "baseIndexer.h"
#include "sharedResources.h"

class colorApplier : public ofxOceanodeNodeModel{
    
public:
    
    colorApplier() : ofxOceanodeNodeModel("Color Applier"){
        isSetup = false;
    };
    ~colorApplier();
    
    void setup() override;
    void draw(ofEventArgs &a) override;
    
private:
    void reloadShader(bool &b);
    vector<ofVec3f> computeNewColorDisplacementVector(float parameter);
    
    ofEventListeners listeners;
    
    void modulationInfoListener(vector<float> &vf);
//    void applyColor(ofTexture* &inputTex);
    void colorDisplacementChanged(float &f);
    void computeNewColorDisplacement(float f);
    
    void colorListener(ofColor &c);
    void colorSliderListener(int &i);
    void colorHueListener(int &i);
        
    ofParameter<bool>       reloadShaderParam;
    ofParameter<ofColor>    colorPickerParam[2];
    ofParameter<int>        colorRParam[2];
    ofParameter<int>        colorGParam[2];
    ofParameter<int>        colorBParam[2];
    ofParameter<int>        colorHParam[2];
    
    ofParameter<float>  colorDisplacement;
//    ofParameter<int>        randomColorStepsParam;
//    ofParameter<int>        randomizeTypeColorParam; //Select the rand way: in change preset, in phasor triggered...
    ofParameter<ofTexture*> textureImage;
    
    //in and outs
    ofParameter<vector<float>>          modulationInfo[2];
    ofParameter<ofTexture*>  grayScaleIn;
    ofParameter<ofTexture*>  gradientPreview;
    ofParameter<ofTexture*>  colorizedValues;
    
    ofFbo outputFbo;
    ofFbo previewFbo;
//    ofTexture colorDisplacementTexture;
    
    bool colorIsChanging;
    float oldColorDisplacement;
    
    int width = 1;
    int height = 1;
    
    sharedResources* resources;
    unsigned int infoTextureOutputShaderTextureLocation;
	unsigned int imageTextureOutputShaderTextureLocation;
	unsigned int infoTexturePreviewShaderTextureLocation;
	unsigned int imageTexturePreviewShaderTextureLocation;
    
    vector<unsigned int> shaderLocations;
    
    ofShader outputShader;
    ofShader previewShader;
    ofFbo whiteFbo;
    
    //TBOs
    ofTexture               modulationInfoTexture;
    ofBufferObject          modulationInfoBuffer;
    
    //TBOs
    ofTexture               colorDisplacementTexture;
    ofBufferObject          colorDisplacementBuffer;
    
    vector<float> modulationInfoXUpdated;
    vector<float> modulationInfoYUpdated;
    float colorDisplacementUpdated;
    
    bool isSetup;
};

#endif /* colorApplier_h */
