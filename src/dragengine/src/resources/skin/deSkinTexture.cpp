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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deSkinTexture.h"
#include "property/deSkinProperty.h"
#include "../../common/exceptions.h"



// Class deSkinTexture
////////////////////////

// Constructor, destructor
////////////////////////////

deSkinTexture::deSkinTexture( const char *name ){
	if( ! name ) DETHROW( deeInvalidParam );
	
	pName = NULL;
	
	pProperties = NULL;
	pPropertyCount = 0;
	pPropertySize = 0;
	
	try{
		pName = new char[ strlen( name ) + 1 ];
		if( ! pName ) DETHROW( deeOutOfMemory );
		strcpy( pName, name );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deSkinTexture::~deSkinTexture(){
	pCleanUp();
}



// Properties
///////////////

deSkinProperty *deSkinTexture::GetPropertyAt( int index ) const{
	if( index < 0 || index >= pPropertyCount ) DETHROW( deeOutOfBoundary );
	
	return pProperties[ index ];
}

deSkinProperty *deSkinTexture::GetPropertyWithType( const char *type ) const{
	int i;
	
	for( i=0; i<pPropertyCount; i++ ){
		if( pProperties[ i ]->GetType().Equals( type ) ){
			return pProperties[ i ];
		}
	}
	
	return NULL;
}

int deSkinTexture::IndexOfProperty( deSkinProperty *property ) const{
	if( ! property ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pPropertyCount; i++ ){
		if( property == pProperties[ i ] ){
			return i;
		}
	}
	
	return -1;
}

bool deSkinTexture::HasProperty( deSkinProperty *property ) const{
	if( ! property ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pPropertyCount; i++ ){
		if( property == pProperties[ i ] ){
			return true;
		}
	}
	
	return false;
}

bool deSkinTexture::HasPropertyWithType( const char *type ) const{
	int i;
	
	for( i=0; i<pPropertyCount; i++ ){
		if( pProperties[ i ]->GetType().Equals( type ) ){
			return true;
		}
	}
	
	return false;
}

void deSkinTexture::AddProperty( deSkinProperty *property ){
	if( ! property || HasPropertyWithType( property->GetType().GetString() ) ){
		DETHROW( deeInvalidParam );
	}
	
	if( pPropertyCount == pPropertySize ){
		int newSize = pPropertySize * 3 / 2 + 1;
		deSkinProperty **newArray = new deSkinProperty*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		
		if( pProperties ){
			memcpy( newArray, pProperties, sizeof( deSkinProperty* ) * pPropertySize );
			delete [] pProperties;
		}
		
		pProperties = newArray;
		pPropertySize = newSize;
	}
	
	pProperties[ pPropertyCount ] = property;
	pPropertyCount++;
}



// Private functions
//////////////////////

void deSkinTexture::pCleanUp(){
	if( pProperties ){
		while( pPropertyCount > 0 ){
			pPropertyCount--;
			delete pProperties[ pPropertyCount ];
		}
		
		delete [] pProperties;
	}
	
	if( pName ) delete [] pName;
}
