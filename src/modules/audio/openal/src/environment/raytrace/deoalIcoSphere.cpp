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

#include "deoalIcoSphere.h"

#include <dragengine/common/exceptions.h>



// Structs
////////////

void deoalIcoSphere::sFace::Set(int vertex1, int vertex2, int vertex3, int edge1, int edge2, int edge3){
	vertices[0] = (unsigned short)vertex1;
	vertices[1] = (unsigned short)vertex2;
	vertices[2] = (unsigned short)vertex3;
	edges[0] = (unsigned short)edge1;
	edges[1] = (unsigned short)edge2;
	edges[2] = (unsigned short)edge3;
}

void deoalIcoSphere::sEdge::Set(int vertex1, int vertex2){
	vertices[0] = (unsigned short)vertex1;
	vertices[1] = (unsigned short)vertex2;
}



// Class deoalIcoSphere
/////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalIcoSphere::deoalIcoSphere() :
pVertices(NULL),
pVertexCount(0),
pEdges(NULL),
pEdgeCount(0),
pFaces(NULL),
pFaceCount(0),
pOpeningAngle(0.0f){
}

deoalIcoSphere::deoalIcoSphere(const deoalIcoSphere &ico) :
pVertices(NULL),
pVertexCount(0),
pEdges(NULL),
pEdgeCount(0),
pFaces(NULL),
pFaceCount(0),
pOpeningAngle(ico.pOpeningAngle)
{
	*this = ico;
}

deoalIcoSphere::~deoalIcoSphere(){
	if(pFaces){
		delete [] pFaces;
	}
	if(pEdges){
		delete [] pEdges;
	}
	if(pVertices){
		delete [] pVertices;
	}
}

deoalIcoSphere deoalIcoSphere::BaseLevel(){
	deoalIcoSphere ico;
	
	ico.pVertexCount = 12;
	ico.pVertices = new decVector[12];
	ico.pVertices[0].Set(-0.000000f, -1.000000f, -0.000000f);
	ico.pVertices[1].Set(-0.723600f, -0.447215f, 0.525720f);
	ico.pVertices[2].Set(0.276385f, -0.447215f, 0.850640f);
	ico.pVertices[3].Set(0.894425f, -0.447215f, -0.000000f);
	ico.pVertices[4].Set(0.276385f, -0.447215f, -0.850640f);
	ico.pVertices[5].Set(-0.723600f, -0.447215f, -0.525720f);
	ico.pVertices[6].Set(-0.276385f, 0.447215f, 0.850640f);
	ico.pVertices[7].Set(0.723600f, 0.447215f, 0.525720f);
	ico.pVertices[8].Set(0.723600f, 0.447215f, -0.525720f);
	ico.pVertices[9].Set(-0.276385f, 0.447215f, -0.850640f);
	ico.pVertices[10].Set(-0.894425f, 0.447215f, -0.000000f);
	ico.pVertices[11].Set(-0.000000f, 1.000000f, -0.000000f);

	ico.pEdgeCount = 30;
	ico.pEdges = new sEdge[30];
	ico.pEdges[0].Set(2, 0);
	ico.pEdges[1].Set(0, 1);
	ico.pEdges[2].Set(1, 2);
	ico.pEdges[3].Set(5, 1);
	ico.pEdges[4].Set(0, 5);
	ico.pEdges[5].Set(3, 0);
	ico.pEdges[6].Set(2, 3);
	ico.pEdges[7].Set(4, 0);
	ico.pEdges[8].Set(3, 4);
	ico.pEdges[9].Set(4, 5);
	ico.pEdges[10].Set(10, 1);
	ico.pEdges[11].Set(5, 10);
	ico.pEdges[12].Set(6, 2);
	ico.pEdges[13].Set(1, 6);
	ico.pEdges[14].Set(7, 3);
	ico.pEdges[15].Set(2, 7);
	ico.pEdges[16].Set(8, 4);
	ico.pEdges[17].Set(3, 8);
	ico.pEdges[18].Set(9, 5);
	ico.pEdges[19].Set(4, 9);
	ico.pEdges[20].Set(10, 6);
	ico.pEdges[21].Set(6, 7);
	ico.pEdges[22].Set(7, 8);
	ico.pEdges[23].Set(8, 9);
	ico.pEdges[24].Set(9, 10);
	ico.pEdges[25].Set(11, 6);
	ico.pEdges[26].Set(10, 11);
	ico.pEdges[27].Set(11, 7);
	ico.pEdges[28].Set(11, 8);
	ico.pEdges[29].Set(11, 9);

	ico.pFaceCount = 20;
	ico.pFaces = new sFace[20];
	ico.pFaces[0].Set(0, 2, 1, 0, 2, 1);
	ico.pFaces[1].Set(1, 5, 0, 3, 4, 1);
	ico.pFaces[2].Set(0, 3, 2, 5, 6, 0);
	ico.pFaces[3].Set(0, 4, 3, 7, 8, 5);
	ico.pFaces[4].Set(0, 5, 4, 4, 9, 7);
	ico.pFaces[5].Set(1, 10, 5, 10, 11, 3);
	ico.pFaces[6].Set(2, 6, 1, 12, 13, 2);
	ico.pFaces[7].Set(3, 7, 2, 14, 15, 6);
	ico.pFaces[8].Set(4, 8, 3, 16, 17, 8);
	ico.pFaces[9].Set(5, 9, 4, 18, 19, 9);
	ico.pFaces[10].Set(1, 6, 10, 13, 20, 10);
	ico.pFaces[11].Set(2, 7, 6, 15, 21, 12);
	ico.pFaces[12].Set(3, 8, 7, 17, 22, 14);
	ico.pFaces[13].Set(4, 9, 8, 19, 23, 16);
	ico.pFaces[14].Set(5, 10, 9, 11, 24, 18);
	ico.pFaces[15].Set(6, 11, 10, 25, 26, 20);
	ico.pFaces[16].Set(7, 11, 6, 27, 25, 21);
	ico.pFaces[17].Set(8, 11, 7, 28, 27, 22);
	ico.pFaces[18].Set(9, 11, 8, 29, 28, 23);
	ico.pFaces[19].Set(10, 11, 9, 26, 29, 24);
	
	ico.pCalculateOpeningAngle();
	
	return ico;
}



