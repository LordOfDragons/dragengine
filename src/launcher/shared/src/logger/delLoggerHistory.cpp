/* 
 * Drag[en]gine Launcher Shared
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include "delLoggerHistory.h"
#include "delLoggerHistoryEntry.h"
#include "delLoggerHistoryListener.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/threading/deMutexGuard.h>



// Class delLoggerHistory
///////////////////////////

// Constructor, destructor
////////////////////////////

delLoggerHistory::delLoggerHistory( int size ) :
pHistorySize( 0 ),
pEntries( nullptr ),
pEntryCount( 0 ),
pEntryOffset( 0 ),
pLogInfo( true ),
pLogWarn( true ),
pLogError( true )
{
	if( size < 0 ){
		DETHROW_INFO( deeInvalidParam, "size < 0" );
	}
	
	if( size > 0 ){
		pEntries = new delLoggerHistoryEntry[ size ];
		pHistorySize = size;
	}
}

delLoggerHistory::~delLoggerHistory(){
	if( pEntries ){
		delete [] pEntries;
	}
}



// Management
///////////////

void delLoggerHistory::SetHistorySize( int size ){
	if( size < 0 ){
		DETHROW_INFO( deeInvalidParam, "size < 0" );
	}
	
	Clear();
	
	const deMutexGuard guard( pMutex );
	
	if( pEntries ){
		delete [] pEntries;
		pEntries = nullptr;
		pHistorySize = 0;
	}
	pEntryCount = 0;
	pEntryOffset = 0;
	
	if( size == 0 ){
		return;
	}
	
	pEntries = new delLoggerHistoryEntry[ size ];
	pHistorySize = size;
}

delLoggerHistoryEntry &delLoggerHistory::GetEntryAt( int index ){
	if( index < 0 ){
		DETHROW_INFO( deeInvalidParam, "index < 0" );
	}
	if( index >= pEntryCount ){
		DETHROW_INFO( deeInvalidParam, "index >= entryCount" );
	}
	
	return pEntries[ ( pEntryOffset + index ) % pHistorySize ];
}

const delLoggerHistoryEntry &delLoggerHistory::GetEntryAt( int index ) const{
	if( index < 0 ){
		DETHROW_INFO( deeInvalidParam, "index < 0" );
	}
	if( index >= pEntryCount ){
		DETHROW_INFO( deeInvalidParam, "index >= entryCount" );
	}
	
	return pEntries[ ( pEntryOffset + index ) % pHistorySize ];
}

delLoggerHistoryEntry &delLoggerHistory::AddEntry(){
	if( pHistorySize == 0 ){
		DETHROW_INFO( deeInvalidParam, "history size is 0" );
	}
	
	const int position = ( pEntryOffset + pEntryCount ) % pHistorySize;
	
	if( pEntryCount == pHistorySize ){
		pEntryOffset = ( pEntryOffset + 1 ) % pHistorySize;
		
	}else{
		pEntryCount++;
	}
	
	pEntries[ position ].Clear();
	return pEntries[ position ];
}

void delLoggerHistory::Clear(){
	const deMutexGuard guard( pMutex );
	
	pEntryOffset = 0;
	pEntryCount = 0;
	
	const int count = pListeners.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( delLoggerHistoryListener* )pListeners.GetAt( i ) )->HistoryCleared( *this );
	}
}



bool delLoggerHistory::CanAddMessage( int type, const char *source ){
	if( pHistorySize == 0 ){
		return false;
	}
	if( type == delLoggerHistoryEntry::emtInfo && ! pLogInfo ){
		return false;
	}
	if( type == delLoggerHistoryEntry::emtWarn && ! pLogWarn ){
		return false;
	}
	if( type == delLoggerHistoryEntry::emtError && ! pLogError ){
		return false;
	}
	return true;
}



void delLoggerHistory::AddListener( delLoggerHistoryListener *listener ){
	if( ! listener ){
		DETHROW_INFO( deeNullPointer, "listener" );
	}
	
	const deMutexGuard guard( pMutex );
	pListeners.Add( listener );
}

void delLoggerHistory::RemoveListener( delLoggerHistoryListener *listener ){
	if( ! listener ){
		DETHROW_INFO( deeNullPointer, "listener" );
	}
	
	const deMutexGuard guard( pMutex );
	pListeners.RemoveIfPresent( listener );
}

void delLoggerHistory::NotifyMessageAdded( const delLoggerHistoryEntry &entry ){
	const int count = pListeners.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( delLoggerHistoryListener* )pListeners.GetAt( i ) )->MessageAdded( *this, entry );
	}
}



void delLoggerHistory::LogInfo( const char *source, const char *message ){
	if( ! source ){
		DETHROW_INFO( deeNullPointer, "source" );
	}
	if( ! message ){
		DETHROW_INFO( deeNullPointer, "message" );
	}
	
	const deMutexGuard guard( pMutex );
	
	if( ! CanAddMessage( delLoggerHistoryEntry::emtInfo, source ) ){
		return;
	}
	
	delLoggerHistoryEntry &entry = AddEntry();
	entry.SetType( delLoggerHistoryEntry::emtInfo );
	entry.SetSource( source );
	entry.SetMessage( message );
	entry.CleanUpMessage();
	
	NotifyMessageAdded( entry );
}

void delLoggerHistory::LogWarn( const char *source, const char *message ){
	if( ! source ){
		DETHROW_INFO( deeNullPointer, "source" );
	}
	if( ! message ){
		DETHROW_INFO( deeNullPointer, "message" );
	}
	
	const deMutexGuard guard( pMutex );
	
	if( ! CanAddMessage( delLoggerHistoryEntry::emtWarn, source ) ){
		return;
	}
	
	delLoggerHistoryEntry &entry = AddEntry();
	entry.SetType( delLoggerHistoryEntry::emtWarn );
	entry.SetSource( source );
	entry.SetMessage( message );
	entry.CleanUpMessage();
	
	NotifyMessageAdded( entry );
}

void delLoggerHistory::LogError( const char *source, const char *message ){
	if( ! source ){
		DETHROW_INFO( deeNullPointer, "source" );
	}
	if( ! message ){
		DETHROW_INFO( deeNullPointer, "message" );
	}
	
	const deMutexGuard guard( pMutex );
	
	if( ! CanAddMessage( delLoggerHistoryEntry::emtError, source ) ){
		return;
	}
	
	delLoggerHistoryEntry &entry = AddEntry();
	entry.SetType( delLoggerHistoryEntry::emtError );
	entry.SetSource( source );
	entry.SetMessage( message );
	entry.CleanUpMessage();
	
	NotifyMessageAdded( entry );
}
