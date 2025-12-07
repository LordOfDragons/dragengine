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

#include "decSmoothDVector.h"



// Class decSmoothDVector
/////////////////////////

// Constructor, destructor
////////////////////////////

decSmoothDVector::decSmoothDVector() :
pAdjustTime(1.0),
pAdjustRange(1.0),
pChangeSpeed(0.0),
pFactorTime(0.0),
pFactorLimit(0.0),
pFactorDrop(0.0),
pSmoothed(true)
{
	pUpdateFactors();
}

decSmoothDVector::decSmoothDVector(const decSmoothDVector &copy) :
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

decSmoothDVector::~decSmoothDVector(){
}



// Management
///////////////

void decSmoothDVector::SetValue(const decDVector &value){
	pValue = value;
}

void decSmoothDVector::SetGoal(const decDVector &goal){
	pGoal = goal;
}

void decSmoothDVector::SetAdjustTime(double adjustTime){
	pAdjustTime = decMath::max(adjustTime, 0.0);
	pUpdateFactors();
}

void decSmoothDVector::SetAdjustRange(double range){
	pAdjustRange = decMath::max(range, 0.0);
	pUpdateFactors();
}

void decSmoothDVector::SetChangeSpeed(double changeSpeed){
	pChangeSpeed = changeSpeed;
}



void decSmoothDVector::Reset(){
	pValue.SetZero();
	pGoal.SetZero();
	pChangeSpeed = 0.0;
}

void decSmoothDVector::Update(double elapsed){
	if(elapsed < 0.001){
		return;
	}
	
	decDVector value(pGoal);
	
	if(pSmoothed){
		const double factorTime = decMath::min(elapsed * pFactorTime, 1.0);
		const decDVector vectorAdjust((value - pValue) * factorTime);
		const double vectorAdjustLength = vectorAdjust.Length();
		
		if(vectorAdjustLength > FLOAT_SAFE_EPSILON){
			double adjustValue = vectorAdjustLength;
			const double changeSpeedDifference = (adjustValue / elapsed) - pChangeSpeed;
			const double limitChangeSpeed = pFactorLimit * elapsed;
			
			if(changeSpeedDifference > limitChangeSpeed && adjustValue > 0.0){
				adjustValue = (pChangeSpeed + limitChangeSpeed) * elapsed;
				value = pValue + vectorAdjust * (adjustValue / vectorAdjustLength);
				
			}else if(changeSpeedDifference < -limitChangeSpeed && adjustValue < 0.0){
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

bool decSmoothDVector::operator==(const decSmoothDVector &other) const{
	return pValue.IsEqualTo(other.pValue);
}

bool decSmoothDVector::operator!=(const decSmoothDVector &other) const{
	return ! pValue.IsEqualTo(other.pValue);
}

decSmoothDVector &decSmoothDVector::operator=(const decSmoothDVector &other){
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

void decSmoothDVector::pUpdateFactors(){
	if(pAdjustTime < 0.001){
		pFactorTime = 0.0;
		pFactorLimit = 0.0;
		pFactorDrop = 0.0;
		pSmoothed = false;
		
	}else{
		pFactorTime = 4.0 / pAdjustTime;
		pFactorLimit = pow(4.0, 1.0 + log(1.0 / pAdjustTime) / log(2.0)) * pAdjustRange;
		pFactorDrop = pAdjustRange * 0.0025 / pAdjustTime; // 0.25% drop of entire range over entire time
		pSmoothed = true;
	}
}