// Manegement
///////////////

deoalIcoSphere deoalIcoSphere::Subdivide() const{
	// every face is split into 4 faces. this splits all edges in the middle and normalizes
	// them. to avoid vertices cluster around the split faces splitting a face does not
	// add the new vertices and faces straight to the new list. instead the first edge of
	// all faces is split and the vertices added. then the second edge of all faces is
	// split then the third. the same happens with the faces adding first the first split
	// face of each face then the second and so forth. this ensures that during the first,
	// second and third batch each face contributes one vertex which avoids clustering.
	deoalIcoSphere ico;
	
	ico.pFaceCount = pFaceCount * 4;
	ico.pFaces = new sFace[ico.pFaceCount];
	
	ico.pEdgeCount = pEdgeCount * 2 + pFaceCount * 3;
	ico.pEdges = new sEdge[ico.pEdgeCount];
	
	ico.pVertexCount = pVertexCount + pEdgeCount;
	ico.pVertices = new decVector[ico.pVertexCount];
	
	// copy vertices
	memcpy(ico.pVertices, pVertices, sizeof(decVector) * pVertexCount);
	
	// split
	const int oe = pEdgeCount;
	const int oef1 = oe + pEdgeCount;
	const int oef2 = oef1 + pFaceCount;
	const int oef3 = oef2 + pFaceCount;
	const int ov = pVertexCount;
	
	int i;
	for(i=0; i<pEdgeCount; i++){
		const int v1 = pEdges[i].vertices[0];
		const int v2 = pEdges[i].vertices[1];
		const int vn = ov + i;
		
		ico.pVertices[vn] = (pVertices[v1] + pVertices[v2]).Normalized();
		
		ico.pEdges[i].Set(v1, vn);
		ico.pEdges[oe + i].Set(vn, v2);
	}
	
	const int of1 = pFaceCount;
	const int of2 = of1 + pFaceCount;
	const int of3 = of2 + pFaceCount;
	
	for(i=0; i<pFaceCount; i++){
		const sFace &face = pFaces[i];
		const int e1 = face.edges[0];
		const int e2 = face.edges[1];
		const int e3 = face.edges[2];
		const int v1 = face.vertices[0];
		const int v2 = face.vertices[1];
		const int v3 = face.vertices[2];
		
		const bool flip1 = v1 == pEdges[e1].vertices[1];
		const bool flip2 = v2 == pEdges[e2].vertices[1];
		const bool flip3 = v3 == pEdges[e3].vertices[1];
		
		const int e1a = flip1 ? oe + e1 : e1;
		const int e1b = flip1 ? e1 : oe + e1;
		const int e2a = flip2 ? oe + e2 : e2;
		const int e2b = flip2 ? e2 : oe + e2;
		const int e3a = flip3 ? oe + e3 : e3;
		const int e3b = flip3 ? e3 : oe + e3;
		
		const int nef1 = oef1 + i;
		const int nef2 = oef2 + i;
		const int nef3 = oef3 + i;
		const int nv1 = ov + e1;
		const int nv2 = ov + e2;
		const int nv3 = ov + e3;
		
		ico.pEdges[nef1].Set(nv1, nv2);
		ico.pEdges[nef2].Set(nv2, nv3);
		ico.pEdges[nef3].Set(nv3, nv1);
		
		ico.pFaces[i].Set(v1, nv1, nv3, e1a, nef3, e3b);
		ico.pFaces[of1 + i].Set(v2, nv2, nv1, e2a, nef1, e1b);
		ico.pFaces[of2 + i].Set(v3, nv3, nv2, e3a, nef2, e2b);
		ico.pFaces[of3 + i].Set(nv1, nv2, nv3, nef1, nef2, nef3);
	}
	
	ico.pCalculateOpeningAngle();
	
	return ico;
}

