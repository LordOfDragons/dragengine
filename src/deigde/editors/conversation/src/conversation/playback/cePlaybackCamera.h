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

#ifndef _CEPLAYBACKCAMERA_H_
#define _CEPLAYBACKCAMERA_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/curve/decCurveBezier.h>
#include "ceTarget.h"

class cePlayback;
class ceConversationActor;
class igdeCamera;



/**
 * \brief Playback Camera.
 * Handles the camera in the playback. Supports simple camera movements. The camera is
 * placed relative to a static coordinate system. The camera position is defined by a
 * position, rotation and distance. The distance and rotation allow for the position
 * of the camera to orbit around a virtual position. The actual orientation of the
 * camera is defined using the look-at position. The camera is always orientated upwards
 * in the coordinate system. The look-at position is relative to the coordinate system
 * too unless a look-at actor is defined. In this case the look-at position is relative
 * to the look-at actor coordinate system. Eventually the tilt of the camera along the
 * looking direction as well as the field of view can be defined. All of these parameters
 * except the coordinate system and the look-at actor can vary in a linear way from a
 * start to an end value.
 */
class cePlaybackCamera{
public:
	/** Camera shot parameters. */
	enum eParameters{
		epPositionX,
		epPositionY,
		epPositionZ,
		epLookAtX,
		epLookAtY,
		epLookAtZ,
		epTilt,
		epFov,
		EP_COUNT
	};
	
private:
	cePlayback &pPlayback;
	
	decMatrix pCoordSys;
	
	ceTarget::Ref pCameraTarget;
	decVector pOffsetCameraFrom;
	decVector pOffsetCameraTo;
	decVector pCameraOrbitFrom;
	decVector pCameraOrbitTo;
	float pCameraDistanceFrom;
	float pCameraDistanceTo;
	
	ceTarget::Ref pLookAtTarget;
	decVector pOffsetLookAtFrom;
	decVector pOffsetLookAtTo;
	
	decVector pPositionFrom;
	decVector pPositionTo;
	decVector pRotationFrom;
	decVector pRotationTo;
	float pTiltFrom;
	float pTiltTo;
	float pFovFrom;
	float pFovTo;
	
	bool pLockUpAxis;
	bool pAlignTargets;
	bool pRelativeToLookAt;
	bool pLockCameraTarget;
	bool pLockLookAtTarget;
	
	decCurveBezier pCurve[EP_COUNT];
	
	float pElapsed;
	float pDuration;
	
	decMatrix pLockedCameraMatrix;
	decMatrix pLockedLookAtMatrix;
	bool pHasLockedCameraMatrix;
	bool pHasLockedLookAtMatrix;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new playback camera. */
	cePlaybackCamera(cePlayback &playback);
	/** Cleans up the playback camera. */
	~cePlaybackCamera();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the coordinate system matrix. */
	inline const decMatrix &GetCoordinateSystem() const{ return pCoordSys; }
	/** Sets the coordinate system matrix. */
	void SetCoordinateSystem(const decMatrix &matrix);
	
	/** Retrieves the camera target or NULL if not set. */
	inline const ceTarget::Ref &GetCameraTarget() const{ return pCameraTarget; }
	/** Sets the camera target or NULL if not set. */
	void SetCameraTarget(ceTarget *target);
	/** Retrieves the start camera position. */
	inline const decVector &GetOffsetCameraFrom() const{ return pOffsetCameraFrom; }
	/** Sets the start camera position. */
	void SetOffsetCameraFrom(const decVector &position);
	/** Retrieves the end camera position. */
	inline const decVector &GetOffsetCameraTo() const{ return pOffsetCameraTo; }
	/** Sets the end camera position. */
	void SetOffsetCameraTo(const decVector &position);
	/** Retrieves the start camera orbit rotation in degrees. */
	inline const decVector &GetCameraOrbitFrom() const{ return pCameraOrbitFrom; }
	/** Sets the start camera orbit rotation in degrees. */
	void SetCameraOrbitFrom(const decVector &rotation);
	/** Retrieves the end camera orbit rotation. */
	inline const decVector &GetCameraOrbitTo() const{ return pCameraOrbitTo; }
	/** Sets the end camera orbit rotation. */
	void SetCameraOrbitTo(const decVector &rotation);
	/** Retrieves the start camera orbit distance. */
	inline float GetCameraDistanceFrom() const{ return pCameraDistanceFrom; }
	/** Sets the start camera orbit distance. */
	void SetCameraDistanceFrom(float distance);
	/** Retrieves the end camera orbit distance. */
	inline float GetCameraDistanceTo() const{ return pCameraDistanceTo; }
	/** Sets the end camera orbit distance. */
	void SetCameraDistanceTo(float distance);
	
	/** Retrieves the look-at target or NULL if not set. */
	inline const ceTarget::Ref &GetLookAtTarget() const{ return pLookAtTarget; }
	/** Sets the look-at target or NULL if not set. */
	void SetLookAtTarget(ceTarget *target);
	/** Retrieves the start look-at position. */
	inline const decVector &GetLookAtFrom() const{ return pOffsetLookAtFrom; }
	/** Sets the start look-at position. */
	void SetLookAtFrom(const decVector &position);
	/** Retrieves the end look-at position. */
	inline const decVector &GetLookAtTo() const{ return pOffsetLookAtTo; }
	/** Sets the end look-at position. */
	void SetLookAtTo(const decVector &position);
	
