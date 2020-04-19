/* 
 * Drag[en]gine Android Launcher
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdlib.h>
#include <string.h>

#include "deLoggerBuffer.h"
#include "../common/exceptions.h"



// Class deLoggerBuffer::sEntry
/////////////////////////////////

deLoggerBuffer::sEntry::sEntry(){
}

deLoggerBuffer::sEntry::sEntry( const deLoggerBuffer::sEntry &entry ) :
type( entry.type ),
source( entry.source ),
message( entry.message ){
}

deLoggerBuffer::sEntry &deLoggerBuffer::sEntry::operator=( const deLoggerBuffer::sEntry &entry ){
	type = entry.type;
	source = entry.source;
	message = entry.message;
	return *this;
}



// Class deLoggerBuffer
/////////////////////////

// Constructor, destructor
////////////////////////////

deLoggerBuffer::deLoggerBuffer() :
pEntries( NULL ),
pEntrySize( 0 ),
pEntryCount( 0 ){
}

deLoggerBuffer::~deLoggerBuffer(){
	if( pEntries ){
		delete [] pEntries;
	}
}



// Management
///////////////

int deLoggerBuffer::GetEntryCount(){
	pMutex.Lock();
	const int count = pEntryCount;
	pMutex.Unlock();
	return count;
}

deLoggerBuffer::sEntry deLoggerBuffer::GetEntryAt( int index ){
	pMutex.Lock();
	
	if( index < 0 || index >= pEntryCount ){
		DETHROW( deeInvalidParam );
	}
	
	const sEntry entry( pEntries[ index ] );
	pMutex.Unlock();
	return entry;
}

void deLoggerBuffer::AddEntry( eMessageTypes type, const decString &source, const decString &message ){
	pMutex.Lock();
	
	try{
		pAddEntry( type, source, message );
		pMutex.Unlock();
		
	}catch( const deException & ){
		pMutex.Unlock();
		throw;
	}
}

void deLoggerBuffer::Clear(){
	pMutex.Lock();
	pClear();
	pMutex.Unlock();
}



void deLoggerBuffer::SendToLogger( deLogger &logger ){
	pMutex.Lock();
	
	try{
		int i;
		for( i=0; i<pEntryCount; i++ ){
			switch( pEntries[ i ].type ){
			case emtInfo:
				logger.LogInfo( pEntries[ i ].source, pEntries[ i ].message );
				break;
				
			case emtWarn:
				logger.LogWarn( pEntries[ i ].source, pEntries[ i ].message );
				break;
				
			case emtError:
				logger.LogError( pEntries[ i ].source, pEntries[ i ].message );
				break;
			}
		}
		
		pClear();
		pMutex.Unlock();
		
	}catch( const deException & ){
		pMutex.Unlock();
		throw;
	}
}



void deLoggerBuffer::LogInfo( const char *source, const char *message ){
	if( ! source || ! message ){
		DETHROW( deeInvalidParam );
	}
	
	pMutex.Lock();
	
	try{
		pAddEntry( emtInfo, source, message );
		pMutex.Unlock();
		
	}catch( const deException & ){
		pMutex.Unlock();
		throw;
	}
}

void deLoggerBuffer::LogWarn( const char *source, const char *message ){
	if( ! source || ! message ){
		DETHROW( deeInvalidParam );
	}
	
	pMutex.Lock();
	
	try{
		pAddEntry( emtWarn, source, message );
		pMutex.Unlock();
		
	}catch( const deException & ){
		pMutex.Unlock();
		throw;
	}
}

void deLoggerBuffer::LogError( const char *source, const char *message ){
	if( ! source || ! message ){
		DETHROW( deeInvalidParam );
	}
	
	pMutex.Lock();
	
	try{
		pAddEntry( emtError, source, message );
		pMutex.Unlock();
		
	}catch( const deException & ){
		pMutex.Unlock();
		throw;
	}
}



// Private Functions
//////////////////////

void deLoggerBuffer::pClear(){
	pEntryCount = 0;
}

void deLoggerBuffer::pAddEntry( eMessageTypes type, const decString &source, const decString &message ){
	if( pEntryCount == pEntrySize ){
		const int newSize = pEntrySize + 10;
		sEntry * const newArray = new sEntry[ newSize ];
		if( pEntries ){
			int i;
			for( i=0; i<pEntrySize; i++ ){
				newArray[ i ] = pEntries[ i ];
			}
			delete [] pEntries;
		}
		pEntries = newArray;
	}
	
	sEntry &entry = pEntries[ pEntryCount++ ];
	entry.type = type;
	entry.source = source;
	entry.message = message;
}
