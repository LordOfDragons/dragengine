/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include <limits>

#include "debpDCollisionHull.h"
#include "debpDCollisionBox.h"
#include "debpDCollisionSphere.h"
#include "debpDCollisionVolumeVisitor.h"

#include <dragengine/common/exceptions.h>


const double vDblMax = std::numeric_limits<double>::max();


// Collision Detection Hull
/////////////////////////////

// constructors, destructors
//////////////////////////////

debpDCollisionHull::debpDCollisionHull() = default;

debpDCollisionHull::debpDCollisionHull(const PointList &points) :
pPoints(points){
}

debpDCollisionHull::debpDCollisionHull(const PointList &points,
	const decDVector &position, const decQuaternion &orientation) :
pPosition(position),
pOrientation(orientation),
pPoints(points){
}

debpDCollisionHull::~debpDCollisionHull() = default;


// Management
///////////////

void debpDCollisionHull::SetPosition(const decDVector &position){
	pPosition = position;
}

void debpDCollisionHull::SetOrientation(const decQuaternion &orientation){
	pOrientation = orientation;
}

void debpDCollisionHull::SetPoints(const PointList &points){
	pPoints = points;
}


// double dispatching calls
/////////////////////////////

bool debpDCollisionHull::VolumeHitsVolume(debpDCollisionVolume *volume){
	return false;
}

double debpDCollisionHull::VolumeMoveHitsVolume(debpDCollisionVolume *volume,
const decDVector &displacement, decDVector *normal){
	return 1.0f;
}


// first stage calls
//////////////////////

bool debpDCollisionHull::SphereHitsVolume(debpDCollisionSphere *sphere){
	return false;
}

bool debpDCollisionHull::CylinderHitsVolume(debpDCollisionCylinder *cylinder){
	return false;
}
	
bool debpDCollisionHull::CapsuleHitsVolume(debpDCollisionCapsule *capsule){
	return false;
}

bool debpDCollisionHull::BoxHitsVolume(debpDCollisionBox *box){
	return false;
}

bool debpDCollisionHull::TriangleHitsVolume(debpDCollisionTriangle *triangle){
	return false;
}

bool debpDCollisionHull::FrustumHitsVolume(debpDCollisionFrustum *frustum){
	return false;
}


double debpDCollisionHull::SphereMoveHitsVolume(debpDCollisionSphere *sphere, const decDVector &displacement, decDVector *normal){
	return 1.0f;
}

double debpDCollisionHull::CylinderMoveHitsVolume(debpDCollisionCylinder *cylinder, const decDVector &displacement, decDVector *normal){
	return 1.0f;
}

double debpDCollisionHull::CapsuleMoveHitsVolume(debpDCollisionCapsule *capsule, const decDVector &displacement, decDVector *normal){
	return 1.0f;
}

double debpDCollisionHull::BoxMoveHitsVolume(debpDCollisionBox *box, const decDVector &displacement, decDVector *normal){
	return 1.0f;
}

double debpDCollisionHull::TriangleMoveHitsVolume(debpDCollisionTriangle *triangle, const decDVector &displacement, decDVector *normal){
	return 1.0f;
}

double debpDCollisionHull::FrustumMoveHitsVolume(debpDCollisionFrustum *frustum, const decDVector &displacement, decDVector *normal){
	return 1.0f;
}

double debpDCollisionHull::PointMoveHitsVolume(const decDVector &point, const decDVector &displacement, decDVector *normal){
	return 1.0;
}


// Enclosing Volumes
//////////////////////

void debpDCollisionHull::GetEnclosingSphere(debpDCollisionSphere *sphere){
	if(pPoints.IsEmpty()){
		sphere->SetCenter({});
		sphere->SetRadius(0.0);
		return;
	
	}
	
	decDVector center;
	pPoints.Visit([&](const decDVector &point){
		center += point;
	});
	center /= pPoints.GetCount();
	sphere->SetCenter(center);
	
	double radiusSquared = 0.0;
	pPoints.Visit([&](const decDVector &point){
		radiusSquared = decMath::max(radiusSquared, (point - center).LengthSquared());
	});
	sphere->SetRadius(sqrt(radiusSquared));
}

void debpDCollisionHull::GetEnclosingBox(debpDCollisionBox *box){
	decDVector minExt(vDblMax, vDblMax, vDblMax);
	decDVector maxExt(-vDblMax, -vDblMax, -vDblMax);
	
	pPoints.Visit([&](const decDVector &point){
		minExt.SetSmallest(point);
		maxExt.SetLargest(point);
	});
	
	box->SetFromExtends(minExt, maxExt);
}


// Miscellaneous Functions
////////////////////////////

bool debpDCollisionHull::IsPointInside(const decDVector &point){
	return false;
}

decDVector debpDCollisionHull::ClosestPointTo(const decDVector &point){
	return point;
}


// Visiting
/////////////

void debpDCollisionHull::Visit(debpDCollisionVolumeVisitor *visitor){
	DEASSERT_NOTNULL(visitor)
	visitor->VisitHull(this);
}
