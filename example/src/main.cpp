//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


#include "ofApp.h"


int main()
{
	ofSetupOpenGL(1280, 720, OF_WINDOW);
    ofRunApp(std::make_shared<ofApp>());
}
