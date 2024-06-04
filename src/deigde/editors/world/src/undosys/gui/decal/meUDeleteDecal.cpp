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

#include "meUDeleteDecal.h"
#include "meUndoDataDecal.h"
#include "../../../world/meWorld.h"
#include "../../../world/decal/meDecal.h"
#include "../../../world/decal/meDecalSelection.h"
#include "../../../world/object/meObject.h"
#include "../../../worldedit.h"

#include <dragengine/common/exceptions.h>



// Class meUDeleteDecal
/////////////////////////

// Constructor, destructor
////////////////////////////

meUDeleteDecal::meUDeleteDecal( meWorld *world ){
	if( ! world ) DETHROW( deeInvalidParam );
	decString text;
	
	const meDecalList &selection = world->GetSelectionDecal().GetSelected();
	int count = selection.GetCount();
	
	pWorld = world;
	pDecals = NULL;
	pDecalCount = 0;
	
	try{
		if( count > 0 ){
			pDecals = new meUndoDataDecal*[ count ];
			if( ! pDecals ) DETHROW( deeOutOfMemory );
			
			while( pDecalCount < count ){
				pDecals[ pDecalCount ] = new meUndoDataDecal( selection.GetAt( pDecalCount ) );
				if( ! pDecals[ pDecalCount ] ) DETHROW( deeOutOfMemory );
				pDecalCount++;
			}
		}
		
		SetShortInfo( "Delete Decals" );
		if( pDecalCount > 1 ){
			text.Format( "%d decals", pDecalCount );
		}else{
			text = "1 object";
		}
		SetLongInfo( text );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

meUDeleteDecal::~meUDeleteDecal(){
	pCleanUp();
}



// Undo and Redo operations
/////////////////////////////

void meUDeleteDecal::Undo(){
	meDecalSelection &selection = pWorld->GetSelectionDecal();
	meDecal *decal;
	int i;
	
	selection.Reset();
	for( i=0; i<pDecalCount; i++ ){
		decal = pDecals[ i ]->GetDecal();
		
		if( pDecals[ i ]->GetParentObject() ){
			pDecals[ i ]->GetParentObject()->InsertDecalAt( decal, pDecals[ i ]->GetIndex() );
		}
		
		pWorld->AddDecal( decal );
		selection.Add( decal );
	}
	selection.ActivateNext();
}

void meUDeleteDecal::Redo(){
	meDecalSelection &selection = pWorld->GetSelectionDecal();
	meDecal *decal;
	int i;
	
	for( i=0; i<pDecalCount; i++ ){
		decal = pDecals[ i ]->GetDecal();
		
		selection.Remove( decal );
		if( decal->GetActive() ){
			selection.ActivateNext();
		}
		
		pWorld->RemoveDecal( pDecals[ i ]->GetDecal() );
		
		if( pDecals[ i ]->GetParentObject() ){
			pDecals[ i ]->GetParentObject()->RemoveDecal( decal );
		}
	}
}



// Private Functions
//////////////////////

void meUDeleteDecal::pCleanUp(){
	if( pDecals ){
		while( pDecalCount > 0 ){
			delete pDecals[ pDecalCount - 1 ];
			pDecalCount--;
		}
		delete [] pDecals;
	}
}
