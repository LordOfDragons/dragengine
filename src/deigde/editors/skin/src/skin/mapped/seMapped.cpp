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

#include "seMapped.h"
#include "../seSkin.h"
#include "../property/seProperty.h"

#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>



// Class seMapped
////////////////////

// Constructor, destructor
////////////////////////////

seMapped::seMapped(const char *name) :
pSkin(nullptr),
pName(name),
pInputType(deSkinMapped::eitTime),
pInputLower(0.0f),
pInputUpper(1.0f),
pInputClamped(false),
pOutputLower(0.0f),
pOutputUpper(1.0f),
pRenderableComponent(deSkinMapped::ercRed),
pSelected(false),
pActive(false)
{
	DEASSERT_FALSE(pName.IsEmpty())
}

seMapped::seMapped(const seMapped &mapped) :
pSkin(nullptr),
pName(mapped.pName),
pCurve(mapped.pCurve),
pInputType(mapped.pInputType),
pInputLower(mapped.pInputLower),
pInputUpper(mapped.pInputUpper),
pInputClamped(mapped.pInputClamped),
pOutputLower(mapped.pOutputLower),
pOutputUpper(mapped.pOutputUpper),
pRenderable(mapped.pRenderable),
pRenderableComponent(mapped.pRenderableComponent),
pSelected(false),
pActive(false){
}

seMapped::~seMapped(){
}



// Management
///////////////

void seMapped::SetSkin(seSkin *skin){
	pSkin = skin;
}



void seMapped::SetName(const char *name){
	if(pName == name){
		return;
	}
	
	pName = name;
	NotifyNameChanged();
}

void seMapped::MakeNameUnique(const seMapped::List &list){
	if(!list.HasMatching([&](const seMapped &m){
		return m.GetName() == pName;
	})){
		return;
	}
	
	decString name(pName);
	int counter = 2;
	
	while(list.HasMatching([&](const seMapped &m){
		return m.GetName() == name;
	})){
		name.Format("%s #%d", pName.GetString(), counter++);
	}
	
	SetName(name);
}

void seMapped::SetCurve(const decCurveBezier &curve){
	if(curve == pCurve){
		return;
	}
	
	pCurve = curve;
	NotifyChanged();
}

void seMapped::SetInputType(deSkinMapped::eInputTypes inputType){
	if(inputType == pInputType){
		return;
	}
	
	pInputType = inputType;
	NotifyChanged();
}

void seMapped::SetInputLower(float lower){
	if(fabsf(lower - pInputLower) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pInputLower = lower;
	NotifyChanged();
}

void seMapped::SetInputUpper(float upper){
	if(fabsf(upper - pInputUpper) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pInputUpper = upper;
	NotifyChanged();
}

void seMapped::SetInputClamped(bool inputClamped){
	if(inputClamped == pInputClamped){
		return;
	}
	
	pInputClamped = inputClamped;
	NotifyChanged();
}

void seMapped::SetOutputLower(float lower){
	if(fabsf(lower - pOutputLower) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pOutputLower = lower;
	NotifyChanged();
}

void seMapped::SetOutputUpper(float upper){
	if(fabsf(upper - pOutputUpper) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pOutputUpper = upper;
	NotifyChanged();
}

void seMapped::SetBone(const char *bone){
	if(pBone == bone){
		return;
	}
	
	pBone = bone;
	NotifyChanged();
}

void seMapped::SetRenderable(const char *renderable){
	if(pRenderable == renderable){
		return;
	}
	
	pRenderable = renderable;
	NotifyChanged();
}

void seMapped::SetRenderableComponent(deSkinMapped::eRenderableComponent component){
	if(pRenderableComponent == component){
		return;
	}
	
	pRenderableComponent = component;
	NotifyChanged();
}



void seMapped::SetSelected(bool selected){
	pSelected = selected;
}

void seMapped::SetActive(bool active){
	pActive = active;
}



void seMapped::NotifyChanged(){
	if(pSkin){
		pSkin->NotifyMappedChanged(this);
	}
}

void seMapped::NotifyNameChanged(){
	if(pSkin){
		pSkin->NotifyMappedNameChanged(this);
	}
}
