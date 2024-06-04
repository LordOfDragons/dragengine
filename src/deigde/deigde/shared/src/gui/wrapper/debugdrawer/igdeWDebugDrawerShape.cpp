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
	deDebugDrawerShape *pDDShape;
	
public:
	cDucpliateShapeVisitor( deDebugDrawerShape *ddShape ){
		if( ! ddShape ){
			DETHROW( deeInvalidParam );
		}
		
		pDDShape = ddShape;
	}
	
	virtual void VisitShapeSphere( decShapeSphere &sphere ){
		decShapeSphere *copy = NULL;
		
		try{
			copy = new decShapeSphere( sphere.GetRadius(), sphere.GetAxisScaling(), sphere.GetPosition() );
			pDDShape->GetShapeList().Add( copy );
			
		}catch( const deException & ){
			if( copy ){
				delete copy;
			}
			throw;
		}
	}
	
	virtual void VisitShapeBox( decShapeBox &box ){
		decShapeBox *copy = NULL;
		
		try{
			copy = new decShapeBox( box.GetHalfExtends(), box.GetTapering(), box.GetPosition(), box.GetOrientation() );
			pDDShape->GetShapeList().Add( copy );
			
		}catch( const deException & ){
			if( copy ){
				delete copy;
			}
			throw;
		}
	}
	
	virtual void VisitShapeCylinder( decShapeCylinder &cylinder ){
		decShapeCylinder *copy = NULL;
		
		try{
			copy = new decShapeCylinder( cylinder.GetHalfHeight(), cylinder.GetTopRadius(),
				cylinder.GetBottomRadius(), cylinder.GetTopAxisScaling(), cylinder.GetBottomAxisScaling(),
				 cylinder.GetPosition(), cylinder.GetOrientation() );
			pDDShape->GetShapeList().Add( copy );
			
		}catch( const deException & ){
			if( copy ){
				delete copy;
			}
			throw;
		}
	}
	
	virtual void VisitShapeCapsule( decShapeCapsule &capsule ){
		decShapeCapsule *copy = NULL;
		
		try{
			copy = new decShapeCapsule( capsule.GetHalfHeight(), capsule.GetTopRadius(),
				capsule.GetBottomRadius(), capsule.GetTopAxisScaling(), capsule.GetBottomAxisScaling(),
				capsule.GetPosition(), capsule.GetOrientation() );
			pDDShape->GetShapeList().Add( copy );
			
		}catch( const deException & ){
			if( copy ){
				delete copy;
			}
			throw;
		}
	}
	
	virtual void VisitShapeHull( decShapeHull &hull ){
		decShapeHull *copy = NULL;
		
		try{
			copy = new decShapeHull( hull.GetPosition(), hull.GetOrientation() );
			const int count = hull.GetPointCount();
			int i;
			copy->SetPointCount( count );
			for( i=0; i<count; i++ ){
				copy->SetPointAt( i, hull.GetPointAt( i ) );
			}
			
			pDDShape->GetShapeList().Add( copy );
			
		}catch( const deException & ){
			if( copy ){
				delete copy;
			}
			throw;
		}
	}
};



// Class igdeWDebugDrawerShape
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeWDebugDrawerShape::igdeWDebugDrawerShape(){
	pEngDebugDrawer = NULL;
	pEngDDShape = NULL;
	
	pScale.Set( 1.0f, 1.0f, 1.0f );
	
	pColorEdge.Set( 0.0f, 0.0f, 0.0f, 1.0f );
	pColorFill.Set( 0.0f, 0.0f, 0.0f, 0.0f );
	pVisible = true;
}

igdeWDebugDrawerShape::~igdeWDebugDrawerShape(){
	SetParentDebugDrawer( NULL );
	RemoveAllFaces();
}



// Management
///////////////

void igdeWDebugDrawerShape::SetParentDebugDrawer( deDebugDrawer *debugDrawer ){
	if( debugDrawer == pEngDebugDrawer ){
		return;
	}
	
	if( pEngDDShape ){
		pEngDebugDrawer->RemoveShape( pEngDDShape );
		pEngDDShape = NULL;
	}
	
	if( pEngDebugDrawer ){
		pEngDebugDrawer->FreeReference();
	}
	
	pEngDebugDrawer = debugDrawer;
	
	if( debugDrawer ){
		debugDrawer->AddReference();
	}
	
	pUpdateDDShape();
}



