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

#include "dedaiNavMeshBuilder.h"
#include "dedaiNavMeshBuilderEdge.h"
#include "dedaiNavMeshBuilderFace.h"

#include <dragengine/common/exceptions.h>



// Class dedaiNavMeshBuilder
//////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dedaiNavMeshBuilder::dedaiNavMeshBuilder(){
}

dedaiNavMeshBuilder::~dedaiNavMeshBuilder(){
	Clear();
}



// Management
///////////////

dedaiNavMeshBuilderEdge *dedaiNavMeshBuilder::GetEdgeWith(int vertex1, int vertex2){
	dedaiNavMeshBuilderEdge *edge = NULL;
	const int count = pEdges.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		edge = (dedaiNavMeshBuilderEdge*)pEdges.GetAt(i);
		
		if((edge->GetVertex1() == vertex1 && edge->GetVertex2() == vertex2)
		||  (edge->GetVertex1() == vertex2 && edge->GetVertex2() == vertex1)){
			return edge;
		}
	}
	
	edge = NULL;
	
	try{
		edge = new dedaiNavMeshBuilderEdge(vertex1, vertex2);
		pEdges.Add(edge);
		
	}catch(const deException &){
		if(edge){
			delete edge;
		}
		throw;
	}
	
	return edge;
}

dedaiNavMeshBuilderFace *dedaiNavMeshBuilder::AddFace(){
	dedaiNavMeshBuilderFace *face = NULL;
	
	try{
		face = new dedaiNavMeshBuilderFace;
		pFaces.Add(face);
		
	}catch(const deException &){
		if(face){
			delete face;
		}
		throw;
	}
	
	return face;
}

void dedaiNavMeshBuilder::UpdateIndices(){
	int i, count;
	
	count = pEdges.GetCount();
	for(i=0; i<count; i++){
		((dedaiNavMeshBuilderEdge*)pEdges.GetAt(i))->SetIndex(i);
	}
	
	count = pFaces.GetCount();
	for(i=0; i<count; i++){
		((dedaiNavMeshBuilderFace*)pFaces.GetAt(i))->SetIndex(i);
	}
}

void dedaiNavMeshBuilder::Clear(){
	int i, count;
	
	count = pFaces.GetCount();
	for(i=0; i<count; i++){
		delete (dedaiNavMeshBuilderFace*)pFaces.GetAt(i);
	}
	pFaces.RemoveAll();
	
	count = pEdges.GetCount();
	for(i=0; i<count; i++){
		delete (dedaiNavMeshBuilderEdge*)pEdges.GetAt(i);
	}
	pEdges.RemoveAll();
}
