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

void deoglShaderDefines::SetDefine( const char *name, const char *value ){
	if( ! name || name[ 0 ] == '\0' || ! value ){
		DETHROW( deeInvalidParam );
	}
	
	sDefine *define = nullptr;
	
	int i, len;
	for( i=0; i<pDefineCount; i++ ){
		if( strcmp( name, pDefines[ i ].name ) == 0 ){
			define = pDefines + i;
			if( define->value ){
				delete [] define->value;
				define->value = nullptr;
			}
			break;
		}
	}
	
	if( ! define ){
		sDefine * const newArray = new sDefine[ pDefineCount + 1 ];
		if( pDefines ){
			memcpy( newArray, pDefines, sizeof( sDefine ) * pDefineCount );
			delete [] pDefines;
		}
		pDefines = newArray;
		
		define = pDefines + pDefineCount;
		len = ( int )strlen( name );
		define->name = new char[ len + 1 ];
		strncpy_s( define->name, len, name, len );
		define->name[ len ] = 0;

		define->value = nullptr;
		
		pDefineCount++;
	}
	
	len = ( int )strlen( value );
	define->value = new char[ len + 1 ];
	strncpy_s( define->value, len, value, len );
	define->value[ len ] = 0;
}

void deoglShaderDefines::SetDefine( const char *name, int value ){
	char buffer[ 16 ];
	snprintf( &buffer[ 0 ], 16, "%d", value );
	SetDefine( name, buffer );
}

void deoglShaderDefines::SetDefine( const char *name, bool value ){
	SetDefine( name, value ? "1" : "0" );
}

void deoglShaderDefines::SetDefines( const char *name1 ){
	SetDefine( name1, true );
}

void deoglShaderDefines::SetDefines( const char *name1, const char *name2 ){
	SetDefine( name1, true );
	SetDefine( name2, true );
}

void deoglShaderDefines::SetDefines( const char *name1, const char *name2, const char *name3 ){
	SetDefine( name1, true );
	SetDefine( name2, true );
	SetDefine( name3, true );
}

void deoglShaderDefines::SetDefines( const char *name1, const char *name2, const char *name3, const char *name4 ){
	SetDefine( name1, true );
	SetDefine( name2, true );
	SetDefine( name3, true );
	SetDefine( name4, true );
}

void deoglShaderDefines::RemoveDefine( const char *name ){
	int i;
	for( i=0; i<pDefineCount; i++ ){
		if( strcmp( pDefines[ i ].name, name ) == 0 ){
			delete [] pDefines[ i ].name;
			delete [] pDefines[ i ].value;
			if( i < pDefineCount - 1 ){
				pDefines[ i ] = pDefines[ pDefineCount - 1 ];
			}
			pDefineCount--;
			return;
		}
	}
}

void deoglShaderDefines::RemoveDefines( const char *name1 ){
	RemoveDefine( name1 );
}

void deoglShaderDefines::RemoveDefines( const char *name1, const char *name2 ){
	RemoveDefine( name1 );
	RemoveDefine( name2 );
}

void deoglShaderDefines::RemoveDefines( const char *name1, const char *name2, const char *name3 ){
	RemoveDefine( name1 );
	RemoveDefine( name2 );
	RemoveDefine( name3 );
}

void deoglShaderDefines::RemoveDefines( const char *name1, const char *name2, const char *name3, const char *name4 ){
	RemoveDefine( name1 );
	RemoveDefine( name2 );
	RemoveDefine( name3 );
	RemoveDefine( name4 );
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
	
	RemoveAllDefines();
	
	if( count > 0 ){
		const char *name, *value;
		int len;

		try{
			pDefines = new sDefine[ count ];
			
			while( pDefineCount < count ){
				name = defines.pDefines[ pDefineCount ].name;
				value = defines.pDefines[ pDefineCount ].value;
				
				len = ( int )strlen( name );
				pDefines[ pDefineCount ].name = new char[ len + 1 ];
				strncpy_s( pDefines[ pDefineCount ].name, len, name, len );
				pDefines[ pDefineCount ].name[ len ] = 0;
				
				len = ( int )strlen( value );
				pDefines[ pDefineCount ].value = new char[ len + 1 ];
				strncpy_s( pDefines[ pDefineCount ].value, len, value, len );
				pDefines[ pDefineCount ].value[ len ] = 0;
				
				pDefineCount++;
			}
			
		}catch( const deException & ){
			pCleanUp();
			throw;
		}
	}
	
	return *this;
}

deoglShaderDefines deoglShaderDefines::operator+( const deoglShaderDefines &defines ) const{
	deoglShaderDefines combined( *this );
	int i;
	for( i=0; i<defines.pDefineCount; i++ ){
		combined.SetDefine( defines.pDefines[ i ].name, defines.pDefines[ i ].value );
	}
	return combined;
}



// Private functions
//////////////////////

void deoglShaderDefines::pCleanUp(){
	RemoveAllDefines();
}