void igdeWDebugDrawerShape::SetPosition( const decVector &position ){
	if( position.IsEqualTo( pPosition ) ){
		return;
	}
	
	pPosition = position;
	
	if( pEngDDShape ){
		pEngDDShape->SetPosition( position );
		
		if( pEngDebugDrawer ){
			pEngDebugDrawer->NotifyShapeGeometryChanged();
		}
	}
}

void igdeWDebugDrawerShape::SetOrientation( const decQuaternion &orientation ){
	if( orientation.IsEqualTo( pOrientation ) ){
		return;
	}
	
	pOrientation = orientation;
	
	if( pEngDDShape ){
		pEngDDShape->SetOrientation( orientation );
		
		if( pEngDebugDrawer ){
			pEngDebugDrawer->NotifyShapeGeometryChanged();
		}
	}
}

void igdeWDebugDrawerShape::SetScale( const decVector &scale ){
	if( scale.IsEqualTo( pScale ) ){
		return;
	}
	
	pScale = scale;
	
	if( pEngDDShape ){
		pEngDDShape->SetScale( scale );
		
		if( pEngDebugDrawer ){
			pEngDebugDrawer->NotifyShapeGeometryChanged();
		}
	}
}



void igdeWDebugDrawerShape::SetEdgeColor( const decColor &color ){
	if( color.IsEqualTo( pColorEdge ) ){
		return;
	}
	
	pColorEdge = color;
	
	if( pEngDDShape ){
		pEngDDShape->SetEdgeColor( color );
		
		if( pEngDebugDrawer ){
			pEngDebugDrawer->NotifyShapeColorChanged();
		}
	}
}

void igdeWDebugDrawerShape::SetFillColor( const decColor &color ){
	if( color.IsEqualTo( pColorFill ) ){
		return;
	}
	
	pColorFill = color;
	
	if( pEngDDShape ){
		pEngDDShape->SetFillColor( color );
		
		if( pEngDebugDrawer ){
			pEngDebugDrawer->NotifyShapeColorChanged();
		}
	}
}

void igdeWDebugDrawerShape::SetVisible( bool visible ){
	if( visible == pVisible ){
		return;
	}
	
	pVisible = visible;
	pUpdateDDShape();
}



// Shapes
///////////

void igdeWDebugDrawerShape::AddShape( decShape *shape ){
	if( ! shape ){
		DETHROW( deeInvalidParam );
	}
	
	pShapes.Add( shape );
	pRebuildShapes();
}

void igdeWDebugDrawerShape::AddShapes( const decShapeList &shapes ){
	const int count = shapes.GetCount();
	decShape *shape = NULL;
	int i;
	
	for( i=0; i<count; i++ ){
		try{
			shape = shapes.GetAt( i )->Copy();
			pShapes.Add( shape );
			shape = NULL;
			
		}catch( const deException & ){
			if( shape ){
				delete shape;
			}
			throw;
		}
	}
	
	pRebuildShapes();
}

void igdeWDebugDrawerShape::AddSphereShape( float radius, const decVector &position ){
	decShapeSphere *sphere = NULL;
	
	try{
		sphere = new decShapeSphere( radius, position );
		AddShape( sphere );
		
	}catch( const deException & ){
		if( sphere ){
			delete sphere;
		}
		throw;
	}
}

void igdeWDebugDrawerShape::AddSphereShape( float radius, const decVector2 &axisScaling, const decVector &position ){
	decShapeSphere *sphere = NULL;
	
	try{
		sphere = new decShapeSphere( radius, axisScaling, position );
		AddShape( sphere );
		
	}catch( const deException & ){
		if( sphere ){
			delete sphere;
		}
		throw;
	}
}

void igdeWDebugDrawerShape::AddBoxShape( const decVector &halfExtends, const decVector &position,
const decQuaternion &orientation ){
	decShapeBox *box = NULL;
	
	try{
		box = new decShapeBox( halfExtends, position, orientation );
		AddShape( box );
		
	}catch( const deException & ){
		if( box ){
			delete box;
		}
		throw;
	}
}

void igdeWDebugDrawerShape::AddBoxShape( const decVector &halfExtends, const decVector2 &axiscaling,
const decVector &position, const decQuaternion &orientation ){
	decShapeBox *box = NULL;
	
	try{
		box = new decShapeBox( halfExtends, axiscaling, position, orientation );
		AddShape( box );
		
	}catch( const deException & ){
		if( box ){
			delete box;
		}
		throw;
	}
}

