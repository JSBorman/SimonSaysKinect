//Body depth code referenced from the BodyIndexImage example in ofxKinectForWindows2

#include "ofApp.h"

#define DEPTH_WIDTH 512
#define DEPTH_HEIGHT 424
#define DEPTH_SIZE DEPTH_WIDTH * DEPTH_HEIGHT

#define COLOR_WIDTH 1920
#define COLOR_HEIGHT 1080

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetWindowShape(DEPTH_WIDTH, DEPTH_HEIGHT);

	//Setup Kinect w/ body tracking
	kinect.open();
	kinect.initDepthSource();
	kinect.initBodySource();
	kinect.initBodyIndexSource();

	if (kinect.getSensor()->get_CoordinateMapper(&coordinateMapper) < 0) {
		ofLogError() << "Could not acquire CoordinateMapper!";
	}

	numBodiesTracked = 0;
	bHaveAllStreams = false;

	bodyIndexImg.allocate(DEPTH_WIDTH, DEPTH_HEIGHT, OF_IMAGE_COLOR);

	lastTime = ofGetElapsedTimef();
	delayTime = ofGetElapsedTimef();
	currentScore = 0;

	//Build the game board
	createBoard();
	game = Pattern();
}

void ofApp::createBoard() {
	int spacing = ofGetWidth() / 5;

	for (int i = 0; i < 5; i++) {
		gameBoard[i] = Button(ofColor::yellow, ofColor::blue, spacing / 2 + (spacing * i), 150, 50, i);
	}
}

void ofApp::clearBoard() {
	for (int i = 0; i < 5; i++) 
		gameBoard[i].setButton(false);
}

void ofApp::highlightButtons() {
	float currentTime = ofGetElapsedTimef();

	//Wait between each item in pattern
	if (currentTime - lastTime > .5) {
		clearBoard();
	}

	//Display next item in pattern
	if (currentTime - lastTime > 1) {

		int currentHighlight = game.getNextItemPattern();

		//Disable if pattern is over
		if (currentHighlight == -1) {	//Turn off when done
			displayPattern = false;
			pauseInput = false;
		}

		// Display next button
		else {
			gameBoard[currentHighlight].setButton(true);
			lastTime = currentTime;
		}
	}
}

//--------------------------------------------------------------
void ofApp::update() {
	//Reset what was touched this frame
	if (!pauseInput) {
		for (int i = 0; i < 5; i++) {
			gameBoard[i].touchedFrame(false);
		}
	}
	
	kinect.update();

	// Get pixel data
	auto& depthPix = kinect.getDepthSource()->getPixels();
	auto& bodyIndexPix = kinect.getBodyIndexSource()->getPixels();

	// Make sure there's some data here, otherwise the cam probably isn't ready yet
	if (!depthPix.size() || !bodyIndexPix.size() ){
		bHaveAllStreams = false;
		return;
	}
	else {
		bHaveAllStreams = true;
	}

	// Count number of tracked bodies
	numBodiesTracked = 0;
	auto& bodies = kinect.getBodySource()->getBodies();
	for (auto& body : bodies) {
		if (body.tracked)
			numBodiesTracked++;
	}

	// Loop through the depth image
	for (int y = 0; y < DEPTH_HEIGHT; y++) {
		for (int x = 0; x < DEPTH_WIDTH; x++) {
			int index = (y * DEPTH_WIDTH) + x;
			bodyIndexImg.setColor(x, y, ofColor::white);

			// This is the check to see if a given pixel is inside a tracked  body or part of the background.
			// If it's part of a body, the value will be that body's id (0-5), or will > 5 if it's
			// part of the background
			float val = bodyIndexPix[index];
			if (val >= bodies.size()) {
				continue;
			}

			// Give each tracked body a color value 
			ofColor c = ofColor::fromHsb(val * 255 / bodies.size(), 200, 255);
			bodyIndexImg.setColor(x, y, c);
			
			//Check if the current pixel maps is colliding with the button
			if (!pauseInput) {
				for (int i = 0; i < 5; i++) {

					// If pressing the button for the first time, register the click
					if (gameBoard[i].checkCollision(x, y) && gameBoard[i].isBeingTouched == false) {
						gameBoard[i].setButton(true);
						gameBoard[i].touchedFrame(true);
						game.userTouched(i);
					}

					//If hovering on the button, make sure to touch this frame still
					else if (gameBoard[i].checkCollision(x, y) && gameBoard[i].isBeingTouched == true) {
						gameBoard[i].touchedFrame(true);
					}
				}
			}
		}
	}

	//Reset the input 
	if (!pauseInput) {
		for (int i = 0; i < 5; i++) {
			if (!gameBoard[i].touchedThisFrame) {
				gameBoard[i].setButton(false);
			}
		}
	}
	// Update the images since we manipulated the pixels manually. This uploads to the
	// pixel data to the texture on the GPU so it can get drawn to screen
	bodyIndexImg.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(ofColor::white);
	bodyIndexImg.draw(0, 0);

	stringstream ss;
	ss << "Current Score : " << currentScore << endl;
	if (!bHaveAllStreams) ss << endl << "Not all streams detected!";

	ofDrawBitmapStringHighlight(ss.str(), 20, 20);

	if(displayPattern) {
		highlightButtons();
	}

	if (!pauseInput) {
		int status = game.checkState();

		switch (status) {
		//Pattern is wrong! Game Over
		case 1:
			flashRed = true;
			pauseInput = true;
			game.resetGame();
			currentScore = 0;
			lastTime = ofGetElapsedTimef();
			break;

		//Pattern is correct! Next level
		case 2:
			flashGreen = true;
			pauseInput = true;
			currentScore += 10;
			game.increaseLevel();
			lastTime = ofGetElapsedTimef();
			break;

		//Continue reading input
		default:		//Pattern is still in progress
			break;
		}
	}

	if (flashGreen || flashRed) {
		float currentTime = ofGetElapsedTimef();
		ofSetColor(ofColor::blue);

		//Alternate between on and off
		if (currentTime - lastTime < .5)
			(flashGreen) ? ofSetColor(ofColor::forestGreen) : ofSetColor(ofColor::darkRed);
		else if (currentTime - lastTime < 1)
			ofSetColor(ofColor::blue);
		else if (currentTime - lastTime < 1.5)
			(flashGreen) ? ofSetColor(ofColor::forestGreen) : ofSetColor(ofColor::darkRed);
		else if (currentTime - lastTime < 2)
			ofSetColor(ofColor::blue);
		else {
			displayPattern = true;
			flashRed = false;
			flashGreen = false;
		}

		for (int i = 0; i < 5; i++)
			ofDrawCircle(gameBoard[i].x, gameBoard[i].y, gameBoard[i].rad);
	}

	//Draw the board normally
	else {
		for (int i = 0; i < 5; i++) {
			if (gameBoard[i].isBeingTouched)
				ofSetColor(gameBoard[i].onColor);
			else
				ofSetColor(gameBoard[i].offColor);

			ofDrawCircle(gameBoard[i].x, gameBoard[i].y, gameBoard[i].rad);
		}
	}
}

