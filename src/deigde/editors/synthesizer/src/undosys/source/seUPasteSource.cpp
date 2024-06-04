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
