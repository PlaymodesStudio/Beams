//
//  vectorToTexture.h
//  PLedNodes
//
//  Created by Eduard Frigola Bagué on 07/08/2019.
//

#ifndef vectorToTexture_h
#define vectorToTexture_h

#include "ofxOceanodeNodeModel.h"

class vectorToTexture : public ofxOceanodeNodeModel{
public:
    vectorToTexture() : ofxOceanodeNodeModel("Vector to Texture"){};
    
    void setup(){
        addParameterToGroupAndInfo(input.set("Input", {0}, {0}, {1}));
        addParameterToGroupAndInfo(size.set("Size", "32x4"));
        addParameterToGroupAndInfo(output.set("Output Tex", nullptr));
        
        listener = size.newListener([this](string &s){
            changedSize = true;
        });
        
        changedSize = true;
    }
    
    void draw(ofEventArgs &a){
        if(changedSize){
            vector<string> dimensions = ofSplitString(size, "x");
            fbo.allocate(ofToInt(dimensions[0]), ofToInt(dimensions[1]), GL_RGBA);
            fbo.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
            changedSize = false;
        }
        vector<float> rgbInput(input->size()*3, 0);
        for(int i = 0; i < input->size(); i++){
            rgbInput[(i*3)] = input->at(i);
            rgbInput[(i*3)+1] = input->at(i);
            rgbInput[(i*3)+2] = input->at(i);
        }
        ofTexture tex;
        tex.loadData(rgbInput.data(), input->size(), 1, GL_RGB);
        tex.setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
        fbo.begin();
        tex.draw(0, 0, fbo.getWidth(), fbo.getHeight());
        fbo.end();
        output = &fbo.getTexture();
    }
    
    void loadBeforeConnections(ofJson &json){
        deserializeParameter(json, size);
    }
    
private:
    ofParameter<vector<float>> input;
    ofParameter<ofTexture*> output;
    ofParameter<string> size;
    ofFbo fbo;
    
    bool changedSize;
    
    ofEventListener listener;
};

#endif /* vectorToTexture_h */
