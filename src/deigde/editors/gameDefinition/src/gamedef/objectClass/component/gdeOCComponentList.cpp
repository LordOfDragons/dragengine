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

#include "gdeOCComponent.h"
#include "gdeOCComponentList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeOCComponent
/////////////////////////

// Constructor, destructor
////////////////////////////

gdeOCComponentList::gdeOCComponentList(){
}

gdeOCComponentList::gdeOCComponentList( const gdeOCComponentList &list ) :
pComponents( list.pComponents ){
}

gdeOCComponentList::~gdeOCComponentList(){
}



// Management
///////////////

int gdeOCComponentList::GetCount() const{
	return pComponents.GetCount();
}

gdeOCComponent *gdeOCComponentList::GetAt( int index ) const{
	return ( gdeOCComponent* )pComponents.GetAt( index );
}

int gdeOCComponentList::IndexOf( gdeOCComponent *component ) const{
	return pComponents.IndexOf( component );
}

bool gdeOCComponentList::Has( gdeOCComponent *component ) const{
	return pComponents.Has( component );
}

void gdeOCComponentList::Add( gdeOCComponent *component ){
	if( ! component || Has( component ) ){
		DETHROW( deeInvalidParam );
	}
	
	pComponents.Add( component );
}

void gdeOCComponentList::Remove( gdeOCComponent *component ){
	pComponents.Remove( component );
}

void gdeOCComponentList::RemoveAll(){
	pComponents.RemoveAll();
}



gdeOCComponentList &gdeOCComponentList::operator=( const gdeOCComponentList &list ){
	pComponents = list.pComponents;
	return *this;
}
