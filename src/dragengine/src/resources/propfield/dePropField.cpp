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

#include "dePropField.h"
#include "dePropFieldInstance.h"
#include "dePropFieldManager.h"
#include "dePropFieldType.h"
#include "../../deEngine.h"
#include "../../systems/modules/graphic/deBaseGraphicPropField.h"
#include "../../systems/modules/physics/deBasePhysicsPropField.h"
#include "../../systems/modules/scripting/deBaseScriptingPropField.h"
#include "../../common/exceptions.h"



// Class dePropField
//////////////////////

// Constructor, destructor
////////////////////////////

dePropField::dePropField(dePropFieldManager *manager) : deResource(manager),
pPeerGraphic(nullptr),
pPeerPhysics(nullptr),
pPeerScripting(nullptr),
pParentWorld(nullptr),
pLLWorld(this){
}

dePropField::~dePropField(){
	if(pPeerScripting){
		delete pPeerScripting;
		pPeerScripting = nullptr;
	}
	if(pPeerPhysics){
		delete pPeerPhysics;
		pPeerPhysics = nullptr;
	}
	if(pPeerGraphic){
		delete pPeerGraphic;
		pPeerGraphic = nullptr;
	}
	
	RemoveAllTypes();
}



// Types
//////////

void dePropField::SetPosition(const decDVector &position){
	if(!position.IsEqualTo(pPosition)){
		pPosition = position;
		
		if(pPeerGraphic) pPeerGraphic->PositionChanged();
		if(pPeerPhysics) pPeerPhysics->PositionChanged();
	}
}



int dePropField::IndexOfType(dePropFieldType *type) const{
	if(!type) DETHROW(deeInvalidParam);
	return pTypes.IndexOf(type);
}

void dePropField::AddType(dePropFieldType::Ref &&type){
	DEASSERT_NOTNULL(type)
	
	pTypes.Add(std::move(type));
	
	if(pPeerGraphic) pPeerGraphic->TypeAdded(pTypes.GetCount() - 1, pTypes.Last());
	if(pPeerPhysics) pPeerPhysics->TypeAdded(pTypes.GetCount() - 1, pTypes.Last());
}

void dePropField::RemoveType(dePropFieldType *type){
	const int index = IndexOfType(type);
	if(index == -1) DETHROW(deeInvalidParam);
	
	if(pPeerGraphic) pPeerGraphic->TypeRemoved(index, type);
	if(pPeerPhysics) pPeerPhysics->TypeRemoved(index, type);
	
	pTypes.RemoveFrom(index);
}

void dePropField::RemoveAllTypes(){
	if(pPeerGraphic) pPeerGraphic->AllTypesRemoved();
	if(pPeerPhysics) pPeerPhysics->AllTypesRemoved();
	
	pTypes.RemoveAll();
}



void dePropField::NotifyTypeChanged(int index){
	if(index < 0 || index >= pTypes.GetCount()) DETHROW(deeOutOfBoundary);
	
	if(pPeerGraphic) pPeerGraphic->TypeChanged(index, pTypes.GetAt(index));
	if(pPeerPhysics) pPeerPhysics->TypeChanged(index, pTypes.GetAt(index));
}

void dePropField::NotifyInstancesChanged(int type){
	if(type < 0 || type >= pTypes.GetCount()) DETHROW(deeOutOfBoundary);
	
	if(pPeerGraphic) pPeerGraphic->InstancesChanged(type, pTypes.GetAt(type));
	if(pPeerPhysics) pPeerPhysics->InstancesChanged(type, pTypes.GetAt(type));
}

void dePropField::NotifyAssignmentsChanged(int type){
	if(type < 0 || type >= pTypes.GetCount()) DETHROW(deeOutOfBoundary);
	
	if(pPeerGraphic) pPeerGraphic->AssignmentsChanged(type, pTypes.GetAt(type));
}

void dePropField::NotifyBendStatesChanged(int type){
	if(type < 0 || type >= pTypes.GetCount()) DETHROW(deeOutOfBoundary);
	
	if(pPeerGraphic) pPeerGraphic->BendStatesChanged(type, pTypes.GetAt(type));
}

void dePropField::NotifyGroundChanged(){
	if(pPeerGraphic) pPeerGraphic->GroundChanged();
}

void dePropField::NotifyCreateInstances(float density){
	if(pPeerScripting){
		pPeerScripting->CreateInstances(density);
		
	}else{
		// usually we do nothing if peers do not exist. in this case though the performance of
		// the modules depends on the result of this call. to avoid any problems in the case
		// of a missing peer we simply remove all instances from all types without touching the
		// types at all. this way no performance hit is taken as the worst that can happen is
		// that no props are rendered at all which is not as bad as cringing to a crawl
		pTypes.Visit([](dePropFieldType &type){
			type.GetInstances().SetCountDiscard(0);
		});
	}
}

void dePropField::NotifyProjectInstances(const dePropFieldGround &ground, const decVector &direction){
	if(pPeerPhysics) pPeerPhysics->ProjectInstances(ground, direction);
}



// System Peers
/////////////////

void dePropField::SetPeerGraphic(deBaseGraphicPropField *peer){
	if(pPeerGraphic) delete pPeerGraphic;
	pPeerGraphic = peer;
}

void dePropField::SetPeerPhysics(deBasePhysicsPropField *peer){
	if(pPeerPhysics) delete pPeerPhysics;
	pPeerPhysics = peer;
}

void dePropField::SetPeerScripting(deBaseScriptingPropField *peer){
	if(pPeerScripting) delete pPeerScripting;
	pPeerScripting = peer;
}



// Linked List
////////////////

void dePropField::SetParentWorld(deWorld *world){
	pParentWorld = world;
}
