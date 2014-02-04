/*
 * Copyright (c) 2011 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxPd for documentation
 *
 */
#include "testApp.h"
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/Shape>
#include <osgViewer/View>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>

#include <Poco/Path.h>

#include <iostream>

using namespace std;
using namespace osg;
using namespace osgUtil;




//--------------------------------------------------------------
void testApp::setup() {

      std::cout << "setup" << std::endl;

    osg::setNotifyLevel(osg::DEBUG_INFO);

	// the number if libpd ticks per buffer,
	// used to compute the audio buffer len: tpb * blocksize (always 64)
	int ticksPerBuffer = 8;	// 8 * 64 = buffer len of 512

	// setup OF sound stream
	ofSoundStreamSetup(2, 2, this, 44100, ofxPd::getBlockSize()*ticksPerBuffer, 4);
  
	// setup the app core
	core.setup(2, 2, 44100, ticksPerBuffer);

    osg::ShapeDrawable* drawable = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0,0,0), 1.0f));
    osg::Geode* geode = new osg::Geode();
    geode->addDrawable(drawable);

    getView()->setSceneData(geode);
    getView()->setCameraManipulator(new osgGA::TrackballManipulator());
    osg::setNotifyLevel(osg::WARN);

}

//--------------------------------------------------------------
void testApp::update() {
	core.update();
}

//--------------------------------------------------------------
void testApp::draw() {
	//core.draw();
}

//--------------------------------------------------------------
void testApp::exit() {
	core.exit();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
	core.keyPressed(key);
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y) {}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h) {}

//--------------------------------------------------------------
void testApp::audioReceived(float * input, int bufferSize, int nChannels) {
	core.audioReceived(input, bufferSize, nChannels);
}

//--------------------------------------------------------------
void testApp::audioRequested(float * output, int bufferSize, int nChannels) {
	core.audioRequested(output, bufferSize, nChannels);
}
