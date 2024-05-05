/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
