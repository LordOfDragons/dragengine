/* 
 * Drag[en]gine IGDE Rig Editor
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
#include "reCLHitList.h"
#include "reCLHitListEntry.h"
#include "dragengine/common/exceptions.h"



// Class reCLHitList
//////////////////////

// Constructor, destructor
////////////////////////////

reCLHitList::reCLHitList(){
	pEntries = NULL;
	pEntryCount = 0;
	pEntrySize = 0;
}

reCLHitList::~reCLHitList(){
	RemoveAllEntries();
	if( pEntries ) delete [] pEntries;
}



// Management
///////////////

reCLHitListEntry *reCLHitList::GetEntryAt( int index ) const{
	if( index < 0 || index >= pEntryCount ) DETHROW( deeInvalidParam );
	
	return pEntries[ index ];
}

int reCLHitList::IndexOfEntry( reCLHitListEntry *entry ) const{
	if( ! entry ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pEntryCount; i++ ){
		if( entry == pEntries[ i ] ) return i;
	}
	
	return -1;
}

bool reCLHitList::HasEntry( reCLHitListEntry *entry ) const{
	if( ! entry ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pEntryCount; i++ ){
		if( entry == pEntries[ i ] ) return true;
	}
	
	return false;
}

void reCLHitList::AddEntry( reCLHitListEntry *entry ){
	if( HasEntry( entry ) ) DETHROW( deeInvalidParam );
	
	if( pEntryCount == pEntrySize ){
		int newSize = pEntryCount * 3 / 2 + 1;
		reCLHitListEntry **newArray = new reCLHitListEntry*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pEntries ){
			memcpy( newArray, pEntries, sizeof( reCLHitListEntry* ) * pEntrySize );
			delete [] pEntries;
		}
		pEntries = newArray;
		pEntrySize = newSize;
	}
	
	pEntries[ pEntryCount ] = entry;
	pEntryCount++;
}

void reCLHitList::RemoveEntry( reCLHitListEntry *entry ){
	int i, index = IndexOfEntry( entry );
	if( index == -1 ) DETHROW( deeInvalidParam );
	
	for( i=index+1; i<pEntryCount; i++ ){
		pEntries[ i - 1 ] = pEntries[ i ];
	}
	pEntryCount--;
	
	delete entry;
}

void reCLHitList::RemoveAllEntries(){
	while( pEntryCount > 0 ){
		pEntryCount--;
		delete pEntries[ pEntryCount ];
	}
}



void reCLHitList::SortByDistance(){
	reCLHitListEntry *exchange;
	int index = 1;
	
	while( index < pEntryCount ){
		if( pEntries[ index - 1 ]->CompareTo( *pEntries[ index ] ) > 0 ){
			exchange = pEntries[ index - 1 ];
			pEntries[ index - 1 ] = pEntries[ index ];
			pEntries[ index ] = exchange;
			
			if( index > 1 ){
				index--;
				
			}else{
				index++;
			}
		
		}else{
			index++;
		}
	}
}
