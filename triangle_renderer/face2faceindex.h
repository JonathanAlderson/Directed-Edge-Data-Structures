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
#include <iomanip>
#include "GeometricSurfaceFaceDS.h"

class Face2faceindex : public GeometricSurfaceFaceDS
{
public:

  // char names
  char *filename;;
  char *outFileName;
  char *objName;

  // So we can change those for other file extensions with same code
  char * filePrefix;
  char * fileSuffix;

  long uniqueVertices;

  std::vector<long> vertexID;

  Face2faceindex();

  bool ReadFileTriangleSoup(char *fileName);

  // File saving functions
  // seperated out for reuse
  void changeFileName();

  bool saveFile();
  std::string saveHeader();

  std::string saveVertices();
  std::string formatVertex(long i, Cartesian3 v);

  std::string saveFaces();
  std::string formatFace(long i);

};

#endif
