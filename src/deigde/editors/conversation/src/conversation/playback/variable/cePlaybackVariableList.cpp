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

#include "cePlaybackVariable.h"
#include "cePlaybackVariableList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class cePlaybackVariable
/////////////////////////////

// Constructor, destructor
////////////////////////////

cePlaybackVariableList::cePlaybackVariableList(){
}

cePlaybackVariableList::~cePlaybackVariableList(){
}



// Management
///////////////

int cePlaybackVariableList::GetCount() const{
	return pVariables.GetCount();
}

cePlaybackVariable *cePlaybackVariableList::GetAt( int index ) const{
	return ( cePlaybackVariable* )pVariables.GetAt( index );
}

cePlaybackVariable *cePlaybackVariableList::GetNamed( const char *name ) const{
	const int count = pVariables.GetCount();
	cePlaybackVariable *variable;
	int i;
	
	for( i=0; i<count; i++ ){
		variable = ( cePlaybackVariable* )pVariables.GetAt( i );
		
		if( variable->GetName().Equals( name ) ){
			return variable;
		}
	}
	
	return NULL;
}

int cePlaybackVariableList::IndexOf( cePlaybackVariable *variable ) const{
	return pVariables.IndexOf( variable );
}

int cePlaybackVariableList::IndexOfNamed( const char *name ) const{
	const int count = pVariables.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( cePlaybackVariable* )pVariables.GetAt( i ) )->GetName().Equals( name ) ){
			return i;
		}
	}
	
	return -1;
}

bool cePlaybackVariableList::Has( cePlaybackVariable *variable ) const{
	return pVariables.Has( variable );
}

bool cePlaybackVariableList::HasNamed( const char *name ) const{
	const int count = pVariables.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( cePlaybackVariable* )pVariables.GetAt( i ) )->GetName().Equals( name ) ){
			return true;
		}
	}
	
	return false;
}

void cePlaybackVariableList::Set( const char *name, int value ){
	cePlaybackVariable *variable = GetNamed( name );
	
	if( variable ){
		variable->SetValue( value );
		
	}else{
		variable = new cePlaybackVariable( name, value );
		
		pVariables.Add( variable );
		variable->FreeReference();
	}
}

void cePlaybackVariableList::Remove( cePlaybackVariable *variable ){
	pVariables.Remove( variable );
}

void cePlaybackVariableList::RemoveAll(){
	pVariables.RemoveAll();
}



cePlaybackVariableList &cePlaybackVariableList::operator=( const cePlaybackVariableList &list ){
	pVariables = list.pVariables;
	return *this;
}
