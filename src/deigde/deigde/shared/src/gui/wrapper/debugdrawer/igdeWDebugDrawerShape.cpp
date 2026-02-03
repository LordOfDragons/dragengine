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

#include "igdeWDebugDrawerShape.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/shape/decShapeCapsule.h>
#include <dragengine/common/shape/decShapeCylinder.h>
#include <dragengine/common/shape/decShapeSphere.h>
#include <dragengine/common/shape/decShapeHull.h>
#include <dragengine/common/shape/decShapeVisitor.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/debug/deDebugDrawerShapeFace.h>
#include <dragengine/resources/occlusionmesh/deOcclusionMesh.h>
#include <dragengine/resources/occlusionmesh/deOcclusionMeshVertex.h>
#include <dragengine/resources/navigation/space/deNavigationSpace.h>
#include <dragengine/resources/navigation/space/deNavigationSpaceCorner.h>
#include <dragengine/resources/navigation/space/deNavigationSpaceEdge.h>
#include <dragengine/resources/navigation/space/deNavigationSpaceFace.h>



// Local class to help us create the shapes
/////////////////////////////////////////////

class cDucpliateShapeVisitor : public decShapeVisitor{
private:
	deDebugDrawerShape &pDDShape;
	
public:
	cDucpliateShapeVisitor(deDebugDrawerShape &ddShape) : pDDShape(ddShape){}
	
	void VisitShapeSphere(decShapeSphere &sphere) override{
		pDDShape.GetShapeList().Add(sphere.Copy());
	}
	
	void VisitShapeBox(decShapeBox &box) override{
		pDDShape.GetShapeList().Add(box.Copy());
	}
	
	void VisitShapeCylinder(decShapeCylinder &cylinder) override{
		pDDShape.GetShapeList().Add(cylinder.Copy());
	}
	
	void VisitShapeCapsule(decShapeCapsule &capsule) override{
		pDDShape.GetShapeList().Add(capsule.Copy());
	}
	
	void VisitShapeHull(decShapeHull &hull) override{
		pDDShape.GetShapeList().Add(hull.Copy());
	}
};



// Class igdeWDebugDrawerShape
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeWDebugDrawerShape::igdeWDebugDrawerShape() :
pEngDDShape(nullptr)
{
	pScale.Set(1.0f, 1.0f, 1.0f);
	
	pColorEdge.Set(0.0f, 0.0f, 0.0f, 1.0f);
	pColorFill.Set(0.0f, 0.0f, 0.0f, 0.0f);
	pVisible = true;
}

igdeWDebugDrawerShape::~igdeWDebugDrawerShape(){
	SetParentDebugDrawer(nullptr);
	RemoveAllFaces();
}



// Management
///////////////

void igdeWDebugDrawerShape::SetParentDebugDrawer(deDebugDrawer *debugDrawer){
	if(debugDrawer == pEngDebugDrawer){
		return;
	}
	
	if(pEngDDShape){
		pEngDebugDrawer->RemoveShape(pEngDDShape);
		pEngDDShape = nullptr;
	}
	pEngDebugDrawer = debugDrawer;
	pUpdateDDShape();
}



void igdeWDebugDrawerShape::SetPosition(const decVector &position){
	if(position.IsEqualTo(pPosition)){
		return;
	}
	
	pPosition = position;
	
	if(pEngDDShape){
		pEngDDShape->SetPosition(position);
		
		if(pEngDebugDrawer){
			pEngDebugDrawer->NotifyShapeGeometryChanged();
		}
	}
}

void igdeWDebugDrawerShape::SetOrientation(const decQuaternion &orientation){
	if(orientation.IsEqualTo(pOrientation)){
		return;
	}
	
	pOrientation = orientation;
	
	if(pEngDDShape){
		pEngDDShape->SetOrientation(orientation);
		
		if(pEngDebugDrawer){
			pEngDebugDrawer->NotifyShapeGeometryChanged();
		}
	}
}

void igdeWDebugDrawerShape::SetScale(const decVector &scale){
	if(scale.IsEqualTo(pScale)){
		return;
	}
	
	pScale = scale;
	
	if(pEngDDShape){
		pEngDDShape->SetScale(scale);
		
		if(pEngDebugDrawer){
			pEngDebugDrawer->NotifyShapeGeometryChanged();
		}
	}
}



