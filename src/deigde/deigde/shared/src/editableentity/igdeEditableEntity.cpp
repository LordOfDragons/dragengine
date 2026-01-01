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

#include "igdeEditableEntity.h"
#include "../engine/igdeEngineController.h"
#include "../environment/igdeEnvironment.h"
#include "../gamedefinition/igdeGameDefinition.h"
#include "../gameproject/igdeGameProject.h"
#include "../undo/igdeUndoSystem.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>



// Class igdeEditableEntity
/////////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditableEntity::igdeEditableEntity(igdeEnvironment *environment){
	if(!environment){
		DETHROW(deeInvalidParam);
	}
	
	pEnvironment = environment;
	
	pChanged = false;
	pSaved = false;
	
	pUndoSystem = nullptr;
	
	try{
		pUndoSystem = new igdeUndoSystem(this);
		
	}catch(const deException &){
		if(pUndoSystem){
			delete pUndoSystem;
		}
		throw;
	}
}

igdeEditableEntity::~igdeEditableEntity(){
	if(pUndoSystem){
		delete pUndoSystem;
	}
}



// Management
///////////////

deEngine *igdeEditableEntity::GetEngine() const{
	return pEnvironment->GetEngineController()->GetEngine();
}

igdeGameDefinition *igdeEditableEntity::GetGameDefinition() const{
	return pEnvironment->GetGameProject()->GetGameDefinition();
}

deLogger *igdeEditableEntity::GetLogger() const{
	return pEnvironment->GetLogger();
}

void igdeEditableEntity::SetFilePath(const char *path){
	if(!path){
		DETHROW(deeInvalidParam);
	}
	
	if(!pPathFile.Equals(path)){
		decPath pathDirectory;
		
		pPathFile = path;
		
		pathDirectory.SetFromUnix(path);
		if(pathDirectory.GetComponentCount() > 1){
			pathDirectory.RemoveLastComponent();
			
		}else{
			pathDirectory.SetFromUnix("/");
		}
		
		pPathDirectory = pathDirectory.GetPathUnix();
	}
}

void igdeEditableEntity::SetChanged(bool changed){
	if(changed != pChanged){
		pChanged = changed;
		NotifyStateChanged();
	}
}

void igdeEditableEntity::SetSaved(bool saved){
	if(saved != pSaved){
		pSaved = saved;
		NotifyStateChanged();
	}
}

void igdeEditableEntity::NotifyStateChanged(){
}

void igdeEditableEntity::NotifyUndoChanged(){
}
