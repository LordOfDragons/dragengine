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
#include <stdlib.h>
#include <string.h>

#include "meModelBuilderNavSpace.h"

#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelFace.h>
#include <dragengine/resources/model/deModelTexture.h>
#include <dragengine/resources/model/deModelVertex.h>
#include <dragengine/resources/model/deModelLOD.h>
#include <dragengine/resources/model/deModelTextureCoordinatesSet.h>
#include <dragengine/resources/navigation/space/deNavigationSpace.h>
#include <dragengine/resources/navigation/space/deNavigationSpaceEdge.h>
#include <dragengine/resources/navigation/space/deNavigationSpaceCorner.h>
#include <dragengine/resources/navigation/space/deNavigationSpaceFace.h>
#include <dragengine/resources/navigation/space/deNavigationSpaceWall.h>
#include <dragengine/resources/navigation/space/deNavigationSpaceRoom.h>
#include <dragengine/common/exceptions.h>



// Class meModelBuilderNavSpace
////////////////////////////////

// Constructor, destructor
////////////////////////////

meModelBuilderNavSpace::meModelBuilderNavSpace(deNavigationSpace *navspace){
	if(! navspace){
		DETHROW(deeInvalidParam);
	}
	
	pNavSpace = navspace;
}

meModelBuilderNavSpace::~meModelBuilderNavSpace(){
}



// Management
///////////////

void meModelBuilderNavSpace::BuildModel(deModel *model){
	if(pNavSpace->GetType() == deNavigationSpace::estGrid){
		BuildNavGrid(model);
		
	}else if(pNavSpace->GetType() == deNavigationSpace::estMesh){
		BuildNavMesh(model);
	}
}



