//
//  ofOsgApp.h
//  emptyExample
//
//  Created by Stephan Huber on 26.03.11.
//  Copyright 2011 Digital Mind. All rights reserved.
//

#ifndef OF_OSG_APP_HEADER
#define OF_OSG_APP_HEADER

namespace osgViewer {
    class View;
    class CompositeViewer;
}

class ofxOsgApp {
public:
    ofxOsgApp() : _viewer(NULL), _view(NULL) {}
    virtual ~ofxOsgApp() {}
    
    osgViewer::View* getView() { return _view; }
    osgViewer::CompositeViewer* getViewer() { return _viewer; }
    
    
protected:
    void setView(osgViewer::View* view) { _view = view; }
    void setViewer(osgViewer::CompositeViewer* viewer) { _viewer = viewer; }
    
    osgViewer::CompositeViewer* _viewer;
    osgViewer::View* _view;
    
    friend class ofxAppOsgWindow;
};

#endif