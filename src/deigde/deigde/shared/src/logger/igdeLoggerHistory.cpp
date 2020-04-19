/* 
 * Drag[en]gine IGDE
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

#include "igdeLoggerHistory.h"
#include "igdeLoggerHistoryEntry.h"
#include "igdeLoggerHistoryListener.h"

#include <dragengine/common/exceptions.h>



// Class igdeLoggerHistory
////////////////////////////

// Constructor, destructor
////////////////////////////

igdeLoggerHistory::igdeLoggerHistory(){
	pHistorySize = 0;
	pEntries = NULL;
	pEntryPointer = 0;
	pEntryCount = 0;
	
	pLogInfo = true;
	pLogWarn = true;
	pLogError = true;
}

igdeLoggerHistory::~igdeLoggerHistory(){
	if( pEntries ){
		delete [] pEntries;
	}
	pListeners.RemoveAll();
}



// Management
///////////////

void igdeLoggerHistory::SetHistorySize( int size ){
	if( size < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pMutex.Lock();
	
	igdeLoggerHistoryEntry *newArray = NULL;
	
	try{
		if( size > 0 ){
			newArray = new igdeLoggerHistoryEntry[ size ];
		}
		
		if( pEntries ){
			delete [] pEntries;
		}
		
		pEntries = newArray;
		pHistorySize = size;
		
		pEntryPointer = 0;
		pEntryCount = 0;
		
	}catch( const deException & ){
		pMutex.Unlock();
		throw;
	}
	
	pMutex.Unlock();
}

igdeLoggerHistoryEntry &igdeLoggerHistory::GetEntryAt( int index ){
	if( index < 0 || index >= pEntryCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pEntries[ ( pEntryPointer + index ) % pHistorySize ];
}

const igdeLoggerHistoryEntry &igdeLoggerHistory::GetEntryAt( int index ) const{
	if( index < 0 || index >= pEntryCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pEntries[ ( pEntryPointer + index ) % pHistorySize ];
}

igdeLoggerHistoryEntry &igdeLoggerHistory::AddEntry(){
	if( pHistorySize == 0 ){
		DETHROW( deeInvalidParam );
	}
	
	const int position = ( pEntryPointer + pEntryCount ) % pHistorySize;
	
	if( pEntryCount == pHistorySize ){
		pEntryPointer = ( pEntryPointer + 1 ) % pHistorySize;
		
	}else{
		pEntryCount++;
	}
	
	return pEntries[ position ];
}

void igdeLoggerHistory::Clear(){
	pMutex.Lock();
	
	const int count = pListeners.GetCount();
	int i;
	
	pEntryPointer = 0;
	pEntryCount = 0;
	
	try{
		for( i=0; i<count; i++ ){
			( ( igdeLoggerHistoryListener* )pListeners.GetAt( i ) )->HistoryCleared( this );
		}
		
	}catch( const deException & ){
		pMutex.Unlock();
		throw;
	}
	
	pMutex.Unlock();
}



bool igdeLoggerHistory::CanAddMessage( int type, const char *source ){
	if( pHistorySize == 0 ){
		return false;
	}
	
	if( type == igdeLoggerHistoryEntry::emtInfo && ! pLogInfo ){
		return false;
	}
	if( type == igdeLoggerHistoryEntry::emtWarn && ! pLogWarn ){
		return false;
	}
	if( type == igdeLoggerHistoryEntry::emtError && ! pLogError ){
		return false;
	}
	
	return true;
}



void igdeLoggerHistory::AddListener( igdeLoggerHistoryListener *listener ){
	if( ! listener ){
		DETHROW( deeInvalidParam );
	}
	
	pListeners.Add( listener );
}

void igdeLoggerHistory::RemoveListener( igdeLoggerHistoryListener *listener ){
	if( ! listener ){
		DETHROW( deeInvalidParam );
	}
	
	pListeners.RemoveIfPresent( listener );
}

void igdeLoggerHistory::NotifyMessageAdded( igdeLoggerHistoryEntry &entry ){
	const int count = pListeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeLoggerHistoryListener* )pListeners.GetAt( i ) )->MessageAdded( this, entry );
	}
}



void igdeLoggerHistory::LogInfo( const char *source, const char *message ){
	if( ! source || ! message ){
		DETHROW( deeInvalidParam );
	}
	
	pMutex.Lock();
	
	try{
		if( CanAddMessage( igdeLoggerHistoryEntry::emtInfo, source ) ){
			igdeLoggerHistoryEntry &entry = AddEntry();
			entry.SetType( igdeLoggerHistoryEntry::emtInfo );
			entry.GetSource().Set( source );
			entry.GetMessage().Set( message );
			entry.CleanUpMessage();
			
			NotifyMessageAdded( entry );
		}
		
	}catch( const deException & ){
		pMutex.Unlock();
		throw;
	}
	
	pMutex.Unlock();
}

void igdeLoggerHistory::LogWarn( const char *source, const char *message ){
	if( ! source || ! message ){
		DETHROW( deeInvalidParam );
	}
	
	pMutex.Lock();
	
	try{
		if( CanAddMessage( igdeLoggerHistoryEntry::emtWarn, source ) ){
			igdeLoggerHistoryEntry &entry = AddEntry();
			entry.SetType( igdeLoggerHistoryEntry::emtWarn );
			entry.GetSource().Set( source );
			entry.GetMessage().Set( message );
			entry.CleanUpMessage();
			
			NotifyMessageAdded( entry );
		}
		
	}catch( const deException & ){
		pMutex.Unlock();
		throw;
	}
	
	pMutex.Unlock();
}

void igdeLoggerHistory::LogError( const char *source, const char *message ){
	if( ! source || ! message ){
		DETHROW( deeInvalidParam );
	}
	
	pMutex.Lock();
	
	try{
		if( CanAddMessage( igdeLoggerHistoryEntry::emtError, source ) ){
			igdeLoggerHistoryEntry &entry = AddEntry();
			entry.SetType( igdeLoggerHistoryEntry::emtError );
			entry.GetSource().Set( source );
			entry.GetMessage().Set( message );
			entry.CleanUpMessage();
			
			NotifyMessageAdded( entry );
		}
		
	}catch( const deException & ){
		pMutex.Unlock();
		throw;
	}
	
	pMutex.Unlock();
}
