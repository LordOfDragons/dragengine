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
#ifndef _AECAMERA_H_
#define _AECAMERA_H_

// includes
#include <deigde/gui/igdeCamera.h>
#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyString.h>
#include <deigde/meta/property/igdeMetaPropertyVector.h>

#include <dragengine/common/string/decString.h>

// predefinitions
class aeAnimator;



/**
 * Camera with attachment possibility.
 *
 * Manages a camera including the wrapped engine camera. Provides also
 * some simple unproject methods to determine a ray shot into the world
 * as seen by this camera. The camera is managed in a lazy way. Hence
 * the engine is not create or updated until it is acquired by the user.
 * This subclass contains attachment specific information.
 */
class aeCamera : public igdeCamera{
public:
	using Ref = deTObjectReference<aeCamera>;
	
private:
	aeAnimator &pAnimator;
	
	decDVector pFreePosition;
	decVector pFreeOrientation;
	float pFreeDistance;
	bool pDirty;
	bool pNoNotify;
	
public:
	igdeMetaPropertyBooleanStorage::Storage mpAttachToBone;
	igdeMetaPropertyStringStorage::Storage mpBone;
	igdeMetaPropertyVectorStorage::Storage mpRelativePosition;
	igdeMetaPropertyVectorStorageQuaternion::Storage mpRelativeRotation;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new camera. */
	aeCamera(aeAnimator &animator, deEngine *engine);
	
protected:
	/** Cleans up the camera object. */
	~aeCamera() override;
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** Bone name. */
	inline const decString &GetBone() const{ return mpBone; }
	
	/** Set bone name. */
	void SetBone(const char *bone);
	
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
	inline const decVector &GetRelativePosition() const{ return mpRelativePosition; }
	/** Sets the relative position. */
	void SetRelativePosition(const decVector &relativePosition);
	/** Retrieves the relative orientation. */
	inline const decVector &GetRelativeOrientation() const{ return mpRelativeRotation; }
	/** Sets the relative orientation. */
	void SetRelativeOrientation(const decVector &relativeOrientation);
	/** Determines if this camera is attached to a bone. */
	inline bool GetAttachToBone() const{ return mpAttachToBone; }
	/** Sets if this camera is attached to a bone. */
	void SetAttachToBone(bool attachToBone);
	
	/** Updates the camera. */
	void Update();
	
	/** Resets the camera. */
	void Reset() override;
	
	/** Parameter changed. */
	void ParameterChanged() override;
	/** Adaption changed. */
	void AdaptionChanged() override;
	/*@}*/
};

// end of include only once
#endif
