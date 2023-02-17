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

// include only once
#ifndef _DEOGLTRANSFORMVOLUME_H_
#define _DEOGLTRANSFORMVOLUME_H_

// includes
#include "../utils/collision/deoglDCollisionVolumeVisitor.h"

// predefintions
class deoglDCollisionVolume;
class deoglDCollisionSphere;
class deoglDCollisionCylinder;
class deoglDCollisionCapsule;
class deoglDCollisionBox;
class deoglDCollisionTriangle;
class deoglDCollisionFrustum;



/**
 * Visitor for transforming a collision volume.
 * Provides support for transforming arbitrary collision volumes using
 * a transformation that can compose of translations, rotations
 * and scaling. The class is written to reuse transformed collision
 * volumes to avoid memory allocations. Therefore the returned collision
 * volume pointer can be the same as one obtained in an earlier call.
 * Hence make sure that nobody uses the same transform volume visitor
 * while you are using a transformed volume.
 */
class deoglTransformVolume : public deoglDCollisionVolumeVisitor{
private:
	deoglDCollisionSphere *pSphere;
	deoglDCollisionCylinder *pCylinder;
	deoglDCollisionCapsule *pCapsule;
	deoglDCollisionBox *pBox;
	deoglDCollisionTriangle *pTriangle;
	deoglDCollisionFrustum *pFrustum;
	
	deoglDCollisionVolume *pVolume;
	
	decDVector pTranslation;
	decQuaternion pRotation;
	decVector pScaling;
	decDMatrix pMatrix;
	bool pDirtyMatrix;
	
public:
	// constructor, destructor
	deoglTransformVolume();
	~deoglTransformVolume();
	
	// management
	inline const decDVector &GetTranslation() const{ return pTranslation; }
	void SetTranslation( const decDVector &translation );
	inline const decQuaternion &GetRotation() const{ return pRotation; }
	void SetRotation( const decQuaternion &rotation );
	inline const decVector &GetScaling() const{ return pScaling; }
	void SetScaling( const decVector &scaling );
	inline deoglDCollisionVolume *GetVolume() const{ return pVolume; }
	
	// convenience function
	deoglDCollisionVolume *GetVolumeFor( deoglDCollisionVolume *volume );
	
	// Visiting
	virtual void VisitSphere( deoglDCollisionSphere *sphere );
	virtual void VisitCylinder( deoglDCollisionCylinder *cylinder );
	virtual void VisitCapsule( deoglDCollisionCapsule *capsule );
	virtual void VisitBox( deoglDCollisionBox *box );
	virtual void VisitTriangle( deoglDCollisionTriangle *triangle );
	virtual void VisitFrustum( deoglDCollisionFrustum *frustum );
	
private:
	void pCleanUp();
	void pUpdateMatrix();
};

// end of include only once
#endif
