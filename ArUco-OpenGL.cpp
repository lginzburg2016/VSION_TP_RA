//
//  ArUco-OpenGL.cpp
//
//  Created by Jean-Marie Normand on 28/02/13.
//  Copyright (c) 2013 Centrale Nantes. All rights reserved.
//

#include <iostream>
#include "ArUco-OpenGL.h"
#include <opencv2/imgproc/imgproc.hpp>

// Constructor
ArUco::ArUco(string intrinFileName, float markerSize) {
	// Initializing attributes
	m_IntrinsicFile= intrinFileName;
	m_MarkerSize   = markerSize;
	// read camera parameters if passed
	m_CameraParams.readFromXMLFile(intrinFileName);
	//pour le saut
	i = 0.0f;
	position = 0.0f;
}

// Destructor
ArUco::~ArUco() {}

// Detect marker and draw things
void ArUco::doWork(Mat inputImg) {
	m_InputImage   = inputImg;
	m_GlWindowSize = m_InputImage.size();
	resize(m_GlWindowSize.width, m_GlWindowSize.height);
}

// Draw axis function
void ArUco::drawAxis(float axisSize) {
	// X
	glColor3f (1,0,0);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f); // origin of the line
	glVertex3f(axisSize,0.0f, 0.0f); // ending point of the line
	glEnd( );

	// Y
	glColor3f (0,1,0);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f); // origin of the line
	glVertex3f(0.0f, axisSize, 0.0f); // ending point of the line
	glEnd( );

	// Z
	glColor3f (0,0,1);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f); // origin of the line
	glVertex3f(0.0f, 0.0f, axisSize); // ending point of the line
	glEnd( );
}

// GLUT functionnalities

//creation du lutin pour les marqueurs
void lutin(float markerSize){
	//chapeau

	glPushMatrix();
	glColor3f(0.8,0.2,0.2);
	glTranslatef(0, 0, markerSize/2.);
	glutSolidCone(markerSize/8., markerSize/2., 10, 10 );	
	glPopMatrix();

	//tete
	glPushMatrix();
	glTranslatef(0, 0, markerSize/2.);
	glColor3f(0.7f,0.5f,0.5f);
	glutSolidSphere(markerSize/8.,10,10);
	glPopMatrix();

	//pompon
	glPushMatrix();
	glColor3f(1.0f,1.0f,1.0f);
	glTranslatef(0, 0, markerSize);
	glutSolidSphere(markerSize/16.,10,10);
	glPopMatrix();

	//yeux
	glPushMatrix();
	glColor3f(0.f,0.0f,0.0f);
	glTranslatef(-markerSize/16., -markerSize/8. + markerSize/32., markerSize/2.);
	glutSolidSphere(markerSize/32.,10,10);
	glPopMatrix();
	glPushMatrix();
	glColor3f(0.0f,0.0f,0.0f);
	glTranslatef(markerSize/16., -markerSize/8. + markerSize/32., markerSize/2.);
	glutSolidSphere(markerSize/32.,10,10);
	glPopMatrix();

	//cou
	glPushMatrix();
	glColor3f(0.7f,0.5f,0.5f);
	glTranslatef(0, 0, markerSize/3.);
	GLUquadric* qobj = gluNewQuadric();
	gluCylinder(qobj, markerSize/24.,markerSize/24.,markerSize/8.,10,10);
	glPopMatrix();

	//corps
	glPushMatrix();
	glColor3f(0.3f,0.7f,0.3f);
	glTranslatef(0, 0, markerSize/8.);
	glutSolidCube(markerSize/3.);
	glPopMatrix();

	//jambes
	glPushMatrix();
	glColor3f(0.8f,0.2f,0.2f);
	glTranslatef(-markerSize/16.,0, -markerSize/16.);
	glRotatef(180.,0,1,0);
	glutSolidCone(markerSize/16., markerSize/4., 10, 10 );
	glPopMatrix();
	glPushMatrix();
	glColor3f(0.8f,0.2f,0.2f);
	glTranslatef(markerSize/16.,0, -markerSize/16.);
	glRotatef(180.,0,1,0);
	glutSolidCone(markerSize/16., markerSize/4., 10, 10 );
	glPopMatrix();

	//bras
	glPushMatrix();
	glColor3f(0.4f,0.2f,0.8f);
	glTranslatef(-markerSize/3.+markerSize/36., 0, markerSize/11.);
	glRotatef(45.,0,1,0);
	GLUquadric* qobj1 = gluNewQuadric();
	gluCylinder(qobj1, markerSize/32.,markerSize/24.,markerSize/4.,10,10);
	glPopMatrix();
	glPushMatrix();
	glColor3f(0.4f,0.2f,0.8f);
	glTranslatef(markerSize/3.-markerSize/36., 0, markerSize/11.);
	glRotatef(-45.,0,1,0);
	GLUquadric* qobj2 = gluNewQuadric();
	gluCylinder(qobj2, markerSize/32.,markerSize/24.,markerSize/4.,10,10);
	glPopMatrix();
}


