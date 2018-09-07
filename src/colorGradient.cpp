//
//  colorGradient.cpp
//  Beams_Controlls
//
//  Created by Eduard Frigola on 07/09/2018.
//

#include "colorGradient.h"

colorGradient::colorGradient() : ofxOceanodeNodeModel("Color Gradient"){
    
}

void colorGradient::setup(){
    parameters->add(r0.set("Red 0", 0, 0, 1));
    parameters->add(g0.set("Green 0", 0, 0, 1));
    parameters->add(b0.set("Blue 0", 0, 0, 1));
    
    parameters->add(r1.set("Red 1", 1, 0, 1));
    parameters->add(g1.set("Green 1", 1, 0, 1));
    parameters->add(b1.set("Blue 1", 1, 0, 1));
    
    parameters->add(indexs.set("Indexs", {1}, {0}, {1}));
    
    parameters->add(rOut.set("Red Out", {1}, {0}, {1}));
    parameters->add(gOut.set("Green Out", {1}, {0}, {1}));
    parameters->add(bOut.set("Blue Out", {1}, {0}, {1}));
    
    listener = indexs.newListener([this](vector<float> &vf){
        vector<float> rOutTemp(vf.size());
        vector<float> gOutTemp(vf.size());
        vector<float> bOutTemp(vf.size());
        for(int i = 0; i < vf.size(); i++){
            rOutTemp[i] = r0*(1-vf[i]) + r1*vf[i];
            gOutTemp[i] = g0*(1-vf[i]) + g1*vf[i];
            bOutTemp[i] = b0*(1-vf[i]) + b1*vf[i];
        }
        rOut = rOutTemp;
        gOut = gOutTemp;
        bOut = bOutTemp;
    });
}