	/** \brief Retrieves the start position. */
	inline const decVector &GetPositionFrom() const{ return pPositionFrom; }
	/** \brief Sets the start camera position. */
	void SetPositionFrom(const decVector &position);
	/** \brief Retrieves the end position. */
	inline const decVector &GetPositionTo() const{ return pPositionTo; }
	/** \brief Sets the end position. */
	void SetPositionTo(const decVector &position);
	/** \brief Retrieves the start rotation in degrees. */
	inline const decVector &GetRotationFrom() const{ return pRotationFrom; }
	/** \brief Sets the start rotation in degrees. */
	void SetRotationFrom(const decVector &rotation);
	/** \brief Retrieves the end rotation. */
	inline const decVector &GetRotationTo() const{ return pRotationTo; }
	/** \brief Sets the end rotation. */
	void SetRotationTo(const decVector &rotation);
	/** Retrieves the start camera tilt in degrees. */
	inline float GetTiltFrom() const{ return pTiltFrom; }
	/** Sets the start camera tilt in degrees. */
	void SetTiltFrom(float tilt);
	/** Retrieves the end camera tilt in degrees. */
	inline float GetTiltTo() const{ return pTiltTo; }
	/** Sets the end camera tilt in degrees. */
	void SetTiltTo(float tilt);
	/** Retrieves the start camera fov in degrees. */
	inline float GetFovFrom() const{ return pFovFrom; }
	/** Sets the start camera fov in degrees. */
	void SetFovFrom(float fov);
	/** Retrieves the end camera fov in degrees. */
	inline float GetFovTo() const{ return pFovTo; }
	/** Sets the end camera fov in degrees. */
	void SetFovTo(float fov);
	
	/** Determines if the targets are aligned along the conversation coordinate system. */
	inline bool GetAlignTargets() const{ return pAlignTargets; }
	/** Sets if the targets are aligned along the conversation coordinate system. */
	void SetAlignTargets(bool alignTargets);
	/**
	 * \brief Determines if the target coordinate system up axes are locked to the
	 *        conversation coordinate system up axis while aligning targets.
	 */
	inline bool GetLockUpAxis() const{ return pLockUpAxis; }
	/**
	 * \brief Sets if the target coordinate system up axes are locked to the
	 *        conversation coordinate system up axis while aligning targets.
	 */
	void SetLockUpAxis(bool lockUpAxis);
	/** Determines if the camera is relative to the camera target or the look-at target. */
	inline bool GetRelativeToLookAt() const{ return pRelativeToLookAt; }
	/** Sets if the camera is relative to the camera target or the look-at target. */
	void SetRelativeToLookAt(bool relativeToLookAt);
	/** \brief Determines if the camera target position is locked during the camera shot. */
	inline bool GetLockCameraTarget() const{ return pLockCameraTarget; }
	/** \brief Sets if the camera target position is locked during the camera shot. */
	void SetLockCameraTarget(bool lock);
	/** \brief Determines if the look-at target position is locked during the camera shot. */
	inline bool GetLockLookAtTarget() const{ return pLockLookAtTarget; }
	/** \brief Sets if the look-at target position is locked during the camera shot. */
	void SetLockLookAtTarget(bool lock);
	
	/** Retrieves a curve. */
	const decCurveBezier &GetParameterCurve(int parameter) const;
	/** Sets a curve. */
	void SetParameterCurve(int parameter, const decCurveBezier &curve);
	/** Sets a parameter curve to a constant value. */
	void SetParameterCurve(int parameter, float value);
	/** Sets a parameter curve to a linear interpolation value. */
	void SetParameterCurve(int parameter, float valueFrom, float valueTo);
	
	/** Retrieves the duration of the camera animation. */
	inline float GetDuration() const{ return pDuration; }
	/** Sets the duration of the camera animation. */
	void SetDuration(float duration);
	/** Rewind the animation. */
	void Rewind();
	/** Reset parameters. */
	void Reset();
	/** Set coordinate system for a single actor. */
	void SetCoordSys1Actor(ceConversationActor &actor1);
	/** Set coordinate system for two actors. */
	void SetCoordSys2Actors(ceConversationActor &actor1, ceConversationActor &actor2);
	
	/** Update camera. */
	void Update(igdeCamera &camera, float elapsed, bool clamp);
	/** Update camera using the constant method. */
	void UpdateConstant(igdeCamera &camera);
	/** Update camera using the linear method. */
	void UpdateLinear(igdeCamera &camera);
	/** Update camera using the curve method. */
	void UpdateCurve(igdeCamera &camera);
	
	/** \brief Update with the given set of values. */
	void UpdateWith(igdeCamera &camera, const decVector &offsetCamera, const decVector &offsetLookAt,
		const decVector &position, const decVector &rotation, float cameraDistance,
		const decVector &cameraOrbit, float fov);
	/*@}*/
};

#endif
