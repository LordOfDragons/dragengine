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

#include "peeUTypeAdd.h"
#include "../../emitter/peeEmitter.h"
#include "../../emitter/peeType.h"

#include <dragengine/common/exceptions.h>



// Class peeUTypeAdd
//////////////////////

// Constructor, destructor
////////////////////////////

peeUTypeAdd::peeUTypeAdd( peeEmitter *emitter, peeType *type ){
	if( ! emitter || ! type ){
		DETHROW( deeInvalidParam );
	}
	
	pEmitter = NULL;
	pType = NULL;
	
	SetShortInfo( "Add Type" );
	
	pEmitter = emitter;
	emitter->AddReference();
	
	pType = type;
	type->AddReference();
}

peeUTypeAdd::~peeUTypeAdd(){
	if( pType ){
		pType->FreeReference();
	}
	if( pEmitter ){
		pEmitter->FreeReference();
	}
}



// Management
///////////////

void peeUTypeAdd::Undo(){
	pEmitter->RemoveType( pType );
}

void peeUTypeAdd::Redo(){
	pEmitter->AddType( pType );
	pEmitter->SetActiveType( pType );
}
