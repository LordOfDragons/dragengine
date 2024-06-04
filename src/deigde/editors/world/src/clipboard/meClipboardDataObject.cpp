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
