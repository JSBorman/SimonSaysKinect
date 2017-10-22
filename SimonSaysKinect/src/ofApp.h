#pragma once

#include "ofMain.h"
#include "ofxKinectForWindows2.h"

class Button {

public:
	ofColor onColor = ofColor::yellow;
	ofColor offColor = ofColor::purple;

	int x, y, rad;
	bool isBeingTouched;

	Button() {}
	Button(int x_in, int y_in, int rad_in) {
		x = x_in;
		y = y_in;
		rad = rad_in;
	}

	//Return true if the input coordinates exist in this button
	bool checkCollision(int x_in, int y_in) {
		int distanceSquared = (x - x_in) * (x - x_in) + (y - y_in) * (y - y_in);
		return distanceSquared <= rad*rad;
	}

	void setButton(bool input) {
		isBeingTouched = input;
	}

};

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	ofxKFW2::Device kinect;
	ICoordinateMapper* coordinateMapper;

	ofImage bodyIndexImg, foregroundImg;
	vector<ofVec2f> colorCoords;
	int numBodiesTracked;
	bool bHaveAllStreams;

	Button testingButton;
};


