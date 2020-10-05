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
#include "face2faceindex.h"

class FaceIndex2DirectedEdge : public Face2faceindex
{
public:

  // So we can change those for other file extensions with same code
  const char filePrefix[13] = "../diredges/";
  const char fileSuffix[9] = ".diredge";

  // Have to remake this function as will be saving different
  bool saveFile();


};
#endif
