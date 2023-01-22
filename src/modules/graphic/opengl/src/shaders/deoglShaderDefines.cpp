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

deoglShaderDefines::deoglShaderDefines() :
pDefines( nullptr ),
pDefineCount( 0 ),
pDefineSize( 0 ){
}

deoglShaderDefines::deoglShaderDefines( const deoglShaderDefines &defines ) :
pDefines( nullptr ),
pDefineCount( 0 ),
pDefineSize( 0 )
{
	*this = defines;
}

deoglShaderDefines::~deoglShaderDefines(){
	pCleanUp();
}



// Management
///////////////

const decString &deoglShaderDefines::GetDefineNameAt( int index ) const{
	DEASSERT_TRUE( index >= 0 )
	DEASSERT_TRUE( index < pDefineCount )
	
	return pDefines[ index ].name;
}

const decString &deoglShaderDefines::GetDefineValueAt( int index ) const{
	DEASSERT_TRUE( index >= 0 )
	DEASSERT_TRUE( index < pDefineCount )
	
	return pDefines[ index ].value;
}

bool deoglShaderDefines::HasDefineNamed( const char *name ) const{
	int i;
	for( i=0; i<pDefineCount; i++ ){
		if( pDefines[ i ].name == name ){
			return true;
		}
	}
	
	return false;
}

const decString &deoglShaderDefines::GetDefineValueFor( const char *name, const decString &defaultValue ) const{
	int i;
	for( i=0; i<pDefineCount; i++ ){
		if( pDefines[ i ].name == name ){
			return pDefines[ i ].value;
		}
	}
	
	return defaultValue;
}

void deoglShaderDefines::SetDefine( const char *name, const char *value ){
	DEASSERT_NOTNULL( name )
	DEASSERT_NOTNULL( name[ 0 ] )
	DEASSERT_NOTNULL( value )
	
	sDefine *define = nullptr;
	
	int i;
	for( i=0; i<pDefineCount; i++ ){
		if( pDefines[ i ].name == name ){
			define = pDefines + i;
			break;
		}
	}
	
	if( ! define ){
		if( pDefineCount == pDefineSize ){
			const int newSize = pDefineSize * 3 / 2 + 1;
			sDefine * const newArray = new sDefine[ newSize ];
			if( pDefines ){
				for( i=0; i<pDefineSize; i++ ){
					newArray[ i ] = pDefines[ i ];
				}
				delete [] pDefines;
			}
			pDefines = newArray;
			pDefineSize = newSize;
		}
		
		define = pDefines + pDefineCount;
		define->name = name;
		pDefineCount++;
	}
	
	define->value = value;
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
		if( pDefines[ i ].name == name ){
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
	pDefineCount = 0;
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
			if( pDefines[ i ].name == defines.pDefines[ j ].name ){
				if( pDefines[ i ].value != defines.pDefines[ j ].value ){
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
	pDefineCount = 0;
	
	int i;
	for( i=0; i<defines.pDefineCount; i++ ){
		SetDefine( defines.pDefines[ i ].name, defines.pDefines[ i ].value );
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
	if( pDefines ){
		delete [] pDefines;
	}
}
