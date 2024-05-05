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
#include <string.h>

#include "peeWPType.h"
#include "peeWPTypeListener.h"
#include "../../emitter/peeEmitter.h"
#include "../../emitter/peeType.h"

#include <dragengine/common/exceptions.h>



// Class peeWPTypeListener
////////////////////////////

// Constructor, destructor
////////////////////////////

peeWPTypeListener::peeWPTypeListener( peeWPType &panel ) :
pPanel( panel ){
}

peeWPTypeListener::~peeWPTypeListener(){
}



// Management
///////////////

void peeWPTypeListener::ControllerStructureChanged( peeEmitter* ){
	pPanel.UpdateControllerList();
}

void peeWPTypeListener::ControllerChanged( peeEmitter*, peeController* ){
	pPanel.UpdateControllerList();
}



void peeWPTypeListener::TypeStructureChanged( peeEmitter* ){
	pPanel.UpdateTypeList();
	pPanel.UpdateParameter();
}

void peeWPTypeListener::TypeChanged( peeEmitter*, peeType* ){
	pPanel.UpdateTypeList();
}

void peeWPTypeListener::TypeActiveTrailControllerChanged( peeEmitter*, peeType *type ){
	if( type->GetActive() ){
		pPanel.SelectActiveTrailController();
		pPanel.UpdateType();
	}
}

void peeWPTypeListener::TypeActiveEmitControllerChanged( peeEmitter*, peeType *type ){
	if( type->GetActive() ){
		pPanel.SelectActiveEmitController();
		pPanel.UpdateType();
	}
}

void peeWPTypeListener::TypeParameterChanged( peeEmitter*, peeType *type, peeParameter *parameter ){
	if( type->GetActive() ){
		pPanel.UpdateParameter();
	}
	pPanel.UpdateParameterUsage( *parameter );
}

void peeWPTypeListener::ActiveTypeParameterChanged( peeEmitter*, peeType *type ){
	if( type->GetActive() ){
		pPanel.SelectActiveParameter();
	}
}

void peeWPTypeListener::ActiveTypeChanged( peeEmitter* ){
	pPanel.SelectActiveType();
	pPanel.UpdateParameter();
}
