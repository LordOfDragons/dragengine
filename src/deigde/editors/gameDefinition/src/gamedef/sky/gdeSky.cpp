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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gdeSky.h"
#include "gdeSkyController.h"
#include "../gdeGameDefinition.h"

#include <dragengine/common/exceptions.h>



// Class gdeSky
/////////////////

// Constructor, destructor
////////////////////////////

gdeSky::gdeSky() :
pGameDefinition(NULL){
}

gdeSky::gdeSky(const char *path, const char *name) :
pGameDefinition(NULL),
pPath(path),
pName(name){
}

gdeSky::gdeSky(const gdeSky &sky) :
pGameDefinition(NULL),

pPath(sky.pPath),
pName(sky.pName),
pDescription(sky.pDescription),
pCategory(sky.pCategory),
pTags(sky.pTags)
{
	const int controllerCount = sky.pControllers.GetCount();
	gdeSkyController *controller = NULL;
	int i;
	
	try{
		for(i=0; i<controllerCount; i++){
			controller = new gdeSkyController(*sky.pControllers.GetAt(i));
			pControllers.Add(controller);
			controller->FreeReference();
			controller = NULL;
		}
		
	}catch(const deException &){
		if(controller){
			controller->FreeReference();
		}
		throw;
	}
}

gdeSky::~gdeSky(){
	SetGameDefinition(NULL);
}



// Management
///////////////

void gdeSky::SetGameDefinition(gdeGameDefinition *gamedef){
	pGameDefinition = gamedef;
}



void gdeSky::SetPath(const char *path){
	if(pPath == path){
		return;
	}
	
	pPath = path;
	
	if(pGameDefinition){
		pGameDefinition->NotifySkyChanged(this);
	}
}

void gdeSky::SetName(const char *name){
	if(pName == name){
		return;
	}
	
	pName = name;
	
	if(pGameDefinition){
		pGameDefinition->NotifySkyNameChanged(this);
	}
}

void gdeSky::SetDescription(const char *description){
	if(pDescription == description){
		return;
	}
	
	pDescription = description;
	
	if(pGameDefinition){
		pGameDefinition->NotifySkyChanged(this);
	}
}

void gdeSky::SetCategory(const char *category){
	if(pCategory == category){
		return;
	}
	
	pCategory = category;
	
	if(pGameDefinition){
		pGameDefinition->NotifySkyChanged(this);
	}
}

void gdeSky::SetTags(const decStringSet &tags){
	if(pTags == tags){
		return;
	}
	
	pTags = tags;
	
	if(pGameDefinition){
		pGameDefinition->NotifySkyChanged(this);
	}
}



// Controllers
////////////////

void gdeSky::AddController(gdeSkyController *controller){
	if(!controller){
		DETHROW(deeInvalidParam);
	}
	pControllers.Add(controller);
	
	if(pGameDefinition){
		pGameDefinition->NotifySkyControllerStructureChanged(this);
	}
}

void gdeSky::RemoveController(gdeSkyController *controller){
	pControllers.Remove(controller);
	
	if(pGameDefinition){
		pGameDefinition->NotifySkyControllerStructureChanged(this);
	}
}

void gdeSky::RemoveAllControllers(){
	pControllers.RemoveAll();
}

void gdeSky::NotifyControllerChanged(gdeSkyController *controller){
	if(pGameDefinition){
		pGameDefinition->NotifySkyControllerChanged(this, controller);
	}
}