#include "../../audiothread/deoalAudioThread.h"
#include "../../audiothread/deoalATLogger.h"
void deoalIcoSphere::DebugPrintBlender(deoalAudioThread &audioThread) const{
	decString text;
	text.Append("import bpy\n");
	text.Append("from mathutils import *\n");
	text.Append("from bpy_extras import object_utils\n");
	text.Append("mesh = bpy.data.meshes.new('import')\n");
	text.Append("obj = object_utils.object_data_add(bpy.context, mesh).object\n");
	text.Append("obj.location = Vector((0,0,0))\n");
	text.AppendFormat("mesh.vertices.add(%d)\n", pVertexCount);
	text.AppendFormat("mesh.loops.add(%d)\n", pFaceCount * 3);
	text.AppendFormat("mesh.polygons.add(%d)\n", pFaceCount);
// 	text.AppendFormat( "mesh.edges.add(%d)\n", pEdgeCount );
	int i;
	for(i=0; i<pVertexCount; i++){
		text.AppendFormat("mesh.vertices[%d].co = Vector((%f,%f,%f))\n", i,
			-pVertices[i].x, -pVertices[i].z, pVertices[i].y);
	}
	for(i=0; i<pFaceCount; i++){
		text.AppendFormat("mesh.loops[%d].vertex_index = %d\n", i*3, pFaces[i].vertices[2]);
		text.AppendFormat("mesh.loops[%d].vertex_index = %d\n", i*3+1, pFaces[i].vertices[1]);
		text.AppendFormat("mesh.loops[%d].vertex_index = %d\n", i*3+2, pFaces[i].vertices[0]);
		text.AppendFormat("mesh.polygons[%d].loop_start = %d\n", i, i*3);
		text.AppendFormat("mesh.polygons[%d].loop_total = 3\n", i);
	}
	// 	for( i=0; i<pEdgeCount; i++ ){
// 		text.AppendFormat( "mesh.edges[%d].vertices[0] = %d\n", i, pEdges[ i ].vertices[ 0 ] );
// 		text.AppendFormat( "mesh.edges[%d].vertices[1] = %d\n", i, pEdges[ i ].vertices[ 1 ] );
// 	}
	text.AppendFormat("mesh.update()\n");
	audioThread.GetLogger().LogInfo(text);
}



// Operators
//////////////

deoalIcoSphere &deoalIcoSphere::operator=(const deoalIcoSphere &ico){
	decVector *vertices = NULL;
	sEdge *edges = NULL;
	sFace *faces = NULL;
	
	try{
		if(ico.pVertexCount > 0){
			vertices = new decVector[ico.pVertexCount];
			memcpy(vertices, ico.pVertices, sizeof(decVector) * ico.pVertexCount);
		}
		if(ico.pEdgeCount > 0){
			edges = new sEdge[ico.pEdgeCount];
			memcpy(edges, ico.pEdges, sizeof(sEdge) * ico.pEdgeCount);
		}
		if(ico.pFaceCount > 0){
			faces = new sFace[ico.pFaceCount];
			memcpy(faces, ico.pFaces, sizeof(sFace) * ico.pFaceCount);
		}
		
	}catch(const deException &){
		if(faces){
			delete [] faces;
		}
		if(edges){
			delete [] edges;
		}
		if(vertices){
			delete [] vertices;
		}
		throw;
	}
	
	if(pVertices){
		delete [] pVertices;
	}
	pVertices = vertices;
	pVertexCount = ico.pVertexCount;
	
	if(pEdges){
		delete [] pEdges;
	}
	pEdges = edges;
	pEdgeCount = ico.pEdgeCount;
	
	if(pFaces){
		delete [] pFaces;
	}
	pFaces = faces;
	pFaceCount = ico.pFaceCount;
	
	pOpeningAngle = ico.pOpeningAngle;
	
	return *this;
}



// Protected Functions
////////////////////////

void deoalIcoSphere::pCalculateOpeningAngle(){
// 	const decVector &v1 = pVertices[ pFaces[ 0 ].vertices[ 0 ] ];
// 	const decVector &v2 = pVertices[ pFaces[ 0 ].vertices[ 1 ] ];
// 	const decVector &v3 = pVertices[ pFaces[ 0 ].vertices[ 2 ] ];
// 	const decVector center( ( v1 + v2 + v3 ) / 3.0f );
// 	const float radius = decMath::max( ( v1 - center ).Length(),
// 		( v2 - center ).Length(), ( v3 - center ).Length() );
	
	const float radius = (pVertices[pEdges[0].vertices[1]]
		- pVertices[pEdges[0].vertices[0]]).Length() * 0.5f;
	
	pOpeningAngle = atanf(radius) * 2.0f;
}
