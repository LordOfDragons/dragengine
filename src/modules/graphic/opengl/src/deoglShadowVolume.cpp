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

#if 0

// includes
#include <stdio.h>
#include <stdlib.h>
#include "deoglBasics.h"
#include "deoglShadowVolume.h"
#include "component/deoglMeshData.h"
#include "dragengine/common/exceptions.h"



// class deoglShadowVolume
////////////////////////////

// constructor, destructor
deoglShadowVolume::deoglShadowVolume( deoglRenderThread &renderThread ) :
pRenderThread( renderThread )
{
	pFaces = NULL;
	pFaceCount = 0;
	pFaceSize = 0;
	pEdges = NULL;
	pEdgeCount = 0;
	pEdgeSize = 0;
	pHasTris = false;
	pHasQuads = false;
}
deoglShadowVolume::~deoglShadowVolume(){
	if(pEdges) delete [] pEdges;
	if(pFaces) delete [] pFaces;
}

// management
void deoglShadowVolume::Clear(){
	pFaceCount = 0;
	pEdgeCount = 0;
}
void deoglShadowVolume::FindOpenFaces(){
	int f, e;
	bool hasChanged = true;
	// reset open flag on all faces
	for(f=0; f<pFaceCount; f++) pFaces[f].isOpen = false;
	// now we loop as long as we find edges which have one of their faces beeing open
	while(hasChanged){
		hasChanged = false;
		// loop through all edges and flag all faces open if needed
		for(e=0; e<pEdgeCount; e++){
			if( (pEdges[e].face[1] == -1 || pFaces[pEdges[e].face[1]].isOpen) && !pFaces[pEdges[e].face[0]].isOpen ){
				pFaces[pEdges[e].face[0]].isOpen = true;
				hasChanged = true;
			}else if( pFaces[pEdges[e].face[0]].isOpen && pEdges[e].face[1] != -1 && !pFaces[pEdges[e].face[1]].isOpen ){
				pFaces[pEdges[e].face[1]].isOpen = true;
				hasChanged = true;
			}
		}
	}
}
void deoglShadowVolume::FindPointSilhouette(const decVector &lightPos, deoglMeshData *mesh){
	int f;
	// determine which faces are hit by the light
	// ATTENTION! code changed. face normal is patch local and nor transformed!
	for(f=0; f<pFaceCount; f++){
		pFaces[f].isLit = (mesh->GetFaceNormal(f) * (lightPos - mesh->GetVertexPosition(pFaces[f].vertex[0]))) > 0;
	}
	// determine silhouette edges
	pFindEdgeSilhouette();
}
void deoglShadowVolume::FindDirectionalSilhouette(const decVector &lightDir, deoglMeshData *mesh){
	int f;
	// determine which faces are hit by the light
	// ATTENTION! code changed. face normal is patch local and nor transformed!
	for(f=0; f<pFaceCount; f++){
		pFaces[f].isLit = (mesh->GetFaceNormal(f) * lightDir) > 0;
	}
	// determine silhouette edges
	pFindEdgeSilhouette();
}
void deoglShadowVolume::RenderShadows(deoglMeshData *mesh, bool renderCaps){
	// first pass
	if(renderCaps){
		OGL_CHECK( pRenderThread, glCullFace(GL_FRONT) );
		OGL_CHECK( pRenderThread, glStencilOp(GL_KEEP, GL_INCR_WRAP, GL_KEEP) );
	}else{
		OGL_CHECK( pRenderThread, glCullFace(GL_BACK) );
		OGL_CHECK( pRenderThread, glStencilOp(GL_KEEP, GL_KEEP, GL_INCR_WRAP) );
	}
	pRenderShadowPass(mesh, renderCaps);
	// second pass
	if(renderCaps){
		OGL_CHECK( pRenderThread, glCullFace(GL_BACK) );
		OGL_CHECK( pRenderThread, glStencilOp(GL_KEEP, GL_DECR_WRAP, GL_KEEP) );
	}else{
		OGL_CHECK( pRenderThread, glCullFace(GL_FRONT) );
		OGL_CHECK( pRenderThread, glStencilOp(GL_KEEP, GL_KEEP, GL_DECR_WRAP) );
	}
	pRenderShadowPass(mesh, renderCaps);
}

