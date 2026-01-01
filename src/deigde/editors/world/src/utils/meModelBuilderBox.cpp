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

#include "meModelBuilderBox.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelFace.h>
#include <dragengine/resources/model/deModelTexture.h>
#include <dragengine/resources/model/deModelVertex.h>
#include <dragengine/resources/model/deModelLOD.h>
#include <dragengine/resources/model/deModelTextureCoordinatesSet.h>



// Class meModelBuilderBox
////////////////////////////

// Constructor, destructor
////////////////////////////

meModelBuilderBox::meModelBuilderBox(float size){
	if(size < FLOAT_SAFE_EPSILON){
		pSize = FLOAT_SAFE_EPSILON;
		
	}else{
		pSize = size;
	}
}

meModelBuilderBox::~meModelBuilderBox(){
}



// Management
///////////////

void meModelBuilderBox::BuildModel(deModel *model){
	deModelTexture *modelTexture = nullptr;
	const float halfSize = pSize * 0.5f;
	deModelLOD *modelLOD = nullptr;
	
	// we need a lod mesh and a dummy texture
	try{
		modelLOD = new deModelLOD;
		model->AddLOD(modelLOD);
		
		modelTexture = new deModelTexture("dummy", 1, 1);
		model->AddTexture(modelTexture);
		
	}catch(const deException &){
		if(modelTexture){
			delete modelTexture;
		}
		if(modelLOD){
			delete modelLOD;
		}
		
		throw;
	}
	
	// add vertices
	modelLOD->SetVertexCount(8);
	modelLOD->GetVertexAt(0).SetPosition(decVector(-halfSize,  halfSize,  halfSize));
	modelLOD->GetVertexAt(1).SetPosition(decVector(halfSize,  halfSize,  halfSize));
	modelLOD->GetVertexAt(2).SetPosition(decVector(halfSize, -halfSize,  halfSize));
	modelLOD->GetVertexAt(3).SetPosition(decVector(-halfSize, -halfSize,  halfSize));
	modelLOD->GetVertexAt(4).SetPosition(decVector(-halfSize,  halfSize, -halfSize));
	modelLOD->GetVertexAt(5).SetPosition(decVector(halfSize,  halfSize, -halfSize));
	modelLOD->GetVertexAt(6).SetPosition(decVector(halfSize, -halfSize, -halfSize));
	modelLOD->GetVertexAt(7).SetPosition(decVector(-halfSize, -halfSize, -halfSize));
	
	// add normals and tangents
	modelLOD->SetNormalCount(6);
	modelLOD->SetTangentCount(6);
	
	// add texture coordinates
	model->GetTextureCoordinatesSetList().Add("default");
	modelLOD->SetTextureCoordinatesCount(4);
	modelLOD->SetTextureCoordinatesSetCount(1);
	
	deModelTextureCoordinatesSet &tcset = modelLOD->GetTextureCoordinatesSetAt(0);
	tcset.SetTextureCoordinatesCount(4);
	tcset.SetTextureCoordinatesAt(0, decVector2(0.0f, 0.0f));
	tcset.SetTextureCoordinatesAt(1, decVector2(1.0f, 0.0f));
	tcset.SetTextureCoordinatesAt(2, decVector2(1.0f, 1.0f));
	tcset.SetTextureCoordinatesAt(3, decVector2(0.0f, 1.0f));
	
	// add faces
	const int indices[] = {0, 1, 2, 3,   1, 5, 6, 2,    5, 4, 7, 6,   4, 0, 3, 7,    4, 5, 1, 0,    3, 2, 6, 7};
	const int normals[] = {0, 0, -1,     1, 0, 0,       0, 0, 1,      -1, 0, 0,      0, 1, 0,       0, -1, 0};
	const int tangents[] = {1, 0, 0,     0, 0, 1,       -1, 0, 0,     0, 0, -1,      1, 0, 0,       -1, 0, 0};
	int i;
	
	modelLOD->SetFaceCount(12);
	
	for(i=0; i<12; i++){
		const int side = i / 2;
		const int * const corners = &indices[side * 4];
		const int * const fnormal = &normals[side * 3];
		const int * const ftangent = &tangents[side * 3];
		deModelFace &face = modelLOD->GetFaceAt(i);
		
		face.SetNormal1(side);
		face.SetNormal2(side);
		face.SetNormal3(side);
		face.SetTangent1(side);
		face.SetTangent2(side);
		face.SetTangent3(side);
		face.SetFaceNormal(decVector((float)fnormal[0], (float)fnormal[1], (float)fnormal[2]));
		face.SetFaceTangent(decVector((float)ftangent[0], (float)ftangent[1], (float)ftangent[2]));
		face.SetVertex1(corners[0]);
		face.SetTextureCoordinates1(0);
		
		if(i % 1 == 0){
			face.SetVertex2(corners[1]);
			face.SetVertex3(corners[2]);
			face.SetTextureCoordinates2(1);
			face.SetTextureCoordinates3(2);
			
		}else{
			face.SetVertex2(corners[2]);
			face.SetVertex3(corners[3]);
			face.SetTextureCoordinates2(3);
			face.SetTextureCoordinates3(2);
		}
	}
}
