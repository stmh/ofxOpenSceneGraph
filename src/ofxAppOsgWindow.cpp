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


extern ofCoreEvents 				ofEvents;
extern ofEventArgs					voidEventArgs;
// some glue-code for event-handling:

inline void notifySetup(ofBaseApp* app) 
{
    if (app)
        app->setup();

    #ifdef OF_USING_POCO
        ofNotifyEvent( ofEvents.setup, voidEventArgs );
    #endif
}


inline void notifyUpdate(ofBaseApp* app) 
{
    if (app)
        app->update();
   
    #ifdef OF_USING_POCO
        ofNotifyEvent( ofEvents.update, voidEventArgs );
    #endif
}

inline void notifyDraw(ofBaseApp* app) 
{
    if (app)
        app->draw();
    
#ifdef OF_USING_POCO
    ofNotifyEvent( ofEvents.draw, voidEventArgs );
#endif
}

inline void notifyExit(ofBaseApp* app) 
{
    if (app)
        app->exit();
    
#ifdef OF_USING_POCO
    ofNotifyEvent( ofEvents.exit, voidEventArgs );
#endif
}

inline void notifyMouseMoved(ofBaseApp* app, int x, int y) 
{
    if (app)
        app->mouseMoved(x,y);

#ifdef OF_USING_POCO
    ofMouseEventArgs mouseEventArgs;
    mouseEventArgs.x = x;
    mouseEventArgs.y = y;
    ofNotifyEvent( ofEvents.mouseMoved, mouseEventArgs );
#endif
}

inline void notifyMouseDragged(ofBaseApp* app, int x, int y, int button) 
{
    if (app)
        app->mouseDragged(x,y, button);
    
#ifdef OF_USING_POCO
    ofMouseEventArgs mouseEventArgs;
    mouseEventArgs.x = x;
    mouseEventArgs.y = y;
    mouseEventArgs.button = button;
    ofNotifyEvent( ofEvents.mouseDragged, mouseEventArgs );
#endif
}

inline void notifyMousePressed(ofBaseApp* app, int x, int y, int button) 
{
    if (app)
        app->mousePressed(x,y, button);
    
#ifdef OF_USING_POCO
    ofMouseEventArgs mouseEventArgs;
    mouseEventArgs.x = x;
    mouseEventArgs.y = y;
    mouseEventArgs.button = button;
    ofNotifyEvent( ofEvents.mousePressed, mouseEventArgs );
#endif
}

inline void notifyMouseReleased(ofBaseApp* app, int x, int y, int button) 
{
    if (app)
        app->mouseReleased(x,y, button);
    
#ifdef OF_USING_POCO
    ofMouseEventArgs mouseEventArgs;
    mouseEventArgs.x = x;
    mouseEventArgs.y = y;
    mouseEventArgs.button = button;
    ofNotifyEvent( ofEvents.mouseReleased, mouseEventArgs );
#endif
}

inline void notifyKeyPressed(ofBaseApp* app, int key) 
{
    if (app)
        app->keyPressed(key);
    
#ifdef OF_USING_POCO
    ofKeyEventArgs args;
    args.key = key;
    ofNotifyEvent( ofEvents.keyPressed, args );
#endif
}

inline void notifyKeyReleased(ofBaseApp* app, int key) 
{
    if (app)
        app->keyReleased(key);
    
#ifdef OF_USING_POCO
    ofKeyEventArgs args;
    args.key = key;
    ofNotifyEvent( ofEvents.keyReleased, args );
#endif
}

inline void notifyWindowResized(ofBaseApp* app, int width, int height) 
{
    if (app)
        app->windowResized(width, height);
    
#ifdef OF_USING_POCO
    ofResizeEventArgs args;
    args.width = width;
    args.height = height;
    ofNotifyEvent( ofEvents.windowResized, args );
#endif
}



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
                notifyMouseMoved(_app, x, y);
                break;
            
            case osgGA::GUIEventAdapter::DRAG:
                notifyMouseDragged(_app, x, y, ea.getButton());
                break;
                
            case osgGA::GUIEventAdapter::PUSH:
                notifyMousePressed(_app, x, y, ea.getButton());
                break;
            
            case osgGA::GUIEventAdapter::RELEASE:
                notifyMouseReleased(_app, x, y, ea.getButton());
                break;
            
            case osgGA::GUIEventAdapter::KEYDOWN:
                notifyKeyPressed(_app, ea.getKey());
                break;
                
            case osgGA::GUIEventAdapter::KEYUP:
                notifyKeyReleased(_app, ea.getKey());
                break;
			
            case osgGA::GUIEventAdapter::RESIZE:
				notifyWindowResized(_app, ea.getWindowWidth(), ea.getWindowHeight());
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

        
        notifyDraw( _app->getApp());
        
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
    notifySetup(appPtr);
    
    // run
    while(!viewer->done()) {
        notifyUpdate(appPtr); 
        viewer->frame();
    }
    
    // make graphics context current
    _view->getCamera()->getGraphicsContext()->makeCurrent();
    
    //notify exit
    notifyExit(appPtr);
    
    // delete app now, because some of-objects assume a valid graphcis-context
    delete appPtr;
    
    // set app-ptr to NULL
    ofRunApp(NULL);
    
    // clear view + viewer
    _view->getCamera()->setPreDrawCallback(NULL);
    viewer->removeView(_view);
    
    _view = NULL;
    viewer = NULL;
    
    OF_EXIT_APP(0);
}