/* 
 * Drag[en]gine Shared Vulkan
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include <stdlib.h>
#include <string.h>

#include "devkSpecialization.h"

#include <dragengine/common/exceptions.h>


// class devkSpecialization
/////////////////////////////

devkSpecialization::devkSpecialization() :
pEntries( nullptr ),
pEntryCount( 0 ),
pData( nullptr ),
pDataSize( 0 ){
}

devkSpecialization::devkSpecialization( const void* data, int dataSize, int entryCount ) :
pEntries( nullptr ),
pEntryCount( 0 ),
pData( nullptr ),
pDataSize( 0 )
{
	try{
		SetEntryCount( entryCount );
		SetData( data, dataSize );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

devkSpecialization::devkSpecialization( const devkSpecialization &configuration ) :
pEntries( nullptr ),
pEntryCount( 0 ),
pData( nullptr ),
pDataSize( 0 )
{
	*this = configuration;
}

devkSpecialization::~devkSpecialization(){
	pCleanUp();
}



// Management
///////////////

void devkSpecialization::SetEntryCount( int count ){
	if( count < 0 ){
		DETHROW_INFO( deeInvalidParam, "count < 0" );
	}
	
	if( pEntries ){
		delete [] pEntries;
		pEntries = nullptr;
		pEntryCount = 0;
	}
	
	if( count == 0 ){
		return;
	}
	
	pEntries = new VkSpecializationMapEntry[ count ];
	memset( pEntries, 0, sizeof( VkSpecializationMapEntry ) * count );
	pEntryCount = count;
}

void devkSpecialization::SetEntryAt( int index, const VkSpecializationMapEntry &entry ){
	if( index < 0 ){
		DETHROW_INFO( deeInvalidParam, "index < 0" );
	}
	if( index >= pEntryCount ){
		DETHROW_INFO( deeInvalidParam, "index >= entryCount" );
	}
	if( entry.constantID < 0 ){
		DETHROW_INFO( deeInvalidParam, "entry.constantID < 0" );
	}
	if( entry.offset < 0 ){
		DETHROW_INFO( deeInvalidParam, "entry.offset < 0" );
	}
	if( entry.size < 0 ){
		DETHROW_INFO( deeInvalidParam, "entry.size < 0" );
	}
	
	pEntries[ index ] = entry;
}

void devkSpecialization::SetEntryAt( int index, int constantID, int offset, int size ){
	VkSpecializationMapEntry entry;
	entry.constantID = constantID;
	entry.offset = offset;
	entry.size = size;
	SetEntryAt( index, entry );
}

void devkSpecialization::SetEntryIntAt( int index, int constantID, int offset ){
	SetEntryAt( index, constantID, offset, sizeof( int32_t ) );
}

void devkSpecialization::SetEntryUIntAt( int index, int constantID, int offset ){
	SetEntryAt( index, constantID, offset, sizeof( uint32_t ) );
}

void devkSpecialization::SetEntryFloatAt( int index, int constantID, int offset ){
	SetEntryAt( index, constantID, offset, sizeof( float ) );
}

void devkSpecialization::SetEntryBoolAt( int index, int constantID, int offset ){
	SetEntryAt( index, constantID, offset, sizeof( bool ) );
}

void devkSpecialization::SetData( const void *data, int size ){
	if( size < 0 ){
		DETHROW_INFO( deeInvalidParam, "size < 0" );
	}
	if( size > 0 && ! data ){
		DETHROW_INFO( deeNullPointer, "data" );
	}
	
	if( pData ){
		delete [] pData;
		pData = nullptr;
		pDataSize = 0;
	}
	
	if( size == 0 ){
		return;
	}
	
	pData = new char[ size ];
	memcpy( pData, data, size );
	pDataSize = size;
}



// Operators
//////////////

bool devkSpecialization::operator==( const devkSpecialization &configuration ) const{
	if( pEntryCount != configuration.pEntryCount
	|| pDataSize != configuration.pDataSize
	|| ( pData && memcmp( pData, configuration.pData, pDataSize ) != 0 ) ){
		return false;
	}
	
	int i;
	for( i=0; i<pEntryCount; i++ ){
		if( pEntries[ i ].constantID != configuration.pEntries[ i ].constantID
		|| pEntries[ i ].offset != configuration.pEntries[ i ].offset
		|| pEntries[ i ].size != configuration.pEntries[ i ].size ){
			return false;
		}
	}
	return true;
}

devkSpecialization &devkSpecialization::operator=(
const devkSpecialization &configuration ){
	SetEntryCount( configuration.pEntryCount );
	if( configuration.pEntryCount > 0 ){
		memcpy( pEntries, configuration.pEntries,
			sizeof( VkSpecializationMapEntry ) * configuration.pEntryCount );
	}
	SetData( configuration.GetData(), configuration.GetDataSize() );
	return *this;
}



// Private Functions
//////////////////////

void devkSpecialization::pCleanUp(){
	if( pData ){
		delete [] pData;
	}
	if( pEntries ){
		delete [] pEntries;
	}
}
