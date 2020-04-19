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

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "deoglEnvironmentMap.h"
#include "deoglEnvironmentMapList.h"

#include <dragengine/common/exceptions.h>



// Class deoglEnvironmentMapList
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglEnvironmentMapList::deoglEnvironmentMapList(){
}

deoglEnvironmentMapList::~deoglEnvironmentMapList(){
}



// Management
///////////////

deoglEnvironmentMap *deoglEnvironmentMapList::GetAt( int index ) const{
	return ( deoglEnvironmentMap* )pEnvMaps.GetAt( index );
}

int deoglEnvironmentMapList::GetCount() const{
	return pEnvMaps.GetCount();
}

int deoglEnvironmentMapList::IndexOf( deoglEnvironmentMap *envmap ) const{
	return pEnvMaps.IndexOf( envmap );
}

bool deoglEnvironmentMapList::Has( deoglEnvironmentMap *envmap ) const{
	return pEnvMaps.Has( envmap );
}

void deoglEnvironmentMapList::Add( deoglEnvironmentMap *envmap ){
	if( ! envmap || pEnvMaps.Has( envmap ) ){
		DETHROW( deeInvalidParam );
	}
	pEnvMaps.Add( envmap );
}

bool deoglEnvironmentMapList::AddIfMissing( deoglEnvironmentMap *envmap ){
	if( ! envmap ){
		DETHROW( deeInvalidParam );
	}
	
	if( pEnvMaps.Has( envmap ) ){
		return false;
		
	}else{
		pEnvMaps.Add( envmap );
		return true;
	}
}

void deoglEnvironmentMapList::Remove( deoglEnvironmentMap *envmap ){
	if( ! RemoveIfExisting( envmap ) ){
		DETHROW( deeInvalidParam );
	}
}

bool deoglEnvironmentMapList::RemoveIfExisting( deoglEnvironmentMap *envmap ){
	const int index = pEnvMaps.IndexOf( envmap );
	if( index == -1 ){
		return false;
	}
	
	const int count = pEnvMaps.GetCount();
	if( index < count - 1 ){
		pEnvMaps.SetAt( index, pEnvMaps.GetAt( count - 1 ) );
	}
	
	pEnvMaps.RemoveFrom( count - 1 );
	return true;
}

void deoglEnvironmentMapList::RemoveFrom( int index ){
	const int count = pEnvMaps.GetCount();
	if( index < 0 || index >= count ){
		DETHROW( deeInvalidParam );
	}
	
	if( index < count - 1 ){
		pEnvMaps.SetAt( index, pEnvMaps.GetAt( count - 1 ) );
	}
	pEnvMaps.RemoveFrom( count - 1 );
}

void deoglEnvironmentMapList::RemoveAll(){
	pEnvMaps.RemoveAll();
}
