/* 
 * Drag[en]gine IGDE
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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

igdeLoadSaveNavSpace::igdeLoadSaveNavSpace( igdeEnvironment *environment, const char *loggingSource ){
	if( ! environment || ! loggingSource ){
		DETHROW( deeInvalidParam );
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

void igdeLoadSaveNavSpace::SetName( const char *name ){
	pName = name;
}

void igdeLoadSaveNavSpace::SetPattern( const char *pattern ){
	pPattern = pattern;
}

void igdeLoadSaveNavSpace::SetDefaultExtension( const char *extension ){
	pDefaultExtension = extension;
}



// Loading and saving
///////////////////////

void igdeLoadSaveNavSpace::Load( deNavigationSpace &navspace, decBaseFileReader &reader ){
	const char signatureCheck[] = "Drag[en]gine Navigation Space";
	int vertexCount = 0;
	int edgeCount = 0;
	int cornerCount = 0;
	int faceCount = 0;
	int wallCount = 0;
	int roomCount = 0;
	char signature[ 29 ];
	int version; //, flags;
	decVector position;
	int i;
	
	// clear the navigation space
	navspace.SetRoomCount( 0 );
	navspace.SetWallCount( 0 );
	navspace.SetFaceCount( 0 );
	navspace.SetCornerCount( 0 );
	navspace.SetEdgeCount( 0 );
	navspace.SetVertexCount( 0 );
	navspace.SetType( deNavigationSpace::estGrid );
	
	// read header
	reader.Read( &signature, 29 );
	if( strncmp( &signature[ 0 ], &signatureCheck[ 0 ], 29 ) != 0 ){
		DETHROW_INFO( deeInvalidFileFormat, reader.GetFilename() );
	}
	
	version = ( int )reader.ReadUShort();
	/*flags = ( int )*/reader.ReadUShort();
	
	if( version == 1 ){
		// read counts
		vertexCount = ( int )reader.ReadUShort();
		edgeCount = ( int )reader.ReadUShort();
		cornerCount = ( int )reader.ReadUShort();
		faceCount = ( int )reader.ReadUShort();
		wallCount = ( int )reader.ReadUShort();
		roomCount = ( int )reader.ReadUShort();
		
		navspace.SetType( ( deNavigationSpace::eSpaceTypes )reader.ReadUShort() );
		
		navspace.SetVertexCount( vertexCount );
		navspace.SetEdgeCount( edgeCount );
		navspace.SetCornerCount( cornerCount );
		navspace.SetFaceCount( faceCount );
		navspace.SetWallCount( wallCount );
		navspace.SetRoomCount( roomCount );
		
		// read vertices
		for( i=0; i<vertexCount; i++ ){
			position.x = reader.ReadFloat();
			position.y = reader.ReadFloat();
			position.z = reader.ReadFloat();
			navspace.SetVertexAt( i, position );
		}
		
		// read edges
		for( i=0; i<edgeCount; i++ ){
			deNavigationSpaceEdge &edge = navspace.GetEdgeAt( i );
			edge.SetVertex1( reader.ReadUShort() );
			edge.SetVertex2( reader.ReadUShort() );
			edge.SetType1( reader.ReadUShort() );
			edge.SetType2( reader.ReadUShort() );
		}
		
		// read corners
		for( i=0; i<cornerCount; i++ ){
			deNavigationSpaceCorner &corner = navspace.GetCornerAt( i );
			corner.SetVertex( reader.ReadUShort() );
			corner.SetType( reader.ReadUShort() );
		}
		
		// read faces
		for( i=0; i<faceCount; i++ ){
			deNavigationSpaceFace &face = navspace.GetFaceAt( i );
			face.SetCornerCount( reader.ReadUShort() );
			face.SetType( reader.ReadUShort() );
		}
		
		// read walls
		for( i=0; i<wallCount; i++ ){
			deNavigationSpaceWall &wall = navspace.GetWallAt( i );
			wall.SetFace( reader.ReadUShort() );
			wall.SetType( reader.ReadUShort() );
		}
		
		// read rooms
		for( i=0; i<roomCount; i++ ){
			deNavigationSpaceRoom &room = navspace.GetRoomAt( i );
			room.SetFrontWallCount( reader.ReadUShort() );
			room.SetBackWallCount( reader.ReadUShort() );
			room.SetType( reader.ReadUShort() );
		}
	}
}

void igdeLoadSaveNavSpace::Save( const deNavigationSpace &navspace, decBaseFileWriter &writer ){
	DETHROW_INFO( deeInvalidAction, "Not implemented yet" );
}
