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
#ifndef _AEANIMATORLOCOMOTION_H_
#define _AEANIMATORLOCOMOTION_H_

// includes
#include "aeALColliderListener.h"
#include "aeALTouchSensorListener.h"
#include "aeALGroundChecker.h"

#include <deigde/gui/wrapper/debugdrawer/igdeWDebugDrawerShape.h>

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/math/smooth/decSmoothFloat.h>
#include <dragengine/common/math/smooth/decSmoothVector.h>
#include <dragengine/src/resources/debug/deDebugDrawer.h>
#include <dragengine/src/resources/sensor/deTouchSensor.h>
#include <dragengine/src/resources/collider/deColliderVolume.h>

// predefinitions
class aeAnimator;
class aeAnimatorLocomotionLeg;

class deLogger;



/**
 * Animator Locomotion.
 *
 * Stores information about locomotion testing of an animator.
 */
class aeAnimatorLocomotion{
public:
	/** Locomotion type. */
	enum eLocomotionTypes{
		/**
		 * Natural locomotion.
		 * \details Actor is oriented along moving direction while upper body
		 *          and head are turned towards view direction with animation.
		 */
		eltNatural,
		
		/**
		 * FPS Locomotion.
		 * \details Actor is oriented along view direction while lower body and
		 *          feet are turned towards moving direction with animation.
		 */
		eltFPS,
		
		/**
		 * Vehicle locomotion.
		 * \details Similar to natural locomotion but without director input on
		 *          the moving direction but turning the actor.
		 */
		eltVehicle
	};
	
	/** Attributes controllers can be linked to. */
	enum eAttributes{
		/** None. */
		eaNone,
		
		/** Elapsed time modified by play speed. */
		eaElapsedTime,
		
		/** Look up and down. */
		eaLookUpDown,
		
		/** Look left and right. */
		eaLookLeftRight,
		
		/** Moving speed in m/s. */
		eaMovingSpeed,
		
		/** Moving direction. */
		eaMovingDirection,
		
		/** Relative moving speed in m/s. */
		eaRelativeMovingSpeed,
		
		/** Turning speed. */
		eaTurningSpeed,
		
		/** Stance. */
		eaStance,
		
		/** Displacement in meters. */
		eaDisplacement,
		
		/** Playback time turn-inplace. */
		eaTimeTurnIP,
		
		/** Body Tilt Offset. */
		eaBodyTiltOffset,
		
		/** Body Tilt Up-Down. */
		eaBodyTiltUpDown,
		
		/** Body Tilt Right-Left. */
		eaBodyTiltRightLeft,
		
		/** Leg ground position. */
		eaLegGroundPosition,
		
		/** Leg ground normal. */
		eaLegGroundNormal,
		
		/** Leg influence. */
		eaLegInfluence,
		
		/** Leg position. */
		eaLegPosition,
		
		/** Leg orientation. */
		eaLegOrientation,
		
		/** Relative displacement in meters. */
		eaRelativeDisplacement
	};
	
	
	
private:
	aeAnimator *pAnimator;
	
	deDebugDrawer::Ref pDebugDrawer;
	igdeWDebugDrawerShape pDDSCollider;
	igdeWDebugDrawerShape pDDSSensorGround;
	
	aeALColliderListener pColliderListener;
	aeALTouchSensorListener pTouchSensorListener;
	aeALGroundChecker pGroundChecker;
	
	deColliderVolume::Ref pCollider;
	decVector pColliderPosition;
	float pColliderRadius;
	
	deTouchSensor::Ref pTouchSensor;
	decVector pTSGroundPosition;
	decVector pTSGroundExtents;
	decVector pTSDragonBodyPosition;
	decVector pTSDragonBodySize;
	deColliderVolume::Ref pGroundCheckCollider;
	deColliderVolume::Ref pTGCollider;
	
	float pGroundPlaneFront;
	float pGroundPlaneBack;
	float pGroundPlaneLeft;
	float pGroundPlaneRight;
	float pGroundPlaneRadius;
	float pGroundPlaneOffsetFL;
	float pGroundPlaneOffsetFR;
	float pGroundPlaneOffsetBL;
	float pGroundPlaneOffsetBR;
	deColliderVolume::Ref pGroundPlaneCollider;
	igdeWDebugDrawerShape pDDSGroundPlane[4];
	
