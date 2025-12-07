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

#include <stdlib.h>
#include <string.h>

#include "deoalRayTraceConfig.h"
#include "deoalIcoSphere.h"

#include <dragengine/common/exceptions.h>



// Class deoalRayTraceConfig
//////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalRayTraceConfig::deoalRayTraceConfig() :
pRayCount(0),
pRayDirections(NULL),
pRayUnitVolume(0.0f),
pRayUnitSurface(0.0f),
pOpeningAngle(0.0f){
}

deoalRayTraceConfig::deoalRayTraceConfig(const deoalRayTraceConfig &config) :
pRayCount(0),
pRayDirections(NULL),
pRayUnitVolume(config.pRayUnitVolume),
pRayUnitSurface(config.pRayUnitSurface),
pOpeningAngle(config.pOpeningAngle)
{
	if(config.pRayCount == 0){
		return;
	}
	
	pRayDirections = new decVector[config.pRayCount];
	memcpy(pRayDirections, config.pRayDirections, sizeof(decVector) * config.pRayCount);
	pRayCount = config.pRayCount;
}

deoalRayTraceConfig::~deoalRayTraceConfig(){
	if(pRayDirections){
		delete [] pRayDirections;
	}
}



// Manegement
///////////////

void deoalRayTraceConfig::SetFromVertices(const decVector *vertices, int count, float openingAngle){
	if(! vertices || count < 1){
		DETHROW(deeInvalidParam);
	}
	
	if(pRayDirections){
		delete [] pRayDirections;
		pRayDirections = NULL;
	}
	
	pRayDirections = new decVector[count];
	memcpy(pRayDirections, vertices, sizeof(decVector) * count);
	pRayCount = count;
	
	// V = 4/3 * PI * r^3
	// A = 4 * PI * r^2
	// V and A distributed equally on all rays. r=1 for these values. during final
	// calculation the radius will factorized into them.
	pRayUnitSurface = 4.0f * PI / (float)count;
	pRayUnitVolume = pRayUnitSurface / 3.0f;
	
	pOpeningAngle = openingAngle;
}

void deoalRayTraceConfig::Rotate(float rx, float ry, float rz){
	const decMatrix matrix(decMatrix::CreateRotation(rx * DEG2RAD, ry * DEG2RAD, rz * DEG2RAD));
	int i;
	for(i=0; i<pRayCount; i++){
		pRayDirections[i] = matrix * pRayDirections[i];
	}
}

void deoalRayTraceConfig::SetRaysEquallySpaced(int rayCount){
	DEASSERT_TRUE(rayCount > 0)
	
	if(pRayDirections){
		delete [] pRayDirections;
		pRayDirections = NULL;
	}
	
	pRayCount = 0;
	pRayDirections = new decVector[rayCount];
	
	const float dLongitude = PI * (3.0f - sqrtf(5.0f));
	const float dZ = 2.0f / (float)rayCount;
	float longitude = 0.0f;
	float z = 1.0f - dZ * 0.5f;
	
	for(pRayCount=0; pRayCount<rayCount; pRayCount++){
		const float radius = sqrtf(1.0f - z * z);
		pRayDirections[pRayCount].x = cosf(longitude) * radius;
		pRayDirections[pRayCount].y = sinf(longitude) * radius;
		pRayDirections[pRayCount].z = (float)z;
		z -= dZ;
		longitude += dLongitude;
	}
	
	// V = 4/3 * PI * r^3
	// A = 4 * PI * r^2
	// V and A distributed equally on all rays. r=1 for these values. during final
	// calculation the radius will factorized into them.
	pRayUnitSurface = 4.0f * PI / (float)rayCount;
	pRayUnitVolume = pRayUnitSurface / 3.0f;
	
	// gauss factor is tan(openingAngle) * 0.466
	if(rayCount > 2){
		pOpeningAngle = atanf((pRayDirections[2] - pRayDirections[1]).Length()) * 2.0f;

	}else{
		pOpeningAngle = TWO_PI;
	}
}

void deoalRayTraceConfig::SetFromIcoSphere(const deoalIcoSphere &icoSphere){
	SetFromVertices(icoSphere.GetVertices(), icoSphere.GetVertexCount(), icoSphere.GetOpeningAngle());
}
