/* 
 * Drag[en]gine IGDE Synthesizer Editor
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
#include <string.h>
#include <stdlib.h>

#include "seUSourceGroupPasteSource.h"
#include "../../../synthesizer/source/seSourceGroup.h"
#include "../../../synthesizer/seSynthesizer.h"
#include "../../../synthesizer/link/seLink.h"

#include <dragengine/common/exceptions.h>



// Class seUSourceGroupPasteSource
////////////////////////////////////

// Constructor, destructor
////////////////////////////

seUSourceGroupPasteSource::seUSourceGroupPasteSource(
seSourceGroup *group, const seSourceList &sourceList, int index ) :
pGroup( NULL ),
pIndex( index )
{
	const int sourceCount = sourceList.GetCount();
	
	if( ! group || sourceCount == 0 ){
		DETHROW( deeInvalidParam );
	}
	
	seSource *source = NULL;
	int i;
	
	try{
		for( i=0; i<sourceCount; i++ ){
			source = sourceList.GetAt( i )->CreateCopy();
			pSourceList.Add( source );
			source->FreeReference();
			source = NULL;
		}
		
	}catch( const deException & ){
		if( source ){
			source->FreeReference();
		}
		throw;
	}
	
	pGroup = group;
	group->AddReference();
}

seUSourceGroupPasteSource::~seUSourceGroupPasteSource(){
	if( pGroup ){
		pGroup->FreeReference();
	}
}



// Management
///////////////

void seUSourceGroupPasteSource::Undo(){
	const int sourceCount = pSourceList.GetCount();
	int i;
	
	for( i=0; i<sourceCount; i++ ){
		pGroup->RemoveSource( pSourceList.GetAt( i ) );
	}
	
	// remove links added in a prior redo
	seSynthesizer * const synthesizer = pGroup->GetSynthesizer();
	
	if( synthesizer ){
		const int linkCount = pRemoveLinkList.GetCount();
		for( i=0; i<linkCount; i++ ){
			synthesizer->RemoveLink( pRemoveLinkList.GetAt( i ) );
		}
		pRemoveLinkList.RemoveAll();
	}
	
	// remove controller added in a prior redo
	if( synthesizer ){
		const int controllerCount = pRemoveControllerList.GetCount();
		for( i=0; i<controllerCount; i++ ){
			synthesizer->RemoveController( pRemoveControllerList.GetAt( i ) );
		}
		pRemoveControllerList.RemoveAll();
	}
}

void seUSourceGroupPasteSource::Redo(){
	seSynthesizer * const synthesizer = pGroup->GetSynthesizer();
	const int sourceCount = pSourceList.GetCount();
	int i, j;
	
	pRemoveLinkList.RemoveAll();
	
	for( i=0; i<sourceCount; i++ ){
		seSource * const source = pSourceList.GetAt( i );
		
		// check if links exist in the synthesizer. if not add them and mark them to remove during undo
		if( synthesizer ){
			seLinkList linkList;
			source->ListLinks( linkList );
			
			const int linkCount = linkList.GetCount();
			for( j=0; j<linkCount; j++ ){
				seLink * const link = linkList.GetAt( j );
				if( ! synthesizer->GetLinks().Has( link ) ){
					seController * const controller = link->GetController();
					if( controller && ! synthesizer->GetControllers().Has( controller ) ){
						pRemoveControllerList.Add( controller );
						synthesizer->AddController( controller );
					}
					
					pRemoveLinkList.Add( link );
					synthesizer->AddLink( link );
				}
			}
		}
		
		// insert the source
		pGroup->InsertSourceAt( source, pIndex + i );
	}
}
