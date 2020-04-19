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
#include <string.h>

#include "deoglShaderDefines.h"

#include <dragengine/common/exceptions.h>



// Class deoglShaderDefines
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglShaderDefines::deoglShaderDefines(){
	pDefines = NULL;
	pDefineCount = 0;
}

deoglShaderDefines::deoglShaderDefines( const deoglShaderDefines &defines ){
	pDefines = NULL;
	pDefineCount = 0;
	*this = defines;
}

deoglShaderDefines::~deoglShaderDefines(){
	pCleanUp();
}



// Management
///////////////

const char *deoglShaderDefines::GetDefineNameAt( int index ) const{
	if( index < 0 || index >= pDefineCount ){
		DETHROW( deeInvalidParam );
	}
	
	return ( const char * )pDefines[ index ].name;
}

const char *deoglShaderDefines::GetDefineValueAt( int index ) const{
	if( index < 0 || index >= pDefineCount ){
		DETHROW( deeInvalidParam );
	}
	
	return ( const char * )pDefines[ index ].value;
}

bool deoglShaderDefines::HasDefineNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	
	for( i=0; i<pDefineCount; i++ ){
		if( strcmp( name, pDefines[ i ].name ) == 0 ){
			return true;
		}
	}
	
	return false;
}

const char *deoglShaderDefines::GetDefineValueFor( const char *name, const char *defaultValue ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	
	for( i=0; i<pDefineCount; i++ ){
		if( strcmp( name, pDefines[ i ].name ) == 0 ){
			return ( const char * )pDefines[ i ].value;
		}
	}
	
	return defaultValue;
}

void deoglShaderDefines::AddDefine( const char *name, const char *value ){
	if( ! name || name[ 0 ] == '\0' || ! value || HasDefineNamed( name ) ){
		DETHROW( deeInvalidParam );
	}
	
	sDefine * const newArray = new sDefine[ pDefineCount + 1 ];
	
	if( pDefines ){
		memcpy( newArray, pDefines, sizeof( sDefine ) * pDefineCount );
		delete [] pDefines;
	}
	pDefines = newArray;
	
	pDefines[ pDefineCount ].name = new char[ strlen( name ) + 1 ];
	strcpy( pDefines[ pDefineCount ].name, name );
	pDefines[ pDefineCount ].value = new char[ strlen( value ) + 1 ];
	strcpy( pDefines[ pDefineCount ].value, value );
	pDefineCount++;
}

void deoglShaderDefines::RemoveAllDefines(){
	if( pDefines ){
		while( pDefineCount > 0 ){
			pDefineCount--;
			if( pDefines[ pDefineCount ].name ){
				delete [] pDefines[ pDefineCount ].name;
			}
			if( pDefines[ pDefineCount ].value ){
				delete [] pDefines[ pDefineCount ].value;
			}
		}
		
		delete [] pDefines;
		pDefines = NULL;
	}
}

bool deoglShaderDefines::Equals( const deoglShaderDefines &defines ) const{
	return *this == defines;
}



// Operators
//////////////

bool deoglShaderDefines::operator==( const deoglShaderDefines &defines ) const{
	if( defines.pDefineCount != pDefineCount ){
		return false;
	}
	
	int i, j;
	
	for( i=0; i<pDefineCount; i++ ){
		for( j=0; j<pDefineCount; j++ ){
			if( strcmp( pDefines[ i ].name, defines.pDefines[ j ].name ) == 0 ){
				if( strcmp( pDefines[ i ].value, defines.pDefines[ j ].value ) != 0 ){
					return false;
				}
				break;
			}
		}
		if( j == pDefineCount ){
			return false;
		}
	}
	
	return true;
}

deoglShaderDefines &deoglShaderDefines::operator=( const deoglShaderDefines &defines ){
	const int count = defines.pDefineCount;
	const char *name, *value;
	
	RemoveAllDefines();
	
	if( count > 0 ){
		try{
			pDefines = new sDefine[ count ];
			
			while( pDefineCount < count ){
				name = defines.pDefines[ pDefineCount ].name;
				value = defines.pDefines[ pDefineCount ].value;
				
				pDefines[ pDefineCount ].name = new char[ strlen( name ) + 1 ];
				strcpy( pDefines[ pDefineCount ].name, name );
				
				pDefines[ pDefineCount ].value = new char[ strlen( value ) + 1 ];
				strcpy( pDefines[ pDefineCount ].value, value );
				
				pDefineCount++;
			}
			
		}catch( const deException & ){
			pCleanUp();
			throw;
		}
	}
	
	return *this;
}



// Private functions
//////////////////////

void deoglShaderDefines::pCleanUp(){
	RemoveAllDefines();
}
