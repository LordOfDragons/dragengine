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

#include "seUPasteSource.h"
#include "../../synthesizer/seSynthesizer.h"
#include "../../synthesizer/source/seSource.h"
#include "../../synthesizer/link/seLink.h"

#include <dragengine/common/exceptions.h>



// Class seUPasteSource
/////////////////////////

// Constructor, destructor
////////////////////////////

seUPasteSource::seUPasteSource( seSynthesizer *synthesizer, const seSourceList &sourceList, int index ) :
pSynthesizer( NULL ),
pIndex( index )
{
	const int sourceCount = sourceList.GetCount();
	
	if( ! synthesizer || sourceCount == 0 ){
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
	
	pSynthesizer = synthesizer;
	synthesizer->AddReference();
}

seUPasteSource::~seUPasteSource(){
	pCleanUp();
}



// Management
///////////////

void seUPasteSource::Undo(){
	const int sourceCount = pSourceList.GetCount();
	int i;
	
	// remove the sources
	for( i=0; i<sourceCount; i++ ){
		pSynthesizer->RemoveSource( pSourceList.GetAt( i ) );
	}
	
	// remove links added in a prior redo
	const int linkCount = pRemoveLinkList.GetCount();
	for( i=0; i<linkCount; i++ ){
		pSynthesizer->RemoveLink( pRemoveLinkList.GetAt( i ) );
	}
	pRemoveLinkList.RemoveAll();
	
	// remove controller added in a prior redo
	const int controllerCount = pRemoveControllerList.GetCount();
	for( i=0; i<controllerCount; i++ ){
		pSynthesizer->RemoveController( pRemoveControllerList.GetAt( i ) );
	}
	pRemoveControllerList.RemoveAll();
}

void seUPasteSource::Redo(){
	const int sourceCount = pSourceList.GetCount();
	int i, j;
	
	pRemoveLinkList.RemoveAll();
	pRemoveControllerList.RemoveAll();
	
	for( i=0; i<sourceCount; i++ ){
		seSource * const source = pSourceList.GetAt( i );
		
		// check if links exist in the synthesizer. if not add them and mark them to remove during undo
		seLinkList linkList;
		source->ListLinks( linkList );
		
		const int linkCount = linkList.GetCount();
		for( j=0; j<linkCount; j++ ){
			seLink * const link = linkList.GetAt( j );
			
			if( ! pSynthesizer->GetLinks().Has( link ) ){
				seController * const controller = link->GetController();
				if( controller && ! pSynthesizer->GetControllers().Has( controller ) ){
					pRemoveControllerList.Add( controller );
					pSynthesizer->AddController( controller );
				}
				
				pRemoveLinkList.Add( link );
				pSynthesizer->AddLink( link );
			}
		}
		
		// insert the source
		pSynthesizer->InsertSourceAt( source, pIndex + i );
	}
}



// Private Functions
//////////////////////

void seUPasteSource::pCleanUp(){
	if( pSynthesizer ){
		pSynthesizer->FreeReference();
	}
}
