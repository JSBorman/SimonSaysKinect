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
 - Create patterns of active buttons, 
 - Keep track of the users input
 - When the user succeeds, update the level & score
 - Keep a current score as user gets the right buttons
 - Flash red if the user hit the wrong button


 Options:
 - When a button is hit, move to a different location
 - When appears, select a color and the user has to hover the right color on it to make it disappear (i.e hold a red ball over the red button
 - Simon Says style, where there is a board of buttons that have to be hit in the given order
*/