void igdeWDebugDrawerShape::SetEdgeColor(const decColor &color){
	if(color.IsEqualTo(pColorEdge)){
		return;
	}
	
	pColorEdge = color;
	
	if(pEngDDShape){
		pEngDDShape->SetEdgeColor(color);
		
		if(pEngDebugDrawer){
			pEngDebugDrawer->NotifyShapeColorChanged();
		}
	}
}

void igdeWDebugDrawerShape::SetFillColor(const decColor &color){
	if(color.IsEqualTo(pColorFill)){
		return;
	}
	
	pColorFill = color;
	
	if(pEngDDShape){
		pEngDDShape->SetFillColor(color);
		
		if(pEngDebugDrawer){
			pEngDebugDrawer->NotifyShapeColorChanged();
		}
	}
}

void igdeWDebugDrawerShape::SetVisible(bool visible){
	if(visible == pVisible){
		return;
	}
	
	pVisible = visible;
	pUpdateDDShape();
}



// Shapes
///////////

void igdeWDebugDrawerShape::AddShape(decShape::Ref &&shape){
	DEASSERT_NOTNULL(shape)
	
	pShapes.Add(std::move(shape));
	pRebuildShapes(pEngDDShape);
}

void igdeWDebugDrawerShape::AddShapes(const decShape::List &shapes){
	shapes.Visit([&](decShape &shape){
		pShapes.Add(shape.Copy());
	});
	pRebuildShapes(pEngDDShape);
}

void igdeWDebugDrawerShape::AddSphereShape(float radius, const decVector &position){
	AddShape(decShapeSphere::Ref::New(radius, position));
}

void igdeWDebugDrawerShape::AddSphereShape(float radius, const decVector2 &axisScaling,
const decVector &position){
	AddShape(decShapeSphere::Ref::New(radius, axisScaling, position));
}

void igdeWDebugDrawerShape::AddBoxShape(const decVector &halfExtends, const decVector &position,
const decQuaternion &orientation){
	AddShape(decShapeBox::Ref::New(halfExtends, position, orientation));
}

void igdeWDebugDrawerShape::AddBoxShape(const decVector &halfExtends, const decVector2 &axiscaling,
const decVector &position, const decQuaternion &orientation){
	AddShape(decShapeBox::Ref::New(halfExtends, axiscaling, position, orientation));
}

void igdeWDebugDrawerShape::AddCylinderShape(float halfHeight, float topRadius, float bottomRadius,
const decVector &position, const decQuaternion &orientation){
	AddShape(decShapeCylinder::Ref::New(halfHeight, topRadius, bottomRadius, position, orientation));
}

void igdeWDebugDrawerShape::AddCylinderShape(float halfHeight, float topRadius, float bottomRadius,
const decVector2 &topAxisScaling, const decVector2 &bottomAxisScaling, const decVector &position,
const decQuaternion &orientation){
	AddShape(decShapeCylinder::Ref::New(halfHeight, topRadius, bottomRadius,
		topAxisScaling, bottomAxisScaling, position, orientation));
}

void igdeWDebugDrawerShape::AddCapsuleShape(float halfHeight, float topRadius, float bottomRadius,
const decVector &position, const decQuaternion &orientation){
	AddShape(decShapeCapsule::Ref::New(halfHeight, topRadius, bottomRadius, position, orientation));
}

void igdeWDebugDrawerShape::AddCapsuleShape(float halfHeight, float topRadius, float bottomRadius,
const decVector2 &topAxisScaling, const decVector2 &bottomAxisScaling, const decVector &position,
const decQuaternion &orientation){
	AddShape(decShapeCapsule::Ref::New(halfHeight, topRadius, bottomRadius,
		topAxisScaling, bottomAxisScaling, position, orientation));
}

void igdeWDebugDrawerShape::AddHullShape(const decVector &position,
const decQuaternion &orientation, int pointCount, const decVector *points){
	DEASSERT_NOTNULL(points)
	DEASSERT_TRUE(pointCount >= 0)
	
	decShapeHull::Ref hull = decShapeHull::Ref::New(position, orientation);
	hull->SetPointCount(pointCount);
	int i;
	for(i=0; i<pointCount; i++){
		hull->SetPointAt(i, points[i]);
	}
	AddShape(std::move(hull));
}

