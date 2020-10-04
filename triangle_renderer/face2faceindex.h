///////////////////////////////////////////////////
//
//	Jonathan Alderson
//	October, 2020
//
//	------------------------
//	face2faceindex.cpp
//	------------------------
//
//	Class Inherited from GeometricSurfaceFaceDS
//  To Save to the .face file format
//
///////////////////////////////////////////////////

#ifndef _H_FACE2FACEINDEX
#define _H_FACE2FACEINDEX

#include <string.h>
#include <iostream>
#include <cmath>
#include <fstream>
#include "GeometricSurfaceFaceDS.h"

class Face2faceindex : public GeometricSurfaceFaceDS
{
public:

  // char names
  char *filename;;
  char *outFileName;
  char *objName;

  long uniqueVertices;

  std::vector<long> vertexID;

  bool saveFile();

  void changeFileName();

  bool ReadFileTriangleSoup(char *fileName);

  std::string saveVertices();
  std::string formatVertex(long i, Cartesian3 v);

  std::string saveFaces();
  std::string formatFace(long i);
};

#endif
