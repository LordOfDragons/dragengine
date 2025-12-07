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

#include "deoglSkinStateMapped.h"
#include "../deoglSkinMapped.h"
#include "../../component/deoglRComponent.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentBone.h>
#include <dragengine/resources/rig/deRig.h>



// Class deoglSkinStateMapped
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSkinStateMapped::deoglSkinStateMapped() :
pValue(0.0f),
pMapped(nullptr),
pComponent(nullptr),
pBone(-1),
pBoneInputValue(0.0f){
}

deoglSkinStateMapped::~deoglSkinStateMapped(){
}



// Management
///////////////

void deoglSkinStateMapped::SetValue(float value){
	pValue = value;
}

void deoglSkinStateMapped::SetMapped(const deoglSkinMapped *mapped){
	pMapped = mapped;
}

void deoglSkinStateMapped::SetComponent(const deoglRComponent *component){
	pComponent = component;
}

void deoglSkinStateMapped::MapBone(const deComponent &component){
	pBone = -1;
	
	if(pMapped && component.GetRig()){
		const deRig &rig = *component.GetRig();
		const decString &name = pMapped->GetMapped().GetBone();
		if(!name.IsEmpty()){
			pBone = rig.IndexOfBoneNamed(name);
		}
	}
}

void deoglSkinStateMapped::UpdateBone(const deComponent &component){
	const int boneCount = component.GetBoneCount();
	if(pBone < 0 || pBone >= boneCount){
		return;
	}
	
	const deComponentBone &cbone = component.GetBoneAt(pBone);
	
	switch(pMapped->GetMapped().GetInputType()){
	case deSkinMapped::eitBonePositionX:
		pBoneInputValue = cbone.GetPosition().x;
		break;
		
	case deSkinMapped::eitBonePositionY:
		pBoneInputValue = cbone.GetPosition().y;
		break;
		
	case deSkinMapped::eitBonePositionZ:
		pBoneInputValue = cbone.GetPosition().z;
		break;
		
	case deSkinMapped::eitBoneRotationX:
		pBoneInputValue = cbone.GetRotation().GetEulerAngles().x / TWO_PI;
		break;
		
	case deSkinMapped::eitBoneRotationY:
		pBoneInputValue = cbone.GetRotation().GetEulerAngles().y / TWO_PI;
		break;
		
	case deSkinMapped::eitBoneRotationZ:
		pBoneInputValue = cbone.GetRotation().GetEulerAngles().z / TWO_PI;
		break;
		
	case deSkinMapped::eitBoneScaleX:
		pBoneInputValue = cbone.GetScale().x;
		break;
		
	case deSkinMapped::eitBoneScaleY:
		pBoneInputValue = cbone.GetScale().y;
		break;
		
	case deSkinMapped::eitBoneScaleZ:
		pBoneInputValue = cbone.GetScale().z;
		break;
		
	default:
		break;
	}
}

void deoglSkinStateMapped::Update(deoglSkinState &skinState){
	if(pMapped){
		pValue = pMapped->Calculate(skinState, *this);
		
	}else{
		pValue = 0.0f;
	}
}
