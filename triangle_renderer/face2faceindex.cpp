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

// read routine returns true on success, failure otherwise
bool Face2faceindex::ReadFileTriangleSoup(char *fileName)
	{ // GeometricSurfaceFaceDS::ReadFileTriangleSoup()
	// these are for accumulating a bounding box for the object
	Cartesian3 minCoords(1000000.0, 1000000.0, 1000000.0);
	Cartesian3 maxCoords(-1000000.0, -1000000.0, -1000000.0);

	// open the input file
	std::ifstream inFile(fileName);
	if (inFile.bad())
		return false;

	// save the filename
	this->filename = (char *) malloc(sizeof(char) * (strlen(fileName) + 1));
	this->filename = strcpy(this->filename, fileName);

	// set the number of vertices and faces
	long nTriangles = 0, nVertices = 0;

	// set the midpoint to the origin
	midPoint = Cartesian3(0.0, 0.0, 0.0);

	// read in the number of vertices
	inFile >> nTriangles;
	nVertices = nTriangles * 3;

	// now allocate space for them all
	vertices.resize(nVertices);

  // now allocate space for all vertexID's
  vertexID.resize(nVertices, -1);


	// now loop to read the vertices in, and hope nothing goes wrong
	for (long vertex = 0; vertex < nVertices; vertex++)
		{ // for each vertex
		inFile >> vertices[vertex].x >> vertices[vertex].y >> vertices[vertex].z;

		// keep running track of midpoint, &c.
		midPoint = midPoint + vertices[vertex];
		if (vertices[vertex].x < minCoords.x) minCoords.x = vertices[vertex].x;
		if (vertices[vertex].y < minCoords.y) minCoords.y = vertices[vertex].y;
		if (vertices[vertex].z < minCoords.z) minCoords.z = vertices[vertex].z;

		if (vertices[vertex].x > maxCoords.x) maxCoords.x = vertices[vertex].x;
		if (vertices[vertex].y > maxCoords.y) maxCoords.y = vertices[vertex].y;
		if (vertices[vertex].z > maxCoords.z) maxCoords.z = vertices[vertex].z;
		} // for each vertex

  // now find all the vertex ID's
  long nextVertexID = 0;
  for (unsigned long vertex = 0; vertex < vertices.size(); vertex++)
  {
    // first see if the vertex already exists
    for (unsigned long other = 0; other < vertex; other ++)
    {
      // check all the vertexes we already went through
      if(vertices[vertex] == vertices[other])
      {
        vertexID[vertex] = vertexID[other];
      }
    }
    // is a new vertex, so give it a new ID
    if (vertexID[vertex] == -1)
    {
      vertexID[vertex] = nextVertexID++;
    }
  }

	// now sort out the size of a bounding sphere for viewing
	// and also set the midpoint's location
	midPoint = midPoint / vertices.size();

	// now go back through the vertices, subtracting the mid point
	for (int vertex = 0; vertex < nVertices; vertex++)
		{ // per vertex
		vertices[vertex] = vertices[vertex] - midPoint;
		} // per vertex

	// the bounding sphere radius is just half the distance between these
	boundingSphereSize = sqrt((maxCoords - minCoords).length()) * 1.0;

	return true;
	} // GeometricSurfaceFaceDS::ReadFileTriangleSoup()

bool Face2faceindex::saveFile()
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

  return true;
}

std::string Face2faceindex::saveHeader()
{
  std::string out = "";
	out.append("# University of Leeds 2020-2021\n");
  out.append("# COMP 5812M Assignment 1\n");
  out.append("# Jonathan Alderson\n");
  out.append("# 201094818\n");
  out.append("#\n");
  out.append("# Object Name: ");
  out.append(this->objName);
  out.append("\n# Vertices=");
  out.append(std::to_string(this->uniqueVertices));
  out.append(" Faces=");
  out.append(std::to_string(this->vertices.size()/3));
  out.append("\n#\n");
  return out;
}

std::string Face2faceindex::saveVertices()
{
  std::string out = "";
  long writeID = 0;
  for(unsigned long i = 0; i < this->vertices.size(); i++)
  {
    if(writeID == vertexID[i])
    {
      out.append(formatVertex(writeID, vertices[i]));
      writeID ++;
    }
  }
  uniqueVertices = writeID;
  return out;
}

std::string Face2faceindex::saveFaces()
{
  std::string out = "";
  for(unsigned long i = 0; i < vertices.size()/3; i++)
  {
    out.append(formatFace(i));
  }
  return out;
}

std::string Face2faceindex::formatVertex(long i, Cartesian3 v)
{
  std::string out = "";
  out.append("Vertex  ");
  out.append(std::to_string(i));
  out.append("  ");
  out.append(std::to_string(v.x));
  out.append("  ");
  out.append(std::to_string(v.y));
  out.append("  ");
  out.append(std::to_string(v.z));
  out.append("\n");
  return out;
}

std::string Face2faceindex::formatFace(long i)
{
  std::string out = "Face  ";
  out.append(std::to_string(i));
  out.append("  ");
  out.append(std::to_string(vertexID[3 * i]));
  out.append("  ");
  out.append(std::to_string(vertexID[3 * i + 1]));
  out.append("  ");
  out.append(std::to_string(vertexID[3 * i + 2]));
  out.append("\n");
  return out;
}

// changes filenames from ../models/*.tri
//                     to ../faces/*.face
void Face2faceindex::changeFileName()
{
  // Split the file name to find the object name
  char delimiters[] = "/.";
  char *index = strtok(this->filename, delimiters);
  index = strtok(NULL, delimiters);

  // index is now the object name, so malloc and save
  this->objName = (char *) malloc(sizeof(char) * (strlen(index) + 1));
  strcpy(this->objName, index);

  // Malloc for new string
  size_t fileLen = strlen(filePrefix) + strlen(fileSuffix) + strlen(this->objName);
  this->outFileName = (char *) malloc(sizeof(char) * (fileLen + 1));

  // Copy components into the new string
	strcpy(this->outFileName, filePrefix);
  strcat(this->outFileName, this->objName);
  strcat(this->outFileName, fileSuffix);
}
