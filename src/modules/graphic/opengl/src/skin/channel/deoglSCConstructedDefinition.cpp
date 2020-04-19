/* 
 * Drag[en]gine OpenGL Graphic Module
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

#include "deoglSCConstructedDefinition.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decMemoryFile.h>
#include <dragengine/common/file/decMemoryFileWriter.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/skin/property/deSkinPropertyConstructed.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeGroup.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeImage.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeShape.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeText.h>



// Definitions
////////////////

enum eNodeTypes{
	entNothing,
	entGroup,
	entImage,
	entShape,
	entText
};



// Class deoglSCConstructedDefinition
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSCConstructedDefinition::deoglSCConstructedDefinition( const deEngine &engine,
decMemoryFile *definition, decMemoryFile *verify, const deSkinPropertyConstructed &property ) :
pEngine( engine ),
pDefinition( NULL ),
pVerify( NULL ),
pCacheValid( true )
{
	if( ! definition || ! verify ){
		DETHROW( deeInvalidParam );
	}
	
	pDefinition = new decMemoryFileWriter( definition, false );
	pVerify = new decMemoryFileWriter( verify, false );
	
	// property contains properties not covered in the content but required for constructing.
	// these properties are stored first
	uint8_t flags = 0;
	if( property.GetTileX() ){
		flags |= 0x1;
	}
	if( property.GetTileY() ){
		flags |= 0x2;
	}
	
	pDefinition->WriteColor( property.GetColor() );
	pDefinition->WriteByte( flags );
}

deoglSCConstructedDefinition::~deoglSCConstructedDefinition(){
	if( pVerify ){
		pVerify->FreeReference();
	}
	if( pDefinition ){
		pDefinition->FreeReference();
	}
}



// Visiting
/////////////

void deoglSCConstructedDefinition::VisitGroup( deSkinPropertyNodeGroup &node ){
	const int count = node.GetNodeCount();
	int i;
	
	pDefinition->WriteByte( ( uint8_t )entGroup );
	pWriteBasicProperties( node );
	if( ! pCacheValid ){
		return;
	}
	
	for( i=0; i<count; i++ ){
		node.GetNodeAt( i )->Visit( *this );
		if( ! pCacheValid ){
			return;
		}
	}
	pDefinition->WriteByte( ( uint8_t )entNothing );
}

void deoglSCConstructedDefinition::VisitImage( deSkinPropertyNodeImage &node ){
	pDefinition->WriteByte( ( uint8_t )entImage );
	pWriteBasicProperties( node );
	if( ! pCacheValid ){
		return;
	}
	
	const deImage * const image = node.GetImage();
	if( image ){
		const deVirtualFileSystem * const vfs = image->GetVirtualFileSystem();
		if( vfs != pEngine.GetVirtualFileSystem() ){
			pCacheValid = false;
			return;
		}
		
		decPath path;
		path.SetFromUnix( image->GetFilename() );
		if( ! vfs->CanReadFile( path ) ){
			pCacheValid = false;
			return;
		}
		
		pDefinition->WriteString16( image->GetFilename() );
		
		pVerify->WriteUInt( ( uint32_t )vfs->GetFileModificationTime( path ) );
		
	}else{
		pDefinition->WriteString16( "" );
	}
	
	pDefinition->WritePoint( node.GetRepeat() );
}

void deoglSCConstructedDefinition::VisitShape( deSkinPropertyNodeShape &node ){
	pDefinition->WriteByte( ( uint8_t )entShape );
	pWriteBasicProperties( node );
	if( ! pCacheValid ){
		return;
	}
	
	pDefinition->WriteByte( ( uint8_t )node.GetShapeType() );
	pWriteColor( node.GetFillColor() );
	pWriteColor( node.GetLineColor() );
	pWriteFloat( node.GetThickness(), 10.0f ); // 0.1 precision
}

void deoglSCConstructedDefinition::VisitText( deSkinPropertyNodeText &node ){
	pDefinition->WriteByte( ( uint8_t )entText );
	pWriteBasicProperties( node );
	if( ! pCacheValid ){
		return;
	}
	
	const deFont * const font = node.GetFont();
	if( font ){
		const deVirtualFileSystem * const vfs = font->GetVirtualFileSystem();
		if( vfs != pEngine.GetVirtualFileSystem() ){
			pCacheValid = false;
			return;
		}
		
		decPath path;
		path.SetFromUnix( font->GetFilename() );
		if( ! vfs->CanReadFile( path ) ){
			pCacheValid = false;
			return;
		}
		
		pDefinition->WriteString16( font->GetFilename() );
		
		pVerify->WriteUInt( ( uint32_t )vfs->GetFileModificationTime( path ) );
		
	}else{
		pDefinition->WriteString16( "" );
	}
	
	pWriteFloat( node.GetFontSize(), 10.0f ); // 0.1 precision
	pDefinition->WriteString16( node.GetText() );
	pWriteColor( node.GetColor() );
}



// Private Functions
//////////////////////

void deoglSCConstructedDefinition::pWriteBasicProperties( const deSkinPropertyNode &node ){
	pDefinition->WritePoint3( node.GetPosition() );
	pDefinition->WritePoint3( node.GetSize() );
	pWriteFloat( node.GetRotation(), 100.0f ); // 0.01 degree precision
	pWriteFloat( node.GetShear(), 100.0f ); // 0.01 degree precision
	
	pWriteFloat( node.GetBrightness(), 100.0f ); // 0.01 precision
	pWriteFloat( node.GetContrast(), 100.0f ); // 0.01 precision
	pWriteFloat( node.GetGamma(), 100.0f ); // 0.01 precision
	pWriteColor( node.GetColorize() );
	
	pWriteFloat( node.GetTransparency(), 255.0f );
	
	if( node.GetMask() ){
		node.GetMask()->Visit( *this );
		
	}else{
		pDefinition->WriteByte( ( uint8_t )entNothing );
	}
}

void deoglSCConstructedDefinition::pWriteFloat( float value, float rounding ){
	pDefinition->WriteShort( ( int16_t )( value * rounding ) );
}

void deoglSCConstructedDefinition::pWriteColor( const decColor &color ){
	pDefinition->WriteShort( ( int16_t )( color.r * 255.0f ) );
	pDefinition->WriteShort( ( int16_t )( color.g * 255.0f ) );
	pDefinition->WriteShort( ( int16_t )( color.b * 255.0f ) );
	pDefinition->WriteShort( ( int16_t )( color.a * 255.0f ) );
}
