/* 
 * Drag[en]gine Game Engine
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

#include "deSkinPropertyConstructed.h"
#include "deSkinPropertyVisitor.h"
#include "node/deSkinPropertyNodeGroup.h"
#include "node/deSkinPropertyNodeMapped.h"
#include "../../../common/exceptions.h"



// Class deSkinPropertyConstructed
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deSkinPropertyConstructed::deSkinPropertyConstructed( const char *type ) :
deSkinProperty( type ),
pContent( NULL ),
pTileX( false ),
pTileY( false ),
pBitCount( 8 )
{
	pContent = new deSkinPropertyNodeGroup;
	pContent->SetSize( decVector( 256.0f, 256.0f, 1.0f ) );
}

deSkinPropertyConstructed::~deSkinPropertyConstructed(){
	if( pContent ){
		delete pContent;
	}
}



// Management
///////////////

void deSkinPropertyConstructed::SetColor( const decColor &color ){
	pColor = color;
}

void deSkinPropertyConstructed::SetTileX( bool tileX ){
	pTileX = tileX;
}

void deSkinPropertyConstructed::SetTileY( bool tileY ){
	pTileY = tileY;
}

void deSkinPropertyConstructed::SetBitCount( int bitCount ){
	switch( bitCount ){
	case 8:
	case 16:
	case 32:
		pBitCount = bitCount;
		break;
		
	default:
		DETHROW( deeInvalidParam );
	}
}



// Mapped Values
//////////////////

int deSkinPropertyConstructed::GetMappedCount() const{
	return pMapped.GetCount();
}

deSkinPropertyNodeMapped *deSkinPropertyConstructed::GetMappedAt( int index ) const{
	return ( deSkinPropertyNodeMapped* )pMapped.GetAt( index );
}

deSkinPropertyNodeMapped *deSkinPropertyConstructed::GetMappedNamed( const char *name ) const{
	const int count = pMapped.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deSkinPropertyNodeMapped * const mapped = ( deSkinPropertyNodeMapped* )pMapped.GetAt( i );
		if( mapped->GetName() == name ){
			return mapped;
		}
	}
	
	return nullptr;
}

int deSkinPropertyConstructed::IndexOfMapped( deSkinPropertyNodeMapped *mapped ) const{
	return pMapped.IndexOf( mapped );
}

bool deSkinPropertyConstructed::HasMapped( deSkinPropertyNodeMapped *mapped ) const{
	return pMapped.Has( mapped );
}

bool deSkinPropertyConstructed::HasMappedNamed( const char *name ) const{
	const int count = pMapped.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( deSkinPropertyNodeMapped* )pMapped.GetAt( i ) )->GetName() == name ){
			return true;
		}
	}
	
	return false;
}

void deSkinPropertyConstructed::AddMapped( deSkinPropertyNodeMapped *mapped ){
	DEASSERT_NOTNULL( mapped )
	DEASSERT_FALSE( HasMapped( mapped ) )
	
	pMapped.Add( mapped );
}



// Visiting
/////////////

void deSkinPropertyConstructed::Visit( deSkinPropertyVisitor &visitor ){
	visitor.VisitConstructed( *this );
}
