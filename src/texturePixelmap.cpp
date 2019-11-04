//
//  texturePixelmap.cpp
//  PLedNodes
//
//  Created by Eduard Frigola Bagué on 14/01/2019.
//

#include "texturePixelmap.h"

void texturePixelmap::setup(){
    int numOutputs = 2;
    parameters->add(texture.set("Texture", nullptr));
    outputs.resize(numOutputs);
    for(int i = 0; i < numOutputs; i++){
        addOutputParameterToGroupAndInfo(outputs[i].set("Output " + ofToString(i), {0}, {0}, {1}));
    }
    
    listener = texture.newListener(this, &texturePixelmap::textureListener);
}

void texturePixelmap::textureListener(ofTexture* &tex){
    if(tex != nullptr){
        ofFloatPixels pixels;
        tex->readToPixels(pixels);
        int texWidth = tex->getWidth();
        float *p = pixels.begin();
        int pixelSize = pixels.getPixelFormat();
        vector<float> tempOutput(432*3);
        for(int i = 0; i < 2; i++){
            for(int j = 0; j < 432; j++){
                int index = (((i+1)*432)+(j))*pixelSize;
                tempOutput[(j*3)] = p[index];
                tempOutput[(j*3)+1] = p[index+1];
                tempOutput[(j*3)+2] = p[index+2];
            }
            outputs[i] = tempOutput;
        }
    }
}
