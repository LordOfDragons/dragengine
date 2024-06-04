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

#include "deoglRLight.h"
#include "deoglLightList.h"

#include <dragengine/common/exceptions.h>



// Class deoglLightList
/////////////////////////

// Constructor, destructor
////////////////////////////

deoglLightList::deoglLightList(){
}

deoglLightList::deoglLightList( const deoglLightList &copy ) :
pLights( copy.pLights ){
}

deoglLightList::~deoglLightList(){
}



// Management
///////////////

int deoglLightList::GetCount() const{
	return pLights.GetCount();
}

deoglRLight *deoglLightList::GetAt( int index ) const{
	return ( deoglRLight* )pLights.GetAt( index );
}

bool deoglLightList::Has( deoglRLight *light ) const{
	return pLights.Has( light );
}

void deoglLightList::Add( deoglRLight *light ){
	pLights.Add( light );
}

bool deoglLightList::AddIfMissing( deoglRLight *light ){
	const int count = pLights.GetCount();
	pLights.AddIfAbsent( light );
	return pLights.GetCount() > count;
}

void deoglLightList::Remove( deoglRLight *light ){
	pLights.Remove( light );
}

bool deoglLightList::RemoveIfExisting( deoglRLight *light ){
	const int count = pLights.GetCount();
	pLights.RemoveIfPresent( light );
	return pLights.GetCount() < count;
}

void deoglLightList::RemoveAll(){
	pLights.RemoveAll();
}
