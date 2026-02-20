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

#include "deModelLOD.h"
#include "deModelWeight.h"
#include "deModelVertex.h"
#include "deModelFace.h"
#include "deModelTextureCoordinatesSet.h"
#include "deModelLodVertexPositionSet.h"
#include "../../common/exceptions.h"



// Class deModelLOD
/////////////////////

// Constructor, destructor
////////////////////////////

deModelLOD::deModelLOD() :
pNormalCount(0),
pTangentCount(0),
pTextureCoordinatesCount(0),
pHasLodError(false),
pLodError(0.01f){
}

deModelLOD::~deModelLOD(){
}



// Management
///////////////////////////

void deModelLOD::SetHasLodError(bool hasError){
	pHasLodError = hasError;
}

void deModelLOD::SetLodError(float error){
	pLodError = decMath::max(error, 0.001f);
}



// Face Management
////////////////////

void deModelLOD::SetNormalCount(int count){
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	
	pNormalCount = count;
}

void deModelLOD::SetTangentCount(int count){
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	
	pTangentCount = count;
}



// Texture Coordinates Sets
/////////////////////////////

void deModelLOD::SetTextureCoordinatesCount(int count){
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	
	pTextureCoordinatesCount = count;
}

