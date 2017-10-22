#pragma once

#include "ofMain.h"
#include "ofxKinectForWindows2.h"

class Button {

public:
	ofColor onColor;
	ofColor offColor;

	int x, y, rad;
	int position;	//Stores the order this button is in row
	bool isBeingTouched;
	bool touchedThisFrame;

	Button() {}
	Button(ofColor on_in, ofColor off_in, int x_in, int y_in, int rad_in, int position) {
		onColor = on_in;
		offColor = off_in;
		x = x_in;
		y = y_in;
		rad = rad_in;
		isBeingTouched = false;
		touchedThisFrame = false;
	}

	//Return true if the input coordinates exist in this button
	bool checkCollision(int x_in, int y_in) {
		int distanceSquared = (x - x_in) * (x - x_in) + (y - y_in) * (y - y_in);
		return distanceSquared <= rad*rad;
	}

	void setButton(bool input) {
		isBeingTouched = input;
	}

	void touchedFrame(bool input) {
		touchedThisFrame = input;
	}

};

//Stores 
class Pattern {
public:

	std::vector<int> correctPattern;
	std::vector<int> currentPattern;

	int currentLevel;
	int index;	//Current index to display in the correct pattern

	Pattern() {
		currentLevel = 1;
		index = 0;

		craftPattern();
	}

	//Add the next selected button to the end of the list
	void userTouched(int selected) {
		currentPattern.push_back(selected);
	}

	//Get the 
	int getNextItemPattern() {
		if (index < correctPattern.size()) {
			index++;
			return correctPattern.at(index - 1);
		}
		else {
			index = 0;
			return -1;
		}
	}

	// Checks the current pattern against the correct one
	// Returns 0 - Pattern is still in progress
	// Returns 1 - Pattern is wrong
	// Returns 2 - Pattern is a match!
	int checkState() {
		for (int i = 0; i < currentPattern.size(); i++) {
			if (currentPattern.at(i) != correctPattern.at(i))
				return 1;
		}

		if (currentPattern.size() == correctPattern.size())
			return 2;
		
		return 0;
	}

	//Game Over
	void resetGame() {
		currentLevel = 1;
		craftPattern();
	}

	//Move to the next level
	void increaseLevel() {
		currentLevel++;
		craftPattern();
	}

private:
	//Create the current solution. Length is based on the current level
	void craftPattern() {
		correctPattern.clear();	//Empty the old pattern
		currentPattern.clear(); //Empty the users work

		correctPattern = { 3, 3, 3 };

	//	for (int i = 0; i < currentLevel; i++) {
	//		correctPattern.push_back( (int) ofRandom(0, 4.99));		//Not 5 so game doesn't break w/ gameBoard[5]
	//	}
	}
};

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void createBoard();		//Custom button to build the board
	void clearBoard();		//Clear the state of all buttons on board
	void highlightButtons();	//Activate correct buttons to show pattern
	void keyPressed(int key);
	void keyReleased(int key);

	ofxKFW2::Device kinect;
	ICoordinateMapper* coordinateMapper;

	ofImage bodyIndexImg;
	vector<ofVec2f> colorCoords;
	int numBodiesTracked;
	bool bHaveAllStreams;

	int currentScore;
	float lastTime;
	float delayTime;			//Delays when to display next button

	bool pauseInput = true;		//Don't read input
	bool displayPattern = true;	//Set the level state to display
	bool flashGreen = false;	//Flash all the buttons green
	bool flashRed = false;		//Flash all the buttons red

	Button gameBoard [5];
	Pattern game;
};


