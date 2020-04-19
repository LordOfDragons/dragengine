/* 
 * Drag[en]gine GUI Launcher
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

#include "deglLoggerHistory.h"
#include "deglLoggerHistoryEntry.h"
#include "deglLoggerHistoryListener.h"

#include <dragengine/common/exceptions.h>



// Class deglLoggerHistory
////////////////////////////

// Constructor, destructor
////////////////////////////

deglLoggerHistory::deglLoggerHistory(){
	pHistorySize = 0;
	pEntries = NULL;
	pEntryPointer = 0;
	pEntryCount = 0;
	
	pLogInfo = true;
	pLogWarn = true;
	pLogError = true;
}

deglLoggerHistory::~deglLoggerHistory(){
	if( pEntries ){
		delete [] pEntries;
	}
	pListeners.RemoveAll();
}



// Management
///////////////

void deglLoggerHistory::SetHistorySize( int size ){
	if( size < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pMutex.Lock();
	
	deglLoggerHistoryEntry *newArray = NULL;
	
	try{
		if( size > 0 ){
			newArray = new deglLoggerHistoryEntry[ size ];
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

deglLoggerHistoryEntry &deglLoggerHistory::GetEntryAt( int index ){
	if( index < 0 || index >= pEntryCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pEntries[ ( pEntryPointer + index ) % pHistorySize ];
}

const deglLoggerHistoryEntry &deglLoggerHistory::GetEntryAt( int index ) const{
	if( index < 0 || index >= pEntryCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pEntries[ ( pEntryPointer + index ) % pHistorySize ];
}

deglLoggerHistoryEntry &deglLoggerHistory::AddEntry(){
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

void deglLoggerHistory::Clear(){
	pMutex.Lock();
	
	const int count = pListeners.GetCount();
	int i;
	
	pEntryPointer = 0;
	pEntryCount = 0;
	
	try{
		for( i=0; i<count; i++ ){
			( ( deglLoggerHistoryListener* )pListeners.GetAt( i ) )->HistoryCleared( this );
		}
		
	}catch( const deException & ){
		pMutex.Unlock();
		throw;
	}
	
	pMutex.Unlock();
}



bool deglLoggerHistory::CanAddMessage( int type, const char *source ){
	if( pHistorySize == 0 ){
		return false;
	}
	
	if( type == deglLoggerHistoryEntry::emtInfo && ! pLogInfo ){
		return false;
	}
	if( type == deglLoggerHistoryEntry::emtWarn && ! pLogWarn ){
		return false;
	}
	if( type == deglLoggerHistoryEntry::emtError && ! pLogError ){
		return false;
	}
	
	return true;
}



void deglLoggerHistory::AddListener( deglLoggerHistoryListener *listener ){
	if( ! listener ){
		DETHROW( deeInvalidParam );
	}
	
	pListeners.Add( listener );
}

void deglLoggerHistory::RemoveListener( deglLoggerHistoryListener *listener ){
	if( ! listener ){
		DETHROW( deeInvalidParam );
	}
	
	if( pListeners.Has( listener ) ){
		pListeners.RemoveFrom( pListeners.IndexOf( listener ) );
	}
}

void deglLoggerHistory::NotifyMessageAdded( deglLoggerHistoryEntry &entry ){
	const int count = pListeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( deglLoggerHistoryListener* )pListeners.GetAt( i ) )->MessageAdded( this, entry );
	}
}



void deglLoggerHistory::LogInfo( const char *source, const char *message ){
	if( ! source || ! message ){
		DETHROW( deeInvalidParam );
	}
	
	pMutex.Lock();
	
	try{
		if( CanAddMessage( deglLoggerHistoryEntry::emtInfo, source ) ){
			deglLoggerHistoryEntry &entry = AddEntry();
			entry.SetType( deglLoggerHistoryEntry::emtInfo );
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

void deglLoggerHistory::LogWarn( const char *source, const char *message ){
	if( ! source || ! message ){
		DETHROW( deeInvalidParam );
	}
	
	pMutex.Lock();
	
	try{
		if( CanAddMessage( deglLoggerHistoryEntry::emtWarn, source ) ){
			deglLoggerHistoryEntry &entry = AddEntry();
			entry.SetType( deglLoggerHistoryEntry::emtWarn );
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

void deglLoggerHistory::LogError( const char *source, const char *message ){
	if( ! source || ! message ){
		DETHROW( deeInvalidParam );
	}
	
	pMutex.Lock();
	
	try{
		if( CanAddMessage( deglLoggerHistoryEntry::emtError, source ) ){
			deglLoggerHistoryEntry &entry = AddEntry();
			entry.SetType( deglLoggerHistoryEntry::emtError );
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
