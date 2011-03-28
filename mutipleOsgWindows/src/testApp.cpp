#include "testApp.h"

#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/Shape>
#include <osgViewer/View>
#include <osgGA/TrackballManipulator>
#include <osgViewer/CompositeViewer>

#include <osgDB/ReadFile>

//--------------------------------------------------------------
void testApp::setup(){
    std::cout << "setup" << std::endl;
    
    osg::ShapeDrawable* drawable = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0,0,0), 1.0f));
    osg::Geode* geode = new osg::Geode();
    geode->addDrawable(drawable);
    
    getView()->setSceneData(geode);
    getView()->setCameraManipulator(new osgGA::TrackballManipulator());
    
    _numViews = 0;
    
}

//--------------------------------------------------------------
void testApp::update()
{
     // std::cout << "update" << std::endl;
}

//--------------------------------------------------------------
void testApp::draw()
{
     // std::cout << "draw" << std::endl;
    ofDrawBitmapString("multipleOsgWindows Demo", 10, 10);
    ofDrawBitmapString("num windows: "+ ofToString(static_cast<int>(_numViews))+" fps: "+ofToString((ofGetFrameRate())), 10, 25);
    
    ofDrawBitmapString("'c': create new view + window", 10,50);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
    std::cout << "keypressed" << std::endl;
    switch(key) {
        case 'c':
            createNewView();
            break;
    }
}

void testApp::createNewView() 
{
    
    osgViewer::View* view = new osgViewer::View();
    view->setUpViewInWindow(100+ 30 * _numViews, 100+ 30 * _numViews, 400, 300);
    view->setCameraManipulator(new osgGA::TrackballManipulator());
    
    getViewer()->addView(view);
    
    osg::setNotifyLevel(osg::DEBUG_INFO);
    osg::Node* node = osgDB::readNodeFile("cow.osg");
    view->setSceneData(node);
    osg::setNotifyLevel(osg::WARN);
    
    _numViews++;
}