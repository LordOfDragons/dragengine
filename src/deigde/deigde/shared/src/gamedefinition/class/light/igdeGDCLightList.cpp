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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "igdeGDCLight.h"
#include "igdeGDCLightList.h"

#include <dragengine/common/exceptions.h>



// Class igdeGDCLight
///////////////////////

// Constructor, destructor
////////////////////////////

igdeGDCLightList::igdeGDCLightList(){
}

igdeGDCLightList::igdeGDCLightList( const igdeGDCLightList &list ){
	pLights = list.pLights;
}

igdeGDCLightList::~igdeGDCLightList(){
}



// Management
///////////////

int igdeGDCLightList::GetCount() const{
	return pLights.GetCount();
}

igdeGDCLight *igdeGDCLightList::GetAt( int index ) const{
	return ( igdeGDCLight* )pLights.GetAt( index );
}

int igdeGDCLightList::IndexOf( igdeGDCLight *light ) const{
	return pLights.IndexOf( light );
}

bool igdeGDCLightList::Has( igdeGDCLight *light ) const{
	return pLights.Has( light );
}

void igdeGDCLightList::Add( igdeGDCLight *light ){
	if( ! light ){
		DETHROW( deeInvalidParam );
	}
	
	pLights.Add( light );
}

void igdeGDCLightList::InsertAt( igdeGDCLight *light, int index ){
	if( ! light ){
		DETHROW( deeInvalidParam );
	}
	
	pLights.Insert( light, index );
}

void igdeGDCLightList::MoveTo( igdeGDCLight *light, int index ){
	pLights.Move( light, index );
}

void igdeGDCLightList::Remove( igdeGDCLight *light ){
	pLights.Remove( light );
}

void igdeGDCLightList::RemoveAll(){
	pLights.RemoveAll();
}



void igdeGDCLightList::SetToDeepCopyFrom( const igdeGDCLightList &list ){
	const int count = list.GetCount();
	igdeGDCLight *light = NULL;
	
	int i;
	
	try{
		for( i=0; i<count; i++ ){
			light = new igdeGDCLight( *list.GetAt( i ) );
			Add( light );
			light->FreeReference();
			light = NULL;
		}
		
	}catch( const deException & ){
		if( light ){
			light->FreeReference();
		}
		throw;
	}
}



// Operators
//////////////

igdeGDCLightList &igdeGDCLightList::operator=( const igdeGDCLightList &list ){
	pLights = list.pLights;
	return *this;
}