void igdeWDebugDrawerShape::AddCylinderShape( float halfHeight, float topRadius, float bottomRadius,
const decVector &position, const decQuaternion &orientation ){
	decShapeCylinder *cylinder = NULL;
	
	try{
		cylinder = new decShapeCylinder( halfHeight, topRadius, bottomRadius, position, orientation );
		AddShape( cylinder );
		
	}catch( const deException & ){
		if( cylinder ){
			delete cylinder;
		}
		throw;
	}
}

void igdeWDebugDrawerShape::AddCylinderShape( float halfHeight, float topRadius, float bottomRadius,
const decVector2 &topAxisScaling, const decVector2 &bottomAxisScaling, const decVector &position,
const decQuaternion &orientation ){
	decShapeCylinder *cylinder = NULL;
	
	try{
		cylinder = new decShapeCylinder( halfHeight, topRadius, bottomRadius, topAxisScaling, bottomAxisScaling, position, orientation );
		AddShape( cylinder );
		
	}catch( const deException & ){
		if( cylinder ){
			delete cylinder;
		}
		throw;
	}
}

void igdeWDebugDrawerShape::AddCapsuleShape( float halfHeight, float topRadius, float bottomRadius,
const decVector &position, const decQuaternion &orientation ){
	decShapeCapsule *capsule = NULL;
	
	try{
		capsule = new decShapeCapsule( halfHeight, topRadius, bottomRadius, position, orientation );
		AddShape( capsule );
		
	}catch( const deException & ){
		if( capsule ){
			delete capsule;
		}
		throw;
	}
}

void igdeWDebugDrawerShape::AddCapsuleShape( float halfHeight, float topRadius, float bottomRadius,
const decVector2 &topAxisScaling, const decVector2 &bottomAxisScaling, const decVector &position,
const decQuaternion &orientation ){
	decShapeCapsule *capsule = NULL;
	
	try{
		capsule = new decShapeCapsule( halfHeight, topRadius, bottomRadius, topAxisScaling, bottomAxisScaling, position, orientation );
		AddShape( capsule );
		
	}catch( const deException & ){
		if( capsule ){
			delete capsule;
		}
		throw;
	}
}

void igdeWDebugDrawerShape::AddHullShape( const decVector &position,
const decQuaternion &orientation, int pointCount, const decVector *points ){
	if( pointCount < 0 || ! points ){
		DETHROW( deeInvalidParam );
	}
	decShapeHull *hull = NULL;
	
	try{
		hull = new decShapeHull( position, orientation );
		hull->SetPointCount( pointCount );
		int i;
		for( i=0; i<pointCount; i++ ){
			hull->SetPointAt( i, points[ i ] );
		}
		AddShape( hull );
		
	}catch( const deException & ){
		if( hull ){
			delete hull;
		}
		throw;
	}
}

void igdeWDebugDrawerShape::RemoveAllShapes(){
	pShapes.RemoveAll();
	pRebuildShapes();
}



// Faces
//////////

void igdeWDebugDrawerShape::AddFace( deDebugDrawerShapeFace *face ){
	// no pFaces.Has(face) check. with larger number of faces this becomes very slow
	if( ! face ){
		DETHROW( deeInvalidParam );
	}
	pFaces.Add( face );
	pRebuildFaces();
}

void igdeWDebugDrawerShape::AddOcclusionMeshFaces( const deOcclusionMesh &occlusionMesh ){
	const deOcclusionMeshVertex * const vertices = occlusionMesh.GetVertices();
	const unsigned short * const corners = occlusionMesh.GetCorners();
	const unsigned short * const faces = occlusionMesh.GetFaces();
	const int faceCount = occlusionMesh.GetFaceCount();
	deDebugDrawerShapeFace *shapeFace = NULL;
	int f, c, cornerIndex = 0;
	decVector normal;
	float normalLen;
	
	try{
		for( f=0; f<faceCount; f++ ){
			const int cornerCount = ( int )faces[ f ];
			
			if( cornerCount > 2 ){
				shapeFace = new deDebugDrawerShapeFace;
				
				for( c=0; c<cornerCount; c++ ){
					shapeFace->AddVertex( vertices[ corners[ cornerIndex++ ] ].GetPosition() );
				}
				
				normal = ( shapeFace->GetVertexAt( 1 ) - shapeFace->GetVertexAt( 0 ) )
					% ( shapeFace->GetVertexAt( 2 ) - ( shapeFace->GetVertexAt( 1 ) ) );
				normalLen = normal.Length();
				if( normalLen > FLOAT_SAFE_EPSILON ){
					shapeFace->SetNormal( normal / normalLen );
					
				}else{
					shapeFace->SetNormal( decVector( 0.0f, 0.0f, 1.0f ) );
				}
				
				pFaces.Add( shapeFace );
				shapeFace = NULL;
				
			}else{
				cornerIndex += cornerCount;
			}
		}
		
	}catch( const deException & ){
		if( shapeFace ){
			delete shapeFace;
		}
		throw;
	}
	
	pRebuildFaces();
}