// faces
const deoglShadowVolume::sFace &deoglShadowVolume::GetFace(int index) const{
	return pFaces[index];
}
void deoglShadowVolume::AddFace(int vertex1, int vertex2, int vertex3, int vertex4){
	int faceIndex = pFaceCount;
	// add face
	pAddFace(vertex1, vertex2, vertex3, vertex4);
	// add edges
	pAddEdge(faceIndex, vertex1, vertex2);
	pAddEdge(faceIndex, vertex2, vertex3);
	if(vertex4 == -1){
		pAddEdge(faceIndex, vertex3, vertex1);
		pHasTris = true;
	}else{
		pAddEdge(faceIndex, vertex3, vertex4);
		pAddEdge(faceIndex, vertex4, vertex1);
		pHasQuads = true;
	}
}
int deoglShadowVolume::GetOpenFaceCount() const{
	int i, count = 0;
	for(i=0; i<pFaceCount; i++){
		if(pFaces[i].isOpen) count++;
	}
	return count;
}
void deoglShadowVolume::CloseVolume(){
	int i, faceCount = pFaceCount;
	for( i=0; i<faceCount; i++ ){
		if( !pFaces[i].isOpen ) continue;
		if( pFaces[i].vertex[3] == -1 ){
			AddFace( pFaces[i].vertex[2], pFaces[i].vertex[1], pFaces[i].vertex[0], -1 );
		}else{
			AddFace( pFaces[i].vertex[3], pFaces[i].vertex[2], pFaces[i].vertex[1], pFaces[i].vertex[0] );
		}
	}
	FindOpenFaces();
//	printf( "[SHADOW] Open Faces after closing volume: %i\n", GetOpenFaceCount() );
}

// edges
const deoglShadowVolume::sEdge &deoglShadowVolume::GetEdge(int index) const{
	return pEdges[index];
}

