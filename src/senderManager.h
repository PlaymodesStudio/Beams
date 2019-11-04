//
//  senderManager.h
//  MIRABCN_Generator
//
//  Created by Eduard Frigola on 01/12/2016.
//
//

#ifndef senderManager_h
#define senderManager_h

#include "ofxOceanodeNodeModel.h"
#ifdef TARGET_OSX
#include "ofxSyphon.h"
#elif TARGET_WIN64
//#include "ofxSpout.h"
#endif

class senderManager : public ofxOceanodeNodeModel{
public:
    
    senderManager();
    ~senderManager(){
#ifdef TARGET_OSX
        delete syphonServer;
#endif
    };
    void setname(string name){syphonName = name;};
    
    void enableSyphonListener(bool &b);
    void syphonNameListener(string &s);
    
private:
    void sendTexture(ofTexture *&info);
    
#ifdef TARGET_OSX
    ofxSyphonServer*   syphonServer;
#else
    void* syphonServer;
#endif

    ofParameter<bool>   enable;
    ofParameter<string> syphonName;
    ofParameter<float>  masterFader;
    ofParameter<ofTexture*>    textureIn;
    
    ofFbo colorFbo;
    
    bool invert;
    
    ofEventListeners listeners;
};

#endif /* senderManager_h */
