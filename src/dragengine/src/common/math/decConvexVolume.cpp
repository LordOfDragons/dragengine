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

#include "decConvexVolume.h"
#include "decConvexVolumeFace.h"
#include "../exceptions.h"


// Class decConvexVolume
//////////////////////////

// Constructors and Destructors
/////////////////////////////////

decConvexVolume::~decConvexVolume() = default;


// Vertex Management
//////////////////////

const decVector &decConvexVolume::GetVertexAt(int index) const{
	return pVertices.GetAt(index);
}

bool decConvexVolume::HasVertex(const decVector &vertex) const{
	return pVertices.Has(vertex);
}

int decConvexVolume::IndexOfVertex(const decVector &vertex) const{
	return pVertices.IndexOf(vertex);
}

void decConvexVolume::AddVertex(const decVector &vertex){
	pVertices.Add(vertex);
}

void decConvexVolume::RemoveVertex(int index){
	pVertices.RemoveFrom(index);
}

void decConvexVolume::RemoveAllVertices(){
	pVertices.RemoveAll();
}


// Management
///////////////

const decConvexVolumeFace::Ref &decConvexVolume::GetFaceAt(int index) const{
	return pFaces.GetAt(index);
}

bool decConvexVolume::HasFace(decConvexVolumeFace *face) const{
	return pFaces.Has(face);
}

int decConvexVolume::IndexOfFace(decConvexVolumeFace *face) const{
	return pFaces.IndexOf(face);
}

void decConvexVolume::AddFace(decConvexVolumeFace::Ref &&face){
	DEASSERT_NOTNULL(face)
	DEASSERT_FALSE(HasFace(face))
	
	pFaces.Add(std::move(face));
}

void decConvexVolume::RemoveFace(decConvexVolumeFace *face){
	pFaces.RemoveFrom(pFaces.IndexOf(face));
}

void decConvexVolume::RemoveAllFaces(){
	pFaces.RemoveAll();
}


// Management
///////////////

void decConvexVolume::SetEmpty(){
	RemoveAllFaces();
	RemoveAllVertices();
}

void decConvexVolume::SetToCube(const decVector &halfSize){
	if(halfSize < decVector(0.0f, 0.0f, 0.0f)) DETHROW(deeInvalidParam);
	
	// remove all faces and vertices
	SetEmpty();
	
	// add the cube corners
	AddVertex(decVector(-halfSize.x, halfSize.y, -halfSize.z));
	AddVertex(decVector(halfSize.x, halfSize.y, -halfSize.z));
	AddVertex(decVector(halfSize.x, -halfSize.y, -halfSize.z));
	AddVertex(decVector(-halfSize.x, -halfSize.y, -halfSize.z));
	AddVertex(decVector(-halfSize.x, halfSize.y, halfSize.z));
	AddVertex(decVector(halfSize.x, halfSize.y, halfSize.z));
	AddVertex(decVector(halfSize.x, -halfSize.y, halfSize.z));
	AddVertex(decVector(-halfSize.x, -halfSize.y, halfSize.z));
	
	// add front face
	decConvexVolumeFace::Ref face = decConvexVolumeFace::Ref::New();
	face->SetNormal(decVector(0.0f, 0.0f, -1.0f));
	face->AddVertex(0);
	face->AddVertex(1);
	face->AddVertex(2);
	face->AddVertex(3);
	AddFace(std::move(face));
	
	// add right face
	face = decConvexVolumeFace::Ref::New();
	face->SetNormal(decVector(1.0f, 0.0f, 0.0f));
	face->AddVertex(1);
	face->AddVertex(5);
	face->AddVertex(6);
	face->AddVertex(2);
	AddFace(std::move(face));
	
	// add back face
	face = decConvexVolumeFace::Ref::New();
	face->SetNormal(decVector(0.0f, 0.0f, 1.0f));
	face->AddVertex(5);
	face->AddVertex(4);
	face->AddVertex(7);
	face->AddVertex(6);
	AddFace(std::move(face));
	
	// add left face
	face = decConvexVolumeFace::Ref::New();
	face->SetNormal(decVector(-1.0f, 0.0f, 0.0f));
	face->AddVertex(4);
	face->AddVertex(0);
	face->AddVertex(3);
	face->AddVertex(7);
	AddFace(std::move(face));
	
	// add top face
	face = decConvexVolumeFace::Ref::New();
	face->SetNormal(decVector(0.0f, 1.0f, 0.0f));
	face->AddVertex(4);
	face->AddVertex(5);
	face->AddVertex(1);
	face->AddVertex(0);
	AddFace(std::move(face));
	
	// add bottom face
	face = decConvexVolumeFace::Ref::New();
	face->SetNormal(decVector(0.0f, -1.0f, 0.0f));
	face->AddVertex(3);
	face->AddVertex(2);
	face->AddVertex(6);
	face->AddVertex(7);
	AddFace(std::move(face));
}

void decConvexVolume::Move(const decVector &direction){
	pVertices.Visit([&](decVector &v){
		v += direction;
	});
}
