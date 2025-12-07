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

#include "dedaiPathFinderFunnel.h"

#include <dragengine/common/exceptions.h>



// Class dedaiPathFinderFunnel
////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dedaiPathFinderFunnel::dedaiPathFinderFunnel(){
	pMesh = NULL;
	pLeftDist = 0.0f;
	pRightDist = 0.0f;
}

dedaiPathFinderFunnel::~dedaiPathFinderFunnel(){
}



// Management
///////////////

void dedaiPathFinderFunnel::SetMesh(dedaiSpaceMesh *mesh){
	pMesh = mesh;
}

void dedaiPathFinderFunnel::SetOrigin(const decVector &origin){
	pOrigin = origin;
	//pLeftDist = origin * pLeftNormal;
	//pRightDist = origin * pRightNormal;
}

void dedaiPathFinderFunnel::SetGoal(const decVector &goal){
	pGoal = goal;
}

void dedaiPathFinderFunnel::SetLeftCorner(const decVector &corner){
	pLeftCorner = corner;
}

void dedaiPathFinderFunnel::SetLeftNormal(const decVector &normal){
	pLeftNormal = normal;
	pLeftDist = pOrigin * normal;
}

void dedaiPathFinderFunnel::SetRightCorner(const decVector &corner){
	pRightCorner = corner;
}

void dedaiPathFinderFunnel::SetRightNormal(const decVector &normal){
	pRightNormal = normal;
	pRightDist = pOrigin * normal;
}



void dedaiPathFinderFunnel::UpdateLeftPlane(const decVector &faceNormal){
	float len;
	
	pLeftNormal = faceNormal % (pLeftCorner - pOrigin);
	len = pLeftNormal.Length();
	if(len > FLOAT_SAFE_EPSILON){
		pLeftNormal /= len;
		
	}else{
		pLeftNormal = faceNormal % (pLeftCorner - pRightCorner);
		len = pLeftNormal.Length();
		if(len > FLOAT_SAFE_EPSILON){
			pLeftNormal /= len;
			
		}else{
			pLeftNormal.SetZero();
		}
	}
	pLeftDist = pOrigin * pLeftNormal;
}

void dedaiPathFinderFunnel::UpdateRightPlane(const decVector &faceNormal){
	float len;
	
	pRightNormal = faceNormal % (pOrigin - pRightCorner);
	len = pRightNormal.Length();
	if(len > FLOAT_SAFE_EPSILON){
		pRightNormal /= len;
		
	}else{
		pRightNormal = faceNormal % (pRightCorner - pLeftCorner);
		len = pRightNormal.Length();
		if(len > FLOAT_SAFE_EPSILON){
			pRightNormal /= len;
			
		}else{
			pRightNormal.SetZero();
		}
	}
	pRightDist = pOrigin * pRightNormal;
}

void dedaiPathFinderFunnel::Transform(const decMatrix &matrix){
	pOrigin = matrix * pOrigin;
	
	pLeftCorner = matrix * pLeftCorner;
	pLeftNormal = matrix.TransformNormal(pLeftNormal);
	pLeftDist = pOrigin * pLeftNormal;
	
	pRightCorner = matrix * pRightCorner;
	pRightNormal = matrix.TransformNormal(pRightNormal);
	pRightDist = pOrigin * pRightNormal;
}
