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

#include "deoglSkinMapped.h"
#include "state/deoglSkinState.h"
#include "state/deoglSkinStateMapped.h"
#include "state/deoglSkinStateRenderable.h"
#include "../skin/dynamic/deoglRDynamicSkin.h"
#include "../skin/dynamic/renderables/render/deoglRDSRenderable.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/curve/decCurveBezierEvaluator.h>



// Class deoglSkinMapped
//////////////////////////

// Constructor, destructor
////////////////////////////

deoglSkinMapped::deoglSkinMapped(const deSkinMapped &mapped) :
pMapped(mapped),
pCurveEvaluator(mapped.GetCurve()),
pRenderable(-1){
}

deoglSkinMapped::~deoglSkinMapped(){
}



// Management
///////////////

void deoglSkinMapped::SetRenderable(int renderable){
	DEASSERT_TRUE(renderable >= -1)
	
	pRenderable = renderable;
}

float deoglSkinMapped::Calculate(const deoglSkinState &skinState, const deoglSkinStateMapped &mapped) const{
	return pCalculateOutputValue(pCalculateInputValue(skinState, mapped));
}



// Private Functions
//////////////////////

float deoglSkinMapped::pCalculateInputValue(const deoglSkinState &skinState,
const deoglSkinStateMapped &mapped) const{
	float input = 0.0f;
	
	switch(pMapped.GetInputType()){
	case deSkinMapped::eitTime:
		input = skinState.GetTime();
		break;
		
	case deSkinMapped::eitBonePositionX:
	case deSkinMapped::eitBonePositionY:
	case deSkinMapped::eitBonePositionZ:
	case deSkinMapped::eitBoneRotationX:
	case deSkinMapped::eitBoneRotationY:
	case deSkinMapped::eitBoneRotationZ:
	case deSkinMapped::eitBoneScaleX:
	case deSkinMapped::eitBoneScaleY:
	case deSkinMapped::eitBoneScaleZ:
		input = mapped.GetBoneInputValue();
		break;
		
	case deSkinMapped::eitRenderable:{
		if(pRenderable == -1 || pRenderable >= skinState.GetRenderableCount()){
			break;
		}
		
		const deoglSkinStateRenderable * const renderable = skinState.GetRenderableAt(pRenderable);
		if(! renderable){
			break;
		}
		
		const deoglRDynamicSkin * const dynamicSkin = skinState.GetOwnerDynamicSkin();
		if(! dynamicSkin){
			break;
		}
		
		const int hostRenderable = renderable->GetHostRenderable();
		if(hostRenderable == -1){
			break;
		}
		
		deoglRDSRenderable * const dsrenderable = dynamicSkin->GetRenderableAt(hostRenderable);
		if(! dsrenderable){
			break;
		}
		
		if(dsrenderable->GetType() == deoglRDSRenderable::etColor){
			const decColor color(dsrenderable->GetRenderColor(decColor()));
			
			switch(pMapped.GetRenderableComponent()){
			case deSkinMapped::ercRed:
				input = color.r;
				break;
				
			case deSkinMapped::ercGreen:
				input = color.g;
				break;
				
			case deSkinMapped::ercBlue:
				input = color.b;
				break;
				
			case deSkinMapped::ercAlpha:
				input = color.a;
				break;
			}
			
		}else{
			input = dsrenderable->GetRenderValue(0.0f);
		}
		}break;
		
	default:
		break;
	}
	
	if(! pMapped.GetInputClamped()){
		input = decMath::normalize(input, pMapped.GetInputLower(), pMapped.GetInputUpper());
	}
	
	return decMath::linearStep(input, pMapped.GetInputLower(), pMapped.GetInputUpper());
}

float deoglSkinMapped::pCalculateOutputValue(float inputValue) const{
	const float lower = pMapped.GetOutputLower();
	const float upper = pMapped.GetOutputUpper();
	
	return lower + (upper - lower) * pCurveEvaluator.EvaluateAt(inputValue);
}
