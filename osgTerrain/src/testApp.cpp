#include "testApp.h"

#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/Shape>
#include <osgViewer/View>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>


#include <osgTerrain/Terrain>
#include <osgTerrain/TerrainTile>
#include <osgTerrain/GeometryTechnique>
#include <osgTerrain/Layer>
#include <iostream>

using namespace std;
using namespace osg;
using namespace osgUtil;

template<class T>
class FindTopMostNodeOfTypeVisitor : public osg::NodeVisitor
{
public:
    FindTopMostNodeOfTypeVisitor():
        osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN),
        _foundNode(0)
    {}

    void apply(osg::Node& node)
    {
        T* result = dynamic_cast<T*>(&node);
        if (result)
        {
            _foundNode = result;
        }
        else
        {
            traverse(node);
        }
    }

    T* _foundNode;
};


template<class T>
T* findTopMostNodeOfType(osg::Node* node)
{
    if (!node) return 0;

    FindTopMostNodeOfTypeVisitor<T> fnotv;
    node->accept(fnotv);

    return fnotv._foundNode;
}

//--------------------------------------------------------------
void testApp::setup(){

     std::cout << "setup" << std::endl;

    osg::setNotifyLevel(osg::DEBUG_INFO);


    // obtain the vertical scale
    float verticalScale = 1.0f;
    

    // obtain the sample ratio
    float sampleRatio = 1.0f;
   
    std::string strBlendingPolicy;
    strBlendingPolicy = "INHERIT";

     osgTerrain::TerrainTile::BlendingPolicy blendingPolicy = osgTerrain::TerrainTile::INHERIT;
   

    // load the nodes from the commandline arguments.
    osg::ref_ptr<osg::Node> rootnode = osgDB::readNodeFile("puget.ive");
    //osg::ref_ptr<osg::Node> rootnode = osgDB::readNodeFile("lz.osg");
    if (!rootnode)
    {
        osg::notify(osg::NOTICE)<<"Warning: no valid data loaded, please specify a database on the command line."<<std::endl;
        return ;
    }

      osg::ref_ptr<osgTerrain::Terrain> terrain = findTopMostNodeOfType<osgTerrain::Terrain>(rootnode.get());
    if (!terrain)
    {
        // no Terrain node present insert one above the loaded model.
        terrain = new osgTerrain::Terrain;

        // if CoordinateSystemNode is present copy it's contents into the Terrain, and discard it.
        osg::CoordinateSystemNode* csn = findTopMostNodeOfType<osg::CoordinateSystemNode>(rootnode.get());
        if (csn)
        {
            terrain->set(*csn);
            for(unsigned int i=0; i<csn->getNumChildren();++i)
            {
                terrain->addChild(csn->getChild(i));
            }
        }
        else
        {
            terrain->addChild(rootnode.get());
        }

        rootnode = terrain.get();
    }
    terrain->setSampleRatio(sampleRatio);
    terrain->setVerticalScale(verticalScale);
    terrain->setBlendingPolicy(blendingPolicy);



    getView()->setSceneData(rootnode.get());
    getView()->setCameraManipulator(new osgGA::TrackballManipulator());
	// of circle do nothing we must use osg graphics i suppose...
	// i tried to setting mipmapping to solve the texture bug... but it dosen't
    
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //glGenerateMipmapEXT( GL_TEXTURE_2D);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

   osg::setNotifyLevel(osg::WARN);

}

//--------------------------------------------------------------
void testApp::update(){
     // std::cout << "update" << std::endl;
}

//--------------------------------------------------------------
void testApp::draw(){
	// also ofDrawBitmapString() do nothing...
     // std::cout << "draw" << std::endl;
    ofDrawBitmapString("ofxOpenSceneGraph Demo", 10, 10);
    ofDrawBitmapString("fps: "+ofToString((ofGetFrameRate())), 10, 25);

    ofDrawBitmapString("'1': set window position\n'2': set window size\n'f': disable full screen \n'F': enable full screen\n's': show osg's stats-handler", 10,50);

    ofCircle(10,20,20);
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

