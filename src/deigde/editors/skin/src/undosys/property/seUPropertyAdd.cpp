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

#include "seUPropertyAdd.h"
#include "../../skin/texture/seTexture.h"
#include "../../skin/property/seProperty.h"

#include <dragengine/common/exceptions.h>



// Class seUPropertyAdd
/////////////////////////

// Constructor, destructor
////////////////////////////

seUPropertyAdd::seUPropertyAdd( seTexture *texture, const sePropertyList &properties ){
	if( ! texture || properties.GetCount() == 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pTexture = NULL;
	
	if( properties.GetCount() == 1 ){
		SetShortInfo( "Add Property" );
		
	}else{
		SetShortInfo( "Add Properties" );
	}
	
	pProperties = properties;
	
	pTexture = texture;
	texture->AddReference();
}

seUPropertyAdd::~seUPropertyAdd(){
	pProperties.RemoveAll();
	if( pTexture ){
		pTexture->FreeReference();
	}
}



// Management
///////////////

void seUPropertyAdd::Undo(){
	const int count = pProperties.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pTexture->RemoveProperty( pProperties.GetAt( i ) );
	}
}

void seUPropertyAdd::Redo(){
	const int count = pProperties.GetCount();
	if( count == 0 ){
		return;
	}
	
	int i;
	for( i=0; i<count; i++ ){
		pTexture->AddProperty( pProperties.GetAt( i ) );
	}
	
	pTexture->SetActiveProperty( pProperties.GetAt( 0 ) );
}
