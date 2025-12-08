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
#ifndef _RECAMERA_H_
#define _RECAMERA_H_

// includes
#include <deigde/gui/igdeCamera.h>

// predefinitions
class reRig;
#include "bone/reRigBone.h"



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
	
	reRigBone::Ref pBone;
	
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
	reCamera(reRig *rig, deEngine *engine);
	/** Cleans up the camera object. */
	virtual ~reCamera();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the bone or NULL. */
	inline const reRigBone::Ref &GetBone() const{ return pBone; }
	/** Sets the bone or NULL. */
	void SetBone(reRigBone *bone);
	/** Retrieves the free position. */
	inline const decDVector &GetFreePosition() const{ return pFreePosition; }
	/** Sets the free position. */
	void SetFreePosition(const decDVector &freePosition);
	/** Retrieves the free orientation. */
	inline const decVector &GetFreeOrientation() const{ return pFreeOrientation; }
	/** Sets the free orientation. */
	void SetFreeOrientation(const decVector &freeOrientation);
	/** Retrieves the distance of camera to the center point along the view direction. */
	inline float GetFreeDistance() const{ return pFreeDistance; }
	/** Sets the distance of camera to the center point along the view direction. */
	void SetFreeDistance(float freeDistance);
	/** Retrieves the relative position. */
	inline const decDVector &GetRelativePosition() const{ return pRelPosition; }
	/** Sets the relative position. */
	void SetRelativePosition(const decDVector &relativePosition);
	/** Retrieves the relative orientation. */
	inline const decVector &GetRelativeOrientation() const{ return pRelOrientation; }
	/** Sets the relative orientation. */
	void SetRelativeOrientation(const decVector &relativeOrientation);
	/** Determines if this camera is attached to a bone. */
	inline bool GetAttachToBone() const{ return pAttachToBone; }
	/** Sets if this camera is attached to a bone. */
	void SetAttachToBone(bool attachToBone);
	
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
