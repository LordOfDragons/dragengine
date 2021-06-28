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

#include "deoglCollideListPropField.h"
#include "deoglCollideListPropFieldType.h"
#include "../propfield/deoglRPropField.h"
#include "../propfield/deoglRPropFieldType.h"
#include "../propfield/deoglPropFieldCluster.h"

#include <dragengine/common/exceptions.h>



// Class deoglCollideListPropField
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglCollideListPropField::deoglCollideListPropField() :
pPropField( NULL ),
pTypeCount( 0 ){
}

deoglCollideListPropField::~deoglCollideListPropField(){
	const int count = pTypes.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		delete ( deoglCollideListPropFieldType* )pTypes.GetAt( i );
	}
}



// Management
///////////////

void deoglCollideListPropField::Clear(){
	RemoveAllTypes();
	SetPropField( NULL );
}

void deoglCollideListPropField::StartOcclusionTest( deoglOcclusionTest &occlusionTest,
const decDVector &referencePosition ){
	if( ! pPropField ){
		DETHROW( deeInvalidParam );
	}
	
	const decVector offset( pPropField->GetPosition() - referencePosition );
	int i;
	for( i=0; i<pTypeCount; i++ ){
		( ( deoglCollideListPropFieldType* )pTypes.GetAt( i ) )->StartOcclusionTest( occlusionTest, offset );
	}
}

bool deoglCollideListPropField::GetIsEmpty() const{
	int i;
	for( i=0; i<pTypeCount; i++ ){
		if( ( ( deoglCollideListPropFieldType* )pTypes.GetAt( i ) )->GetIsNotEmpty() ){
			return false;
		}
	}
	
	return true;
}

void deoglCollideListPropField::SetPropField( deoglRPropField *propField ){
	RemoveAllTypes();
	pPropField = propField;
}



deoglCollideListPropFieldType *deoglCollideListPropField::GetTypeAt( int index ) const{
	return ( ( deoglCollideListPropFieldType* )pTypes.GetAt( index ) );
}

deoglCollideListPropFieldType *deoglCollideListPropField::AddType( deoglRPropFieldType *type ){
	deoglCollideListPropFieldType *cltype = NULL;
	
	if( pTypeCount < pTypes.GetCount() ){
		cltype = ( deoglCollideListPropFieldType* )pTypes.GetAt( pTypeCount );
		
	}else{
		cltype = new deoglCollideListPropFieldType( *this );
		pTypes.Add( cltype );
	}
	
	cltype->SetType( type );
	pTypeCount++;
	return cltype;
}

void deoglCollideListPropField::RemoveAllTypes(){
	while( pTypeCount > 0 ){
		( ( deoglCollideListPropFieldType* )pTypes.GetAt( --pTypeCount ) )->Clear();
	}
}
