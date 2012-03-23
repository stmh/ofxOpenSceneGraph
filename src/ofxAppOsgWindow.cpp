//
//  ofxAppOsgWindow.cpp
//  emptyExample
//
//  Created by Stephan Huber on 26.03.11.
//  Copyright 2011 Digital Mind. All rights reserved.
//
#include "ofMain.h"
#include "ofxAppOsgWindow.h"
#include "ofBaseApp.h"
#include "ofxOsgApp.h"
#include "ofEvents.h"
#include "ofAppRunner.h"

#include <osgViewer/CompositeViewer>
#include <osgGA/GUIEventHandler>
#include <osgViewer/ViewerEventHandlers>
#include <osgDB/Registry>


extern ofCoreEvents 				ofEvents;
extern ofEventArgs					voidEventArgs;



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
                break;

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
        glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
        
        int width, height;

        width  = ofGetWidth();
        height = ofGetHeight();

        // set viewport, clear the screen
        ofViewport(0, 0, width, height);		// used to be glViewport( 0, 0, width, height );
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
            ofClear(bgPtr[0]*255,bgPtr[1]*255,bgPtr[2]*255, bgPtr[3]*255);
        }
        
        
        if(_app->setupScreenEnabled()) ofSetupScreen();

        ofNotifyDraw();
        
        glPopClientAttrib();
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
    _fullscreen(false),
    _frameNumber(0),
    _frameRate(60.0),
    _lastFrameTime(0.0)
{
}

