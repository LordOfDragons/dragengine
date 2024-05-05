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

#include <stdlib.h>

#include "meIDGroup.h"
#include "meIDGroupID.h"

#include <dragengine/common/exceptions.h>



// Class meIDGroup
////////////////////

// Constructor, destructor
////////////////////////////

meIDGroup::meIDGroup( const char *name ) :
pName( name ){
}

meIDGroup::~meIDGroup(){
	RemoveAll();
}



// Management
///////////////

int meIDGroup::GetCount() const{
	return pIDs.GetCount();
}

decStringList meIDGroup::GetIDList() const{
	return pIDs.GetKeys();
}

int meIDGroup::GetUsageCountFor( const char *id ) const{
	deObject *object;
	
	if( pIDs.GetAt( id, &object ) ){
		return ( ( meIDGroupID* )object )->GetUsageCount();
		
	}else{
		return 0;
	}
}

bool meIDGroup::Has( const char *id ) const{
	return pIDs.Has( id );
}

void meIDGroup::Add( const char *id ){
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	deObject *object;
	
	if( pIDs.GetAt( id, &object ) ){
		( ( meIDGroupID* )object )->Increment();
		return;
	}
	
	meIDGroupID *groupID = NULL;
	
	try{
		groupID = new meIDGroupID( id );
		pIDs.SetAt( id, groupID );
		groupID->FreeReference();
		
	}catch( const deException & ){
		if( groupID ){
			groupID->FreeReference();
		}
		throw;
	}
}

void meIDGroup::Remove( const char *id ){
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	deObject *object;
	
	if( ! pIDs.GetAt( id, &object ) ){
		DETHROW( deeInvalidParam );
	}
	
	meIDGroupID &groupID = *( ( meIDGroupID* )object );
	groupID.Decrement();
	if( groupID.GetUsageCount() == 0 ){
		pIDs.Remove( id );
	}
}

void meIDGroup::RemoveAll(){
	pIDs.RemoveAll();
}
