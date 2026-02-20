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

#include <string.h>

#include "deoalPanningCollector.h"
#include "../audiothread/deoalAudioThread.h"
#include "../audiothread/deoalATLogger.h"

#include <dragengine/common/exceptions.h>



// Class deoalPanningCollector
////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalPanningCollector::deoalPanningCollector(int resolution) :
pResolution(resolution),
pPixelsPerFace(resolution * resolution),
pPixelsXPos(nullptr),
pPixelsXNeg(nullptr),
pPixelsYPos(nullptr),
pPixelsYNeg(nullptr),
pPixelsZPos(nullptr),
pPixelsZNeg(nullptr)
{
	if(resolution < 1){
		DETHROW(deeInvalidParam);
	}
	
	pPixels.AddRange(resolution * resolution * 6, {});
	
	pPixelsXPos = pPixels.GetArrayPointer();
	pPixelsXNeg = pPixels.GetArrayPointer() + pPixelsPerFace;
	pPixelsYPos = pPixels.GetArrayPointer() + pPixelsPerFace * 2;
	pPixelsYNeg = pPixels.GetArrayPointer() + pPixelsPerFace * 3;
	pPixelsZPos = pPixels.GetArrayPointer() + pPixelsPerFace * 4;
	pPixelsZNeg = pPixels.GetArrayPointer() + pPixelsPerFace * 5;
	
	Clear();
}

deoalPanningCollector::deoalPanningCollector(const deoalPanningCollector &collector) :
pResolution(collector.pResolution),
pPixelsPerFace(collector.pPixelsPerFace),
pPixelsXPos(nullptr),
pPixelsXNeg(nullptr),
pPixelsYPos(nullptr),
pPixelsYNeg(nullptr),
pPixelsZPos(nullptr),
pPixelsZNeg(nullptr)
{
	pPixels.AddRange(collector.pPixels.GetCount(), {});
	
	pPixelsXPos = pPixels.GetArrayPointer();
	pPixelsXNeg = pPixels.GetArrayPointer() + pPixelsPerFace;
	pPixelsYPos = pPixels.GetArrayPointer() + pPixelsPerFace * 2;
	pPixelsYNeg = pPixels.GetArrayPointer() + pPixelsPerFace * 3;
	pPixelsZPos = pPixels.GetArrayPointer() + pPixelsPerFace * 4;
	pPixelsZNeg = pPixels.GetArrayPointer() + pPixelsPerFace * 5;
	
	*this = collector;
}

deoalPanningCollector::~deoalPanningCollector(){
}



// Manegement
///////////////

void deoalPanningCollector::Clear(){
	pPixels.SetRangeAt(0, pPixels.GetCount(), 0.0f);
}



void deoalPanningCollector::Add(const decVector &, float){
	DETHROW(deeInvalidAction);
}



decVector deoalPanningCollector::CalcPanningVector() const{
	DETHROW(deeInvalidAction);
}



void deoalPanningCollector::DebugPrint(deoalAudioThread &audioThread) const{
	const char * const faceNames[6] = {
		"Positive X-Axis", "Negative X-Axis", "Positive Y-Axis",
		"Negative Y-Axis", "Positive Z-Axis", "Negative Z-Axis"};
	deoalATLogger &logger = audioThread.GetLogger();
	decString text;
	int i, j, k;
	
	logger.LogInfoFormat("Panning Collector: Resolution %d", pResolution);
	for(i=0; i<6; i++){
		const float * const facePixels = pPixels.GetArrayPointer() + pPixelsPerFace * i;
		logger.LogInfoFormat("- Face %s", faceNames[i]);
		for(j=0; j<pResolution; j++){
			const float * const rowPixels = facePixels + pResolution * j;;
			text.Format("  - Row %02d: [", j);
			for(k=0; k<pResolution; k++){
				if(k > 0){
					text += ", ";
				}
				text.AppendFormat("%.3f", rowPixels[k]);
			}
			text += "]";
			logger.LogInfo(text);
		}
	}
}



// Operators
//////////////

deoalPanningCollector &deoalPanningCollector::operator=(const deoalPanningCollector &collector){
	DEASSERT_TRUE(collector.pResolution == pResolution)
	memcpy(pPixels.GetArrayPointer(), collector.pPixels.GetArrayPointer(), sizeof(float) * pPixels.GetCount());
	return *this;
}

deoalPanningCollector &deoalPanningCollector::operator+=(const deoalPanningCollector &collector){
	if(collector.pResolution != pResolution){
		DETHROW(deeInvalidParam);
	}
	
	pPixels.VisitIndexed([&](int i, float &value){
		const float otherValue = collector.pPixels[i];
		if(otherValue > value){
			value = otherValue;
		}
	});
	
	return *this;
}
