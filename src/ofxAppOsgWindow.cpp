//
//  ofxAppOsgWindow.cpp
//  emptyExample
//
//  Created by Stephan Huber on 26.03.11.
//  Copyright 2011 Digital Mind. All rights reserved.
//

#include "ofxAppOsgWindow.h"
#include "ofBaseApp.h"
#include "ofxOsgApp.h"
#include "ofEvents.h"
#include "ofAppRunner.h"

#include <osgViewer/CompositeViewer>
#include <osgGA/GUIEventHandler>
#include <osgViewer/ViewerEventHandlers>

class ofEventHandler : public osgGA::GUIEventHandler {
public:
    ofEventHandler(ofBaseApp* app) : osgGA::GUIEventHandler(), _app(app) {}
    
    virtual bool handle (const osgGA::GUIEventAdapter & ea, osgGA::GUIActionAdapter &aa)
    {
        int x = ea.getX();
        int y = ea.getWindowHeight() - ea.getY();
        
        _app->mouseX = x;
        _app->mouseY = y;
        
        switch(ea.getEventType()) {
            case osgGA::GUIEventAdapter::MOVE:
                ofNotifyMouseMoved(x, y);
                break;
            
            case osgGA::GUIEventAdapter::DRAG:
                ofNotifyMouseDragged(x, y, ea.getButton());
                break;
                
            case osgGA::GUIEventAdapter::PUSH:
                ofNotifyMousePressed(x, y, ea.getButton());
                break;
            
            case osgGA::GUIEventAdapter::RELEASE:
                ofNotifyMouseReleased(x, y, ea.getButton());
                break;
            
            case osgGA::GUIEventAdapter::KEYDOWN:
                ofNotifyKeyPressed(ea.getKey());
                break;
                
            case osgGA::GUIEventAdapter::KEYUP:
                ofNotifyKeyReleased(ea.getKey());
                break;
			case osgGA::GUIEventAdapter::RESIZE:
				ofNotifyWindowResized(ea.getWindowWidth(), ea.getWindowHeight());
            default:
                break;
            
        }
        
        return false;
    }

private:
    ofBaseApp* _app;
};

class ofCameraPreDrawCallback : public osg::Camera::DrawCallback {

public:
    ofCameraPreDrawCallback(ofxAppOsgWindow* app = NULL) 
    :   osg::Camera::DrawCallback(), 
        _app(app),
        _frameCount(0)
    {
    }
    
    virtual void operator()(osg::RenderInfo &renderInfo) const 
    {
        glPushAttrib(GL_ALL_ATTRIB_BITS); 
        
        int width, height;
        
        width  = ofGetWidth();
        height = ofGetHeight();
        
        height = height > 0 ? height : 1;
        // set viewport, clear the screen
        glViewport( 0, 0, width, height );
        float * bgPtr = ofBgColorPtr();
        bool bClearAuto = ofbClearBg();
        
        // to do non auto clear on PC for now - we do something like "single" buffering --
        // it's not that pretty but it work for the most part
        
#ifdef TARGET_WIN32
        if (bClearAuto == false){
            glDrawBuffer (GL_FRONT);
        }
#endif
        
        if ( bClearAuto == true || _frameCount < 3){
            glClearColor(bgPtr[0],bgPtr[1],bgPtr[2], bgPtr[3]);
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
        
        if(_app->setupScreenEnabled())
            ofSetupScreen();

        
        ofNotifyDraw();
        
        glPopAttrib(); 
        
        osg::Timer_t tick = _t.tick();
        double frameDuration = _t.delta_s(_lastFrameTimeStamp, tick);
        _app->_frameNumber = _frameCount;
        _app->_lastFrameTime = frameDuration;
        _app->_frameRate = (_app->_frameRate * 0.1) + (1.0 / frameDuration) * 0.9;
        
        _frameCount++;
        _lastFrameTimeStamp = tick;
        
    }
    
private:
    ofxAppOsgWindow* _app;
    mutable unsigned int _frameCount;
    
private:
    mutable osg::Timer_t _lastFrameTimeStamp;
    osg::Timer _t;
     
};


ofxAppOsgWindow::ofxAppOsgWindow()
:   _view(NULL),
    _app(NULL),
    _setupScreen(true),
    _frameNumber(0),
    _frameRate(0.0),
    _lastFrameTime(0.0)
{
}

void ofxAppOsgWindow::setupOpenGL(int w, int h, int screenMode)
{
    std::cout << "setupOpenGL << " << w << "x" << h << std::endl;
    _w = w; _h = h;
    
}


void ofxAppOsgWindow::initializeWindow()
{
    std::cout << "initializeWindow" << std::endl;    
}


ofPoint	ofxAppOsgWindow::getScreenSize()
{
    
    osg::GraphicsContext::WindowingSystemInterface* wsi = osg::GraphicsContext::getWindowingSystemInterface();
    unsigned int w, h;
    wsi->getScreenResolution(0, w, h);
    return ofPoint(w,h);
}

void ofxAppOsgWindow::runAppViaInfiniteLoop(ofBaseApp * appPtr)
{
    if (appPtr == NULL)
        return;
    
    {
        
        osg::ref_ptr<osgViewer::CompositeViewer> viewer = new osgViewer::CompositeViewer();
        viewer->setThreadingModel(osgViewer::CompositeViewer::SingleThreaded);
        
        ofxOsgApp* osg_app_ptr = dynamic_cast<ofxOsgApp*>(appPtr);
        
        if(osg_app_ptr) {
            osg_app_ptr->setViewer(viewer.get());
            _view = osg_app_ptr->getView();
        }
		
        if(!_view) {
            _view = new osgViewer::View();
            _view->setUpViewInWindow(0,0,_w, _h);
        }
        
        _view->addEventHandler(new ofEventHandler(appPtr));
        _view->addEventHandler(new osgViewer::StatsHandler());
        
        _view->getCamera()->setClearMask(0x0);
        _view->getCamera()->setPreDrawCallback(new ofCameraPreDrawCallback(this));
        
        viewer->addView(_view);
        
        if (osg_app_ptr)
            osg_app_ptr->setView(_view);
        
        viewer->realize();
        _view->getCamera()->getGraphicsContext()->makeCurrent();
        
        ofNotifySetup();
        
        while(!viewer->done()) {
            ofNotifyUpdate(); 
            viewer->frame();
        }
        _view->getCamera()->getGraphicsContext()->makeCurrent();
        
        delete appPtr;
        ofRunApp(NULL);
        
        _view->getCamera()->setPreDrawCallback(NULL);
        viewer->removeView(_view);
        
        _view = NULL;
        viewer = NULL;
    }
    
    OF_EXIT_APP(0);
}