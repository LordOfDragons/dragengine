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

#include "deoglSkinTextureProperty.h"
#include "dynamic/deoglRDynamicSkin.h"
#include "dynamic/renderables/render/deoglRDSRenderable.h"
#include "state/deoglSkinState.h"
#include "state/deoglSkinStateBone.h"
#include "state/deoglSkinStateRenderable.h"
#include "state/deoglSkinStateCalculated.h"

#include <dragengine/common/exceptions.h>



// Class deoglSkinTextureProperty
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSkinTextureProperty::deoglSkinTextureProperty() :
pRenderable(-1),
pCalculatedProperty(-1),
pBone(-1){
}

deoglSkinTextureProperty::~deoglSkinTextureProperty(){
}



// Management
///////////////

void deoglSkinTextureProperty::SetRenderable(int index){
	DEASSERT_TRUE(index >= -1)
	pRenderable = index;
}

void deoglSkinTextureProperty::SetCalculatedProperty(int index){
	DEASSERT_TRUE(index >= -1)
	pCalculatedProperty = index;
}

void deoglSkinTextureProperty::SetBone(int index){
	DEASSERT_TRUE(index >= -1)
	pBone = index;
}

bool deoglSkinTextureProperty::IsDynamic() const{
	return pRenderable != -1 || pCalculatedProperty != -1;
}



float deoglSkinTextureProperty::ResolveAsFloat(const deoglSkinState *skinState,
const deoglRDynamicSkin *dynamicSkin, float defaultValue) const{
	if(!skinState){
		return defaultValue;
	}
	
	if(dynamicSkin && pRenderable >= 0 && pRenderable < skinState->GetRenderableCount()){
		const deoglSkinStateRenderable &stateRenderable = *skinState->GetRenderableAt(pRenderable);
		if(stateRenderable.GetHostRenderable() != -1){
			return dynamicSkin->GetRenderableAt(stateRenderable.GetHostRenderable())
				->GetRenderValue(defaultValue);
		}
	}
	
	if(pCalculatedProperty >= 0 && pCalculatedProperty < skinState->GetCalculatedPropertyCount()){
		return skinState->GetCalculatedPropertyAt(pCalculatedProperty).GetColor().r;
	}
	
	return defaultValue;
}

bool deoglSkinTextureProperty::ResolveAsBool(const deoglSkinState *skinState,
const deoglRDynamicSkin *dynamicSkin, bool defaultValue) const{
	if(!skinState){
		return defaultValue;
	}
	
	if(dynamicSkin && pRenderable >= 0 && pRenderable < skinState->GetRenderableCount()){
		const deoglSkinStateRenderable &stateRenderable = *skinState->GetRenderableAt(pRenderable);
		if(stateRenderable.GetHostRenderable() != -1){
			return dynamicSkin->GetRenderableAt(stateRenderable.GetHostRenderable())
				->GetRenderValue(defaultValue ? 1.0f : 0.0f) > 0.5f;
		}
	}
	
	if(pCalculatedProperty >= 0 && pCalculatedProperty < skinState->GetCalculatedPropertyCount()){
		return skinState->GetCalculatedPropertyAt(pCalculatedProperty).GetColor().r > 0.5f ? 1.0f : 0.0f;
	}
	
	return defaultValue;
}

decColor deoglSkinTextureProperty::ResolveColor(const deoglSkinState *skinState,
const deoglRDynamicSkin *dynamicSkin, const decColor &defaultValue) const{
	if(!skinState){
		return defaultValue;
	}
	
	if(dynamicSkin && pRenderable >= 0 && pRenderable < skinState->GetRenderableCount()){
		const deoglSkinStateRenderable &stateRenderable = *skinState->GetRenderableAt(pRenderable);
		if(stateRenderable.GetHostRenderable() != -1){
			return dynamicSkin->GetRenderableAt(stateRenderable.GetHostRenderable())
				->GetRenderColor(defaultValue);
		}
	}
	
	if(pCalculatedProperty >= 0 && pCalculatedProperty < skinState->GetCalculatedPropertyCount()){
		return skinState->GetCalculatedPropertyAt(pCalculatedProperty).GetColor();
	}
	
	return defaultValue;
}

decVector2 deoglSkinTextureProperty::ResolveVector2(const deoglSkinState *skinState,
const deoglRDynamicSkin *dynamicSkin, const decVector2 &defaultValue) const{
	if(!skinState){
		return defaultValue;
	}
	
	if(dynamicSkin && pRenderable >= 0 && pRenderable < skinState->GetRenderableCount()){
		const deoglSkinStateRenderable &stateRenderable = *skinState->GetRenderableAt(pRenderable);
		if(stateRenderable.GetHostRenderable() != -1){
			const decColor color(dynamicSkin->GetRenderableAt(stateRenderable.GetHostRenderable())
				->GetRenderColor(decColor(defaultValue.x, defaultValue.y, 0.0f)));
			return decVector2(color.r, color.g);
		}
	}
	
	if(pCalculatedProperty >= 0 && pCalculatedProperty < skinState->GetCalculatedPropertyCount()){
		const decColor &color = skinState->GetCalculatedPropertyAt(pCalculatedProperty).GetColor();
		return decVector2(color.r, color.g);
	}
	
	return defaultValue;
}

decVector deoglSkinTextureProperty::ResolveVector(const deoglSkinState *skinState,
const deoglRDynamicSkin *dynamicSkin, const decVector &defaultValue) const{
	if(!skinState){
		return defaultValue;
	}
	
	if(dynamicSkin && pRenderable >= 0 && pRenderable < skinState->GetRenderableCount()){
		const deoglSkinStateRenderable &stateRenderable = *skinState->GetRenderableAt(pRenderable);
		if(stateRenderable.GetHostRenderable() != -1){
			const decColor color(dynamicSkin->GetRenderableAt(stateRenderable.GetHostRenderable())
				->GetRenderColor(decColor(defaultValue.x, defaultValue.y, defaultValue.z)));
			return decVector(color.r, color.g, color.b);
		}
	}
	
	if(pCalculatedProperty >= 0 && pCalculatedProperty < skinState->GetCalculatedPropertyCount()){
		const decColor &color = skinState->GetCalculatedPropertyAt(pCalculatedProperty).GetColor();
		return decVector(color.r, color.g, color.b);
	}
	
	return defaultValue;
}

decMatrix deoglSkinTextureProperty::ResolveMatrix(const deoglSkinState *skinState,
const decMatrix &defaultValue) const{
	if(skinState && pBone >= 0 && pBone < skinState->GetBoneCount()){
		return skinState->GetBoneAt(pBone).GetBoneMatrix();
	}
	return defaultValue;
}