void igdeWDebugDrawerShape::RemoveAllShapes(){
	pShapes.RemoveAll();
	pRebuildShapes(pEngDDShape);
}



// Faces
//////////

void igdeWDebugDrawerShape::AddFace(deDebugDrawerShapeFace::Ref &&face){
	DEASSERT_NOTNULL(face)
	
	pFaces.Add(std::move(face));
	pRebuildFaces(pEngDDShape);
}

void igdeWDebugDrawerShape::AddOcclusionMeshFaces(const deOcclusionMesh &occlusionMesh){
	const deOcclusionMeshVertex * const vertices = occlusionMesh.GetVertices().GetArrayPointer();
	const unsigned short * const corners = occlusionMesh.GetCorners().GetArrayPointer();
	const unsigned short * const faces = occlusionMesh.GetFaces().GetArrayPointer();
	const int faceCount = occlusionMesh.GetFaces().GetCount();
	int f, c, cornerIndex = 0;
	decVector normal;
	float normalLen;
	
	for(f=0; f<faceCount; f++){
		const int cornerCount = (int)faces[f];
		
		if(cornerCount > 2){
			auto shapeFace = deDebugDrawerShapeFace::Ref::New();
			
			for(c=0; c<cornerCount; c++){
				shapeFace->AddVertex(vertices[corners[cornerIndex++]].GetPosition());
			}
			
			normal = (shapeFace->GetVertexAt(1) - shapeFace->GetVertexAt(0))
				% (shapeFace->GetVertexAt(2) - (shapeFace->GetVertexAt(1)));
			normalLen = normal.Length();
			if(normalLen > FLOAT_SAFE_EPSILON){
				shapeFace->SetNormal(normal / normalLen);
				
			}else{
				shapeFace->SetNormal(decVector(0.0f, 0.0f, 1.0f));
			}
			
			pFaces.Add(std::move(shapeFace));
			
		}else{
			cornerIndex += cornerCount;
		}
	}
	
	pRebuildFaces(pEngDDShape);
}

void igdeWDebugDrawerShape::AddNavSpaceFaces(const deNavigationSpace &navSpace){
	if(navSpace.GetType() == deNavigationSpace::estGrid){
		pAddNavGrid(navSpace, false, 0);
		
	}else if(navSpace.GetType() == deNavigationSpace::estMesh){
		pAddNavMesh(navSpace, false, 0);
		
	}else{ // deNavigationSpace::estVolume
	}
}

void igdeWDebugDrawerShape::AddNavSpaceFaces(const deNavigationSpace &navSpace, int type){
	if(navSpace.GetType() == deNavigationSpace::estGrid){
		pAddNavGrid(navSpace, true, type);
		
	}else if(navSpace.GetType() == deNavigationSpace::estMesh){
		pAddNavMesh(navSpace, true, type);
		
	}else{ // deNavigationSpace::estVolume
	}
}

void igdeWDebugDrawerShape::RemoveAllFaces(){
	pFaces.RemoveAll();
	pRebuildFaces(pEngDDShape);
}



// Private Functions
//////////////////////

void igdeWDebugDrawerShape::pUpdateDDShape(){
	// check if the debug drawer shape has to be existing
	if(pVisible && pEngDebugDrawer){
		// if not existing create the debug drawer shape and populate it with the stored parameters
		if(!pEngDDShape){
			auto shape = deDebugDrawerShape::Ref::New();
			shape->SetPosition(pPosition);
			shape->SetOrientation(pOrientation);
			shape->SetScale(pScale);
			shape->SetEdgeColor(pColorEdge);
			shape->SetFillColor(pColorFill);
			
			pBareRebuildShapes(shape);
			pBareRebuildFaces(shape);
			
			pEngDDShape = shape;
			pEngDebugDrawer->AddShape(std::move(shape));
		}
		
	// otherwise remove the debug drawer shape if existing
	}else if(pEngDDShape){
		pEngDebugDrawer->RemoveShape(pEngDDShape);
		pEngDDShape = nullptr;
	}
}

void igdeWDebugDrawerShape::pRebuildShapes(deDebugDrawerShape *ddshape){
	// if the debug drawer shape exists just re-populate it with shapes
	if(ddshape){
		pBareRebuildShapes(ddshape);
		
	// otherwise update it which already does the population if required
	}else{
		pUpdateDDShape();
	}
}

