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

#ifndef _CECAMERASHOT_H_
#define _CECAMERASHOT_H_

#include <dragengine/deObject.h>
#include <dragengine/common/curve/decCurveBezier.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class ceConversation;



/**
 * \brief Camera Shot.
 */
class ceCameraShot : public deObject{
	public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<ceCameraShot> Ref;
	
	
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
	ceConversation *pConversation;
	
	decString pName;
	
	int pActorCount;
	
	decString pCameraTarget;
	decVector pOffsetCameraFrom;
	decVector pOffsetCameraTo;
	
	decString pLookAtTarget;
	decVector pOffsetLookAtFrom;
	decVector pOffsetLookAtTo;
	
	decVector pCameraOrbitFrom;
	decVector pCameraOrbitTo;
	float pCameraDistanceFrom;
	float pCameraDistanceTo;
	
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
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new camera shot. */
	ceCameraShot(const char *name = "Camera Shot");
	/** Creates a new camera shot. */
	ceCameraShot(const ceCameraShot &cameraShot);
	/** Cleans up the camera shot. */
	virtual ~ceCameraShot();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the conversation or nullptr if not set. */
	inline ceConversation *GetConversation() const{ return pConversation; }
	/** Sets the conversation or nullptr if not set. */
	void SetConversation(ceConversation *conversation);
	
	/** Retrieves the name. */
	inline const decString &GetName() const{ return pName; }
	/** Sets the name. */
	void SetName(const char *name);
	
	/** Retrieves the number of actors required for this camera shot. */
	inline int GetActorCount() const{ return pActorCount; }
	/** Sets the number of actors required for this camera shot. */
	void SetActorCount(int count);
	
	/** Retrieves the camera target id. */
	inline const decString &GetCameraTarget() const{ return pCameraTarget; }
	/** Sets the camera target id. */
	void SetCameraTarget(const char *target);
	/** Retrieves the start camera position. */
	inline const decVector &GetOffsetCameraFrom() const{ return pOffsetCameraFrom; }
	/** Sets the start camera position. */
	void SetOffsetCameraFrom(const decVector &offset);
	/** Retrieves the end camera position. */
	inline const decVector &GetOffsetCameraTo() const{ return pOffsetCameraTo; }
	/** Sets the end camera position. */
	void SetOffsetCameraTo(const decVector &offset);
	
	/** Retrieves the look-at target id. */
	inline const decString &GetLookAtTarget() const{ return pLookAtTarget; }
	/** Sets the look-at target id. */
	void SetLookAtTarget(const char *target);
	/** Retrieves the start look-at position. */
	inline const decVector &GetOffsetLookAtFrom() const{ return pOffsetLookAtFrom; }
	/** Sets the start look-at position. */
	void SetOffsetLookAtFrom(const decVector &offset);
	/** Retrieves the end look-at position. */
	inline const decVector &GetOffsetLookAtTo() const{ return pOffsetLookAtTo; }
	/** Sets the end look-at position. */
	void SetOffsetLookAtTo(const decVector &offset);
	
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
	
	/** Determines if the conversation coordinate system up axis is locked to the look-at up axis. */
	inline bool GetLockUpAxis() const{ return pLockUpAxis; }
	/** Sets if the conversation coordinate system up axis is locked to the look-at up axis. */
	void SetLockUpAxis(bool lockUpAxis);
	/** Determines if the targets are aligned along the conversation coordinate system. */
	inline bool GetAlignTargets() const{ return pAlignTargets; }
	/** Sets if the targets are aligned along the conversation coordinate system. */
	void SetAlignTargets(bool alignTargets);
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
	/*@}*/
};

#endif
