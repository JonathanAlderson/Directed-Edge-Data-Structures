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


		// read the .tri file to .face file
		if (faceSurface.ReadFile(argv[1]))
		{ // reading .tri failed
			// now save the file we just made
			faceSurface.saveFile();

			std::cout << "File Saved" << '\n';

		
			// now read the .face file we just saved
			if(faceIndexSurface.ReadFile(faceSurface.outFileName))
			{
				// save file
				faceIndexSurface.saveFile();

				std::cout << "File Saved: "  << '\n';

				//	create a window
				GeometricWidget aWindow(&faceIndexSurface, NULL);

				std::cout << "Resize" << '\n';
				// 	set the initial size
				aWindow.resize(600, 600);

				std::cout << "Show" << '\n';
				// show the window
				aWindow.show();

				// set QT running
				return app.exec();
			}
			else
			{	// reading .face failed
				printf("Read failed for .face file %s\n", argv[1]);
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
