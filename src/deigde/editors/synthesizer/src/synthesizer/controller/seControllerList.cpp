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

#include "seController.h"
#include "seControllerList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class seController
///////////////////////

// Constructor, destructor
////////////////////////////

seControllerList::seControllerList(){
}

seControllerList::seControllerList( const seControllerList &copy ) :
pControllers( copy.pControllers ){
}

seControllerList::~seControllerList(){
}



// Management
///////////////

int seControllerList::GetCount() const{
	return pControllers.GetCount();
}

seController *seControllerList::GetAt( int index ) const{
	return ( seController* )pControllers.GetAt( index );
}

seController *seControllerList::GetNamed( const char *name ) const{
	const int count = pControllers.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		seController * const controller = ( seController* )pControllers.GetAt( i );
		if( controller->GetName() == name ){
			return controller;
		}
	}
	
	return NULL;
}

int seControllerList::IndexOf( seController *controller ) const{
	return pControllers.IndexOf( controller );
}

bool seControllerList::Has( seController *controller ) const{
	return pControllers.Has( controller );
}

bool seControllerList::HasNamed( const char *name ) const{
	return GetNamed( name ) != NULL;
}

void seControllerList::Add( seController *controller ){
	if( ! controller || HasNamed( controller->GetName() ) ){
		DETHROW( deeInvalidParam );
	}
	pControllers.Add( controller );
}

void seControllerList::Insert( seController *controller, int index ){
	if( ! controller || HasNamed( controller->GetName() ) ){
		DETHROW( deeInvalidParam );
	}
	pControllers.Insert( controller, index );
}

void seControllerList::Move( seController *controller, int index ){
	pControllers.Move( controller, index );
}

void seControllerList::Remove( seController *controller ){
	pControllers.Remove( controller );
}

void seControllerList::RemoveAll(){
	pControllers.RemoveAll();
}



// Operators
//////////////

seControllerList &seControllerList::operator=( const seControllerList &list ){
	pControllers = list.pControllers;
	return *this;
}

seControllerList &seControllerList::operator+=( const seControllerList &list ){
	pControllers += list.pControllers;
	return *this;
}
