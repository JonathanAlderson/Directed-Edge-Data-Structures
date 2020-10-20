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

FaceIndex2DirectedEdge::FaceIndex2DirectedEdge() : Face2faceindex::Face2faceindex()
{
  // Sets our file prefixes and suffixes for this file type
  filePrefix = "../diredges/";
  fileSuffix = ".diredge";

  // reset
  vertices.clear();
  vertexID.clear();
  otherHalf.clear();
  firstDirectedEdge.clear();
  midPoint = Cartesian3(0., 0., 0.);
  boundingSphereSize = 1.0;
  manifold = 1; // innocent until proven guilty
}

// read routine returns true on success, failure otherwise
bool FaceIndex2DirectedEdge::ReadFile(std::string fileName)
	{ // FaceIndex2DirectedEdge::ReadFileFace()
	// these are for accumulating a bounding box for the object
	Cartesian3 minCoords(1000000.0, 1000000.0, 1000000.0);
	Cartesian3 maxCoords(-1000000.0, -1000000.0, -1000000.0);

	// open the input file
	std::ifstream inFile(fileName.c_str());
	if (inFile.bad())
		return false;

	// set the midpoint to the origin
	midPoint = Cartesian3(0.0, 0.0, 0.0);

  std::string cLine; // current line
  std::string delim = " ";
  std::string token; // for reading floats from line
  long pos = 0;

  std::string a;
  float thisX, thisY, thisZ;
  int cVertexID; // current vertexID

  // read in the header
  while(token != "Vertex")
  {
    inFile >> token;
    if(inFile.eof()){ std::cout << "Reached EOF Unexpectedly" << '\n'; exit(0); }
  }

  // read in unique vertices
  while(true)
  {
    // Eat Vertex after first line
    if(pos > 0){ inFile >> token; }
    // stop once we start reading faces
    if(token == "Face"){ break; }
    // Eat Vertex ID
    inFile >> token;
    // read float values
    inFile >> thisX >> thisY >> thisZ;
    // add to vertex
    vertices.push_back(Cartesian3(thisX, thisY, thisZ));

    if(inFile.eof()){ std::cout << "Reached EOF Unexpectedly" << '\n'; exit(0); }
    pos++;
  }
  pos = 0;

  // read in the order of vertices that make the faces
  while(true)
  {
    // Eat Face after first line
    if(pos > 0){ inFile >> token;}
    // check for end of file
    if(inFile.eof()){ break;} // not an error this time as we are at end of file
    // Eat face ID
    inFile >> token;
    // read in the 3 vertex ID's from the face
    for(int i = 0; i < 3; i ++)
    {
      inFile >> cVertexID;
      vertexID.push_back(cVertexID);
    }
    pos++;
  }

  // calculate the midPoint / min / max / ect
  Cartesian3 thisVertex;
  for(unsigned long i = 0; i < vertexID.size(); i++)
  {
    // find the current vertex so we only lookup once
    thisVertex = vertices[vertexID[i]];
    midPoint = midPoint + thisVertex;

    if (thisVertex.x < minCoords.x) minCoords.x = thisVertex.x;
    if (thisVertex.y < minCoords.y) minCoords.y = thisVertex.y;
    if (thisVertex.z < minCoords.z) minCoords.z = thisVertex.z;

    if (thisVertex.x > maxCoords.x) maxCoords.x = thisVertex.x;
    if (thisVertex.y > maxCoords.y) maxCoords.y = thisVertex.y;
    if (thisVertex.z > maxCoords.z) maxCoords.z = thisVertex.z;
  }

	// now sort out the size of a bounding sphere for viewing
	// and also set the midpoint's location
	midPoint = midPoint / vertexID.size();

	// now go back through the vertices, subtracting the mid point
  for (unsigned long vertex = 0; vertex < vertices.size(); vertex++)
		{ // per vertex
		vertices[vertex] = vertices[vertex] - midPoint * 2.;
		} // per vertex

	// the bounding sphere radius is just half the distance between these
	boundingSphereSize = sqrt((maxCoords - minCoords).length()) * 1.0;

	// save the filename
	this->filename = fileName;

  inFile.close();

  // find the first directed edges from every vertex
  calculateFirstDirectedEdge();

  // find the other halfs
  calculateOtherHalf();

  // check there are no pinch points
  checkPinchPoint();

  // now we should know if it is manifold or not
  if(manifold == 1)
  {
    std::cout << "\n\n\n\nMesh Is Manifold" << '\n';
    calculateGenus();
  }
  else
  {
    std::cout << "\n\n\nMesh Is Not Manifold" << '\n';
    std::cout << "Not Calculating Genus as not Manifold" << '\n';
  }

	return true;
} // FaceIndex2DirectedEdge::ReadFileFace()


