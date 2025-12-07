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
#include <string.h>

#include "deoglCollideListComponent.h"
#include "../component/deoglRComponent.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../occlusiontest/deoglOcclusionTest.h"
#include "../utils/deoglCubeHelper.h"

#include <dragengine/common/exceptions.h>



// Class deoglCollideListComponent
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglCollideListComponent::deoglCollideListComponent() :
pComponent(NULL),
pLODLevel(0),
pComponentLOD(NULL),
pCulled(false),
pCascadeMask(0),
pCubeFaceMask(0x2f),
pSpecialFlags(0){
}

deoglCollideListComponent::~deoglCollideListComponent(){
}



// Management
///////////////

void deoglCollideListComponent::Clear(){
	pComponent = NULL;
	pLODLevel = 0;
	pComponentLOD = NULL;
	pCulled = false;
	pCascadeMask = 0;
	pCubeFaceMask = 0x2f;
	pSpecialFlags = 0;
}

void deoglCollideListComponent::SetComponent(deoglRComponent *component){
	pComponent = component;
	pLODLevel = 0;
	pComponentLOD = &pComponent->GetLODAt(pLODLevel);
}

void deoglCollideListComponent::SetLODLevel(int lodLevel){
	pLODLevel = lodLevel;
	pComponentLOD = &pComponent->GetLODAt(pLODLevel);
}

void deoglCollideListComponent::SetLODLevelMax(){
	pLODLevel = pComponent->GetLODCount() - 1;
	pComponentLOD = &pComponent->GetLODAt(pLODLevel);
}

void deoglCollideListComponent::SetCulled(bool culled){
	pCulled = culled;
}

void deoglCollideListComponent::SetCascadeMask(int mask){
	pCascadeMask = mask;
}

void deoglCollideListComponent::UpdateCubeFaceMask(const decDVector &cubePosition){
	pCubeFaceMask = deoglCubeHelper::CalcFaceVisibility(
		pComponent->GetMinimumExtend() - cubePosition,
		pComponent->GetMaximumExtend() - cubePosition);
	
	// DEBUG
	/*
	const deoglDMatrix &m = pComponent.GetMatrix();
	pRenderThread.GetLogger().LogInfoFormat("DEBUG: (%g,%g,%g) [%d, %d, %d, %d, %d, %d] {(%g,%g,%g), (%g,%g,%g)}",
		m.GetPosition().x, m.GetPosition().y, m.GetPosition().z, (pCubeFaceMask & 0x1) == 0x1,
		(pCubeFaceMask & 0x2) == 0x2, (pCubeFaceMask & 0x4) == 0x4, (pCubeFaceMask & 0x8) == 0x8,
		(pCubeFaceMask & 0x10) == 0x10, (pCubeFaceMask & 0x20) == 0x20,
		(pComponent->GetMinimumExtend() - cubePosition).x, (pComponent->GetMinimumExtend() - cubePosition).y,
		(pComponent->GetMinimumExtend() - cubePosition).z, (pComponent->GetMaximumExtend() - cubePosition).x,
		(pComponent->GetMaximumExtend() - cubePosition).y, (pComponent->GetMaximumExtend() - cubePosition).z);
	*/
}

bool deoglCollideListComponent::GetCubeFaceMaskAt(int face) const{
	if(face < 0 || face > 5){
		DETHROW(deeInvalidParam);
	}
	return (pCubeFaceMask & (1 << face)) != 0;
}

void deoglCollideListComponent::SetSpecialFlags(int flags){
	pSpecialFlags = flags;
}

void deoglCollideListComponent::StartOcclusionTest(deoglOcclusionTest &occlusionTest,
const decDVector &referencePosition){
	// this check is wrong. components can have no skin assigned but having skins assigned
	// to individual textures. this check should instead check all textures if they have
	// a UseTexture which is not NULL. doing this though over and over again is bad.
	// requires adding a flag to check if the component has at least one visible texture
// 	if( ! pComponent->GetModel() || ! pComponent->GetSkin() ){
	if(! pComponent->GetModel()){
		OcclusionTestInvisible();
		return;
	}
	
	pCulled = false;
	occlusionTest.AddInputData(
		(pComponent->GetMinimumExtend() - referencePosition).ToVector(),
		(pComponent->GetMaximumExtend() - referencePosition).ToVector(), this);
}

void deoglCollideListComponent::OcclusionTestInvisible(){
	pCulled = true;
}