// private functions
void deoglShadowVolume::pAddFace(int vertex1, int vertex2, int vertex3, int vertex4){
	// enlarge array if needed
	if(pFaceCount == pFaceSize){
		int newSize = pFaceCount * 3 / 2 + 1;
		sFace *newArray = new sFace[newSize];
		if(!newArray) DETHROW(deeOutOfMemory);
		if(pFaces){
			for(int i=0; i<pFaceCount; i++) newArray[i] = pFaces[i];
			delete [] pFaces;
		}
		pFaces = newArray;
		pFaceSize = newSize;
	}
	// add face
	pFaces[pFaceCount].vertex[0] = vertex1;
	pFaces[pFaceCount].vertex[1] = vertex2;
	pFaces[pFaceCount].vertex[2] = vertex3;
	pFaces[pFaceCount].vertex[3] = vertex4;
	pFaces[pFaceCount].isOpen = false;
	pFaces[pFaceCount].isLit = false;
	pFaceCount++;
}
void deoglShadowVolume::pAddEdge(int face, int vertex1, int vertex2){
	int edge = pFindEdge(vertex2, vertex1);
	// if the edge does not exist yet add it
	if(edge == -1){
		// enlarge array if needed
		if(pEdgeCount == pEdgeSize){
			int newSize = pEdgeCount * 3 / 2 + 1;
			sEdge *newArray = new sEdge[newSize];
			if(!newArray) DETHROW(deeOutOfMemory);
			if(pEdges){
				for(int i=0; i<pEdgeCount; i++) newArray[i] = pEdges[i];
				delete [] pEdges;
			}
			pEdges = newArray;
			pEdgeSize = newSize;
		}
		// add edge
		pEdges[pEdgeCount].vertex[0] = vertex1;
		pEdges[pEdgeCount].vertex[1] = vertex2;
		pEdges[pEdgeCount].face[0] = face;
		pEdges[pEdgeCount].face[1] = -1;
		pEdges[pEdgeCount].isSilhouette = false;
		pEdgeCount++;
	// otherwise assign the face to it
	}else{
		pEdges[edge].face[1] = face;
	}
}
int deoglShadowVolume::pFindEdge(int vertex1, int vertex2){
	int i;
	// find first edge that matches the vertices and has one or no face assigned
	for(i=0; i<pEdgeCount; i++){
		if( pEdges[i].vertex[0] == vertex1 && pEdges[i].vertex[1] == vertex2
		&& pEdges[i].face[1] == -1 ){
			return i;
		}
	}
	// no matching edge found
	return -1;
}
void deoglShadowVolume::pFindEdgeSilhouette(){
	int e;
	for(e=0; e<pEdgeCount; e++){
		if( pFaces[pEdges[e].face[0]].isLit ){
			pEdges[e].isSilhouette = pEdges[e].face[1] == -1 || !pFaces[pEdges[e].face[1]].isLit;
		}else{
			pEdges[e].isSilhouette = pEdges[e].face[1] != -1 && pFaces[pEdges[e].face[1]].isLit;
		}
	}
}
void deoglShadowVolume::pRenderShadowPass(deoglMeshData *mesh, bool renderCaps){
	decVector pt1, pt2, pt3, pt4;
	int e, f;
	float w;
	// render silhouette edges projected to infinity
	glBegin( GL_TRIANGLE_FAN );
	for(e=0; e<pEdgeCount; e++){
		if( !pEdges[e].isSilhouette ) continue;
		pt1 = mesh->GetVertexPosition(pEdges[e].vertex[0]);
		pt2 = mesh->GetVertexPosition(pEdges[e].vertex[1]);
		if( pFaces[pEdges[e].face[0]].isLit ){
			glVertex4f(pt2.x, pt2.y, pt2.z, 1);
			glVertex4f(pt2.x, pt2.y, pt2.z, 0);
			glVertex4f(pt1.x, pt1.y, pt1.z, 0);
			glVertex4f(pt1.x, pt1.y, pt1.z, 1);
		}else{
			glVertex4f(pt1.x, pt1.y, pt1.z, 1);
			glVertex4f(pt1.x, pt1.y, pt1.z, 0);
			glVertex4f(pt2.x, pt2.y, pt2.z, 0);
			glVertex4f(pt2.x, pt2.y, pt2.z, 1);
		}
	}
	glEnd();
	// render caps if needed
	if(renderCaps){
		// quad caps
		if(pHasQuads){
			glBegin( GL_TRIANGLE_FAN );
			for(f=0; f<pFaceCount; f++){
				if(pFaces[f].vertex[3] == -1) continue;
				if(pFaces[f].isOpen){
					if(!pFaces[f].isLit) continue;
					// fetch points
					pt1 = mesh->GetVertexPosition(pFaces[f].vertex[0]);
					pt2 = mesh->GetVertexPosition(pFaces[f].vertex[1]);
					pt3 = mesh->GetVertexPosition(pFaces[f].vertex[2]);
					pt4 = mesh->GetVertexPosition(pFaces[f].vertex[3]);
					// front face
					glVertex4f(pt4.x, pt4.y, pt4.z, 1);
					glVertex4f(pt3.x, pt3.y, pt3.z, 1);
					glVertex4f(pt2.x, pt2.y, pt2.z, 1);
					glVertex4f(pt1.x, pt1.y, pt1.z, 1);
					// back face only if open
					glVertex4f(pt1.x, pt1.y, pt1.z, 0);
					glVertex4f(pt2.x, pt2.y, pt2.z, 0);
					glVertex4f(pt3.x, pt3.y, pt3.z, 0);
					glVertex4f(pt4.x, pt4.y, pt4.z, 0);
				}else{
					w = pFaces[f].isLit ? 1 : 0;
					// fetch points
					pt1 = mesh->GetVertexPosition(pFaces[f].vertex[0]);
					pt2 = mesh->GetVertexPosition(pFaces[f].vertex[1]);
					pt3 = mesh->GetVertexPosition(pFaces[f].vertex[2]);
					pt4 = mesh->GetVertexPosition(pFaces[f].vertex[3]);
					// front face
					glVertex4f(pt4.x, pt4.y, pt4.z, w);
					glVertex4f(pt3.x, pt3.y, pt3.z, w);
					glVertex4f(pt2.x, pt2.y, pt2.z, w);
					glVertex4f(pt1.x, pt1.y, pt1.z, w);
				}
			}
			glEnd();
		}
		// tri caps
		if(pHasTris){
			glBegin(GL_TRIANGLES);
			for(f=0; f<pFaceCount; f++){
				if(pFaces[f].vertex[3] != -1) continue;
				if(pFaces[f].isOpen){
					if(!pFaces[f].isLit) continue;
					// fetch points
					pt1 = mesh->GetVertexPosition(pFaces[f].vertex[0]);
					pt2 = mesh->GetVertexPosition(pFaces[f].vertex[1]);
					pt3 = mesh->GetVertexPosition(pFaces[f].vertex[2]);
					// front face
					glVertex4f(pt3.x, pt3.y, pt3.z, 1);
					glVertex4f(pt2.x, pt2.y, pt2.z, 1);
					glVertex4f(pt1.x, pt1.y, pt1.z, 1);
					// back face only if open
					glVertex4f(pt1.x, pt1.y, pt1.z, 0);
					glVertex4f(pt2.x, pt2.y, pt2.z, 0);
					glVertex4f(pt3.x, pt3.y, pt3.z, 0);
				}else{
					w = pFaces[f].isLit ? 1 : 0;
					// fetch points
					pt1 = mesh->GetVertexPosition(pFaces[f].vertex[0]);
					pt2 = mesh->GetVertexPosition(pFaces[f].vertex[1]);
					pt3 = mesh->GetVertexPosition(pFaces[f].vertex[2]);
					// front face
					glVertex4f(pt3.x, pt3.y, pt3.z, w);
					glVertex4f(pt2.x, pt2.y, pt2.z, w);
					glVertex4f(pt1.x, pt1.y, pt1.z, w);
				}
			}
			glEnd();
		}
	}
}

#endif
