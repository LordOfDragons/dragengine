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
#include <stdlib.h>

#include "meUDecalPropertyCopyToSelected.h"
#include "meUndoDataDecalProperty.h"
#include "../../../../world/meWorld.h"
#include "../../../../world/decal/meDecal.h"

#include <dragengine/common/exceptions.h>



// Class meUDecalPropertyCopyToSelected
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

meUDecalPropertyCopyToSelected::meUDecalPropertyCopyToSelected(
const meDecalList &list, const char *key, const char *value ){
	meUndoDataDecalProperty *undoData = NULL;
	const int count = list.GetCount();
	meDecal *decal;
	int i;
	
	if( ! key || ! value || count < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	try{
		SetShortInfo( "Copy decal property to selected" );
		pKey = key;
		pValue = value;
		
		for( i=0; i<count; i++ ){
			decal = list.GetAt( i );
			if( ! decal->GetWorld() ){
				DETHROW( deeInvalidParam );
			}
			
			const decStringDictionary &properties = decal->GetProperties();
			
			undoData = new meUndoDataDecalProperty( decal );
			undoData->SetPropertyExists( properties.Has( key ) );
			if( undoData->GetPropertyExists() ){
				undoData->SetOldValue( properties.GetAt( key ) );
			}
			
			pList.Add( undoData );
			undoData->FreeReference();
			undoData = NULL;
		}
		
	}catch( const deException & ){
		if( undoData ){
			undoData->FreeReference();
		}
		pCleanUp();
		throw;
	}
}

meUDecalPropertyCopyToSelected::~meUDecalPropertyCopyToSelected(){
	pCleanUp();
}



// Management
///////////////

void meUDecalPropertyCopyToSelected::SetValue( const char *value ){
	pValue = value;
}



void meUDecalPropertyCopyToSelected::Undo(){
	const int count = pList.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const meUndoDataDecalProperty &undoData = *( ( meUndoDataDecalProperty* )pList.GetAt( i ) );
		
		if( undoData.GetPropertyExists() ){
			undoData.GetDecal()->SetProperty( pKey, undoData.GetOldValue() );
			
		}else{
			undoData.GetDecal()->RemoveProperty( pKey );
		}
	}
}

void meUDecalPropertyCopyToSelected::Redo(){
	const int count = pList.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const meUndoDataDecalProperty &undoData = *( ( meUndoDataDecalProperty* )pList.GetAt( i ) );
		
		undoData.GetDecal()->SetProperty( pKey, pValue );
	}
}



// Private Functions
//////////////////////

void meUDecalPropertyCopyToSelected::pCleanUp(){
	pList.RemoveAll();
}