// Drawing function
void ArUco::drawScene() {
	// If we do not have an image we don't do anyhting
	if (m_ResizedImage.rows==0)
		return;

	// Setting up  OpenGL matrices
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, m_GlWindowSize.width, 0, m_GlWindowSize.height, -1.0, 1.0);
	glViewport(0, 0, m_GlWindowSize.width , m_GlWindowSize.height);
	glDisable(GL_TEXTURE_2D);
	glPixelZoom( 1, -1);

	//////glRasterPos3f( 0, m_GlWindowSize.height  - 0.5f, -1.0f );
	glRasterPos3f(0, m_GlWindowSize.height, -1.0f);

	glDrawPixels (m_GlWindowSize.width, m_GlWindowSize.height, GL_RGB, GL_UNSIGNED_BYTE, m_ResizedImage.ptr(0));

	// Enabling depth test
	glEnable(GL_DEPTH_TEST);

	// Set the appropriate projection matrix so that rendering is done 
	// in an environment like the real camera (without distorsion)
	glMatrixMode(GL_PROJECTION);
	double proj_matrix[16];
	//m_CameraParams.glGetProjectionMatrix(m_InputImage.size(),m_GlWindowSize,proj_matrix,0.05,10);
	m_CameraParams.glGetProjectionMatrix(m_ResizedImage.size(),m_GlWindowSize,proj_matrix,0.05,10);
	glLoadIdentity();
	glLoadMatrixd(proj_matrix);

	// Debug : outputting projection matrix
	/*
	std::cout<<"Projection Matrix"<<std::endl;
	for(int i=0;i<4;i++) {
	std::cout<<"| ";
	for(int j=0;j<4;j++) {
	std::cout<<proj_matrix[j*4+i]<<"\t\t\t";
	}
	std::cout<<" |"<<std::endl;
	}
	*/

	//now, for each marker,
	double modelview_matrix[16];
	std::cout << "Number of markers: " << m_Markers.size() << std::endl;


	// For each detected marker
	for (unsigned int m=0;m<m_Markers.size();m++)
	{
		m_Markers[m].glGetModelViewMatrix(modelview_matrix);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glLoadMatrixd(modelview_matrix);

		// Disabling light if it is on
		GLboolean lightOn = false;
		glGetBooleanv(GL_LIGHTING, &lightOn);
		if(lightOn) {
			glDisable(GL_LIGHTING);
		}

		// Drawing axis
		drawAxis(m_MarkerSize);

		// Drawing an object
		glColor3f(1,0.4f,0.4f);
		glTranslatef(0, 0, m_MarkerSize/2.);

		//on considere tous les objets sauf le 5eme
		if(m != 4){

			//si on a strictement moins de 5 objets, les lutins sautent
			if(m_Markers.size() < 5){
			//faire sauter l objet
			//plus il y a de marqueurs detectes et plus l objet ira vite
			if (position < 0.0f){
				i = 0.0f;
			}
			position += 0.005*(2-20*i);
			//std::cout<< position << endl;
			i += 0.005;
			glTranslatef(0, 0, m_MarkerSize/2. + position);
			}
			//si on a un 5eme objet, qui sera un icosaedre, les lutins meurent
			else{
				glRotatef(90.0,1,0,0);
				//on ajoute des petits icosaedres (meteorites)
				glPushMatrix();
				glColor3f(0.52,0.02,0.02);
				glTranslatef(0,m_MarkerSize/8.,0);
				glScalef(0.01,0.01,0.01);
				glutSolidIcosahedron();
				glPopMatrix();
			}
			

			glPushMatrix();

			/*ce qui etait donne*/
			//affichage d un cube
			//glutWireCube( m_MarkerSize );

			/*ce qu on choisit d afficher a la place*/
			//affichage d un cone filaire
			//glutWireCone( m_MarkerSize/2, m_MarkerSize, 20, 20 );

			//affichage d un lutin avec la fonction lutin(float markersize)
			lutin(2.0*m_MarkerSize/m_Markers.size());
			

			
		
		}else{
			glPushMatrix();
			//affichage d un icosaedre
			glColor3f(0.52,0.02,0.02);
			glScalef(0.1,0.1,0.1);
			glutSolidIcosahedron();
		}



		// Re-enabling light if it is on
		if(lightOn) {
			glEnable(GL_LIGHTING);
		}

		glPopMatrix();
	}

	// Disabling depth test
	glDisable(GL_DEPTH_TEST);

	m_pixels.create(m_GlWindowSize.height , m_GlWindowSize.width, CV_8UC3);
	//use fast 4-byte alignment (default anyway) if possible
	glPixelStorei(GL_PACK_ALIGNMENT, (m_pixels.step & 3) ? 1 : 4);
	//set length of one complete row in destination data (doesn't need to equal img.cols)
	glPixelStorei(GL_PACK_ROW_LENGTH, m_pixels.step/m_pixels.elemSize());
	// Reading back the pixels
	glReadPixels(0, 0, m_GlWindowSize.width , m_GlWindowSize.height, GL_RGB, GL_UNSIGNED_BYTE, m_pixels.data);
	// Flip the pixels since OpenCV stores top to bottom and OpenGL from bottom to top
	cv::flip(m_pixels, m_pixels, 0);
}