void igdeWDebugDrawerShape::pBareRebuildShapes(deDebugDrawerShape *ddshape){
	if(ddshape){
		ddshape->GetShapeList().RemoveAll();
		
		cDucpliateShapeVisitor visitor(*ddshape);
		pShapes.Visit([&](decShape &shape){
			shape.Visit(visitor);
		});
	}
	
	if(pEngDebugDrawer){
		pEngDebugDrawer->NotifyShapeContentChanged();
	}
}

void igdeWDebugDrawerShape::pRebuildFaces(deDebugDrawerShape *ddshape){
	// if the debug drawer shape exists just re-populate it with faces
	if(ddshape){
		pBareRebuildFaces(ddshape);
		
	// otherwise update it which already does the population if required
	}else{
		pUpdateDDShape();
	}
}

void igdeWDebugDrawerShape::pBareRebuildFaces(deDebugDrawerShape *ddshape){
	if(ddshape){
		ddshape->RemoveAllFaces();
		
		pFaces.Visit([&](const deDebugDrawerShapeFace &face){
			auto ddsFace = deDebugDrawerShapeFace::Ref::New();
			ddsFace->SetNormal(face.GetNormal());
			face.GetVertices().Visit([&](const decVector &vertex){
				ddsFace->AddVertex(vertex);
			});
			ddshape->AddFace(std::move(ddsFace));
		});
	}
	
	if(pEngDebugDrawer){
		pEngDebugDrawer->NotifyShapeContentChanged();
	}
}



void igdeWDebugDrawerShape::pAddNavGrid(const deNavigationSpace &navSpace, bool filterType, int type){
	const decVector * const vertices = navSpace.GetVertices().GetArrayPointer();
	
	navSpace.GetEdges().Visit([&](const deNavigationSpaceEdge &edge){
		if(filterType && edge.GetType1() != type && edge.GetType2() != type){
			return;
		}
		
		const decVector &vertex1 = vertices[edge.GetVertex1()];
		const decVector &vertex2 = vertices[edge.GetVertex2()];
		
		auto ddsFace = deDebugDrawerShapeFace::Ref::New();
		ddsFace->AddVertex(vertex1);
		ddsFace->AddVertex(vertex2);
		ddsFace->AddVertex(vertex2);
		ddsFace->SetNormal(decVector(0.0f, 1.0f, 0.0f));
		pFaces.Add(std::move(ddsFace));
	});
	
	pRebuildFaces(pEngDDShape);
}

void igdeWDebugDrawerShape::pAddNavMesh(const deNavigationSpace &navSpace, bool filterType, int type){
	const deNavigationSpaceCorner * const corners = navSpace.GetCorners().GetArrayPointer();
	const decVector * const vertices = navSpace.GetVertices().GetArrayPointer();
	decVector normal;
	float normalLen;
	
	int firstCorner = 0;
	
	navSpace.GetFaces().Visit([&](const deNavigationSpaceFace &face){
		const unsigned short cornerCount = face.GetCornerCount();
		
		if(cornerCount > 2){
			if(filterType){
				bool ignore = true;
				
				if(face.GetType() == type){
					ignore = false;
					
				}else{
					int i;
					for(i=0; i<cornerCount; i++){
						if(corners[firstCorner + i].GetType() == type){
							ignore = false;
							break;
						}
					}
				}
				
				if(ignore){
					return;
				}
			}
			
			auto ddsFace = deDebugDrawerShapeFace::Ref::New();
			int i;
			for(i=0; i<cornerCount; i++){
				ddsFace->AddVertex(vertices[corners[firstCorner + i].GetVertex()]);
			}
			
			normal = (ddsFace->GetVertexAt(1) - ddsFace->GetVertexAt(0))
				% (ddsFace->GetVertexAt(2) - (ddsFace->GetVertexAt(1)));
			normalLen = normal.Length();
			if(normalLen > FLOAT_SAFE_EPSILON){
				ddsFace->SetNormal(normal / normalLen);
				
			}else{
				ddsFace->SetNormal(decVector(0.0f, 0.0f, 1.0f));
			}
			
			pFaces.Add(std::move(ddsFace));
		}
		
		firstCorner += cornerCount;
	});
	
	pRebuildFaces(pEngDDShape);
}
