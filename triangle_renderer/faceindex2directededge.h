///////////////////////////////////////////////////
//
//	Jonathan Alderson
//	October, 2020
//
//	------------------------
//	faceindex2directededge.cpp
//	------------------------
//
//	Computes the directed edges
//  To Save to the .diredge file format
//
///////////////////////////////////////////////////

#ifndef _H_FACEINDEX2DIRECTEDEDGE
#define _H_FACEINDEX2DIRECTEDEDGE

#include <string.h>
#include <iostream>
#include <cmath>
#include <fstream>
#include <iomanip>
#include "face2faceindex.h"

class FaceIndex2DirectedEdge : public Face2faceindex
{
public:

  FaceIndex2DirectedEdge();

  // Have to remake this function as will be saving different
  bool saveFile();

  // Writes all the directed edges to the file
  std::string saveDirEdges();

  // Formats into a string
  std::string formatDirEdge(int id, int edge);

};
#endif
