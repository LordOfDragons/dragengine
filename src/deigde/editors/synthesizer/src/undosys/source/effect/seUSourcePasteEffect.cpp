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

#include "seUSourcePasteEffect.h"
#include "../../../synthesizer/effect/seEffect.h"
#include "../../../synthesizer/source/seSource.h"
#include "../../../synthesizer/link/seLink.h"
#include "../../../synthesizer/seSynthesizer.h"

#include <dragengine/common/exceptions.h>



// Class seUSourcePasteEffect
///////////////////////////////

// Constructor, destructor
////////////////////////////

seUSourcePasteEffect::seUSourcePasteEffect( seSource *source, const seEffectList &effectList, int index ) :
pSource( NULL ),
pIndex( index )
{
	const int effectCount = effectList.GetCount();
	
	if( ! source || effectCount == 0 ){
		DETHROW( deeInvalidParam );
	}
	
	seEffect *effect = NULL;
	int i;
	
	try{
		for( i=0; i<effectCount; i++ ){
			effect = effectList.GetAt( i )->CreateCopy();
			pEffectList.Add( effect );
			effect->FreeReference();
			effect = NULL;
		}
		
	}catch( const deException & ){
		if( effect ){
			effect->FreeReference();
		}
		throw;
	}
	
	pSource = source;
	source->AddReference();
}

seUSourcePasteEffect::~seUSourcePasteEffect(){
	pCleanUp();
}



// Management
///////////////

void seUSourcePasteEffect::Undo(){
	seSynthesizer * const synthesizer = pSource->GetSynthesizer();
	const int effectCount = pEffectList.GetCount();
	int i;
	
	// remove the effects
	for( i=0; i<effectCount; i++ ){
		pSource->RemoveEffect( pEffectList.GetAt( i ) );
	}
	
	// remove links added in a prior redo
	const int linkCount = pRemoveLinkList.GetCount();
	for( i=0; i<linkCount; i++ ){
		synthesizer->RemoveLink( pRemoveLinkList.GetAt( i ) );
	}
	pRemoveLinkList.RemoveAll();
	
	// remove controller added in a prior redo
	const int controllerCount = pRemoveControllerList.GetCount();
	for( i=0; i<controllerCount; i++ ){
		synthesizer->RemoveController( pRemoveControllerList.GetAt( i ) );
	}
	pRemoveControllerList.RemoveAll();
}

void seUSourcePasteEffect::Redo(){
	seSynthesizer * const synthesizer = pSource->GetSynthesizer();
	const int effectCount = pEffectList.GetCount();
	int i, j;
	
	pRemoveLinkList.RemoveAll();
	pRemoveControllerList.RemoveAll();
	
	for( i=0; i<effectCount; i++ ){
		seEffect * const effect = pEffectList.GetAt( i );
		
		// check if links exist in the synthesizer. if not add them and mark them to remove during undo
		seLinkList linkList;
		effect->ListLinks( linkList );
		
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
		
		// insert the source
		pSource->InsertEffectAt( effect, pIndex + i );
	}
}



// Private Functions
//////////////////////

void seUSourcePasteEffect::pCleanUp(){
	if( pSource ){
		pSource->FreeReference();
	}
}
