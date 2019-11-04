//
//  texturePixelmap.h
//  PLedNodes
//
//  Created by Eduard Frigola Bagué on 14/01/2019.
//

#ifndef texturePixelmap_h
#define texturePixelmap_h

#include "ofxOceanodeNodeModel.h"

class texturePixelmap : public ofxOceanodeNodeModel{
public:
    texturePixelmap() : ofxOceanodeNodeModel("Texture Pixelmap"){};
    ~texturePixelmap(){};
    
    void setup() override;
    
private:
    void textureListener(ofTexture* &tex);
    ofEventListener listener;
    
    ofParameter<ofTexture*> texture;
    vector<ofParameter<vector<float>>> outputs;
};

#endif /* texturePixelmap_h */
