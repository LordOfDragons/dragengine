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

#include "ceProp.h"
#include "../ceConversation.h"

#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gui/wrapper/igdeWObject.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gameproject/igdeGameProject.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/world/deWorld.h>



// Definitions
////////////////

#define LOGSOURCE "Conversation Editor"



// Class ceProp
/////////////////

// Constructor, destructor
////////////////////////////

ceProp::ceProp(){
	pConversation = NULL;
	
	pName = "Prop";
	pVisible = true;
	
	pObjectClass = "GenericProp";
}

ceProp::~ceProp(){
	pCleanUp();
}



// Management
///////////////

void ceProp::SetConversation(ceConversation *conversation){
	if(conversation != pConversation){
		if(pConversation){
			pObjectWrapper = nullptr;
		}
		
		pConversation = conversation;
		
		if(conversation){
			const decQuaternion &orientation = decMatrix::CreateRotation(pOrientation * DEG2RAD).ToQuaternion();
			
			pObjectWrapper.TakeOver(new igdeWObject(*conversation->GetEnvironment()));
			pObjectWrapper->SetGDClassName(pObjectClass.GetString());
			pObjectWrapper->SetPosition(pPosition);
			pObjectWrapper->SetOrientation(orientation);
			pObjectWrapper->SetVisible(pVisible);
			pObjectWrapper->SetDynamicCollider(false);
			pObjectWrapper->SetWorld(conversation->GetEngineWorld());
		}
	}
}



void ceProp::SetName(const char *name){
	if(!name){
		DETHROW(deeInvalidParam);
	}
	
	if(!pName.Equals(name)){
		pName = name;
		
		if(pConversation){
			pConversation->NotifyPropChanged(this);
		}
	}
}

void ceProp::SetObjectClass(const char *objectClass){
	if(!objectClass){
		DETHROW(deeInvalidParam);
	}
	
	if(!pObjectClass.Equals(objectClass)){
		pObjectClass = objectClass;
		if(pObjectWrapper){
			pObjectWrapper->SetGDClassName(pObjectClass.GetString());
		}
		
		if(pConversation){
			pConversation->NotifyPropChanged(this);
		}
	}
}

void ceProp::SetPosition(const decVector &position){
	if(!position.IsEqualTo(pPosition)){
		pPosition = position;
		if(pObjectWrapper){
			pObjectWrapper->SetPosition(decDVector(position));
		}
		
		if(pConversation){
			pConversation->NotifyPropChanged(this);
		}
	}
}

void ceProp::SetOrientation(const decVector &orientation){
	if(!orientation.IsEqualTo(pOrientation)){
		pOrientation = orientation;
		if(pObjectWrapper){
			pObjectWrapper->SetOrientation(decMatrix::CreateRotation(orientation * DEG2RAD).ToQuaternion());
		}
		
		if(pConversation){
			pConversation->NotifyPropChanged(this);
		}
	}
}

void ceProp::SetVisible(bool visible){
	if(visible != pVisible){
		pVisible = visible;
		if(pObjectWrapper){
			pObjectWrapper->SetVisible(visible);
		}
		
		if(pConversation){
			pConversation->NotifyPropChanged(this);
		}
	}
}



// Private Functions
//////////////////////

void ceProp::pCleanUp(){
	SetConversation(nullptr);
}
