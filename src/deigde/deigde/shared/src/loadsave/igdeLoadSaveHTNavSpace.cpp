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

#include "igdeLoadSaveHTNavSpace.h"

#include "../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrainNavSpace.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrainNavSpaceEdge.h>
#include <dragengine/resources/navigation/space/deNavigationSpaceFace.h>



// Definitions
////////////////

static const char vSignatureCheck[] = "Drag[en]gine Height Terrain Navigation Space";
static const int vSignatureCheckLen = 44;



// Class igdeLoadSaveHTNavSpace
/////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeLoadSaveHTNavSpace::igdeLoadSaveHTNavSpace( igdeEnvironment &environment, const char *loggingSource ) :
pEnvironment( environment ),
pLoggingSource( loggingSource ),
pName( "Drag[en]gine Height Terrain Navigation Space" ),
pPattern( "*.dehtnavspace" ),
pDefaultExtension( ".dehtnavspace" ){
}

igdeLoadSaveHTNavSpace::~igdeLoadSaveHTNavSpace(){
}



// Management
///////////////

void igdeLoadSaveHTNavSpace::SetName( const char *name ){
	pName = name;
}

void igdeLoadSaveHTNavSpace::SetPattern( const char *pattern ){
	pPattern = pattern;
}

void igdeLoadSaveHTNavSpace::SetDefaultExtension( const char *extension ){
	pDefaultExtension = extension;
}



// Loading and saving
///////////////////////

void igdeLoadSaveHTNavSpace::Load( deHeightTerrainNavSpace &navspace, decBaseFileReader &reader ){
	char signature[ vSignatureCheckLen ];
	int cornerCount = 0;
	int faceCount = 0;
	int edgeCount = 0;
	int version;
	int flags;
	int i;
	
	// clear the navigation space
	navspace.SetEdgeCount( 0 );
	navspace.SetFaceCount( 0 );
	navspace.SetCornerCount( 0 );
	navspace.SetType( deNavigationSpace::estMesh );
	
	// read header
	reader.Read( signature, vSignatureCheckLen );
	if( strncmp( signature, vSignatureCheck, vSignatureCheckLen ) != 0 ){
		DETHROW_INFO( deeInvalidFileFormat, reader.GetFilename() );
	}
	
	version = ( int )reader.ReadUShort();
	flags = ( int )reader.ReadUShort();
	( void )flags;
	
	if( version == 1 ){
		// read counts
		cornerCount = ( int )reader.ReadUShort();
		edgeCount = ( int )reader.ReadUShort();
		faceCount = ( int )reader.ReadUShort();
		
		navspace.SetType( ( deNavigationSpace::eSpaceTypes )reader.ReadUShort() );
		
		navspace.SetCornerCount( cornerCount );
		navspace.SetFaceCount( faceCount );
		
		// read corners
		for( i=0; i<cornerCount; i++ ){
			navspace.SetCornerAt( i, reader.ReadUInt() );
		}
		
		// read edges
		for( i=0; i<edgeCount; i++ ){
			deHeightTerrainNavSpaceEdge &edge = navspace.GetEdgeAt( i );
			edge.SetPoint1( reader.ReadUInt() );
			edge.SetPoint2( reader.ReadUInt() );
			edge.SetType1( reader.ReadUShort() );
			edge.SetType2( reader.ReadUShort() );
		}
		
		// read faces
		for( i=0; i<faceCount; i++ ){
			deNavigationSpaceFace &face = navspace.GetFaceAt( i );
			face.SetCornerCount( reader.ReadUShort() );
			face.SetType( reader.ReadUShort() );
		}
	}
}

void igdeLoadSaveHTNavSpace::Save( const deHeightTerrainNavSpace &navspace, decBaseFileWriter &writer ){
	const int cornerCount = navspace.GetCornerCount();
	const int faceCount = navspace.GetFaceCount();
	const int edgeCount = navspace.GetEdgeCount();
	int i;
	
	// write header
	writer.Write( vSignatureCheck, vSignatureCheckLen );
	
	writer.WriteUShort( 1 ); // version
	writer.WriteUShort( 0 ); // flags
	
	// write counts
	writer.WriteUShort( ( uint16_t )cornerCount );
	writer.WriteUShort( ( uint16_t )edgeCount );
	writer.WriteUShort( ( uint16_t )faceCount );
	
	writer.WriteUShort( ( uint16_t )navspace.GetType() );
	
	// write corners
	for( i=0; i<cornerCount; i++ ){
		writer.WriteUInt( ( uint32_t )navspace.GetCornerAt( i ) );
	}
	
	// write edges
	for( i=0; i<edgeCount; i++ ){
		deHeightTerrainNavSpaceEdge &edge = navspace.GetEdgeAt( i );
		writer.WriteUInt( ( uint32_t )edge.GetPoint1() );
		writer.WriteUInt( ( uint32_t )edge.GetPoint2() );
		writer.WriteUShort( ( uint16_t )edge.GetType1() );
		writer.WriteUShort( ( uint16_t )edge.GetType2() );
	}
	
	// write faces
	for( i=0; i<faceCount; i++ ){
		deNavigationSpaceFace &face = navspace.GetFaceAt( i );
		writer.WriteUShort( ( uint16_t )face.GetCornerCount() );
		writer.WriteUShort( ( uint16_t )face.GetType() );
	}
}