void meModelBuilderNavSpace::BuildNavGrid(deModel* model){
	const int boxVertexIndices[24] = {0,4,5,1, 1,5,6,2, 2,6,7,3, 3,7,4,0, 0,1,2,3, 4,5,6,7};
	const decVector axisX = decVector(1.0f, 0.0f, 0.0f);
	const decVector axisY = decVector(0.0f, 1.0f, 0.0f);
	const deNavigationSpaceEdge * const edges = pNavSpace->GetEdges();
	const decVector * const vertices = pNavSpace->GetVertices();
	const int edgeCount = pNavSpace->GetEdgeCount();
	deModelTexture *modelTexture = NULL;
	const float halfExtend = 0.05f;
	int indexNormal, indexVertex;
	deModelLOD *modelLOD = NULL;
	decVector boxVertices[8];
	decVector boxNormals[6];
	decMatrix matrix;
	decVector view;
	float len;
	int v, e;
	
	// we need a lod mesh and a dummy texture
	try{
		modelLOD = new deModelLOD;
		model->AddLOD(modelLOD);
		
		modelTexture = new deModelTexture("skin", 1, 1);
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
	
	// add edges. we add them as a box. this is not perfect but for collision testing this is enough
	model->GetTextureCoordinatesSetList().Add("default");
	
	modelLOD->SetVertexCount(edgeCount * 8);
	modelLOD->SetFaceCount(edgeCount * 12);
	modelLOD->SetNormalCount(edgeCount * 6);
	modelLOD->SetTangentCount(edgeCount * 6);
	modelLOD->SetTextureCoordinatesCount(modelLOD->GetVertexCount());
	modelLOD->SetTextureCoordinatesSetCount(1);
	
	deModelTextureCoordinatesSet &tcset = modelLOD->GetTextureCoordinatesSetAt(0);
	tcset.SetTextureCoordinatesCount(modelLOD->GetVertexCount());
	
	for(e=0; e<edgeCount; e++){
		const decVector &bp1 = vertices[edges[e].GetVertex1()];
		const decVector &bp2 = vertices[edges[e].GetVertex2()];
		
		view = bp2 - bp1;
		len = view.Length();
		if(len < FLOAT_SAFE_EPSILON){
			view.Set(0.0f, 0.0f, 1.0f);
			
		}else{
			view /= len;
		}
		
		if(fabsf(view * axisY) < 0.999f){
			matrix.SetWorld(bp1, view, axisY);
			
		}else{
			matrix.SetWorld(bp1, view, axisX);
		}
		
		boxVertices[0] = matrix.Transform(-halfExtend, halfExtend, -halfExtend);
		boxVertices[1] = matrix.Transform(halfExtend, halfExtend, -halfExtend);
		boxVertices[2] = matrix.Transform(halfExtend, -halfExtend, -halfExtend);
		boxVertices[3] = matrix.Transform(-halfExtend, -halfExtend, -halfExtend);
		boxVertices[4] = matrix.Transform(-halfExtend, halfExtend, len + halfExtend);
		boxVertices[5] = matrix.Transform(halfExtend, halfExtend, len + halfExtend);
		boxVertices[6] = matrix.Transform(halfExtend, -halfExtend, len + halfExtend);
		boxVertices[7] = matrix.Transform(-halfExtend, -halfExtend, len + halfExtend);
		boxNormals[0] = matrix.TransformUp();
		boxNormals[1] = matrix.TransformRight();
		boxNormals[2] = -matrix.TransformUp();
		boxNormals[3] = -matrix.TransformRight();
		boxNormals[4] = -matrix.TransformView();
		boxNormals[5] = matrix.TransformView();
		
		indexVertex = e * 8;
		for(v=0; v<8; v++){
			modelLOD->GetVertexAt(indexVertex + v).SetPosition(boxVertices[v]);
			tcset.SetTextureCoordinatesAt(indexVertex + v, decVector2(boxVertices[v].x, boxVertices[v].z));
		}
		
		for(v=0; v<6; v++){
			const int fp1 = indexVertex + boxVertexIndices[v * 4];
			const int fp2 = indexVertex + boxVertexIndices[v * 4 + 1];
			const int fp3 = indexVertex + boxVertexIndices[v * 4 + 2];
			const int fp4 = indexVertex + boxVertexIndices[v * 4 + 3];
			//const decVector &tp1 = boxVertices[ boxVertexIndices[ v * 4 ] ];
			//const decVector &tp2 = boxVertices[ boxVertexIndices[ v * 4 + 1 ] ];
			//const decVector &tp3 = boxVertices[ boxVertexIndices[ v * 4 + 2 ] ];
			//const decVector &tp4 = boxVertices[ boxVertexIndices[ v * 4 + 3 ] ];
			const decVector &fnor = boxNormals[v];
			
			indexNormal = e * 6 + v;
			
			deModelFace &modelFace1 = modelLOD->GetFaceAt(e * 12 + v * 2);
			modelFace1.SetVertex1(fp1);
			modelFace1.SetVertex2(fp2);
			modelFace1.SetVertex3(fp3);
			modelFace1.SetNormal1(indexNormal);
			modelFace1.SetNormal2(indexNormal);
			modelFace1.SetNormal3(indexNormal);
			modelFace1.SetTangent1(indexNormal);
			modelFace1.SetTangent2(indexNormal);
			modelFace1.SetTangent3(indexNormal);
			modelFace1.SetTextureCoordinates1(fp1); //decVector2(tp1.x, tp1.z));
			modelFace1.SetTextureCoordinates2(fp2); //decVector2(tp2.x, tp2.z));
			modelFace1.SetTextureCoordinates3(fp3); //decVector2(tp3.x, tp3.z));
			modelFace1.SetFaceNormal(fnor);
			modelFace1.SetFaceTangent(axisX);
			
			deModelFace &modelFace2 = modelLOD->GetFaceAt(e * 12 + v * 2 + 1);
			modelFace2.SetVertex1(fp1);
			modelFace2.SetVertex2(fp3);
			modelFace2.SetVertex3(fp4);
			modelFace2.SetNormal1(indexNormal);
			modelFace2.SetNormal2(indexNormal);
			modelFace2.SetNormal3(indexNormal);
			modelFace2.SetTangent1(indexNormal);
			modelFace2.SetTangent2(indexNormal);
			modelFace2.SetTangent3(indexNormal);
			modelFace2.SetTextureCoordinates1(fp1); //decVector2(tp1.x, tp1.z));
			modelFace2.SetTextureCoordinates2(fp2); //decVector2(tp3.x, tp3.z));
			modelFace2.SetTextureCoordinates3(fp3); //decVector2(tp4.x, tp4.z));
			modelFace2.SetFaceNormal(fnor);
			modelFace2.SetFaceTangent(axisX);
		}
	}
}

void meModelBuilderNavSpace::BuildNavMesh(deModel* model){
	const deNavigationSpaceFace * const faces = pNavSpace->GetFaces();
	const decVector * const vertices = pNavSpace->GetVertices();
	const decVector tangent = decVector(1.0f, 0.0f, 0.0f);
	const int vertexCount = pNavSpace->GetVertexCount();
	const int faceCount = pNavSpace->GetFaceCount();
	deModelTexture *modelTexture = NULL;
	deNavigationSpaceCorner *corners;
	deModelLOD *modelLOD = NULL;
	int totalFaceCount = 0;
	int normalCount = 0;
	decVector normal;
	int v, f, c;
	float len;
	
	// we need a lod mesh and a dummy texture
	try{
		modelLOD = new deModelLOD;
		model->AddLOD(modelLOD);
		
		modelTexture = new deModelTexture("skin", 1, 1);
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
	
	// add faces. in contrary to navigation spaces models can only have triangles.
	// faces are split into triangle fans. every model face has its own set of normals
	// and tangents. hence all faces are considered to not share any normals or tangents.
	// texture coordinates are set to a top down projection just in case somebody
	// has the glorious idea to place a texture on it
	totalFaceCount = 0;
	
	for(f=0; f<faceCount; f++){
		const deNavigationSpaceFace &face = faces[f];
		
		if(face.GetCornerCount() > 2){
			totalFaceCount += face.GetCornerCount() - 2;
		}
	}
	
	model->GetTextureCoordinatesSetList().Add("default");
	
	modelLOD->SetVertexCount(vertexCount);
	modelLOD->SetFaceCount(totalFaceCount);
	modelLOD->SetTextureCoordinatesCount(modelLOD->GetVertexCount());
	modelLOD->SetTextureCoordinatesSetCount(1);
	
	deModelTextureCoordinatesSet &tcset = modelLOD->GetTextureCoordinatesSetAt(0);
	tcset.SetTextureCoordinatesCount(modelLOD->GetVertexCount());
	
	for(v=0; v<vertexCount; v++){
		modelLOD->GetVertexAt(v).SetPosition(vertices[v]);
		tcset.SetTextureCoordinatesAt(v, decVector2(vertices[v].x, vertices[v].z));
	}
	
	corners = pNavSpace->GetCorners();
	totalFaceCount = 0;
	
	for(f=0; f<faceCount; f++){
		const deNavigationSpaceFace &face = faces[f];
		const int cornerCount = face.GetCornerCount();
		
		if(cornerCount > 2){
			const decVector &bp1 = vertices[corners[0].GetVertex()];
			const decVector &bp2 = vertices[corners[1].GetVertex()];
			const decVector &bp3 = vertices[corners[2].GetVertex()];
			//const decVector2 &tp1 = decVector2( decVector2( bp1.x, bp1.z ) );
			
			normal = (bp2 - bp1) % (bp3 - bp2);
			len = normal.Length();
			if(len > FLOAT_SAFE_EPSILON){
				normal /= len;
				
			}else{
				normal.Set(0.0f, 1.0f, 0.0f);
			}
			
			for(c=2; c<cornerCount; c++){
				//const decVector &fp2 = vertices[ corners[ c - 1 ].GetVertex() ];
				//const decVector &fp3 = vertices[ corners[ c ].GetVertex() ];
				
				deModelFace &modelFace = modelLOD->GetFaceAt(totalFaceCount++);
				modelFace.SetVertex1(corners[0].GetVertex());
				modelFace.SetVertex2(corners[c - 1].GetVertex());
				modelFace.SetVertex3(corners[c].GetVertex());
				modelFace.SetNormal1(normalCount);
				modelFace.SetNormal2(normalCount);
				modelFace.SetNormal3(normalCount);
				modelFace.SetTangent1(normalCount);
				modelFace.SetTangent2(normalCount);
				modelFace.SetTangent3(normalCount);
				modelFace.SetTextureCoordinates1(modelFace.GetVertex1()); //tp1);
				modelFace.SetTextureCoordinates2(modelFace.GetVertex2()); //decVector2(fp2.x, fp2.z));
				modelFace.SetTextureCoordinates3(modelFace.GetVertex3()); //decVector2(fp3.x, fp3.z));
				modelFace.SetFaceNormal(normal);
				modelFace.SetFaceTangent(tangent);
			}
			
			normalCount++;
		}
		
		corners += cornerCount;
	}
	
	modelLOD->SetNormalCount(normalCount);
	modelLOD->SetTangentCount(normalCount);
}
