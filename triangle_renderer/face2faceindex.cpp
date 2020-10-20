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
#include "face2faceindex.h"

Face2faceindex::Face2faceindex()// : GeometricSurfaceFaceDS::GeometricSurfaceFaceDS()
{
	// Sets our file prefixes and suffixes for this file type
  filePrefix = strdup("../faces/");
  fileSuffix = strdup(".face");

  // reset
  //vertices.resize(0);
  vertexID.resize(0);
  //midPoint = Cartesian3(0., 0., 0.);
  boundingSphereSize = 1.0;
}

// read routine returns true on success, failure otherwise
bool Face2faceindex::ReadFile(std::string fileName)
	{ // Face2faceindex::ReadFile()
	// these are for accumulating a bounding box for the object
	Cartesian3 minCoords(1000000.0, 1000000.0, 1000000.0);
	Cartesian3 maxCoords(-1000000.0, -1000000.0, -1000000.0);

	// open the input file
	std::ifstream inFile(fileName.c_str());
	if (inFile.bad())
		return false;

	// set the number of vertices and faces
	long nTriangles = 0, nVertices = 0;

	// reset the midpoint to the origin
	midPoint.x = 0.;
  midPoint.y = 0.;
  midPoint.z = 0.;

	// read in the number of vertices
	inFile >> nTriangles;
	nVertices = nTriangles * 3;


  // read in vertex
  // if new, add to vertices
  // add the ID to vertexID
  Cartesian3 thisVertex(0., 0. ,0.);
  float x, y, z;
  int uniqueCount = 0;
  int foundIndex;

	// now loop to read the vertices in, and hope nothing goes wrong
	for (int vertex = 0; vertex < nVertices; vertex++)
	{ // for each vertex

    // read in the floats
		inFile >> x >> y >> z;

    // we haven't found it yet
    foundIndex = -1;

    // construct temporary vertex
    thisVertex.x = x;
    thisVertex.y = y;
    thisVertex.z = z;

    // check if we have already seen this vertex
    for(unsigned int i = 0; i < vertices.size(); i++)
    {
      if(thisVertex == vertices[i]){ foundIndex = i; break; }
    }

    if(foundIndex == -1)
    {
      // add to list of unique vertices
      vertices.push_back(thisVertex);
      //vertices.push_back(Cartesian3(thisVertex.x, thisVertex.y, thisVertex.z));
      // add the vertexID
      vertexID.push_back(uniqueCount);
      uniqueCount++;
    }
    else
    { // not unique so add the index
      vertexID.push_back(foundIndex);
    }

		// keep running track of midpoint, &c.
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
	midPoint = midPoint / nVertices;

	// now go back through the vertices, subtracting the mid point
	for (unsigned long vertex = 0; vertex < vertices.size(); vertex++)
		{ // per vertex
  		vertices[vertex] = vertices[vertex] - midPoint;
		} // per vertex

	// the bounding sphere radius is just half the distance between these
	boundingSphereSize = sqrt((maxCoords - minCoords).length()) * 1.0;

	// save the filename
	this->filename = fileName;

  inFile.close();

	return true;
	} // Face2faceindex::ReadFile()

bool Face2faceindex::saveFile()
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

  // Save all the faces to the file
  outFile << saveFaces();

  outFile.close();

  return true;
}

// save with the standard header
std::string Face2faceindex::saveHeader()
{
  std::string out = "";
	out.append("# University of Leeds 2020-2021\n");
  out.append("# COMP 5812M Assignment 1\n");
  out.append("# Jonathan Alderson\n");
  out.append("# 201094818\n");
  out.append("#\n");
  out.append("# Object Name: ");
  out.append(objName);
  out.append("\n# Vertices=");
  out.append(std::to_string(vertices.size()));
  out.append(" Faces=");
  out.append(std::to_string(vertexID.size()/3));
  out.append("\n#\n");
  return out;
}

// loop through and save vertices
std::string Face2faceindex::saveVertices()
{
  std::string out = "";
  for(unsigned long i = 0; i < vertices.size(); i++)
  {
    // add the midpoint back for saving
    vertices[i] = vertices[i] + midPoint;
    out.append("Vertex  ");
    out.append(std::to_string(i));
    out.append("  ");
    out.append(std::to_string(vertices[i].x));
    out.append("  ");
    out.append(std::to_string(vertices[i].y));
    out.append("  ");
    out.append(std::to_string(vertices[i].z));
    out.append("\n");
  }
  return out;
}

// loop through and save faces
std::string Face2faceindex::saveFaces()
{
  std::string out = "";
  for(unsigned long i = 0; i < vertexID.size()/3; i++)
  {
    out.append("Face  ");
    out.append(std::to_string(i));
    out.append("  ");
    out.append(std::to_string(vertexID[3 * i]));
    out.append("  ");
    out.append(std::to_string(vertexID[3 * i + 1]));
    out.append("  ");
    out.append(std::to_string(vertexID[3 * i + 2]));
    out.append("\n");
  }
  return out;
}


// changes filenames from ../models/*.tri
//                     to ../faces/*.face
void Face2faceindex::changeFileName()
{
  // Vector of string to save tokens
  std::vector <std::string> tokens;

  // stringstream class check1
  std::stringstream splitter(filename);
  std::string intermediate;

  // Tokenize first by /
  while(getline(splitter, intermediate, '/'))
  {
      tokens.push_back(intermediate);
  }
  // then tokenise again by .
  std::stringstream splitter2(tokens[2]);
  tokens.clear();
  while(getline(splitter2, intermediate, '.'))
  {
      tokens.push_back(intermediate);
  }
  // save the important information
  objName = tokens[0];
  outFileName = filePrefix + objName + fileSuffix;
}

// we need a new render routine since
// storage of vertices is different
void Face2faceindex::Render()
	{ // FaceIndex2DirectedEdge::Render()
	// walk through the faces rendering each one
	glBegin(GL_TRIANGLES);

	// we will loop in 3's, assuming CCW order
	for (unsigned long vertex = 0; vertex < vertexID.size(); )
		{ // per triangle
		// use increment to step through them
		Cartesian3 *v0 = &(vertices[vertexID[vertex++]]);
		Cartesian3 *v1 = &(vertices[vertexID[vertex++]]);
		Cartesian3 *v2 = &(vertices[vertexID[vertex++]]);
		// now compute the normal vector
		Cartesian3 uVec = *v1 - *v0;
		Cartesian3 vVec = *v2 - *v0;
		Cartesian3 normal = uVec.cross(vVec).normalise();

		glNormal3fv(&normal.x);
		glVertex3fv(&v0->x);
		glVertex3fv(&v1->x);
		glVertex3fv(&v2->x);
		} // per triangle
	glEnd();
} // FaceIndex2DirectedEdge::Render()
