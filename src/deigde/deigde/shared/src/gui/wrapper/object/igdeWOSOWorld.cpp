/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#include "igdeWOSOWorld.h"
#include "igdeWOSOVisitor.h"
#include "../igdeWObject.h"
#include "../../../engine/igdeEngineController.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../gamedefinition/igdeGameDefinition.h"
#include "../../../gamedefinition/class/igdeGDClass.h"
#include "../../../gamedefinition/class/world/igdeGDCWorld.h"
#include "../../../gameproject/igdeGameProject.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/world/deWorldManager.h>


// Class igdeWOSOWorld
////////////////////////

// Constructor, destructor
////////////////////////////

igdeWOSOWorld::igdeWOSOWorld(igdeWObject &wrapper, const igdeGDCWorld &gdcWorld,
	const decString &prefix) :
igdeWOSubObject(wrapper, prefix),
pGDWorld(gdcWorld)
{
	wrapper.SubObjectFinishedLoading(*this, true);
}

igdeWOSOWorld::~igdeWOSOWorld(){
	pDestroyWorld();
}


// Management
///////////////

void igdeWOSOWorld::UpdateParameters(){
	GetWrapper().SubObjectFinishedLoading(*this, true);
}

void igdeWOSOWorld::OnAllSubObjectsFinishedLoading(){
	pUpdateWorld();
}

void igdeWOSOWorld::UpdateVisibility(){
	// const bool visible = GetWrapper().GetVisible();
}

void igdeWOSOWorld::Visit(igdeWOSOVisitor &visitor){
	visitor.VisitWorld(*this);
}


// Protected Functions
////////////////////////

void igdeWOSOWorld::AttachToCollider(){
}

void igdeWOSOWorld::DetachFromCollider(){
}


// Private Functions
//////////////////////

void igdeWOSOWorld::pLoadWorld(const decString &path){
	const decString pathWorld(GetStringProperty(
		pGDWorld.GetPropertyName(igdeGDCWorld::epPath), pGDWorld.GetPath()));
	if(pathWorld.IsEmpty()){
		return;
	}
	
	// TODO load world
	GetEnvironment().GetLogger()->LogInfoFormat("IGDE", "Wrapper load world: %s", path.GetString());
	
	// pWorld.TakeOver(GetEngine().GetWorldManager()->CreateWorld());
}

void igdeWOSOWorld::pUpdateWorld(){
	decString pathWorld(GetStringProperty(
		pGDWorld.GetPropertyName(igdeGDCWorld::epPath), pGDWorld.GetPath()));
	if(pathWorld != pPathWorld){
		pDestroyWorld();
	}
	
	const decDVector position(GetVectorProperty(
		pGDWorld.GetPropertyName(igdeGDCWorld::epPosition), pGDWorld.GetPosition()));
	const decQuaternion orientation(GetRotationProperty(
		pGDWorld.GetPropertyName(igdeGDCWorld::epRotation), pGDWorld.GetOrientation()));
	
	if(!position.IsEqualTo(pPosition) || !orientation.IsEqualTo(pOrientation)){
		// TODO move objects
		pPosition = position;
		pOrientation = orientation;
	}
	
	if(pathWorld != pPathWorld){
		pLoadWorld(pathWorld);
		pPathWorld = pathWorld;
	}
}

void igdeWOSOWorld::pDestroyWorld(){
	const int count = pObjects.GetCount();
	int i;
	for(i=0; i<count; i++){
		// TODO
	}
	pObjects.RemoveAll();
	// GetWrapper().GetWorld()->RemoveWorld( pWorld );
}