// Idle function
void ArUco::idle(Mat newImage) {
	// Getting new image
	m_InputImage = newImage.clone();

	// Do that here ?
	resize(m_InputImage.size().width, m_InputImage.size().height);

	// Undistort image based on distorsion parameters
	m_UndInputImage.create(m_InputImage.size(),CV_8UC3);

	//transform color that by default is BGR to RGB because windows systems do not allow reading BGR images with opengl properly
	cv::cvtColor(m_InputImage,m_InputImage,CV_BGR2RGB);

	//remove distorion in image
	// Jim commented next line and added the clone line
	//cv::undistort(m_InputImage,m_UndInputImage, m_CameraParams.CameraMatrix, m_CameraParams.Distorsion);
	m_UndInputImage = m_InputImage.clone();

	//detect markers
	m_PPDetector.detect(m_UndInputImage, m_Markers, m_CameraParams.CameraMatrix, Mat(), m_MarkerSize);
	//m_PPDetector.detect(m_UndInputImage, m_Markers, m_CameraParams, m_MarkerSize);

	//resize the image to the size of the GL window
	cv::resize(m_UndInputImage,m_ResizedImage,m_GlWindowSize);
}

// Resize function
void ArUco::resize(GLsizei iWidth, GLsizei iHeight) {
	m_GlWindowSize=Size(iWidth,iHeight);

	//not all sizes are allowed. OpenCv images have padding at the end of each line in these that are not aligned to 4 bytes
	if (iWidth*3%4!=0) {
		iWidth+=iWidth*3%4;//resize to avoid padding
		resize(iWidth, m_GlWindowSize.height);
	}
	else {
		//resize the image to the size of the GL window
		//if (m_UndInputImage.rows!=0)
		//cv::resize(m_UndInputImage, m_ResizedImage, m_GlWindowSize);
	}
}


// Jim
cv::Mat ArUco::getPixels() {
	return m_pixels.clone();
}

// Test using ArUco to display a 3D cube in OpenCV
void ArUco::draw3DCube(cv::Mat img, int markerInd) {
	if(m_Markers.size() > markerInd) {
		aruco::CvDrawingUtils::draw3dCube(img, m_Markers[markerInd], m_CameraParams); 
	}
}

void ArUco::draw3DAxis(cv::Mat img, int markerInd) {
	if(m_Markers.size() > markerInd) {
		aruco::CvDrawingUtils::draw3dAxis(img, m_Markers[markerInd], m_CameraParams); 
	}

}
