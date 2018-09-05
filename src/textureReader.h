//
//  textureReader.h
//  Espills_controller
//
//  Created by Eduard Frigola Bagué on 02/07/2018.
//

#ifndef textureReader_h
#define textureReader_h

#include "ofxOceanodeNodeModel.h"

class textureReader : public ofxOceanodeNodeModel{
public:
    textureReader() : ofxOceanodeNodeModel("Texture Reader"){
        parameters->add(input.set("Input", nullptr));
        parameters->add(output.set("Output", {0}, {0}, {1}));
        listener = input.newListener([this](ofTexture* &tex){
            if(tex != nullptr){
                if(tempOutput.size() != tex->getWidth() * tex->getHeight()){
                    tempOutput.resize(tex->getWidth() * tex->getHeight(), 0);
                }
                ofFloatPixels pixels;
                tex->readToPixels(pixels);
                float *p = pixels.begin();
                int pixelSize = pixels.getPixelFormat();
                for(int i = 0; i < tex->getWidth() * tex->getHeight(); i++){
                    tempOutput[i] = p[i*4];
                }
                output = tempOutput;
            }
        });
    };
    
    ~textureReader(){};
    
private:
    ofParameter<ofTexture*> input;
    ofParameter<vector<float>> output;
    
    vector<float> tempOutput;
    ofEventListener listener;
};

#endif /* textureReader_h */
