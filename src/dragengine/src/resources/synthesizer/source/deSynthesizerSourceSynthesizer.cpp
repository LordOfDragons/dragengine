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
#include <string.h>

#include "deSynthesizerSourceSynthesizer.h"
#include "deSynthesizerSourceVisitor.h"
#include "../deSynthesizer.h"
#include "../../sound/deSound.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"



// Class deSynthesizerSourceSynthesizer
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deSynthesizerSourceSynthesizer::deSynthesizerSourceSynthesizer() :
pSynthesizer(NULL),
pConnections(NULL),
pConnectionCount(0){
}

deSynthesizerSourceSynthesizer::~deSynthesizerSourceSynthesizer(){
	if(pConnections){
		delete [] pConnections;
	}
}



// Management
///////////////

void deSynthesizerSourceSynthesizer::SetSynthesizer(deSynthesizer *synthesizer){
	if(synthesizer == pSynthesizer){
		return;
	}
	
	
	pSynthesizer = synthesizer;
	
	UpdateConnectionCount();
}



void deSynthesizerSourceSynthesizer::UpdateConnectionCount(){
	if(pConnections){
		delete [] pConnections;
		pConnections = NULL;
		pConnectionCount = 0;
	}
	
	if(!pSynthesizer){
		return;
	}
	
	const int controllerCount = pSynthesizer->GetControllerCount();
	if(controllerCount == 0){
		return;
	}
	
	pConnections = new int[controllerCount];
	for(pConnectionCount=0; pConnectionCount<controllerCount; pConnectionCount++){
		pConnections[pConnectionCount] = -1;
	}
}

int deSynthesizerSourceSynthesizer::GetConnectionAt(int childController) const{
	if(childController < 0 || childController >= pConnectionCount){
		DETHROW(deeInvalidParam);
	}
	
	return pConnections[childController];
}

void deSynthesizerSourceSynthesizer::SetConnectionAt(int childController, int localController){
	if(childController < 0 || childController >= pConnectionCount){
		DETHROW(deeInvalidParam);
	}
	if(localController < -1){
		DETHROW(deeInvalidParam);
	}
	
	pConnections[childController] = localController;
}



// Visiting
/////////////

void deSynthesizerSourceSynthesizer::Visit(deSynthesizerSourceVisitor &visitor){
	visitor.VisitSynthesizer(*this);
}
