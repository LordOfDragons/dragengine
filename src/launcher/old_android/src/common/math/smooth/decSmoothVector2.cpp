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

#include <math.h>
#include <stdio.h>

#include "decSmoothVector2.h"



// Class decSmoothVector2
///////////////////////////

// Constructor, destructor
////////////////////////////

decSmoothVector2::decSmoothVector2() :
pAdjustTime(1.0f),
pAdjustRange(1.0f),
pChangeSpeed(0.0f),
pFactorTime(0.0f),
pFactorLimit(0.0f),
pFactorDrop(0.0f),
pSmoothed(true)
{
	pUpdateFactors();
}

decSmoothVector2::decSmoothVector2(const decSmoothVector2 &copy) :
pValue(copy.pValue),
pGoal(copy.pGoal),
pAdjustTime(copy.pAdjustTime),
pAdjustRange(copy.pAdjustRange),
pChangeSpeed(copy.pChangeSpeed),
pFactorTime(copy.pFactorTime),
pFactorLimit(copy.pFactorLimit),
pFactorDrop(copy.pFactorDrop),
pSmoothed(copy.pSmoothed)
{
}

decSmoothVector2::~decSmoothVector2(){
}



// Management
///////////////

void decSmoothVector2::SetValue(const decVector2 &value){
	pValue = value;
}

void decSmoothVector2::SetGoal(const decVector2 &goal){
	pGoal = goal;
}

void decSmoothVector2::SetAdjustTime(float adjustTime){
	pAdjustTime = decMath::max(adjustTime, 0.0f);
	pUpdateFactors();
}

void decSmoothVector2::SetAdjustRange(float range){
	pAdjustRange = decMath::max(range, 0.0f);
	pUpdateFactors();
}

void decSmoothVector2::SetChangeSpeed(float changeSpeed){
	pChangeSpeed = changeSpeed;
}



void decSmoothVector2::Reset(){
	pValue.SetZero();
	pGoal.SetZero();
	pChangeSpeed = 0.0f;
}

void decSmoothVector2::Update(float elapsed){
	if(elapsed < 0.001f){
		return;
	}
	
	decVector2 value(pGoal);
	
	if(pSmoothed){
		const float factorTime = decMath::min(elapsed * pFactorTime, 1.0f);
		const decVector2 vectorAdjust((value - pValue) * factorTime);
		const float vectorAdjustLength = vectorAdjust.Length();
		
		if(vectorAdjustLength > FLOAT_SAFE_EPSILON){
			float adjustValue = vectorAdjustLength;
			const float changeSpeedDifference = (adjustValue / elapsed) - pChangeSpeed;
			const float limitChangeSpeed = pFactorLimit * elapsed;
			
			if(changeSpeedDifference > limitChangeSpeed && adjustValue > 0.0f){
				adjustValue = (pChangeSpeed + limitChangeSpeed) * elapsed;
				value = pValue + vectorAdjust * (adjustValue / vectorAdjustLength);
				
			}else if(changeSpeedDifference < -limitChangeSpeed && adjustValue < 0.0f){
				adjustValue = (pChangeSpeed - limitChangeSpeed) * elapsed;
				value = pValue + vectorAdjust * (adjustValue / vectorAdjustLength);
				
			}else{
				value = pValue + vectorAdjust;
			}
		}
	}
	
	pChangeSpeed = (value - pValue).Length() / elapsed;
	pValue = value;
}



// Operators
//////////////

bool decSmoothVector2::operator==(const decSmoothVector2 &other) const{
	return pValue.IsEqualTo(other.pValue);
}

bool decSmoothVector2::operator!=(const decSmoothVector2 &other) const{
	return ! pValue.IsEqualTo(other.pValue);
}

decSmoothVector2 &decSmoothVector2::operator=(const decSmoothVector2 &other){
	pValue = other.pValue;
	pGoal = other.pGoal;
	pAdjustTime = other.pAdjustTime;
	pAdjustRange = other.pAdjustRange;
	pChangeSpeed = other.pChangeSpeed;
	pFactorTime = other.pFactorTime;
	pFactorLimit = other.pFactorLimit;
	pFactorDrop = other.pFactorDrop;
	pSmoothed = other.pSmoothed;
	return *this;
}



// Private Functions
//////////////////////

void decSmoothVector2::pUpdateFactors(){
	if(pAdjustTime < 0.001f){
		pFactorTime = 0.0f;
		pFactorLimit = 0.0f;
		pFactorDrop = 0.0f;
		pSmoothed = false;
		
	}else{
		pFactorTime = 4.0f / pAdjustTime;
		pFactorLimit = powf(4.0f, 1.0f + logf(1.0f / pAdjustTime) / logf(2.0f)) * pAdjustRange;
		pFactorDrop = pAdjustRange * 0.0025f / pAdjustTime; // 0.25% drop of entire range over entire time
		pSmoothed = true;
	}
}
