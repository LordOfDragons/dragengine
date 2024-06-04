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

#include "aeURuleMirrorRemoveAllMatchNames.h"

#include <dragengine/common/exceptions.h>


// Class aeURuleMirrorRemoveAllMatchNames
///////////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeURuleMirrorRemoveAllMatchNames::aeURuleMirrorRemoveAllMatchNames( aeRuleMirror *rule ) :
pRule( rule )
{
	if( ! rule || rule->GetMatchNameCount() == 0 ){
		DETHROW( deeInvalidAction );
	}
	
	const int count = rule->GetMatchNameCount();
	int i;
	for( i=0; i<count; i++ ){
		pMatchNames.Add( rule->GetMatchNameAt( i ) );
	}
	
	SetShortInfo( "Mirror rule remove all match names" );
}

aeURuleMirrorRemoveAllMatchNames::~aeURuleMirrorRemoveAllMatchNames(){
}



// Management
///////////////

void aeURuleMirrorRemoveAllMatchNames::Undo(){
	const int count = pMatchNames.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		pRule->AddMatchName( ( aeRuleMirror::cMatchName* )pMatchNames.GetAt( i ) );
	}
}

void aeURuleMirrorRemoveAllMatchNames::Redo(){
	pRule->RemoveAllMatchNames();
}
