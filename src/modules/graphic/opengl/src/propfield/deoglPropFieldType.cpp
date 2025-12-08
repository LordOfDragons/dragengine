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

#include "deoglPropField.h"
#include "deoglPropFieldType.h"
#include "deoglRPropFieldType.h"
#include "../deGraphicOpenGl.h"
#include "../model/deoglModel.h"
#include "../model/deoglRModel.h"
#include "../skin/deoglRSkin.h"
#include "../skin/deoglSkin.h"
#include "../sky/deoglSky.h"
#include "../world/deoglWorld.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/propfield/dePropField.h>
#include <dragengine/resources/propfield/dePropFieldType.h>
#include <dragengine/resources/propfield/dePropFieldInstance.h>
#include <dragengine/resources/propfield/dePropFieldBendState.h>
#include <dragengine/resources/skin/deSkin.h>



// Class deoglPropFieldType
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPropFieldType::deoglPropFieldType(deoglPropField &propField, const dePropFieldType &type) :
pPropField(propField),
pType(type),
pRType(NULL),
 
pDirtyType(true),
pDirtyInstances(true),
pDirtyBendStates(true)
{
	try{
		pRType.TakeOverWith(*propField.GetRPropField());
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoglPropFieldType::~deoglPropFieldType(){
	pCleanUp();
}



// Management
///////////////

void deoglPropFieldType::SyncToRender(){
	if(pDirtyType){
		deoglRModel *model = NULL;
		deoglRSkin *skin = NULL;
		
		if(pType.GetModel()){
			model = ((deoglModel*)pType.GetModel()->GetPeerGraphic())->GetRModel();
		}
		if(pType.GetSkin()){
			skin = ((deoglSkin*)pType.GetSkin()->GetPeerGraphic())->GetRSkin();
		}
		
		pRType->SetModel(model);
		pRType->SetSkin(skin);
		
		pDirtyType = false;
		pDirtyInstances = true; // if model or skin changes instances change too
	}
	
	if(pDirtyInstances){
		pRType->RebuildInstances(pType);
		pDirtyInstances = false;
		pDirtyBendStates = true;
	}
	
	if(pDirtyBendStates){
		pRType->PrepareBendStateData(pType);
		pDirtyBendStates = false;
	}
}



void deoglPropFieldType::TypeChanged(){
	pDirtyType = true;
}

void deoglPropFieldType::InstancesChanged(){
	pDirtyInstances = true;
}

void deoglPropFieldType::AssignmentsChanged(){
	pDirtyInstances = true;
}

void deoglPropFieldType::BendStatesChanged(){
	pDirtyBendStates = true;
}

void deoglPropFieldType::InstanceCountChanged(){
	pDirtyInstances = true;
}


// Private functions
//////////////////////

void deoglPropFieldType::pCleanUp(){
		pRType = NULL;
	}
}
