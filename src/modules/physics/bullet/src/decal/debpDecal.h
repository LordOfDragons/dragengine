/* 
 * Drag[en]gine Bullet Physics Module
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
	debpDecal( deDecal *decal );
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
	void SetParentComponent( debpComponent *component );
	
	/** Determines if the decal touches with the given volume. */
	bool TouchesVolume( debpDCollisionVolume *volume ) const;
	/** Determines if a point is in the decal. */
	bool TouchesPoint( const decVector &point ) const;
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
