/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "deoglVolumeShape.h"
#include "../shapes/deoglShape.h"
#include "../shapes/deoglShapeManager.h"
#include "../shapes/deoglShapeBox.h"
#include "../shapes/deoglShapeSphere.h"
#include "../shapes/deoglShapeCylinder.h"
#include "../shapes/deoglShapeCapsule.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTBufferObject.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/shape/decShapeSphere.h>
#include <dragengine/common/shape/decShapeCylinder.h>
#include <dragengine/common/shape/decShapeCapsule.h>
#include <dragengine/common/shape/decShapeHull.h>



// Class deoglVolumeShape
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglVolumeShape::deoglVolumeShape( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pShape( NULL ){
}

deoglVolumeShape::~deoglVolumeShape(){
}



// Management
///////////////

void deoglVolumeShape::Reset(){
	pShape = NULL;
}



// Visiting
/////////////

void deoglVolumeShape::VisitShape( decShape &shape ){
	Reset();
}

void deoglVolumeShape::VisitShapeSphere( decShapeSphere &sphere ){
	pShape = pRenderThread.GetBufferObject().GetShapeManager().GetShapeAt( deoglRTBufferObject::esSphere );
	
	( ( deoglShapeSphere* )pShape )->CalcMatrix( pMatrix1, sphere.GetPosition(), sphere.GetRadius() );
	pMatrix2.SetIdentity();
}

void deoglVolumeShape::VisitShapeBox( decShapeBox &box ){
	pShape = pRenderThread.GetBufferObject().GetShapeManager().GetShapeAt( deoglRTBufferObject::esBox );
	
	( ( deoglShapeBox* )pShape )->CalcMatrix( pMatrix1, box.GetPosition(),
		box.GetOrientation(), box.GetHalfExtends() );
	pMatrix2.SetIdentity();
}

void deoglVolumeShape::VisitShapeCylinder( decShapeCylinder &cylinder ){
	pShape = pRenderThread.GetBufferObject().GetShapeManager().GetShapeAt( deoglRTBufferObject::esCylinder );
	
	( ( deoglShapeCylinder* )pShape )->CalcMatrices( pMatrix1, pMatrix2, cylinder.GetPosition(),
		cylinder.GetOrientation(), cylinder.GetHalfHeight(), cylinder.GetTopRadius(),
		cylinder.GetBottomRadius() );
}

void deoglVolumeShape::VisitShapeCapsule( decShapeCapsule &capsule ){
	pShape = pRenderThread.GetBufferObject().GetShapeManager().GetShapeAt( deoglRTBufferObject::esCapsule );
	
	( ( deoglShapeCapsule* )pShape )->CalcMatrices( pMatrix1, pMatrix2, capsule.GetPosition(),
		capsule.GetOrientation(), capsule.GetHalfHeight(), capsule.GetTopRadius(),
		capsule.GetBottomRadius() );
}

void deoglVolumeShape::VisitShapeHull( decShapeHull &hull ){
	// not supported yet
	VisitShape( hull );
}