	igdeWDebugDrawerShape pDDSDragonColBody;
	decVector pDragonColBodyPos;
	decVector pDragonColBodyRot;
	decVector pDragonColBodySize;
	deColliderVolume::Ref pDragonColBody;
	igdeWDebugDrawerShape pDDSDragonColHead;
	decVector pDragonColHeadPos;
	decVector pDragonColHeadRot;
	decVector pDragonColHeadSize;
	igdeWDebugDrawerShape pDDSDragonColHands;
	decVector pDragonColHandsPos;
	float pDragonColHandsRadius;
	igdeWDebugDrawerShape pDDSDragonColFeet;
	decVector pDragonColFeetPos;
	float pDragonColFeetRadius;
	
	deColliderVolume::Ref pFootCollider;
	float pDragonFootGround;
	float pDragonFootFront;
	float pDragonFootBack;
	float pDragonFootSpread;
	float pDragonFootRadius;
	
	aeAnimatorLocomotionLeg **pLegs;
	int pLegCount;
	int pUseLegPairCount;
	float pLegBlendTime;
	deColliderVolume *pLegGCCollider;
	float pLegGCColliderRadius;
	
	eLocomotionTypes pLocomotionType;
	
	float pLimitLookDown;
	float pLimitLookUp;
	decSmoothFloat pLookUpDown;
	
	float pLimitLookRight;
	float pLimitLookLeft;
	decSmoothFloat pLookLeftRight;
	
	float pTurnLeftRight;
	
	float pAdjustTimeTurnIP;
	
	float pSpeedWalk;
	float pSpeedRun;
	
	decDVector pPosition;
	decSmoothFloat pOrientation;
	float pTurnVelocity;
	float pTiltOffset;
	float pTiltUpDown;
	float pTiltLeftRight;
	decVector pView;
	decSmoothVector pLinearVelocity;
	decVector pAngularVelocity;
	decQuaternion pOrientQuat;
	float pVelocityOrientation;
	float pMovingSpeed;
	float pRelativeMovingSpeed;
	float pMovingDirection;
	decSmoothFloat pStance;
	bool pTouchingGround;
	
	float pMotionTime;
	
	bool pKeyForward;
	bool pKeyBackward;
	bool pKeyStepLeft;
	bool pKeyStepRight;
	float pKeyInputDirection;
	bool pToggleCrouch;
	bool pToggleRun;
	
	bool pEnabled;
	bool pUseFoGIK;
	
	bool pIsMoving;
	bool pIsTurningIP;
	float pTurnIP;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a animator locomotion. */
	aeAnimatorLocomotion(aeAnimator *animator);
	/** Cleans up the animator locomotion. */
	~aeAnimatorLocomotion();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Resets the locomotion system for a new simulation round. */
	void Reset();
	
	/** Retrieves the animator. */
	inline aeAnimator *GetAnimator() const{ return pAnimator; }
	/** Retrieves the logger. */
	deLogger *GetLogger() const;
	
	/** Retrieves the debug drawer. */
	inline const deDebugDrawer::Ref &GetDebugDrawer() const{ return pDebugDrawer; }
	/** Retrieves the collider to use for locomotion. */
	inline const deColliderVolume::Ref &GetCollider() const{ return pCollider; }
	/** Retrieves the position of the collider relative to the actor. */
	inline const decVector &GetColliderPosition() const{ return pColliderPosition; }
	/** Sets the position of the collider relative to the actor. */
	void SetColliderPosition(const decVector &position);
	/** Retrieves the radius of the collider. */
	inline float GetColliderRadius() const{ return pColliderRadius; }
	/** Sets the radius of the collider. */
	void SetColliderRadius(float radius);
	/** Updates the collider. */
	void UpdateCollider();
	/** Updates the touch sensors shapes. */
	void UpdateTouchSensors();
	/** Updates the touch sensor colors. */
	void UpdateTSColors();
	
	/** Determines if collider and touch sensor shapes are shown. */
	bool GetShowShapes() const;
	/** Sets if collider and touch sensor shapes are shown. */
	void SetShowShapes(bool showShapes);
	
	/** Retrieves the touch sensor. */
	inline const deTouchSensor::Ref &GetTouchSensor() const{ return pTouchSensor; }
	/** Retrieves the collider used for touch ground rules. */
	inline const deColliderVolume::Ref &GetTGCollider() const{ return pTGCollider; }
	
	/** Locomotion type. */
	inline eLocomotionTypes GetLocomotionType() const{ return pLocomotionType; }
	/** Set locomotion type. */
	void SetLocomotionType(eLocomotionTypes type);
	
