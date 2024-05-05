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
