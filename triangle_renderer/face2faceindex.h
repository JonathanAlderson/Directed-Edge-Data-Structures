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
#include <sstream>
#include <algorithm>
#include "GeometricSurfaceFaceDS.h"
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

class Face2faceindex : public GeometricSurfaceFaceDS
{
public:

  // char names
  std::string filename;;
  std::string outFileName;
  std::string objName;

  // So we can change those for other file extensions with same code
  std::string filePrefix;
  std::string fileSuffix;

  long uniqueVertices;

  std::vector<long> vertexID;

  Face2faceindex();

  bool ReadFile(std::string fileName) override;

  // File saving functions
  // seperated out for reuse
  void changeFileName();

  bool saveFile();
  std::string saveHeader();

  std::string saveVertices();

  std::string saveFaces();

  // we need a new render function
  void Render() override;

};

#endif