void ofxAppOsgWindow::setupOpenGL(int w, int h, int screenMode)
{
    std::cout << "setupOpenGL << " << w << "x" << h << std::endl;
    _w = w; _h = h;
    _screenMode = screenMode;

    if (_screenMode == OF_GAME_MODE)
    {
        osg::GraphicsContext::WindowingSystemInterface* wsi = osg::GraphicsContext::getWindowingSystemInterface();
        wsi->setScreenResolution(0, w, h);
    }
    
    _view = new osgViewer::View();

    if(_screenMode == OF_FULLSCREEN || _screenMode == OF_GAME_MODE)
    {
       _view->setUpViewOnSingleScreen();
    }
    else
    {
        _view->setUpViewInWindow(0, 0, _w, _h);
    }
    _view->getCamera()->getGraphicsContext()->realize();
    _view->getCamera()->getGraphicsContext()->makeCurrent();
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

osgViewer::GraphicsWindow* ofxAppOsgWindow::getGraphicsWindow()
{
    return (_view.valid()) ? dynamic_cast<osgViewer::GraphicsWindow*>(_view->getCamera()->getGraphicsContext()) : NULL;
}


void ofxAppOsgWindow::setWindowTitle(string title)
{
    if (osgViewer::GraphicsWindow* win = getGraphicsWindow())
        win->setWindowName(title);
}


void ofxAppOsgWindow::hideCursor()
{
    if (osgViewer::GraphicsWindow* win = getGraphicsWindow())
        win->useCursor(false);
}


void ofxAppOsgWindow::showCursor()
{
    if (osgViewer::GraphicsWindow* win = getGraphicsWindow())
        win->useCursor(true);
}


void ofxAppOsgWindow::setWindowPosition(int x, int y)
{
    if (osgViewer::GraphicsWindow* win = getGraphicsWindow()) {
        int w = win->getTraits()->width;
        int h = win->getTraits()->height;
        win->setWindowRectangle(x,y,w,h);
    }
}


void ofxAppOsgWindow::setWindowShape(int w, int h)
{
    if (osgViewer::GraphicsWindow* win = getGraphicsWindow()) {
        int x = win->getTraits()->x;
        int y = win->getTraits()->y;
        win->setWindowRectangle(x,y,w,h);
    }
}


ofPoint	ofxAppOsgWindow::getWindowPosition()
{
    ofPoint pos;
    if (osgViewer::GraphicsWindow* win = getGraphicsWindow()) {
        pos.x = win->getTraits()->x;
        pos.y = win->getTraits()->y;
    }

    return pos;
}

ofPoint	ofxAppOsgWindow::getWindowSize()
{
    ofPoint pos;
    if (osgViewer::GraphicsWindow* win = getGraphicsWindow()) {
        _w = pos.x = win->getTraits()->width;
        _h = pos.y = win->getTraits()->height;
    }

    return pos;
}


void ofxAppOsgWindow::setFullscreen(bool fullscreen)
{
    if (fullscreen == _fullscreen) return;
    
    _fullscreen = fullscreen;
    
    int x, y;
    unsigned int w, h;
    bool decoration;
    osgViewer::GraphicsWindow* win = getGraphicsWindow();


    if (fullscreen)
    {
        _savedTraits = new osg::GraphicsContext::Traits(*win->getTraits());
        osg::GraphicsContext::WindowingSystemInterface* wsi = osg::GraphicsContext::getWindowingSystemInterface();
        wsi->getScreenResolution(*win->getTraits(), w, h);
        x = y = 0;
        decoration = false;
    }
    else
    {
        if (!_savedTraits)
            _savedTraits = new osg::GraphicsContext::Traits(*win->getTraits());

        x = _savedTraits->x;
        y = _savedTraits->y;
        w = _savedTraits->width;
        h = _savedTraits->height;

        decoration = _savedTraits->windowDecoration;
    }
    win->setWindowDecoration(decoration);
    win->setWindowRectangle(x, y, w, h);
}


void ofxAppOsgWindow::toggleFullscreen()
{

}


void ofxAppOsgWindow::runAppViaInfiniteLoop(ofBaseApp * appPtr)
{
    if (appPtr == NULL)
        return;

    _app = appPtr;

    std::string data_path = ofToDataPath("");
    osgDB::Registry::instance()->getDataFilePathList().push_back(data_path);


    // create the viewer

    osg::ref_ptr<osgViewer::CompositeViewer> viewer = new osgViewer::CompositeViewer();
    viewer->setThreadingModel(osgViewer::CompositeViewer::SingleThreaded);

    ofxOsgApp* osg_app_ptr = dynamic_cast<ofxOsgApp*>(appPtr);

    if(osg_app_ptr)
    {
        osg_app_ptr->setViewer(viewer.get());

        // do we have a view?
        _view = osg_app_ptr->getView();

        if(osgViewer::GraphicsWindow* win = getGraphicsWindow())
        {
            _w = win->getTraits()->width;
            _h = win->getTraits()->height;
        }
    }

    if(!_view)
    {
        // create a simple view
        _view = new osgViewer::View();

        if(_screenMode == OF_FULLSCREEN || _screenMode == OF_GAME_MODE)
        {
           _view->setUpViewOnSingleScreen();
        }
        else
        {
            _view->setUpViewInWindow(0, 0, _w, _h);
        }
    }

    _view->addEventHandler(new ofEventHandler(appPtr));
    _view->addEventHandler(new osgViewer::StatsHandler());

    // disable clear mask, as its done by OpenFrameworks
    _view->getCamera()->setClearMask(0x0);
    _view->getCamera()->setPreDrawCallback(new ofCameraPreDrawCallback(this));

    // register view
    viewer->addView(_view);

    if (osg_app_ptr)
        osg_app_ptr->setView(_view);

    // realize it
    viewer->realize();

    // make the graphics context current
    _view->getCamera()->getGraphicsContext()->makeCurrent();

    // notify app
    ofNotifySetup();

    // run
    while(!viewer->done()) {
        ofNotifyUpdate();
        viewer->frame();
    }

    // make graphics context current
    _view->getCamera()->getGraphicsContext()->makeCurrent();

    //notify exit
    ofNotifyExit();

    // delete app now, because some of-objects assume a valid graphcis-context
    delete appPtr;
    
    ofSetAppPtr(ofPtr<ofBaseApp>());
    
    // set app-ptr to NULL
    // ofRunApp(NULL);

    // clear view + viewer
    _view->getCamera()->setPreDrawCallback(NULL);
    viewer->removeView(_view);

    _view = NULL;
    viewer = NULL;
    
    OF_EXIT_APP(0);
}
