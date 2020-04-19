/* 
 * Drag[en]gine IGDE World Editor
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

#include "meObjectShape.h"
#include "meObjectShapeList.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShapeList.h>
#include <dragengine/common/shape/decShape.h>
#include <deigde/codec/igdeCodecPropertyString.h>



// Class meObjectShapeList
////////////////////////////

// Constructor, destructor
////////////////////////////

meObjectShapeList::meObjectShapeList(){
}

meObjectShapeList::~meObjectShapeList(){
}



// Management
///////////////

int meObjectShapeList::GetCount() const{
	return pObjectShapes.GetCount();
}

meObjectShape *meObjectShapeList::GetAt( int position ) const{
	return ( meObjectShape* )pObjectShapes.GetAt( position );
}

int meObjectShapeList::IndexOf( meObjectShape *objectShape ) const{
	return pObjectShapes.IndexOf( objectShape );
}

bool meObjectShapeList::Has( meObjectShape *objectShape ) const{
	return pObjectShapes.Has( objectShape );
}

void meObjectShapeList::Add( meObjectShape *objectShape ){
	if( ! objectShape ){
		DETHROW( deeInvalidParam );
	}
	
	pObjectShapes.Add( objectShape );
}

void meObjectShapeList::AddIfAbsent( meObjectShape *objectShape ){
	if( ! objectShape ){
		DETHROW( deeInvalidParam );
	}
	
	pObjectShapes.AddIfAbsent( objectShape );
}

void meObjectShapeList::Remove( meObjectShape *objectShape ){
	if( ! objectShape ){
		DETHROW( deeInvalidParam );
	}
	
	pObjectShapes.Remove( objectShape );
}

void meObjectShapeList::RemoveIfPresent( meObjectShape *objectShape ){
	if( ! objectShape ){
		DETHROW( deeInvalidParam );
	}
	
	pObjectShapes.RemoveIfPresent( objectShape );
}

void meObjectShapeList::RemoveAll(){
	pObjectShapes.RemoveAll();
}



void meObjectShapeList::CreateShapeList( decShapeList &list ) const{
	const int count = pObjectShapes.GetCount();
	decShape *shape = NULL;
	int i;
	
	list.RemoveAll();
	
	try{
		for( i=0; i<count; i++ ){
			shape = ( ( meObjectShape* )pObjectShapes.GetAt( i ) )->GetShape()->Copy();
			list.Add( shape );
			shape = NULL;
		}
		
	}catch( const deException & ){
		if( shape ){
			delete shape;
		}
		throw;
	}
}

void meObjectShapeList::CreatePropertyString( decString &string ) const{
	igdeCodecPropertyString codec;
	decShapeList shapeList;
	
	CreateShapeList( shapeList );
	codec.EncodeShapeList( shapeList, string );
}



meObjectShapeList &meObjectShapeList::operator=( const meObjectShapeList &list ){
	pObjectShapes = list.pObjectShapes;
	return *this;
}
