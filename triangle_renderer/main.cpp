///////////////////////////////////////////////////
//
//	Hamish Carr
//	January, 2018
//
//	------------------------
//	main.cpp
//	------------------------
//
///////////////////////////////////////////////////

#include <QApplication>
#include "GeometricWidget.h"
#include "face2faceindex.h"
#include "faceindex2directededge.h"
#include <stdio.h>

int main(int argc, char **argv)
	{ // main()
	// initialize QT
	QApplication app(argc, argv);


	// the geometric surface
	//GeometricSurfaceFaceDS geoSurface;

	// face reader
	Face2faceindex faceSurface;


	// directed edge reader
	FaceIndex2DirectedEdge faceIndexSurface;


	// check the args to make sure there's an input file
	if (argc == 2)
		{ // two parameters - read a file

		if(faceSurface.ReadFile(argv[1]))
		{
			faceSurface.saveFile();

			// now read the file we just made
			if(faceIndexSurface.ReadFile(faceSurface.outFileName))
			{
				faceIndexSurface.saveFile();

				//	create a window
				GeometricWidget aWindow(&faceIndexSurface, NULL);

				// 	set the initial size
				aWindow.resize(600, 600);

				// show the window
				aWindow.show();

				// set QT running
				return app.exec();
			}
			else
			{
				std::cout << "Read failed for file " << faceSurface.outFileName.c_str() << '\n';
				exit(0);
			}

		} // surface read failed
		else
		{
			printf("Read failed for file %s\n", argv[1]);
			exit(0);
		}
		} // two parameters - read a file

	else
		{ // too many parameters
		printf("Usage: %s filename\n", argv[0]);
		exit (0);
		} // too many parameters



	// paranoid return value
	exit(0);
	} // main()
