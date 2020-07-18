/* 
 * Drag[en]gine IGDE Rig Editor
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
#ifndef _RECAMERA_H_
#define _RECAMERA_H_

// includes
#include <deigde/gui/igdeCamera.h>

// predefinitions
class reRig;
class reRigBone;



/**
 * @brief Camera with attachment possibility.
 *
 * Manages a camera including the wrapped engine camera. Provides also
 * some simple unproject methods to determine a ray shot into the world
 * as seen by this camera. The camera is managed in a lazy way. Hence
 * the engine is not create or updated until it is acquired by the user.
 * This subclass contains attachment specific information.
 */
class reCamera : public igdeCamera{
private:
	reRig *pRig;
	
	reRigBone *pBone;
	
	decDVector pFreePosition;
	decVector pFreeOrientation;
	float pFreeDistance;
	decDVector pRelPosition;
	decVector pRelOrientation;
	bool pDirty;
	bool pAttachToBone;
	bool pNoNotify;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new camera. */
	reCamera( reRig *rig, deEngine *engine );
	/** Cleans up the camera object. */
	virtual ~reCamera();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the bone or NULL. */
	inline reRigBone *GetBone() const{ return pBone; }
	/** Sets the bone or NULL. */
	void SetBone( reRigBone *bone );
	/** Retrieves the free position. */
	inline const decDVector &GetFreePosition() const{ return pFreePosition; }
	/** Sets the free position. */
	void SetFreePosition( const decDVector &freePosition );
	/** Retrieves the free orientation. */
	inline const decVector &GetFreeOrientation() const{ return pFreeOrientation; }
	/** Sets the free orientation. */
	void SetFreeOrientation( const decVector &freeOrientation );
	/** Retrieves the distance of camera to the center point along the view direction. */
	inline float GetFreeDistance() const{ return pFreeDistance; }
	/** Sets the distance of camera to the center point along the view direction. */
	void SetFreeDistance( float freeDistance );
	/** Retrieves the relative position. */
	inline const decDVector &GetRelativePosition() const{ return pRelPosition; }
	/** Sets the relative position. */
	void SetRelativePosition( const decDVector &relativePosition );
	/** Retrieves the relative orientation. */
	inline const decVector &GetRelativeOrientation() const{ return pRelOrientation; }
	/** Sets the relative orientation. */
	void SetRelativeOrientation( const decVector &relativeOrientation );
	/** Determines if this camera is attached to a bone. */
	inline bool GetAttachToBone() const{ return pAttachToBone; }
	/** Sets if this camera is attached to a bone. */
	void SetAttachToBone( bool attachToBone );
	
	/** Updates the camera. */
	void Update();
	
	/** Resets the camera. */
	virtual void Reset();
	
	/** Parameter changed. */
	virtual void ParameterChanged();
	/** Adaption changed. */
	virtual void AdaptionChanged();
	/*@}*/
};

// end of include only once
#endif
