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

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "projPanelUndoHistory.h"
#include "projPanelUndoHistoryListener.h"
#include "../project/projProject.h"

#include <dragengine/common/exceptions.h>



// Class projPanelUndoHistory
//////////////////////////////

// Constructor, destructor
////////////////////////////

projPanelUndoHistory::projPanelUndoHistory(igdeEnvironment &environment) :
igdeWPUndoHistory(environment),
pListener(projPanelUndoHistoryListener::Ref::New(*this)){
}

projPanelUndoHistory::~projPanelUndoHistory(){
	SetProject(NULL);
}



// Management
///////////////

void projPanelUndoHistory::SetProject(projProject *project){
	if(project == pProject){
		return;
	}
	
	SetUndoSystem(NULL);
	
	if(pProject){
		pProject->RemoveListener(pListener);
	}
	
	pProject = project;
	
	if(project){
		project->AddListener(pListener);
		
		SetUndoSystem(project->GetUndoSystem());
	}
}
