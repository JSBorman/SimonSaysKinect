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
 - Flash green when the user hits the right button
 - Flash red if the user hit the wrong button
 - Fix awkwardness when pattern displays same button in a row

 Future Work Ideas:
 - Buttons bounce around the screen
 - Buttons set to different colors and the user has to hover the right color on it to progress (i.e hold a red ball over the red button)
*/