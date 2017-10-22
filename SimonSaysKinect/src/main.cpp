#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main() {
	ofSetupOpenGL(1024, 768, OF_WINDOW);			// <-------- setup the GL context

													// this kicks off the running of my app
													// can be OF_WINDOW or OF_FULLSCREEN
													// pass in width and height too:
	ofRunApp(new ofApp());

}

/*

TODO:
 - Create buttons on screen
 - Be aware if someone is colliding with buttons on screen
 - Create an active state to buttons, when the user hits the active button, change
 - Create pattersn of active buttons, when the user hits the right order, change
 - Keep a current score as user gets the right buttons



*/