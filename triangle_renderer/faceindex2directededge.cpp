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

bool FaceIndex2DirectedEdge::saveFile()
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

  // Save all the directed edges to the file
  outFile << saveDirEdges();

  // Save all the faces to the file
  outFile << saveFaces();

  return 1;
}

void FaceIndex2DirectedEdge::testFunction()
{
  std::cout << "Test function" << '\n';
  std::cout << this->filePrefix << '\n';
  std::cout << this->fileSuffix << '\n';
}

std::string FaceIndex2DirectedEdge::saveDirEdges()
{
  std::string out = "";

  long h;
  long d;
  long f;
  long i;

  long writeID = 0;
  for(unsigned long vert = 0; vert < this->vertices.size(); vert++)
  {
    h = vert;
    if(writeID == vertexID[vert])
    {
      i = h % 3;
      f = h / 3;
      d = 3 * f + i;
      out.append(formatDirEdge(writeID, d));
      writeID ++;
    }
  }
  uniqueVertices = writeID;

  return out;
}

std::string FaceIndex2DirectedEdge::formatDirEdge(int id, int edge)
{
  std::string out = "";
  out.append("FirstDirectedEdge ");
  out.append(std::to_string(id) + "   ");
  out.append(std::to_string(edge) + "\n");
  return out;
}
