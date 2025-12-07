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

#ifndef _DEBPDECAL_H_
#define _DEBPDECAL_H_

#include <dragengine/systems/modules/physics/deBasePhysicsDecal.h>
#include <dragengine/common/math/decMath.h>

class deDecal;
class debpComponent;
class debpDecalFace;
class debpDCollisionVolume;



/**
 * @brief Bullet Physics Decal Peer
 * The peer for decal resources.
 */
class debpDecal : public deBasePhysicsDecal{
private:
	deDecal *pDecal;
	
	bool pDirtyGeometry;
	
	debpDecalFace *pFaces;
	int pFaceCount;
	
	debpComponent *pParentComponent;
	
public:
	// constructor, destructor
	debpDecal(deDecal *decal);
	~debpDecal();
	
	// management
	inline deDecal *GetDecal() const{ return pDecal; }
	
	/** Retrieves the number of faces. */
	inline int GetFaceCount() const{ return pFaceCount; }
	/** Retrieves the faces array. */
	inline debpDecalFace *GetFaces() const{ return pFaces; }
	
	/** Marks the decal dirty. */
	void MarkDirty();
	/** Updates the decal. */
	void Update();
	
	/** Retrieves the parent component. */
	inline debpComponent *GetParentComponent() const{ return pParentComponent; }
	/** Sets the parent component or NULL. */
	void SetParentComponent(debpComponent *component);
	
	/** Determines if the decal touches with the given volume. */
	bool TouchesVolume(debpDCollisionVolume *volume) const;
	/** Determines if a point is in the decal. */
	bool TouchesPoint(const decVector &point) const;
	/*@}*/
	
	/** @name Notifications */
	/*@{*/
	/** Skin changed. */
	virtual void SkinChanged();
	/** Geometry changed. */
	virtual void GeometryChanged();
	/** Visible changed. */
	virtual void VisibleChanged();
	/*@}*/
	
private:
	void pCleanUp();
	void pCreateMeshComponent();
};

// end of include only once
#endif
