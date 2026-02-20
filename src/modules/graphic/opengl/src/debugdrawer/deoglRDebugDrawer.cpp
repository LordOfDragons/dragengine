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

#include "deoglRDebugDrawer.h"
#include "deoglDebugDrawerShape.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../vao/deoglVAO.h"
#include "../vbo/deoglVBOLayout.h"
#include "../vbo/deoglVBOAttribute.h"
#include "../delayedoperation/deoglDelayedOperations.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/debug/deDebugDrawerShapeFace.h>



// Class deoglRDebugDrawer
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRDebugDrawer::deoglRDebugDrawer(deoglRenderThread &renderThread) :
pRenderThread(renderThread),

pParentWorld(nullptr),

pVisible(false),
pXRay(false),

pHasShapes(false),
pHasFaces(false),

pVBO(0),
pVAO(nullptr),
pDirtyVBO(true),

pWorldMarkedRemove(false){
	LEAK_CHECK_CREATE(renderThread, DebugDrawer);
}

deoglRDebugDrawer::~deoglRDebugDrawer(){
	LEAK_CHECK_FREE(pRenderThread, DebugDrawer);
	if(pVAO){
		delete pVAO;
	}
	
	deoglDelayedOperations &dops = pRenderThread.GetDelayedOperations();
	dops.DeleteOpenGLBuffer(pVBO);
}



// Management
///////////////

void deoglRDebugDrawer::SetParentWorld(deoglRWorld *parentWorld){
	if(parentWorld == pParentWorld){
		return;
	}
	
	pParentWorld = parentWorld;
	
	//if( pOctreeNode ){
	//	pOctreeNode->GetBillboardList().Remove( this );
	//}
}



void deoglRDebugDrawer::SetMatrix(const decDMatrix &matrix){
	pMatrix = matrix;
}

void deoglRDebugDrawer::SetVisible(bool visible){
	pVisible = visible;
}

void deoglRDebugDrawer::SetXRay(bool xray){
	pXRay = xray;
}



void deoglRDebugDrawer::UpdateShapes(const deDebugDrawer &debugDrawer){
	pShapes.SetCount(debugDrawer.GetShapes().GetCount(), {});
	
	debugDrawer.GetShapes().VisitIndexed([&](int i, const deDebugDrawerShape &s){
		pShapes[i].SetMatrix(decMatrix::CreateWorld(s.GetPosition(), s.GetOrientation(), s.GetScale()));
		pShapes[i].SetEdgeColor(s.GetEdgeColor());
		pShapes[i].SetFillColor(s.GetFillColor());
		pShapes[i].SetShapeList(s.GetShapeList());
	});
	
	pWriteVBOData(debugDrawer);
	pUpdateHasFlags(debugDrawer);
	
	pDirtyVBO = true;
}



deoglDebugDrawerShape &deoglRDebugDrawer::GetShapeAt(int index){
	return pShapes[index];
}



void deoglRDebugDrawer::UpdateVBO(){
	if(!pDirtyVBO){
		return;
	}
	
	if(pVBOData.IsEmpty()){
		pDirtyVBO = false;
		return;
	}
	
	// create vbo if not existing
	if(!pVBO){
		OGL_CHECK(pRenderThread, pglGenBuffers(1, &pVBO));
	}
	
	// write vbo data to vbo
	OGL_CHECK(pRenderThread, pglBindBuffer(GL_ARRAY_BUFFER, pVBO));
	OGL_CHECK(pRenderThread, pglBufferData(GL_ARRAY_BUFFER,
		sizeof(oglVector3) * pVBOData.GetCount(), nullptr, GL_STATIC_DRAW));
	OGL_CHECK(pRenderThread, pglBufferData(GL_ARRAY_BUFFER,
		sizeof(oglVector3) * pVBOData.GetCount(), pVBOData.GetArrayPointer(), GL_STATIC_DRAW));
	
	// create vao if not existing
	if(!pVAO){
		deoglVBOLayout vboLayout;
		vboLayout.GetAttributes().SetAll(1, {});
		vboLayout.SetStride(12);
		vboLayout.SetSize(12 * pVBOData.GetCount());
		vboLayout.SetIndexType(deoglVBOLayout::eitNone);
		
		deoglVBOAttribute &attrPos = vboLayout.GetAttributes()[0];
		attrPos.SetComponentCount(3);
		attrPos.SetDataType(deoglVBOAttribute::edtFloat);
		attrPos.SetOffset(0);
		
		pVAO = new deoglVAO(pRenderThread);
		pVAO->SetIndexType(deoglVBOLayout::eitNone);
		OGL_CHECK(pRenderThread, pglBindVertexArray(pVAO->GetVAO()));
		
		OGL_CHECK(pRenderThread, pglBindBuffer(GL_ARRAY_BUFFER, pVBO));
		vboLayout.SetVAOAttributeAt(pRenderThread, 0, 0);
		
		OGL_CHECK(pRenderThread, pglBindBuffer(GL_ARRAY_BUFFER, 0));
		OGL_CHECK(pRenderThread, pglBindVertexArray(0));
	}
	
	pDirtyVBO = false;
}



void deoglRDebugDrawer::PrepareQuickDispose(){
	pParentWorld = nullptr;
	//pOctreeNode = NULL;
}



// Render world usage
///////////////////////

void deoglRDebugDrawer::SetWorldMarkedRemove(bool marked){
	pWorldMarkedRemove = marked;
}



// Private Functions
//////////////////////

void deoglRDebugDrawer::pUpdateHasFlags(const deDebugDrawer &debugDrawer){
	pHasShapes = false;
	//pHasFaces = false;
	
	debugDrawer.GetShapes().Visit([&](const deDebugDrawerShape &ddshape){
		if(ddshape.GetShapeList().IsNotEmpty()){
			pHasShapes = true;
		}
		//if(ddshape.GetFaceCount() > 0){
		//	pHasFaces = true;
		//}
	});
	
	pHasFaces = pVBOData.IsNotEmpty(); // this covers also convex hull shapes
}

void deoglRDebugDrawer::pWriteVBOData(const deDebugDrawer &debugDrawer){
	int pointCount = 0;
	
	pShapes.VisitIndexed([&](int i, deoglDebugDrawerShape &shape){
		pointCount += shape.CalcRequiredPoints(*debugDrawer.GetShapes()[i], pointCount);
	});
	
	pVBOData.SetCountDiscard(pointCount);
	
	if(pVBOData.IsNotEmpty()){
		pShapes.VisitIndexed([&](int i, deoglDebugDrawerShape &shape){
			shape.WriteVBOData(debugDrawer.GetShapes()[i], pVBOData.GetArrayPointer());
		});
	}
}
