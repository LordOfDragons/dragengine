/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "igdeLoadSaveNavSpace.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/resources/navigation/space/deNavigationSpace.h>
#include <dragengine/resources/navigation/space/deNavigationSpaceEdge.h>
#include <dragengine/resources/navigation/space/deNavigationSpaceCorner.h>
#include <dragengine/resources/navigation/space/deNavigationSpaceFace.h>
#include <dragengine/resources/navigation/space/deNavigationSpaceWall.h>
#include <dragengine/resources/navigation/space/deNavigationSpaceRoom.h>



// Class igdeLoadSaveNavSpace
///////////////////////////////

// Constructor, destructor
////////////////////////////

igdeLoadSaveNavSpace::igdeLoadSaveNavSpace(igdeEnvironment *environment, const char *loggingSource){
	if(!environment || !loggingSource){
		DETHROW(deeInvalidParam);
	}
	
	pEnvironment = environment;
	pLoggingSource = loggingSource;
	
	pName = "Drag[en]gine Navigation Space";
	pPattern = "*.denavspace";
	pDefaultExtension = ".denavspace";
}

igdeLoadSaveNavSpace::~igdeLoadSaveNavSpace(){
}



// Management
///////////////

void igdeLoadSaveNavSpace::SetName(const char *name){
	pName = name;
}

void igdeLoadSaveNavSpace::SetPattern(const char *pattern){
	pPattern = pattern;
}

void igdeLoadSaveNavSpace::SetDefaultExtension(const char *extension){
	pDefaultExtension = extension;
}



// Loading and saving
///////////////////////

void igdeLoadSaveNavSpace::Load(deNavigationSpace &navspace, decBaseFileReader &reader){
	const char signatureCheck[] = "Drag[en]gine Navigation Space";
	int vertexCount = 0;
	int edgeCount = 0;
	int cornerCount = 0;
	int faceCount = 0;
	int wallCount = 0;
	int roomCount = 0;
	char signature[29];
	int version; //, flags;
	decVector position;
	int i;
	
	// clear the navigation space
	navspace.GetRooms().RemoveAll();
	navspace.GetWalls().RemoveAll();
	navspace.GetFaces().RemoveAll();
	navspace.GetCorners().RemoveAll();
	navspace.GetEdges().RemoveAll();
	navspace.GetVertices().RemoveAll();
	navspace.SetType(deNavigationSpace::estGrid);
	
	// read header
	reader.Read(&signature, 29);
	if(strncmp(&signature[0], &signatureCheck[0], 29) != 0){
		DETHROW_INFO(deeInvalidFileFormat, reader.GetFilename());
	}
	
	version = (int)reader.ReadUShort();
	/*flags = ( int )*/reader.ReadUShort();
	
	if(version == 1){
		// read counts
		vertexCount = (int)reader.ReadUShort();
		edgeCount = (int)reader.ReadUShort();
		cornerCount = (int)reader.ReadUShort();
		faceCount = (int)reader.ReadUShort();
		wallCount = (int)reader.ReadUShort();
		roomCount = (int)reader.ReadUShort();
		
		navspace.SetType((deNavigationSpace::eSpaceTypes)reader.ReadUShort());
		
		navspace.GetVertices().SetCountDiscard(vertexCount);
		navspace.GetEdges().SetCountDiscard(edgeCount);
		navspace.GetCorners().SetCountDiscard(cornerCount);
		navspace.GetFaces().SetCountDiscard(faceCount);
		navspace.GetWalls().SetCountDiscard(wallCount);
		navspace.GetRooms().SetCountDiscard(roomCount);
		
		// read vertices
		for(i=0; i<vertexCount; i++){
			position.x = reader.ReadFloat();
			position.y = reader.ReadFloat();
			position.z = reader.ReadFloat();
			navspace.GetVertices()[i] = position;
		}
		
		// read edges
		for(i=0; i<edgeCount; i++){
			deNavigationSpaceEdge &edge = navspace.GetEdges()[i];
			edge.SetVertex1(reader.ReadUShort());
			edge.SetVertex2(reader.ReadUShort());
			edge.SetType1(reader.ReadUShort());
			edge.SetType2(reader.ReadUShort());
		}
		
		// read corners
		for(i=0; i<cornerCount; i++){
			deNavigationSpaceCorner &corner = navspace.GetCorners()[i];
			corner.SetVertex(reader.ReadUShort());
			corner.SetType(reader.ReadUShort());
		}
		
		// read faces
		for(i=0; i<faceCount; i++){
			deNavigationSpaceFace &face = navspace.GetFaces()[i];
			face.SetCornerCount(reader.ReadUShort());
			face.SetType(reader.ReadUShort());
		}
		
		// read walls
		for(i=0; i<wallCount; i++){
			deNavigationSpaceWall &wall = navspace.GetWalls()[i];
			wall.SetFace(reader.ReadUShort());
			wall.SetType(reader.ReadUShort());
		}
		
		// read rooms
		for(i=0; i<roomCount; i++){
			deNavigationSpaceRoom &room = navspace.GetRooms()[i];
			room.SetFrontWallCount(reader.ReadUShort());
			room.SetBackWallCount(reader.ReadUShort());
			room.SetType(reader.ReadUShort());
		}
	}
}

void igdeLoadSaveNavSpace::Save(const deNavigationSpace &navspace, decBaseFileWriter &writer){
	DETHROW_INFO(deeInvalidAction, "Not implemented yet");
}
