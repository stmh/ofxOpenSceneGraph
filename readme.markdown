## ofxOpenSceneGraph ReadMe
by <http://stephanmaximilianhuber.com>

this small add-on helps OpenFrameworks-code to live side by side with OpenSceneGraph.

### Requirements

* OpenSceneGraph from <http://openscenegraph.org>
* OpenFrameworks 0062 from <http://openframeworks.org>

### Install (OS X only for the moment, should work with linux/win32, too)

* download, configure and install OpenFrameworks
* download and copy this addon into OpenFramworks/addons
* download, configure and install OpenSceneGraph
  * use cmake  with the following options:
         /usr/bin/cmake -G Xcode \
         -D OSG_BUILD_FRAMEWORKS:BOOL=1 \
         -D OSG_WINDOWING_SYSTEM:STRING=Cocoa \
         -D OSG_BUILD_PLATFORM_IPHONE:BOOL=0 \
         -D CMAKE_OSX_ARCHITECTURES:STRING=i386 \
         -D CMAKE_OSX_SYSROOT:STRING=/Developer/SDKs/MacOSX10.5.sdk .
  * open the xcode-project, build all targets and run the install-target (the run-part is essential!)
* Move the compiled frameworks from OpenSceneGraph to openFrameworks/addons/ofxOpenSceneGraph/lib/osx/frameworks
* Move all plugins from OpenSceneGraph to  openFrameworks/addons/ofxOpenSceneGraph/lib/osx/plugins
* Open one of the examples and try to build and run it.

### How it works

ofxOpenSceneGraph provides a customized ofAppBaseWindow-class which 

* creates an osgViewer::CompositeViewer
* creates an osgViewer::View and register it 
* creates an event-handler to forward events from OpenSceneGraph to OpenFrameworks
* creates a CameraPreDrawCallback which calls the draw-function of the OpenFrameworks-app.
* runs the run-loop and calls the update-function of the OpenFrameworks-app.

There's even a ofxOsgApp-class, so you can get the composite-viewer and view from your app-code. Just inherit from ofBaseApp and ofxOsgApp

    class myApp : public ofBaseApp, public ofxOsgApp {

    };

### Caveats

* the OpenFrameworks-draw-routine is called before any drawing is done by OpenSceneGraph. Future versions may implement a postDraw-function which get called after all osg-related drawing.
* the current implementation does funky things when destroying the app-instance, as some OpenFrameworks-objects need an active OpenGL-context when they get destroyed.
* osg's multi-threaded rendering is disabled, as the update and draw-routines of the framework-app may overlap.

Have fun, 

Stephan ( me at stephanmaximimilanhuber.com )

### Linux instructions by kalwalt

this version of ofxOpenSceneGraph is an adapted version to run with OpenFramework 007 and OSG 3.0.1 .
there are some modification in ofxOsgApp.h , only an include header :
 #include <cstddef> 
 due to issue with the compiler. more details in the OF forum : http://forum.openframeworks.cc/index.php/topic,5843.0.html
in the examples you will find codeblocks projects with makefile. Foloow this instructions:
* download, configure and install OpenFrameworks
* download and copy this addon into OpenFramworks/addons
* download, configure and install OpenSceneGraph
* build OSG with Cmake , after configure and Generate, cd to OSG and run make , and sudo make install.
* the codeblocks project assume that you have installed OSG in your system . but you must set the include flags.... this is not the best solution we will see...
* You can see that i put the lib and the OSG source in linux/lib probably you must replace them with the yours .
* try the examples , look in the osgTerrain example and the bug with the texture ... How we can solve it?