bool FaceIndex2DirectedEdge::saveFile()
{
  // Find the new filename
  changeFileName();

  // open the input file
  std::ofstream outFile(outFileName.c_str());
  if (outFile.bad())
    return false;

  // Need to do this first, so we can find unique vertices
  std::string verticesInfo = saveVertices();

	// Next add the header
	outFile << saveHeader();

  // Save all the vertices to the file
  outFile << verticesInfo;

  // Save all the directed edges to the file
  outFile << saveDirEdges();

  // Save all the faces to the file
  outFile << saveFaces();

  // Save all the faces to the file
  outFile << saveOtherHalf();

  return 1;
}

// traverse through one ring
void FaceIndex2DirectedEdge::checkPinchPoint()
{
  // Calculate the degree of each face in one pass
  std::vector<long> degrees;
  degrees.resize(vertices.size());
  for(unsigned long i = 0; i < vertexID.size(); i++)
  {
    degrees[vertexID[i]]++;
  }

  // start:                   first directed edge
  // first, second and third: edges of current face
  // nextFirst:               next faces first edge
  long startEdge;
  long firstEdge;
  long secondEdge;
  long thirdEdge;
  long nextFirst;
  long thisDegree;

  for(unsigned long i = 0; i < vertices.size(); i++)
  {
    startEdge = firstDirectedEdge[i];

    firstEdge = startEdge;
    nextFirst = -1;

    thisDegree = 0;

    while(nextFirst != startEdge)
    {
      if(thisDegree > 0)
      {
        firstEdge = nextFirst;
      }
      secondEdge = 3 * (firstEdge / 3) + (firstEdge + 1) % 3;
      thirdEdge = 3 *  (secondEdge / 3) + (secondEdge + 1) % 3;
      nextFirst = otherHalf[thirdEdge];

      thisDegree++;
    }
    if(thisDegree != degrees[i])
    {
      std::cout << "Not Manifold: Pinch Point Detected On Vertex: " << i << '\n';
      manifold = 0;
    }
  }
}

// uses formulas from slides to calculate first directed edge from each point
void FaceIndex2DirectedEdge::calculateFirstDirectedEdge()
{
  long h;
  long d;
  long f;
  long i;

  long writeID = 0;
  for(unsigned long vert = 0; vert < vertexID.size(); vert ++)
  {
    h = vert;
    // if first time we've seen this vertex
    if(writeID == vertexID[vert])
    {
      i = h % 3;
      f = h / 3;
      d = 3 * f + i;
      firstDirectedEdge.push_back(d);
      writeID++;
    }
  }
}
// formats and saves to file
std::string FaceIndex2DirectedEdge::saveDirEdges()
{
  std::string out = "";

  for(unsigned long i = 0; i < firstDirectedEdge.size(); i++)
  {
      out.append("FirstDirectedEdge ");
      out.append(std::to_string(i) + "   ");
      out.append(std::to_string(firstDirectedEdge[i]) + "\n");
  }
  return out;
}

// most time consuming algorithm
void FaceIndex2DirectedEdge::calculateOtherHalf()
{
  // start and end vertexes of edges
  int startA, startB;
  int endA, endB;

  int matches;

  otherHalf.resize(vertexID.size());

  // vertexA vertexB
  for(unsigned long vA = 0; vA < vertexID.size(); vA++)
  {
    matches = 0;

    // modular arithmetic to calculate start and end
    startA = vertexID[((vA / 3) * 3) + vA % 3];
    endA = vertexID[(((vA) / 3) * 3) + (vA+1) % 3];


    for(unsigned long vB = 0; vB < vertexID.size(); vB++)
    {
      startB = vertexID[((vB / 3) * 3) + vB % 3];
      endB = vertexID[(((vB) / 3) * 3) + (vB+1) % 3];

      // we have found the other
      if(startA == endB && endA == startB)
      {
        otherHalf[vA] = vB;
        otherHalf[vB] = vA;
        matches++;
      }
    }

    // check for errors
    if(matches < 1 || matches > 1)
    {
      std::cout << "Not Manifold: Vertex " << vertexID[vA] << " has " << matches << " shared edges instead of 1." << '\n';
      manifold = 0;
    }
  }
}

// formats and saves to file
std::string FaceIndex2DirectedEdge::saveOtherHalf()
{
  std::string out = "";

  for(unsigned int i = 0; i < otherHalf.size(); i++)
  {
      out.append("OtherHalf ");
      out.append(std::to_string(i) + "   ");
      out.append(std::to_string(otherHalf[i]) + "\n");
  }
  return out;
}

void FaceIndex2DirectedEdge::calculateGenus()
{
  long v = vertices.size();
  long e = vertexID.size() / 2;
  long f = vertexID.size() / 3;

  // v - e + f = 2 - 2g
  // v - e + f - 2 = -2g
  // e + 2 - f - v = 2g
  // g = (e + 2 - f - v) / 2

  long g = (e + 2 - f - v) / 2;
  std::cout << "Genus: " << g << '\n';
}
