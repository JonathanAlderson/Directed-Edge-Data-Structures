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

Face2faceindex::Face2faceindex() : GeometricSurfaceFaceDS::GeometricSurfaceFaceDS()
{
	// Sets our file prefixes and suffixes for this file type
  filePrefix = strdup("../faces/");
  fileSuffix = strdup(".face");
}

// read routine returns true on success, failure otherwise
bool Face2faceindex::ReadFile(char *fileName)
	{ // GeometricSurfaceFaceDS::ReadFileTriangleSoup()
  std::cout << "Reading File: " << fileName << '\n';
	// these are for accumulating a bounding box for the object
	Cartesian3 minCoords(1000000.0, 1000000.0, 1000000.0);
	Cartesian3 maxCoords(-1000000.0, -1000000.0, -1000000.0);

	// open the input file
	std::ifstream inFile(fileName);
	if (inFile.bad())
		return false;

	// set the number of vertices and faces
	long nTriangles = 0, nVertices = 0;

	// set the midpoint to the origin
	midPoint = Cartesian3(0.0, 0.0, 0.0);

	// read in the number of vertices
	inFile >> nTriangles;
	nVertices = nTriangles * 3;


  // read in vertex
  // if new, add to vertices
  // add the ID to vertexID
  Cartesian3 thisVertex;
  float x, y, z;
  int uniqueCount = 0;
  int index;


	// now loop to read the vertices in, and hope nothing goes wrong
	for (long vertex = 0; vertex < nVertices; vertex++)
	{ // for each vertex

    // read in the floats
		inFile >> x >> y >> z;

    // construct temporary vertex
    thisVertex = Cartesian3(x, y, z);

    std::cout << "This vertex: " << thisVertex << '\n';

    // check if this is new
    auto foundVertex = std::find(vertices.begin(), vertices.end(), thisVertex);
    if(foundVertex == vertices.end())
    {
      std::cout << "Unique adding new" << '\n';
      // add to list of unique vertices
      vertices.push_back(thisVertex);
      // add the vertexID
      vertexID.push_back(uniqueCount);
      uniqueCount++;
    }
    else
    { // not unique so add the index
      index = std::distance(vertices.begin(), foundVertex);
      vertexID.push_back(index);
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

  for(unsigned int i = 0; i < vertexID.size(); i++)
  {
    std::cout << vertexID[i] << '\n';
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

	// save the filename
	this->filename = (char *) malloc(sizeof(char) * (strlen(fileName) + 1));
	this->filename = strcpy(this->filename, fileName);

  inFile.close();

	return true;
	} // GeometricSurfaceFaceDS::ReadFileTriangleSoup()

bool Face2faceindex::saveFile()
{
  std::cout << "Saving File " << '\n';
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

  std::cout << "Saved File" << '\n';

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
  out.append(std::to_string(vertices.size()));
  out.append(" Faces=");
  out.append(std::to_string(vertexID.size()/3));
  out.append("\n#\n");
  return out;
}

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
  std::cout << "out: " << out << '\n';
  return out;
}

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
	std::cout << "Change file name" << '\n';
  // Split the file name to find the object name
  char delimiters[] = "/.";
  char *index = strtok(this->filename, delimiters);
  index = strtok(NULL, delimiters);

  // index is now the object name, so malloc and save
  this->objName = (char *) malloc(sizeof(char) * (strlen(index) + 1));
  strcpy(this->objName, index);

  // Malloc for new string
  size_t fileLen = strlen(this->filePrefix) + strlen(this->fileSuffix) + strlen(this->objName);
  this->outFileName = (char *) malloc(sizeof(char) * (fileLen + 1));

  // Copy components into the new string
	strcpy(this->outFileName, this->filePrefix);
  strcat(this->outFileName, this->objName);
  strcat(this->outFileName, this->fileSuffix);

	std::cout << ": " << outFileName << '\n';
}

// we need a new render routine since
// storage of vertices is different
void Face2faceindex::Render()
	{ // FaceIndex2DirectedEdge::Render()
	// walk through the faces rendering each one
	glBegin(GL_TRIANGLES);

  std::cout << "Rendering " << '\n';

	// we will loop in 3's, assuming CCW order
	for (unsigned int vertex = 0; vertex < vertexID.size(); )
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
