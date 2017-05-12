#include "ofApp.h"
#include<iostream>
#include <vector>         //  Vector Header file is added
using namespace std;
int step = int(1);
int point_size = int(6.5);
int mode = 0;
float x=0;
float y=0;
float ro_degree = 0;
int flag = 0;

ofVec3f  center_pix;
//--------------------------------------------------------------
void ofApp::setup(){
	device_ = new ofxKinect2::Device();
	device_->setup();

	if(depth_.setup(*device_))
	{
		depth_.open();
	}

	if (color_.setup(*device_))
	{
		color_.open();
	}

	if (ir_.setup(*device_))
	{
		ir_.open();
	}
	
	if(body_stream_.setup(*device_))
	{
		body_stream_.open();
	}
    
    if(mapper_.setup(*device_))
    {
		mapper_.setColor(color_);
		mapper_.setDepth(depth_);
    }

	scale_ = 20;
	mapped_pix_.allocate(512, 424, OF_PIXELS_RGBA);

	ofVec3f scale = cam_.getScale();
	cam_.setScale(scale.x, scale.y, -scale.z);
	cam_.setFarClip(6500);
}

//--------------------------------------------------------------
void ofApp::update(){
	device_->update();
}
void init() {
	
}
//--------------------------------------------------------------
void ofApp::draw(){
	ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
	if (!mapper_.isReady())
	{
		return;
	}
    // point cloud
	vector<ofVec3f>& pointcloud_pos = mapper_.mapDepthToCameraSpace();
	vector<ofFloatColor>& pointcloud_colors = mapper_.getFloatColorsCoordinatesToDepthFrame();

	cam_.begin();
	
	ofPushMatrix();
	ofEnableDepthTest();
	ofScale(100 * scale_, 100 * scale_, 100 * scale_);

	
	ofMesh mesh;

	vector <ofVec3f>   points;
	vector <ofFloatColor>  colors;
	vector <int>   indexs;
	int width = depth_.getWidth();
	int height = depth_.getHeight();
	
	int total = 0;

	
	//mesh.setMode(OF_PRIMITIVE_TRIANGLES);
	mesh.setMode(OF_PRIMITIVE_POINTS); //HANK 2017/5/12
		 
	// 深度情報をindexを付与
	int ind = 0;
	 
	float pix_center_x = 0;
	float pix_center_y = 0;
	float pix_center_z = 0;
	

	/// NEW ADD
	
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	glPointSize(point_size);
	ofPushMatrix();
	// the projected points are 'upside down' and 'backwards' 
	ofScale(1, -1, -1);
	//ofTranslate(0, 0, -1000); // center the points a bit

	ofPopMatrix();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	for (int n = 0; n < pointcloud_pos.size(); n += step)
	{

		//          cout << points[m][n] << endl;
		mesh.addVertex(pointcloud_pos[n]);
		mesh.addColor(pointcloud_colors[n]);


		pix_center_x = pix_center_x + pointcloud_pos[n].x;
		pix_center_y = pix_center_y + pointcloud_pos[n].y;
		pix_center_z = pix_center_z + pointcloud_pos[n].z;


	}
	pix_center_x = pix_center_x / pointcloud_pos.size();
	pix_center_y = pix_center_y / pointcloud_pos.size();
	pix_center_z = pix_center_z / pointcloud_pos.size();
	if (flag == 0) {
		center_pix = ofVec3f(pix_center_x, pix_center_y, pix_center_z);
		flag = 1;
	}
	/*
	// TEMP DASH HANK 5/12
	for (int y = 0; y < height/step ; y++)
	{
		for (int x = 0; x < width/step ; x++)
		{
			mesh.addIndex(x + y*width);               // 0
			mesh.addIndex((x + 1) + y*width);           // 1
			mesh.addIndex(x + (y + 1)*width);           // 10

			mesh.addIndex((x + 1) + y*width);           // 1
			mesh.addIndex((x + 1) + (y + 1)*width);       // 11
			mesh.addIndex(x + (y + 1)*width);   // 10

		}
	}
	*/
	
	//mesh.draw();

	//cout << pointcloud_pos.size()/step<<endl;
	//cout << pointcloud_colors.size()/step << endl;
	//cout << width/step << endl;
	//init();
	
	cout << center_pix << endl;
	
	//mesh.addVertices(pointcloud_pos);
	//mesh.addColors(pointcloud_colors);

//	int width = depth_.getWidth();
	//int height = depth_.getHeight();
	/*for (int y = 0; y < height - 1; y++)
	{
		for (int x = 0; x < width - 1; x++)
		{
			mesh.addIndex(x + y*width);               // 0
			mesh.addIndex((x + 1) + y*width);           // 1
			mesh.addIndex(x + (y + 1)*width);           // 10

			mesh.addIndex((x + 1) + y*width);           // 1
			mesh.addIndex((x + 1) + (y + 1)*width);       // 11
			mesh.addIndex(x + (y + 1)*width);   // 10

		}
	}*/

	if (mode == 0) {
		mesh.draw();
	}
	else {
		mesh.drawWireframe();
	}
	//ofSetRectMode(OF_RECTMODE_CENTER);
	//ofPushMatrix();
	//ofTranslate(400, 300);
	//ofRotateZ(ofGetElapsedTimef() * 10);
	//ofRect(-10, -10, 20, 20);
	//glRotatef(ofGetElapsedTimef() * 10,0,0,0);
	//ofRect(0, 0, 20, 20);
	
	//ofRotateZ(22.49);  // <- rotate the circle around the z axis by some amount.   
	//ofCircle(0, 0, 0);
	
	
	ofPopMatrix();//temp dash 5/12
	
	//ofPopMatrix();
	cam_.end();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key)
	{
	case 'd':
		x += 1;
		target_ = ofVec3f(x, y, 5.0);
		cam_.setTarget(target_);
		break;
	case 'a':
		x += -1;
		target_ = ofVec3f(x, y, 5.0);
		cam_.setTarget(target_);
		break;
	case 'w':
		y += 1;
		target_ = ofVec3f(x, y, 5.0);
		cam_.setTarget(target_);
		break;
	case 's':
		y += -1;
		target_ = ofVec3f(x, y, 5.0);
		cam_.setTarget(target_);
		break; 
	case 'r':
		 
		//target_ = ofVec3f(x, y, 0.0);
		//cam_.setTarget(target_);
		ro_degree += 2;
		if (ro_degree == 360 || ro_degree == 0) {
			ro_degree = 0;
		}
		cam_.rotateAround(ro_degree, cam_.getUpDir(),center_pix);
		break;
	case 'o':
		 
		cam_.orbit(0, -20, -10);
		break;

	case '.':
		point_size = point_size + 0.5;
		break;
	case ',':
		point_size = point_size - 0.5;
		if (point_size <= 0.5) {
			point_size = 0.5;
		}
		break;

	case 'R':
		 
		//target_ = ofVec3f(x, y, 0.0);
		//cam_.setTarget(target_);
		ro_degree -= 2;
		if (ro_degree == 360 || ro_degree == 0) {
			ro_degree = 0;
		}
		cam_.rotateAround(ro_degree, cam_.getUpDir(), center_pix);
		//cam_.setupPerspective(false,60,0,-10);
		break;
	case OF_KEY_UP:
		step++;
		break;
	case OF_KEY_RIGHT:
		scale_++;
		if (scale_ > 100)
		{
			scale_ = 100;
		}
		break;
	case OF_KEY_DOWN:
		step--;
		if (step <= 1) {
			step = 1;
		}
		break;
	case OF_KEY_LEFT:
		scale_--;
		if (scale_ < 1)
		{
			scale_ = 1;
		}
		break;
	case 'm':
		if (mode == 0) {
			mode = 1;
		}
		else {
			mode = 0;
		}
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	target_ = ofVec3f(0, 0, 5.0);
	cam_.setTarget(target_);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::exit()
{
	color_.close();
	depth_.close();
	ir_.close();
	body_stream_.close();
	device_->exit();
	delete device_;
	device_ = NULL;
}