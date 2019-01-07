/*
TP VSION Realite augmentee
DELANNAY Anselme - GINZBURG Lea
17-12-2018
*/


#include <iostream>
#include <cstdlib>
#include <string.h>
#include <aruco.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

using namespace aruco;
using namespace cv;
using namespace std;

#define ESC_KEY   27
int retK;

int main(int argc, char * argv[]) {

	//on recupere le nom de l image passe en ligne de commande
	string imName = argv[1]; 


	// lecture de l image
	Mat im = imread(imName);


	if(!im.data || im.empty() || (im.rows == 0)|| (im.cols == 0)) {
		cout << "Could not load image 1 !" << endl;
		cout << "Exiting now..." << endl;
		return 1;
	}

	//creation d un detecteur de marqueurs 
	MarkerDetector myDetector;
	//liste de marqueurs : sera remplie par ArUco
	vector<Marker> markers;
	//detection
	myDetector.detect(im,markers);
	//on affiche le resultat de la detection sur une image
	for (unsigned int i=0; i<markers.size(); i++){
		cout << markers[i];
		markers[i].draw(im, Scalar(0,0,255), 2);
	}

	// affichage de l image
	namedWindow("Image");
	imshow("Image", im);

	/*
	// DETECTEUR MARQUEURS FLUX CAMERA
	Mat im;
	while (key != ESC_KEY){
		cap >> im;
		//creation d un detecteur de marqueurs 
		MarkerDetector myDetector;
		//liste de marqueurs : sera remplie par ArUco
		vector<Marker> markers;
		//detection
		myDetector.detect(im,markers);
		//on affiche le resultat de la detaction sur une image
		for (unsigned int i=0; i<markers.size(); i++){
			cout << markers[i];
			markers[i].draw(im, Scalar(0,0,255), 2);
		}
		// affichage de l image
		namedWindow("Image detectors");
		imshow("Image detectors", im);

		key = waitKey(1);
	}
	
	// affichage de l image avec detecteurs de marqueurs 
	namedWindow("Image detectors");
	imshow("Image detectors", im);
	*/






	// pour boucle infinie
	retK = 0;

	while(retK != ESC_KEY) {
		// on attend une touche
		retK = waitKey(1);
	}


	return 0;
}

