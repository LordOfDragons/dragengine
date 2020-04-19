/* 
 * Drag[en]gine OpenGL Graphic Module
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

// includes
#include <stdio.h>
#include <string.h>

#include "deoglCollideListManager.h"
#include "deoglCollideList.h"

#include <dragengine/common/exceptions.h>



// class deoglCollideListManager
///////////////////////////////////

// constructor, destructor
////////////////////////////

deoglCollideListManager::deoglCollideListManager(){
	pLists = NULL;
	pListCount = 0;
	pListSize = 0;
}

deoglCollideListManager::~deoglCollideListManager(){
	int i, inUseCount = 0;
	for( i=0; i<pListCount; i++ ){
		if( pLists[ i ].inUse ) inUseCount++;
	}
	if( inUseCount > 0 ){
		printf( "[OPENGL] WARNING! %i collide lists have not been released!\n", inUseCount );
	}
	if( pLists ){
		for( i=0; i<pListCount; i++ ){
			if( pLists[ i ].list ) delete pLists[ i ].list;
		}
		delete [] pLists;
	}
}



// list management
////////////////////

deoglCollideList *deoglCollideListManager::GetList(){
	int index = pFindNextList();
	if( index == -1 ){
		if( pListCount == pListSize ){
			int i, newSize = pListSize * 3 / 2 + 1;
			sList *newArray = new sList[ newSize ];
			if( ! newArray ) DETHROW( deeOutOfMemory );
			for( i=pListSize; i<newSize; i++ ){
				newArray[ i ].list = NULL;
				newArray[ i ].inUse = false;
			}
			if( pLists ){
				for( i=0; i<pListSize; i++ ){
					newArray[ i ].list = pLists[ i ].list;
					newArray[ i ].inUse = pLists[ i ].inUse;
				}
				delete [] pLists;
			}
			pLists = newArray;
			pListSize = newSize;
		}
		pLists[ pListCount ].list = new deoglCollideList;
		if( ! pLists[ pListCount ].list ) DETHROW( deeOutOfMemory );
		index = pListCount;
		pListCount++;
	}
	pLists[ index ].inUse = true;
	return pLists[ index ].list;
}

void deoglCollideListManager::ReleaseList( deoglCollideList *list ){
	if( ! list ) DETHROW( deeInvalidParam );
	int i;
	for( i=0; i<pListCount; i++ ){
		if( pLists[ i ].list == list ){
			if( ! pLists[ i ].inUse ) DETHROW( deeInvalidParam );
			pLists[ i ].inUse = false;
			break;
		}
	}
}



// private functions
//////////////////////

int deoglCollideListManager::pFindNextList() const{
	int i;
	for( i=0; i<pListCount; i++ ){
		if( ! pLists[ i ].inUse ) return i;
	}
	return -1;
}
