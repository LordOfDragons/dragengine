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

#include "ceCoordSystem.h"
#include "ceCoordSystemList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class ceCoordSystem
////////////////////////

// Constructor, destructor
////////////////////////////

ceCoordSystemList::ceCoordSystemList(){
}

ceCoordSystemList::~ceCoordSystemList(){
}



// Management
///////////////

int ceCoordSystemList::GetCount() const{
	return pCoordSystems.GetCount();
}

ceCoordSystem *ceCoordSystemList::GetAt( int index ) const{
	return ( ceCoordSystem* )pCoordSystems.GetAt( index );
}

ceCoordSystem *ceCoordSystemList::GetWithID( const char *id ) const{
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	ceCoordSystem *coordSystem;
	const int count = pCoordSystems.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		coordSystem = ( ceCoordSystem* )pCoordSystems.GetAt( i );
		
		if( coordSystem->GetID().Equals( id ) ){
			return coordSystem;
		}
	}
	
	return NULL;
}

ceCoordSystem *ceCoordSystemList::GetWithAliasID( const char *id ) const{
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	ceCoordSystem *coordSystem;
	const int count = pCoordSystems.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		coordSystem = ( ceCoordSystem* )pCoordSystems.GetAt( i );
		
		if( ! coordSystem->GetAliasID().IsEmpty() && coordSystem->GetAliasID().Equals( id ) ){
			return coordSystem;
		}
	}
	
	return NULL;
}

ceCoordSystem *ceCoordSystemList::GetWithIDOrAliasID( const char *id ) const{
	ceCoordSystem *coordSystem = GetWithAliasID( id );
	
	if( ! coordSystem ){
		coordSystem = GetWithID( id );
	}
	
	return coordSystem;
}

int ceCoordSystemList::IndexOf( ceCoordSystem *coordSystem ) const{
	return pCoordSystems.IndexOf( coordSystem );
}

int ceCoordSystemList::IndexWithID( const char *id ) const{
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pCoordSystems.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const ceCoordSystem &coordSystem = *( ( ceCoordSystem* )pCoordSystems.GetAt( i ) );
		
		if( coordSystem.GetID().Equals( id ) ){
			return i;
		}
	}
	
	return -1;
}

int ceCoordSystemList::IndexWithAliasID( const char *id ) const{
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pCoordSystems.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const ceCoordSystem &coordSystem = *( ( ceCoordSystem* )pCoordSystems.GetAt( i ) );
		
		if( ! coordSystem.GetAliasID().IsEmpty() && coordSystem.GetAliasID().Equals( id ) ){
			return i;
		}
	}
	
	return -1;
}

int ceCoordSystemList::IndexWithIDOrAliasID( const char *id ) const{
	int index = IndexWithAliasID( id );
	
	if( index == -1 ){
		index = IndexWithID( id );
	}
	
	return index;
}

bool ceCoordSystemList::Has( ceCoordSystem *coordSystem ) const{
	return pCoordSystems.Has( coordSystem );
}

bool ceCoordSystemList::HasWithID( const char *id ) const{
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pCoordSystems.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const ceCoordSystem &coordSystem = *( ( ceCoordSystem* )pCoordSystems.GetAt( i ) );
		
		if( coordSystem.GetID().Equals( id ) ){
			return true;
		}
	}
	
	return false;
}

bool ceCoordSystemList::HasWithAliasID( const char *id ) const{
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pCoordSystems.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const ceCoordSystem &coordSystem = *( ( ceCoordSystem* )pCoordSystems.GetAt( i ) );
		
		if( ! coordSystem.GetAliasID().IsEmpty() && coordSystem.GetAliasID().Equals( id ) ){
			return true;
		}
	}
	
	return false;
}

bool ceCoordSystemList::HasWithIDOrAliasID( const char *id ) const{
	return HasWithAliasID( id ) || HasWithID( id );
}

void ceCoordSystemList::Add( ceCoordSystem *coordSystem ){
	if( ! coordSystem ) DETHROW( deeInvalidParam );
	
	pCoordSystems.Add( coordSystem );
}

void ceCoordSystemList::InsertAt( ceCoordSystem *coordSystem, int index ){
	if( ! coordSystem ) DETHROW( deeInvalidParam );
	
	pCoordSystems.Insert( coordSystem, index );
}

void ceCoordSystemList::MoveTo( ceCoordSystem *coordSystem, int index ){
	pCoordSystems.Move( coordSystem, index );
}

void ceCoordSystemList::Remove( ceCoordSystem *coordSystem ){
	pCoordSystems.Remove( coordSystem );
}

void ceCoordSystemList::RemoveAll(){
	pCoordSystems.RemoveAll();
}



ceCoordSystemList &ceCoordSystemList::operator=( const ceCoordSystemList &list ){
	pCoordSystems = list.pCoordSystems;
	return *this;
}