void igdeWDebugDrawerShape::AddNavSpaceFaces( const deNavigationSpace &navSpace ){
	if( navSpace.GetType() == deNavigationSpace::estGrid ){
		pAddNavGrid( navSpace, false, 0 );
		
	}else if( navSpace.GetType() == deNavigationSpace::estMesh ){
		pAddNavMesh( navSpace, false, 0 );
		
	}else{ // deNavigationSpace::estVolume
	}
}

void igdeWDebugDrawerShape::AddNavSpaceFaces( const deNavigationSpace &navSpace, int type ){
	if( navSpace.GetType() == deNavigationSpace::estGrid ){
		pAddNavGrid( navSpace, true, type );
		
	}else if( navSpace.GetType() == deNavigationSpace::estMesh ){
		pAddNavMesh( navSpace, true, type );
		
	}else{ // deNavigationSpace::estVolume
	}
}

void igdeWDebugDrawerShape::RemoveAllFaces(){
	const int count = pFaces.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		delete ( deDebugDrawerShapeFace* )pFaces.GetAt( i );
	}
	pFaces.RemoveAll();
	
	pRebuildFaces();
}



// Private Functions
//////////////////////

void igdeWDebugDrawerShape::pUpdateDDShape(){
	// check if the debug drawer shape has to be existing
	if( pVisible && pEngDebugDrawer ){
		// if not existing create the debug drawer shape and populate it with the stored parameters
		if( ! pEngDDShape ){
			try{
				pEngDDShape = new deDebugDrawerShape;
				
				pEngDDShape->SetPosition( pPosition );
				pEngDDShape->SetOrientation( pOrientation );
				pEngDDShape->SetScale( pScale );
				pEngDDShape->SetEdgeColor( pColorEdge );
				pEngDDShape->SetFillColor( pColorFill );
				
				pBareRebuildShapes();
				pBareRebuildFaces();
				
				pEngDebugDrawer->AddShape( pEngDDShape );
				
			}catch( const deException & ){
				if( pEngDDShape ){
					delete pEngDDShape;
					pEngDDShape = NULL;
				}
				throw;
			}
		}
		
	// otherwise remove the debug drawer shape if existing
	}else if( pEngDDShape ){
		pEngDebugDrawer->RemoveShape( pEngDDShape );
		pEngDDShape = NULL;
	}
}

void igdeWDebugDrawerShape::pRebuildShapes(){
	// if the debug drawer shape exists just re-populate it with shapes
	if( pEngDDShape ){
		pBareRebuildShapes();
		
	// otherwise update it which already does the population if required
	}else{
		pUpdateDDShape();
	}
}

void igdeWDebugDrawerShape::pBareRebuildShapes(){
	if( pEngDDShape ){
		cDucpliateShapeVisitor visitor( pEngDDShape );
		const int shapeCount = pShapes.GetCount();
		int i;
		
		pEngDDShape->GetShapeList().RemoveAll();
		
		for( i=0; i<shapeCount; i++ ){
			pShapes.GetAt( i )->Visit( visitor );
		}
	}
	
	if( pEngDebugDrawer ){
		pEngDebugDrawer->NotifyShapeContentChanged();
	}
}

void igdeWDebugDrawerShape::pRebuildFaces(){
	// if the debug drawer shape exists just re-populate it with faces
	if( pEngDDShape ){
		pBareRebuildFaces();
		
	// otherwise update it which already does the population if required
	}else{
		pUpdateDDShape();
	}
}

