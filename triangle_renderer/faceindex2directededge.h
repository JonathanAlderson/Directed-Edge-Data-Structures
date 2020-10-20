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

  // extra info we need
  std::vector<long> firstDirectedEdge;
  std::vector<long> otherHalf;

  int manifold;

  FaceIndex2DirectedEdge();

  // Have to remake this function as will be saving different
  bool saveFile();

  // Reads a face file
  bool ReadFile(std::string filename) override;

  // find the new info for .diredge files
  void calculateFirstDirectedEdge();
  void calculateOtherHalf();

  // Check if the mesh has any pinch points
  void checkPinchPoint();

  // Writes all the directed edges to the file
  std::string saveDirEdges();
  std::string saveOtherHalf();

  // Do this only if we have a manifold mesh
  void calculateGenus();


};
#endif
