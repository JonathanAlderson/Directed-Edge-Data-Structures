///////////////////////////////////////////////////
//
//	Jonathan Alderson
//	October, 2020
//
//	------------------------
//	faceindex2directededge.cpp
//	------------------------
//
//	Class Inherited from faceindex2directededge
//  To Save to the .diredge file format
//
///////////////////////////////////////////////////
#include "faceindex2directededge.h"

bool FaceIndex2DirectedEdge::saveFile()
{
  // Find the new filename
  changeFileName();

  // open the input file
  std::ofstream outFile(this->outFileName);
  if (outFile.bad())
    return false;

  // Need to do this first, so we can find unique vertices
  std::string verticesInfo = saveVertices();

	// Next add the header
	outFile << saveHeader();

  // Save all the vertices to the file
  outFile << verticesInfo;

  // Save all the faces to the file
  outFile << saveFaces();

  return 1;
}
