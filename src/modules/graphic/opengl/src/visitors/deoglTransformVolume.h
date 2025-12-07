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
	void SetTranslation(const decDVector &translation);
	inline const decQuaternion &GetRotation() const{ return pRotation; }
	void SetRotation(const decQuaternion &rotation);
	inline const decVector &GetScaling() const{ return pScaling; }
	void SetScaling(const decVector &scaling);
	inline deoglDCollisionVolume *GetVolume() const{ return pVolume; }
	
	// convenience function
	deoglDCollisionVolume *GetVolumeFor(deoglDCollisionVolume *volume);
	
	// Visiting
	virtual void VisitSphere(deoglDCollisionSphere *sphere);
	virtual void VisitCylinder(deoglDCollisionCylinder *cylinder);
	virtual void VisitCapsule(deoglDCollisionCapsule *capsule);
	virtual void VisitBox(deoglDCollisionBox *box);
	virtual void VisitTriangle(deoglDCollisionTriangle *triangle);
	virtual void VisitFrustum(deoglDCollisionFrustum *frustum);
	
private:
	void pCleanUp();
	void pUpdateMatrix();
};

// end of include only once
#endif
