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

#include "cePlaybackCommand.h"
#include "cePlaybackCommandList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class cePlaybackCommand
////////////////////////////

// Constructor, destructor
////////////////////////////

cePlaybackCommandList::cePlaybackCommandList(){
}

cePlaybackCommandList::cePlaybackCommandList( const cePlaybackCommandList &list ){
	*this = list;
}

cePlaybackCommandList::~cePlaybackCommandList(){
}



// Management
///////////////

int cePlaybackCommandList::GetCount() const{
	return pCommands.GetCount();
}

cePlaybackCommand *cePlaybackCommandList::GetAt( int index ) const{
	return ( cePlaybackCommand* )pCommands.GetAt( index );
}

cePlaybackCommand *cePlaybackCommandList::GetWith( const char *command ) const{
	const int count = pCommands.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		cePlaybackCommand * const entry = ( cePlaybackCommand* )pCommands.GetAt( i );
		
		if( entry->GetCommand().Equals( command ) ){
			return entry;
		}
	}
	
	return NULL;
}

int cePlaybackCommandList::IndexOf( cePlaybackCommand *command ) const{
	return pCommands.IndexOf( command );
}

int cePlaybackCommandList::IndexWith( const char *command ) const{
	const int count = pCommands.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const cePlaybackCommand &entry = *( ( cePlaybackCommand* )pCommands.GetAt( i ) );
		
		if( entry.GetCommand().Equals( command ) ){
			return i;
		}
	}
	
	return -1;
}

bool cePlaybackCommandList::Has( cePlaybackCommand *command ) const{
	return pCommands.Has( command );
}

bool cePlaybackCommandList::HasWith( const char *command ) const{
	return IndexWith( command ) != -1;
}

void cePlaybackCommandList::Add( cePlaybackCommand *command ){
	if( ! command || Has( command ) ){
		DETHROW( deeInvalidParam );
	}
	
	pCommands.Add( command );
}

void cePlaybackCommandList::Remove( cePlaybackCommand *command ){
	pCommands.Remove( command );
}

void cePlaybackCommandList::RemoveAll(){
	pCommands.RemoveAll();
}



cePlaybackCommandList &cePlaybackCommandList::operator=( const cePlaybackCommandList &list ){
	pCommands = list.pCommands;
	return *this;
}
