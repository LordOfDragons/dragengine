/* 
 * Drag[en]gine IGDE World Editor
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

#include "meCDOObject.h"
#include "meClipboardDataObject.h"
#include "../world/meWorld.h"
#include "../world/object/meObject.h"
#include "../world/object/meObjectSelection.h"

#include <dragengine/common/exceptions.h>



// Class meClipboardDataObject
////////////////////////////////

const char * const meClipboardDataObject::TYPE_NAME = "object";

// Constructor, destructor
////////////////////////////

meClipboardDataObject::meClipboardDataObject( const meWorld &world ) :
igdeClipboardData( TYPE_NAME )
{
	const meObjectList &list = world.GetSelectionObject().GetSelected();
	const int count = list.GetCount();
	
	if( count == 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pObjects = NULL;
	pObjectCount = 0;
	
	int i;
	
	try{
		pObjects = new meCDOObject*[ count ];
		while( pObjectCount < count ){
			meObject &object = *list.GetAt( pObjectCount );
			pObjects[ pObjectCount ] = new meCDOObject( object );
			
			if( object.GetAttachedTo() ){
				// if the object is attached to another copied object store the relative index
				for( i=0; i<count; i++ ){
					if( list.GetAt( i ) == object.GetAttachedTo() ){
						pObjects[ pObjectCount ]->SetAttachToIndex( i );
						break;
					}
				}
				
				// otherwise the object is attached to a not copied object.
				// in this case store the hex-string id of the object
				if( pObjects[ pObjectCount ]->GetAttachToIndex() == -1 ){
					pObjects[ pObjectCount ]->SetAttachToID(
						object.GetAttachedTo()->GetID().ToHexString() );
				}
			}
			
			pObjectCount++;
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

meClipboardDataObject::~meClipboardDataObject(){
	pCleanUp();
}



// Management
///////////////

meCDOObject *meClipboardDataObject::GetObjectAt( int index ) const{
	if( index < 0 || index >= pObjectCount ){
		DETHROW( deeOutOfBoundary );
	}
	return pObjects[ index ];
}



// Private Functions
//////////////////////

void meClipboardDataObject::pCleanUp(){
	if( pObjects ){
		while( pObjectCount > 0 ){
			if( pObjects[ pObjectCount - 1 ] ){
				delete pObjects[ pObjectCount - 1 ];
			}
			pObjectCount--;
		}
		delete [] pObjects;
	}
}
