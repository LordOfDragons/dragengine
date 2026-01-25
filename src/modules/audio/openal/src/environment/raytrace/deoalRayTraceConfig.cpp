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

#include "deoalRayTraceConfig.h"
#include "deoalIcoSphere.h"

#include <dragengine/common/exceptions.h>



// Class deoalRayTraceConfig
//////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalRayTraceConfig::deoalRayTraceConfig() :
pRayUnitVolume(0.0f),
pRayUnitSurface(0.0f),
pOpeningAngle(0.0f){
}

deoalRayTraceConfig::deoalRayTraceConfig(const deoalRayTraceConfig &config) :
pRayDirections(config.pRayDirections),
pRayUnitVolume(config.pRayUnitVolume),
pRayUnitSurface(config.pRayUnitSurface),
pOpeningAngle(config.pOpeningAngle){
}

deoalRayTraceConfig::~deoalRayTraceConfig() = default;



// Manegement
///////////////

void deoalRayTraceConfig::SetFromVertices(const decVector *vertices, int count, float openingAngle){
	if(!vertices || count < 1){
		DETHROW(deeInvalidParam);
	}
	
	pRayDirections.SetCountDiscard(count);
	memcpy(pRayDirections.GetArrayPointer(), vertices, sizeof(decVector) * count);
	
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
	pRayDirections.Visit([&](decVector &d){
		d = matrix * d;
	});
}

void deoalRayTraceConfig::SetRaysEquallySpaced(int rayCount){
	DEASSERT_TRUE(rayCount > 0)
	
	pRayDirections.RemoveAll();
	pRayDirections.EnlargeCapacity(rayCount);
	
	const float dLongitude = PI * (3.0f - sqrtf(5.0f));
	const float dZ = 2.0f / (float)rayCount;
	float longitude = 0.0f;
	float z = 1.0f - dZ * 0.5f;
	int i;
	
	for(i=0; i<rayCount; i++){
		const float radius = sqrtf(1.0f - z * z);
		pRayDirections.Add({cosf(longitude) * radius, sinf(longitude) * radius, (float)z});
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