	/** Retrieves the number of legs. */
	inline int GetLegCount() const{ return pLegCount; }
	/** Retrieves a leg by index. */
	aeAnimatorLocomotionLeg *GetLegAt(int index) const;
	/** Retrieves the number of leg pairs to use. */
	inline int GetUseLegPairCount() const{ return pUseLegPairCount; }
	/** Sets the number of leg pairs to use. */
	void SetUseLegPairCount(int pairCount);
	/** Retrieves the leg blend time in seconds. */
	inline float GetLegBlendTime() const{ return pLegBlendTime; }
	/** Sets the leg blend time in seconds. */
	void SetLegBlendTime(float time);
	
	
	
	/** Look down limit in degrees. */
	inline float GetLimitLookDown() const{ return pLimitLookDown; }
	
	/** Set look down limit in degrees. */
	void SetLimitLookDown(float degrees);
	
	/** Look up limit in degrees. */
	inline float GetLimitLookUp() const{ return pLimitLookUp; }
	
	/** Set look up limit in degrees. */
	void SetLimitLookUp(float degrees);
	
	/** Look up down angle in degrees. */
	inline decSmoothFloat &GetLookUpDown(){ return pLookUpDown; }
	inline const decSmoothFloat &GetLookUpDown() const{ return pLookUpDown; }
	
	/** Set look up down angle in degrees clamped to range. */
	void SetLookUpDown(float degrees);
	
	/** Set look up down goal angle in degrees clamped to range. */
	void SetLookUpDownGoal(float degrees);
	
	
	
	/** Look left limit in degrees. */
	inline float GetLimitLookLeft() const{ return pLimitLookLeft; }
	
	/** Set look left limit in degrees. */
	void SetLimitLookRight(float degrees);
	
	/** Look right limit in degrees. */
	inline float GetLimitLookRight() const{ return pLimitLookRight; }
	
	/** Set look right limit in degrees. */
	void SetLimitLookLeft(float degrees);
	
	/** Look right angle in degrees. */
	inline decSmoothFloat &GetLookLeftRight(){ return pLookLeftRight; }
	inline const decSmoothFloat &GetLookLeftRight() const{ return pLookLeftRight; }
	
	
	
	/** Adjustment speed for turning in place. */
	inline float GetAdjustTimeTurnIP() const{ return pAdjustTimeTurnIP; }
	
	/** Set adjustment speed for turning in place. */
	void SetAdjustTimeTurnIP(float adjustTime);
	
	
	
	/** Retrieves the walk speed. */
	inline float GetWalkSpeed() const{ return pSpeedWalk; }
	/** Sets the walk speed. */
	void SetWalkSpeed(float speed);
	/** Retrieves the run speed. */
	inline float GetRunSpeed() const{ return pSpeedRun; }
	/** Sets the run speed. */
	void SetRunSpeed(float speed);
	
	/** Retrieves the position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	/** Sets the position. */
	void SetPosition(const decDVector &position);
	/** Retrieves the view vector. */
	inline const decVector &GetView() const{ return pView; }
	/** Retrieves the orientation quaternion. */
	inline const decQuaternion &GetOrientationQuaternion() const{ return pOrientQuat; }
	
	/** Orientation angle in degrees. */
	inline decSmoothFloat &GetOrientation(){ return pOrientation; }
	inline const decSmoothFloat &GetOrientation() const{ return pOrientation; }
	
	/** Set orientation in degrees. */
	void SetOrientation(float orientation);
	
	/** Retrieves the tilt offset in meters. */
	inline float GetTiltOffset() const{ return pTiltOffset; }
	/** Sets the tilt offset in meters. */
	void SetTiltOffset(float offset);
	/** Retrieves the up-down tilt in degrees. */
	inline float GetTiltUpDown() const{ return pTiltUpDown; }
	/** Sets the up-down tilt in degrees. */
	void SetTiltUpDown(float tilt);
	/** Retrieves the left-right tilt in degrees. */
	inline float GetTiltLeftRight() const{ return pTiltLeftRight; }
	/** Sets the left-right tilt in degrees. */
	void SetTiltLeftRight(float tilt);
	
	/** Linear velocity. */
	inline decSmoothVector &GetLinearVelocity(){ return pLinearVelocity; }
	inline const decSmoothVector &GetLinearVelocity() const{ return pLinearVelocity; }
	
	/** Angular velocity. */
	inline const decVector &GetAngularVelocity() const{ return pAngularVelocity; }
	
	/** Set linearvelocity. */
	void SetAngularVelocity(const decVector &velocity);
	