void igdeWDebugDrawerShape::pBareRebuildFaces(){
	if( pEngDDShape ){
		deDebugDrawerShapeFace *newFace = NULL;
		const int faceCount = pFaces.GetCount();
		int i, j;
		
		pEngDDShape->RemoveAllFaces();
		
		try{
			for( i=0; i<faceCount; i++ ){
				const deDebugDrawerShapeFace &face = *( ( deDebugDrawerShapeFace* )pFaces.GetAt( i ) );
				const int vertexCount = face.GetVertexCount();
				
				newFace = new deDebugDrawerShapeFace;
				newFace->SetNormal( face.GetNormal() );
				for( j=0; j<vertexCount; j++ ){
					newFace->AddVertex( face.GetVertexAt( j ) );
				}
				
				pEngDDShape->AddFace( newFace );
				newFace = NULL;
			}
			
		}catch( const deException & ){
			if( newFace ){
				delete newFace;
			}
			throw;
		}
	}
	
	if( pEngDebugDrawer ){
		pEngDebugDrawer->NotifyShapeContentChanged();
	}
}



void igdeWDebugDrawerShape::pAddNavGrid( const deNavigationSpace &navSpace, bool filterType, int type ){
	const deNavigationSpaceEdge * const edges = navSpace.GetEdges();
	const decVector * const vertices = navSpace.GetVertices();
	const int edgeCount = navSpace.GetEdgeCount();
	deDebugDrawerShapeFace *shapeFace = NULL;
	int i;
	
	try{
		for( i=0; i<edgeCount; i++ ){
			const deNavigationSpaceEdge &edge = edges[ i ];
			
			if( filterType ){
				if( edge.GetType1() != type && edge.GetType2() != type ){
					continue;
				}
			}
			
			const decVector &vertex1 = vertices[ edge.GetVertex1() ];
			const decVector &vertex2 = vertices[ edge.GetVertex2() ];
			
			shapeFace = new deDebugDrawerShapeFace;
			
			shapeFace->AddVertex( vertex1 );
			shapeFace->AddVertex( vertex2 );
			shapeFace->AddVertex( vertex2 );
			
			shapeFace->SetNormal( decVector( 0.0f, 1.0f, 0.0f ) );
			
			pFaces.Add( shapeFace );
			shapeFace = NULL;
		}
		
	}catch( const deException & ){
		if( shapeFace ){
			delete shapeFace;
		}
		throw;
	}
	
	pRebuildFaces();
}

void igdeWDebugDrawerShape::pAddNavMesh( const deNavigationSpace &navSpace, bool filterType, int type ){
	const deNavigationSpaceCorner * const corners = navSpace.GetCorners();
	const deNavigationSpaceFace * const faces = navSpace.GetFaces();
	const decVector * const vertices = navSpace.GetVertices();
	const int faceCount = navSpace.GetFaceCount();
	deDebugDrawerShapeFace *shapeFace = NULL;
	int i, j, firstCorner;
	decVector normal;
	float normalLen;
	
	try{
		firstCorner = 0;
		
		for( i=0; i<faceCount; i++ ){
			const deNavigationSpaceFace &face = faces[ i ];
			const unsigned short cornerCount = face.GetCornerCount();
			
			if( cornerCount > 2 ){
				if( filterType ){
					bool ignore = true;
					
					if( face.GetType() == type ){
						ignore = false;
						
					}else{
						for( j=0; j<cornerCount; j++ ){
							if( corners[ firstCorner + j ].GetType() == type ){
								ignore = false;
								break;
							}
						}
					}
					
					if( ignore ){
						continue;
					}
				}
				
				shapeFace = new deDebugDrawerShapeFace;
				
				for( j=0; j<cornerCount; j++ ){
					shapeFace->AddVertex( vertices[ corners[ firstCorner + j ].GetVertex() ] );
				}
				
				normal = ( shapeFace->GetVertexAt( 1 ) - shapeFace->GetVertexAt( 0 ) ) % ( shapeFace->GetVertexAt( 2 ) - ( shapeFace->GetVertexAt( 1 ) ) );
				normalLen = normal.Length();
				if( normalLen > FLOAT_SAFE_EPSILON ){
					shapeFace->SetNormal( normal / normalLen );
					
				}else{
					shapeFace->SetNormal( decVector( 0.0f, 0.0f, 1.0f ) );
				}
				
				pFaces.Add( shapeFace );
				shapeFace = NULL;
			}
			
			firstCorner += cornerCount;
		}
		
	}catch( const deException & ){
		if( shapeFace ){
			delete shapeFace;
		}
		throw;
	}
	
	pRebuildFaces();
}
