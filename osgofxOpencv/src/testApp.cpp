#include "testApp.h"

#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/Shape>
#include <osg/BlendFunc>
#include <osg/Material>
#include <osgViewer/View>
#include <osgGA/TrackballManipulator>

//--------------------------------------------------------------
void testApp::setup(){

    osg::setNotifyLevel(osg::DEBUG_INFO);
    #ifdef _USE_LIVE_VIDEO
        vidGrabber.setVerbose(true);
        vidGrabber.listDevices();
        vidGrabber.setDeviceID(1);
        vidGrabber.initGrabber(320,240);

	#else
        //vidPlayer.loadMovie("fingers.mov");
        //vidPlayer.play();
	#endif

    Img.loadImage("osgshaders2.png");
    Img.allocate(489,478,OF_IMAGE_COLOR);
    colorImg.allocate(320,240);
	grayImage.allocate(320,240);
	grayBg.allocate(320,240);
	grayDiff.allocate(320,240);

	bLearnBakground = true;
	threshold = 80;
    std::cout << "setup" << std::endl;
    synth.loadSound("sounds/synth.wav");
    synth.setVolume(0.75f);
    synth.play();
    osg::ShapeDrawable* drawable = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0,0,0), 1.0f));
    osg::Geode* geode = new osg::Geode();
    geode->addDrawable(drawable);


     // Create StateSet and Material

     osg::StateSet* state2 = geode->getOrCreateStateSet();
    osg::ref_ptr<osg::Material> mat2 = new osg::Material;

    // Set alpha to 0.1
    mat2->setAlpha(osg::Material::FRONT_AND_BACK, 0.4);
    state2->setAttributeAndModes( mat2.get() , osg::StateAttribute::ON |
osg::StateAttribute::OVERRIDE);

     // Turn on blending
    osg::BlendFunc* bf = new osg::BlendFunc(osg::BlendFunc::SRC_ALPHA,
osg::BlendFunc::ONE_MINUS_SRC_ALPHA );
    state2->setAttributeAndModes(bf);

    getView()->setSceneData(geode);
    getView()->setCameraManipulator(new osgGA::TrackballManipulator());

     osg::setNotifyLevel(osg::WARN);
}

//--------------------------------------------------------------
void testApp::update(){

    ofBackground(100,100,100);

    bool bNewFrame = false;

	#ifdef _USE_LIVE_VIDEO
       vidGrabber.grabFrame();
	   bNewFrame = vidGrabber.isFrameNew();
    #else
        //vidPlayer.idleMovie();
        //bNewFrame = vidPlayer.isFrameNew();
	#endif


    //bNewFrame = true;
	if (bNewFrame){

		#ifdef _USE_LIVE_VIDEO
            colorImg.setFromPixels(vidGrabber.getPixels(), 320,240);
	    #else
            //colorImg.setFromPixels(vidPlayer.getPixels(), 320,240);
        #endif


        grayImage = colorImg;
		if (bLearnBakground == true){
			grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
			bLearnBakground = false;
		}

		// take the abs value of the difference between background and incoming and then threshold:
		grayDiff.absDiff(grayBg, grayImage);
		grayDiff.threshold(threshold);

		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayDiff, 20, (340*240)/3, 10, true);	// find holes
	}

     // std::cout << "update" << std::endl;
     ofSoundUpdate();
}

//--------------------------------------------------------------
void testApp::draw(){
    // draw the incoming, the grayscale, the bg and the thresholded difference
	ofSetHexColor(0xffffff);
	colorImg.draw(20,20);
	grayImage.draw(360,20);
	grayBg.draw(20,280);
	grayDiff.draw(360,280);
    Img.draw(700,20);
	// then draw the contours:

	ofFill();
	ofSetHexColor(0x333333);
	ofRect(360,540,320,240);
	ofSetHexColor(0xffffff);

	// we could draw the whole contour finder
	contourFinder.draw(360,540);

	// or, instead we can draw each blob individually,
	// this is how to get access to them:
    for (int i = 0; i < contourFinder.nBlobs; i++){
        contourFinder.blobs[i].draw(360,540);
    }

	// finally, a report:

	ofSetHexColor(0xffffff);
	char reportStr[1024];
	cout << "nnum blobs found " << contourFinder.nBlobs <<endl;
	sprintf(reportStr, "bg subtraction and blob detection\npress ' ' to capture bg\nthreshold %i (press: +/-)\nnum blobs found %i, fps: %f", threshold, contourFinder.nBlobs, ofGetFrameRate());
	ofDrawBitmapString(reportStr, 20, 600);
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
        case 'p':
            synth.play();
            break;
		case ' ':
			bLearnBakground = true;
			break;
		case '+':
			threshold ++;
			if (threshold > 255) threshold = 255;
			break;
		case '-':
			threshold --;
			if (threshold < 0) threshold = 0;
			break;
	}


}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
     //std::cout << "keyReleased" << std::endl;
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
     //std::cout << "mouseMoved" << x << "/" << y << std::endl;
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
   // std::cout << "mouseDragged" << x << "/" << y <<  std::endl;
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
   // std::cout << "mousePressed" << x << "/" << y <<  std::endl;

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
   // std::cout << "mouseReleased" << x << "/" << y <<  std::endl;

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    //std::cout << "windowResized" << w << "x" << h <<  std::endl;
}

