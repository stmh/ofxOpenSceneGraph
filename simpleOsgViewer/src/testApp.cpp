#include "testApp.h"

#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/Shape>
#include <osgViewer/View>
#include <osgGA/TrackballManipulator>

//--------------------------------------------------------------
void testApp::setup(){
    std::cout << "setup" << std::endl;
    
    osg::ShapeDrawable* drawable = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0,0,0), 1.0f));
    osg::Geode* geode = new osg::Geode();
    geode->addDrawable(drawable);
    
    getView()->setSceneData(geode);
    getView()->setCameraManipulator(new osgGA::TrackballManipulator());
    
}

//--------------------------------------------------------------
void testApp::update(){
     // std::cout << "update" << std::endl;
}

//--------------------------------------------------------------
void testApp::draw(){
     // std::cout << "draw" << std::endl;
    ofDrawBitmapString("ofxOpenSceneGraph Demo", 10, 10);
    ofDrawBitmapString("fps: "+ofToString((ofGetFrameRate())), 10, 25);
    
    ofDrawBitmapString("'1': set window position\n'2': set window size\n'f': disable full screen \n'F': enable full screen\n's': show osg's stats-handler", 10,50);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    std::cout << "keypressed" << std::endl;
    switch(key) {
        case '1':
            ofSetWindowPosition(100,100);
            break;
        case '2':
            ofSetWindowShape(400, 300);
            break;
        case 'f':
            ofSetFullscreen(false);
            break;
        case 'F':
            ofSetFullscreen(true);
            break;
            
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
     std::cout << "keyReleased" << std::endl;
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
     std::cout << "mouseMoved" << x << "/" << y << std::endl;
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    std::cout << "mouseDragged" << x << "/" << y <<  std::endl;
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    std::cout << "mousePressed" << x << "/" << y <<  std::endl;

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    std::cout << "mouseReleased" << x << "/" << y <<  std::endl;

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    std::cout << "windowResized" << w << "x" << h <<  std::endl;
}

