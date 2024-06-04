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

#include "gdeSkyController.h"
#include "gdeSkyControllerList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeSkyController
/////////////////

// Constructor, destructor
////////////////////////////

gdeSkyControllerList::gdeSkyControllerList(){
}

gdeSkyControllerList::gdeSkyControllerList( const gdeSkyControllerList &list ) :
pControllers( list.pControllers ){
}

gdeSkyControllerList::~gdeSkyControllerList(){
}



// Management
///////////////

int gdeSkyControllerList::GetCount() const{
	return pControllers.GetCount();
}

gdeSkyController *gdeSkyControllerList::GetAt( int index ) const{
	return ( gdeSkyController* )pControllers.GetAt( index );
}

gdeSkyController *gdeSkyControllerList::GetNamed( const char *name ) const{
	const int count = pControllers.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		gdeSkyController * const controller = ( gdeSkyController* )pControllers.GetAt( i );
		if( controller->GetName() == name ){
			return controller;
		}
	}
	
	return NULL;
}

int gdeSkyControllerList::IndexOf( gdeSkyController *controller ) const{
	return pControllers.IndexOf( controller );
}

bool gdeSkyControllerList::Has( gdeSkyController *controller ) const{
	return pControllers.Has( controller );
}

bool gdeSkyControllerList::HasNamed( const char *name ) const{
	return GetNamed( name ) != NULL;
}

void gdeSkyControllerList::Add( gdeSkyController *controller ){
	if( ! controller || HasNamed( controller->GetName() ) ){
		DETHROW( deeInvalidParam );
	}
	pControllers.Add( controller );
}

void gdeSkyControllerList::Remove( gdeSkyController *controller ){
	pControllers.Remove( controller );
}

void gdeSkyControllerList::RemoveAll(){
	pControllers.RemoveAll();
}



gdeSkyControllerList &gdeSkyControllerList::operator=( const gdeSkyControllerList &list ){
	pControllers = list.pControllers;
	return *this;
}