	/** Retrieves the turning velocity. */
	inline float GetTurnVelocity() const{ return pTurnVelocity; }
	/** Sets the turning velocity. */
	void SetTurnVelocity(float velocity);
	
	/** Velocity orientation around world Y axis. */
	inline float GetVelocityOrientation() const{ return pVelocityOrientation; }
	
	/** Set velocity orientation around world Y axis. */
	void SetVelocityOrientation(float orientation);
	
	/** Moving speed in meter per seconds. */
	inline float GetMovingSpeed() const{ return pMovingSpeed; }
	
	/** Set moving speed in meter per seconds. */
	void SetMovingSpeed(float speed);
	
	/**
	 * Relative moving speed in meter per seconds.
	 * 
	 * Moving backwards returns negative speed.
	 */
	inline float GetRelativeMovingSpeed() const{ return pRelativeMovingSpeed; }
	
	/**
	 * Set relative moving speed in meter per seconds.
	 * 
	 * For moving backwards set negative speed.
	 */
	void SetRelativeMovingSpeed(float speed);
	
	/** Retrieves the moving direction in degrees relative to the view direction. */
	inline float GetMovingDirection() const{ return pMovingDirection; }
	/** Sets the moving direction in degrees relative to the view direction. */
	void SetMovingDirection(float movingDirection);
	
	/** Stance ranging from 0 for uprect to 1 crouching. */
	inline decSmoothFloat &GetStance(){ return pStance; }
	inline const decSmoothFloat &GetStance() const{ return pStance; }
	
	/** Determines if the forward key is pressed. */
	inline bool GetKeyForward() const{ return pKeyForward; }
	/** Sets if the forward key is pressed. */
	void SetKeyForward(bool pressed);
	/** Determines if the backward key is pressed. */
	inline bool GetKeyBackward() const{ return pKeyBackward; }
	/** Sets if the backward key is pressed. */
	void SetKeyBackward(bool pressed);
	/** Determines if the step left key is pressed. */
	inline bool GetKeyStepLeft() const{ return pKeyStepLeft; }
	/** Sets if the step left key is pressed. */
	void SetKeyStepLeft(bool pressed);
	/** Determines if the step right key is pressed. */
	inline bool GetKeyStepRight() const{ return pKeyStepRight; }
	/** Sets if the step right key is pressed. */
	void SetKeyStepRight(bool pressed);
	
	/** Key input direction. */
	inline float GetKeyInputDirection() const{ return pKeyInputDirection; }
	
	/** Set key input direction. */
	void SetKeyInputDirection(float direction);
	
	/** Determines if the crouching is switched on. */
	inline bool GetToggleCrouch() const{ return pToggleCrouch; }
	/** Sets if the crouching is switched on. */
	void SetToggleCrouch(bool toggled);
	/** Determines if running is switched on. */
	inline bool GetToggleRun() const{ return pToggleRun; }
	/** Sets if running is switched on. */
	void SetToggleRun(bool toggled);
	
	/** Updates the locomotion using the given elapsed time in seconds. */
	void Update(float elapsed);
	/** Post physics. */
	void PostPhysics();
	/** Feet on ground. */
	void FeetOnGround();
	/** Post update. */
	void PostUpdate();
	
	/**
	 * Updates the position after physics testing. Also projects the actor to the
	 * ground doing typical tests for missing ground and alike.
	 */
	void OnColliderChanged();
	
	/** Determines if the locomotion testing is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	/** Sets if the locomotion testing is enabled. */
	void SetEnabled(bool enabled);
	/** Determines if the feet-on-ground IK is used. */
	inline bool GetUseFoGIK() const{ return pUseFoGIK; }
	/** Sets if the feet-on-ground IK is used. */
	void SetUseFoGIK(bool use);
	/*@}*/
	
private:
	void pCleanUp();
	void pCreateCollider();
	void pCreateTouchSensor();
	
	void pUpdateLooking(float elapsed);
	void pUpdateTurnLeftRight(float elapsed);
	void pUpdateIsMoving();
	void pUpdateOrientation(float elapsed);
	void pUpdateOrientationMoving(float elapsed, float &adjustOrientation);
	void pUpdateOrientationNotMoving(float elapsed, float &adjustOrientation);
	float pCalcMovingDirectionNatural() const;
	void pCheckLookingRangeViolation(float &adjustOrientation);
	void pUpdateLinearVelocity(float elapsed);
	void pUpdateStance(float elapsed);
};

#endif
