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

#include "deoglPersistentRenderTaskOwner.h"
#include "../../../component/deoglRComponent.h"

#include <dragengine/common/exceptions.h>



// Class deoglPersistentRenderTaskOwner
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPersistentRenderTaskOwner::deoglPersistentRenderTaskOwner() :
pLLTask( this ),
pOwner( NULL ),
pHash( 0 ),
pComponent( NULL ){
}

deoglPersistentRenderTaskOwner::~deoglPersistentRenderTaskOwner(){
}



// Management
///////////////

void deoglPersistentRenderTaskOwner::SetOwner( void *owner, unsigned int hash ){
	pOwner = owner;
	pHash = hash;
}

void deoglPersistentRenderTaskOwner::SetExtends( const decDVector &minExtend, const decDVector &maxExtend ){
	pMinExtend = minExtend;
	pMaxExtend = maxExtend;
}



void deoglPersistentRenderTaskOwner::SetComponent( deoglRComponent *component ){
	pComponent = component;
}



int deoglPersistentRenderTaskOwner::GetInstanceCount() const{
	return pInstances.GetCount();
}

deoglPersistentRenderTaskInstance *deoglPersistentRenderTaskOwner::GetInstanceAt( int index ) const{
	return ( deoglPersistentRenderTaskInstance* )pInstances.GetAt( index );
}

void deoglPersistentRenderTaskOwner::AddInstance( deoglPersistentRenderTaskInstance *instance ){
	if( ! instance ){
		DETHROW( deeInvalidParam );
	}
	pInstances.Add( instance );
}

void deoglPersistentRenderTaskOwner::RemoveAllInstances(){
	pInstances.RemoveAll();
}



int deoglPersistentRenderTaskOwner::GetSubInstanceCount() const{
	return pSubInstances.GetCount();
}

deoglPersistentRenderTaskSubInstance *deoglPersistentRenderTaskOwner::GetSubInstanceAt( int index ) const{
	return ( deoglPersistentRenderTaskSubInstance* )pSubInstances.GetAt( index );
}

void deoglPersistentRenderTaskOwner::AddSubInstance( deoglPersistentRenderTaskSubInstance *subInstance ){
	if( ! subInstance ){
		DETHROW( deeInvalidParam );
	}
	pSubInstances.Add( subInstance );
}

void deoglPersistentRenderTaskOwner::RemoveAllSubInstances(){
	pSubInstances.RemoveAll();
}



void deoglPersistentRenderTaskOwner::Clear(){
	pOwner = NULL;
	pComponent = NULL;
	pInstances.RemoveAll();
	pSubInstances.RemoveAll();
}
