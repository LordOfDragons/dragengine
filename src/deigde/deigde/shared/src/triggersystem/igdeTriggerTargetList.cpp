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

#include "igdeTriggerTarget.h"
#include "igdeTriggerTargetList.h"

#include <dragengine/common/exceptions.h>



// Class igdeTriggerTargetList
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeTriggerTargetList::igdeTriggerTargetList(){
}

igdeTriggerTargetList::~igdeTriggerTargetList(){
}



// Management
///////////////

int igdeTriggerTargetList::GetCount() const{
	return pTargets.GetCount();
}

igdeTriggerTarget *igdeTriggerTargetList::GetAt( int position ) const{
	return ( igdeTriggerTarget* )pTargets.GetAt( position );
}

igdeTriggerTarget *igdeTriggerTargetList::GetNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pTargets.GetCount();
	igdeTriggerTarget *target;
	int i;
	
	for( i=0; i<count; i++ ){
		target = ( igdeTriggerTarget* )pTargets.GetAt( i );
		if( target->GetName().Equals( name ) ){
			return target;
		}
	}
	
	return NULL;
}

igdeTriggerTarget *igdeTriggerTargetList::GetNamedAddIfMissing( const char *name ){
	igdeTriggerTarget *target = GetNamed( name );
	
	if( ! target ){
		target = new igdeTriggerTarget( name );
		pTargets.Add( target );
		target->FreeReference();
	}
	
	return target;
}

int igdeTriggerTargetList::IndexOf( igdeTriggerTarget *target ) const{
	return pTargets.IndexOf( target );
}

bool igdeTriggerTargetList::Has( igdeTriggerTarget *target ) const{
	return pTargets.Has( target );
}

bool igdeTriggerTargetList::HasNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pTargets.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( igdeTriggerTarget* )pTargets.GetAt( i ) )->GetName().Equals( name ) ){
			return true;
		}
	}
	
	return false;
}

void igdeTriggerTargetList::Add( igdeTriggerTarget *target ){
	if( ! target || HasNamed( target->GetName().GetString() ) ){
		DETHROW( deeInvalidParam );
	}
	
	pTargets.Add( target );
}

void igdeTriggerTargetList::Remove( igdeTriggerTarget *target ){
	if( ! target ){
		DETHROW( deeInvalidParam );
	}
	
	pTargets.Remove( target );
}

void igdeTriggerTargetList::RemoveAll(){
	pTargets.RemoveAll();
}

void igdeTriggerTargetList::RemoveUnused(){
	const int count = pTargets.GetCount();
	int i;
	
	for( i=count-1; i>=0; i-- ){
		igdeTriggerTarget * const target = ( igdeTriggerTarget* )pTargets.GetAt( i );
		if( target->GetRefCount() == 1 ){
			pTargets.Remove( target );
		}
	}
}
