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

#include "deAnimation.h"
#include "deAnimationMove.h"
#include "deAnimationKeyframe.h"

#include "deAnimationKeyframeVertexPositionSet.h"
#include "../../common/exceptions.h"
#include "../../common/curve/decCurveBezier.h"
#include "../../common/curve/decCurveBezierPoint.h"


// class deAnimationMove
//////////////////////////

// constructor, destructor
////////////////////////////

deAnimationMove::deAnimationMove() :
pPlaytime(0),
pFPS(25.0f){
}

deAnimationMove::~deAnimationMove(){
}



// management
///////////////

void deAnimationMove::SetName(const char *name){
	pName = name;
}

void deAnimationMove::SetPlaytime(float playtime){
	pPlaytime = decMath::max(playtime, 0.0f);
}

void deAnimationMove::SetFPS(float fps){
	pFPS = decMath::max(fps, 1.0f);
}

const deAnimationMove::KeyframeListRef &deAnimationMove::GetKeyframeList(int index) const{
	return pLists.GetAt(index);
}

void deAnimationMove::AddKeyframeList(KeyframeListRef &&list){
	pLists.Add(std::move(list));
}

const deAnimationMove::VertexPositionSetKeyframeListRef &deAnimationMove::GetVertexPositionSetKeyframeList(int index) const{
	return pVertexPositionSetLists.GetAt(index);
}

void deAnimationMove::AddVertexPositionSetKeyframeList(VertexPositionSetKeyframeListRef &&list){
	pVertexPositionSetLists.Add(std::move(list));
}

void deAnimationMove::GetKeyframeCurve(decCurveBezier &curve, int index,
BoneParameter parameter) const{
	curve.RemoveAllPoints();
	curve.SetInterpolationMode(decCurveBezier::eimLinear);
	
	float value = 0.0f;
	
	GetKeyframeList(index)->Visit([&](const deAnimationKeyframe &kf){
		switch(parameter){
		case BoneParameter::positionX:
			value = kf.GetPosition().x;
			break;
			
		case BoneParameter::positionY:
			value = kf.GetPosition().y;
			break;
			
		case BoneParameter::positionZ:
			value = kf.GetPosition().z;
			break;
			
		case BoneParameter::rotationX:
			value = kf.GetRotation().x;
			break;
			
		case BoneParameter::rotationY:
			value = kf.GetRotation().y;
			break;
			
		case BoneParameter::rotationZ:
			value = kf.GetRotation().z;
			break;
			
		case BoneParameter::scaleX:
			value = kf.GetScale().x;
			break;
			
		case BoneParameter::scaleY:
			value = kf.GetScale().y;
			break;
			
		case BoneParameter::scaleZ:
			value = kf.GetScale().z;
			break;
		}
		
		curve.AddPoint(decCurveBezierPoint(decVector2(kf.GetTime(), value)));
	});
}

void deAnimationMove::GetVertexPositionSetKeyframeCurve(decCurveBezier &curve, int index) const{
	curve.RemoveAllPoints();
	curve.SetInterpolationMode(decCurveBezier::eimLinear);
	
	GetVertexPositionSetKeyframeList(index)->Visit([&](const deAnimationKeyframeVertexPositionSet &kf){
		curve.AddPoint(decCurveBezierPoint(decVector2(kf.GetTime(), kf.GetWeight())));
	});
}
