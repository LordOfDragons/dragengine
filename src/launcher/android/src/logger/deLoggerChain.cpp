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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "deLoggerChain.h"
#include "../common/exceptions.h"



// Class deLoggerChain
////////////////////////

// Constructor, destructor
////////////////////////////

deLoggerChain::deLoggerChain(){
}

deLoggerChain::~deLoggerChain(){
	pLoggerList.RemoveAll();
}



// Management
///////////////

int deLoggerChain::GetLoggerCount(){
	int count;
	pMutex.Lock();
	count = pLoggerList.GetCount();
	pMutex.Unlock();
	return count;
}

deLogger *deLoggerChain::GetLoggerAt( int index ){
	deLogger *logger;
	
	pMutex.Lock();
	
	try{
		logger = ( deLogger* )pLoggerList.GetAt( index );
		pMutex.Unlock();
		
	}catch( const deException & ){
		pMutex.Unlock();
		throw;
	}
	
	return logger;
}

void deLoggerChain::AddLogger( deLogger *logger ){
	if( ! logger ){
		DETHROW( deeInvalidParam );
	}
	
	pMutex.Lock();
	
	try{
		pLoggerList.Add( logger );
		pMutex.Unlock();
		
	}catch( const deException & ){
		pMutex.Unlock();
		throw;
	}
}

void deLoggerChain::RemoveLogger( deLogger *logger ){
	pMutex.Lock();
	
	try{
		pLoggerList.Remove( logger );
		pMutex.Unlock();
		
	}catch( const deException & ){
		pMutex.Unlock();
		throw;
	}
}

void deLoggerChain::RemoveAllLoggers(){
	pMutex.Lock();
	
	try{
		pLoggerList.RemoveAll();
		pMutex.Unlock();
		
	}catch( const deException & ){
		pMutex.Unlock();
		throw;
	}
}



void deLoggerChain::LogInfo( const char *source, const char *message ){
	if( ! source || ! message ){
		DETHROW( deeInvalidParam );
	}
	
	pMutex.Lock();
	
	try{
		const int count = pLoggerList.GetCount();
		int i;
		for( i=0; i<count; i++ ){
			( ( deLogger* )pLoggerList.GetAt( i ) )->LogInfo( source, message );
		}
		
		pMutex.Unlock();
		
	}catch( const deException & ){
		pMutex.Unlock();
		throw;
	}
}

void deLoggerChain::LogWarn( const char *source, const char *message ){
	if( ! source || ! message ) DETHROW( deeInvalidParam );
	
	pMutex.Lock();
	
	try{
		const int count = pLoggerList.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			( ( deLogger* )pLoggerList.GetAt( i ) )->LogWarn( source, message );
		}
		
		pMutex.Unlock();
		
	}catch( const deException & ){
		pMutex.Unlock();
		throw;
	}
}

void deLoggerChain::LogError( const char *source, const char *message ){
	if( ! source || ! message ) DETHROW( deeInvalidParam );
	
	pMutex.Lock();
	
	try{
		const int count = pLoggerList.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			( ( deLogger* )pLoggerList.GetAt( i ) )->LogError( source, message );
		}
		
		pMutex.Unlock();
		
	}catch( const deException & ){
		pMutex.Unlock();
		throw;
	}
}
