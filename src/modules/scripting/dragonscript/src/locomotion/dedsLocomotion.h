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

#ifndef _DEDSLOCOMOTION_H_
#define _DEDSLOCOMOTION_H_

#include "dedsLControllerMapping.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/math/smooth/decSmoothVector.h>
#include <dragengine/common/math/smooth/decSmoothFloat.h>
#include <dragengine/deObject.h>

class deColliderCollisionTest;
class deCollider;
class deAnimatorInstance;
class decBaseFileReader;
class decBaseFileWriter;


/**
 * \brief Locomotion implementation similar to animator editor.
 */
class dedsLocomotion : public deObject{
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<dedsLocomotion> Ref;


public:
	/** \brief Tilt modes. */
	enum eTiltModes{
		/** \brief No tilt calculation. */
		etmNone,
		
		/**
		 * \brief Calculate tilt from the hit normal of a a single ground test.
		 * 
		 * The single test is cast down at the center of the actor. The hit normal is
		 * used to calculate the tilt in both directions relative to the actor coordinate
		 * system. This test mode is the fastest and suitable for simple bipedal actors
		 * in simple shaped worlds.
		 */
		etmSingle,
		
		/**
		 * \brief Calculate tilt from 4 weighted ground tests in a box shape.
		 * 
		 * 4 samples arranged in a box around front and back feet are cast down and weighted.
		 * This test mode improves the quality over dedsLocomotion::etmSingle but is more
		 * expensive to calculate. It is suited for all kinds of actors or complex worlds.
		 */
		etmWeighted
	};
	
private:
	bool pCanTurn, pAlwaysLimitLeftRight;
	
	float pLimitLookUp;
	float pLimitLookDown;
	decSmoothFloat pLookVertical;
	
	float pLimitLookLeft;
	float pLimitLookRight;
	decSmoothFloat pLookHorizontal;
	bool pTurnAdjustLookHorizontal;
	
	decSmoothFloat pAnalogMovingHorizontal; // rename to pMoveDirection (input moving direction)
	float pAnalogMovingVertical; // not used
	float pTurnHorizontal;
	
	float pAnalogMovingSpeed; // rename to pMoveSpeed (input moving speed)
	
	float pAdjustTimeOrientation;
	float pCLimAccelTimeOrientation;
	float pOrientation;
	decQuaternion pOrientationQuaternion;
	float pTurningSpeed;
	decVector pAngularVelocity;
	
	bool pIsMoving;
	float pMovingSpeed; // rename to avoid name confusion with pMoveSpeed
	float pMovingOrientation; // rename to avoid name confusion with pMove*
	float pMovingDirection; // rename to avoid name confusion with pMoveDirection
	
	decSmoothVector pLinearVelocity;
	
	decSmoothFloat pStance;
	
	bool pIsTurningIP;
	float pAdjustTimeTurnIP;
	float pTurnIP;
	bool pResetTimeTurnIP;
	bool pReverseTimeTurnIP;
	
	bool pCanTurnInPlace;
	float pLimitTurnInPlaceLeft;
	float pLimitTurnInPlaceRight;
	
	bool pResetTimeWalk;
	
	bool pCanTilt;
	eTiltModes pTiltMode;
	deColliderCollisionTest::Ref pCCTTiltSingle;
	deColliderCollisionTest::Ref pCCTTiltFrontLeft;
	deColliderCollisionTest::Ref pCCTTiltFrontRight;
	deColliderCollisionTest::Ref pCCTTiltBackLeft;
	deColliderCollisionTest::Ref pCCTTiltBackRight;
	
	float pLimitTiltUp;
	float pLimitTiltDown;
	decSmoothFloat pTiltVertical;
	
	float pLimitTiltLeft;
	float pLimitTiltRight;
	decSmoothFloat pTiltHorizontal;
	
	float pTiltOffset;
	
	dedsLControllerMapping *pControllerMappings;
	int pControllerMappingCount;
	int pControllerMappingSize;
	
	deCollider::Ref pAICollider;
	bool pUpdateAIColliderAngularVelocity;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new locomotion. */
	dedsLocomotion();
	
	/** \brief Clean up the locomotion. */
	virtual ~dedsLocomotion();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Actor can turn. */
	inline bool GetCanTurn() const{ return pCanTurn; }
	
	/** \brief Set if actor can turn. */
	void SetCanTurn(bool canTurn);
	
	/** \brief Look left/right is always limited. */
	inline bool GetAlwaysLimitLeftRight() const{ return pAlwaysLimitLeftRight; }
	
	/** \brief Set if look left/right is always limited. */
	void SetAlwaysLimitLeftRight(bool alwaysLimitLeftRight);
	
	
	
	/** \brief Looking up limit angle in degrees. */
	inline float GetLimitLookUp() const{ return pLimitLookUp; }
	
	/** \brief Set looking up limit angle in degrees. */
	void SetLimitLookUp(float limit);
	
	/** \brief Looking down limit angle in degrees. */
	inline float GetLimitLookDown() const{ return pLimitLookDown; }
	
	/** \brief Set looking down limit angle in degrees. */
	void SetLimitLookDown(float limit);
	
	/** \brief Looking up-down. */
	inline decSmoothFloat &GetLookVertical(){ return pLookVertical; }
	inline const decSmoothFloat &GetLookVertical() const{ return pLookVertical; }
	
	/** \brief Set looking up-down goal. Clamps to limits. */
	void SetLookVerticalGoal(float goal);
	
	/** \brief Looking up-down value. Clamps to limits. */
	void SetLookVertical(float value);
	
	
	
	/** \brief Looking left limit in degrees. */
	inline float GetLimitLookLeft() const{ return pLimitLookLeft; }
	
	/** \brief Set looking left limit in degrees. */
	void SetLimitLookLeft(float limit);
	
	/** \brief Looking right limit in degrees. */
	inline float GetLimitLookRight() const{ return pLimitLookRight; }
	
	/** \brief Set looking right limit in degrees. */
	void SetLimitLookRight(float limit);
	
	/** \brief Look left-right. */
	inline decSmoothFloat &GetLookHorizontal(){ return pLookHorizontal; }
	inline const decSmoothFloat &GetLookHorizontal() const{ return pLookHorizontal; }
	
	/** \brief Adjust looking left-right while turning. */
	inline bool GetTurnAdjustLookHorizontal() const{ return pTurnAdjustLookHorizontal; }
	
	/** \brief Set if looking left-right is adjusted while turning. */
	void SetTurnAdjustLookHorizontal(bool turnAdjust);
	
	
	
	/** \brief Analog looking left-right. */
	inline decSmoothFloat &GetAnalogMovingHorizontal(){ return pAnalogMovingHorizontal; }
	inline const decSmoothFloat &GetAnalogMovingHorizontal() const{ return pAnalogMovingHorizontal; }
	
	/** \brief Analog looking up-down. */
	inline float GetAnalogMovingVertical() const{ return pAnalogMovingVertical; }
	
	/** \brief Set analog looking up-down. */
	void SetAnalogMovingVertical(float value);
	
	/** \brief Turn left-right . */
	inline float GetTurnHorizontal() const{ return pTurnHorizontal; }
	
	/** \brief Set turn left-right. */
	void SetTurnHorizontal(float value);
	
	
	
	/** \brief Analog moving speed. */
	inline float GetAnalogMovingSpeed() const{ return pAnalogMovingSpeed; }
	
	/** \brief Set analog moving speed. */
	void SetAnalogMovingSpeed(float value);
	
	
	
	/** \brief Orientation adjustment time in seconds. */
	inline float GetAdjustTimeOrientation() const{ return pAdjustTimeOrientation; }
	
	/** \brief Set orientation adjustment time in seconds. */
	void SetAdjustTimeOrientation(float time);
	
	/** \brief Actor orientation in world space. */
	inline float GetOrientation() const{ return pOrientation; }
	
	/** \brief Set actor orientation in world space. */
	void SetOrientation(float orientation);
	
	/** \brief Actor orientation as quaternion. */
	inline const decQuaternion &GetOrientationQuaternion() const{ return pOrientationQuaternion; }
	
	/** \brief Set actor orientation as quaternion. */
	void SetOrientationQuaternion(const decQuaternion &orientation);
	
	/** \brief Turning speed in meters per second. */
	inline float GetTurningSpeed() const{ return pTurningSpeed; }
	
	/** \brief Set turning speed in meters per second. */
	void SetTurningSpeed(float speed);
	
	/** \brief Angular velocity of the actor turning. */
	inline const decVector &GetAngularVelocity() const{ return pAngularVelocity; }
	
	/** \brief Set angular velocity of the actor turning. */
	void SetAngularVelocity(const decVector &velocity);
	
	
	
	/** \brief Determine if the actor is moving direct or analog. */
	inline bool GetIsMoving() const{ return pIsMoving; }
	
	/** \brief Set determine if the actor is moving direct or analog. */
	void SetIsMoving(bool isMoving);
	
	/** \brief Movement speed. */
	inline float GetMovingSpeed() const{ return pMovingSpeed; }
	
	/** \brief Set movement speed. */
	void SetMovingSpeed(float speed);
	
	/** \brief Movement orientation in world space. */
	inline float GetMovingOrientation() const{ return pMovingOrientation; }
	
	/** \brief Set movement orientation in world space. */
	void SetMovingOrientation(float orientation);
	
	/** \brief Movement direction relative to view orientation. */
	inline float GetMovingDirection() const{ return pMovingDirection; }
	
	/** \brief Set movement direction relative to view orientation. */
	void SetMovingDirection(float direction);
	
	
	
	/** \brief Linear velocity. */
	inline decSmoothVector &GetLinearVelocity(){ return pLinearVelocity; }
	inline const decSmoothVector &GetLinearVelocity() const{ return pLinearVelocity; }
	
	
	
	/** \brief Stance value. */
	inline decSmoothFloat &GetStance(){ return pStance; }
	inline const decSmoothFloat &GetStance() const{ return pStance; }
	
	
	
	/** \brief Determine if the actor is turning in place. */
	inline bool GetIsTurningIP() const{ return pIsTurningIP; }
	
	/** \brief Set if the actor is turning in place. */
	void SetIsTurningIP(bool turnInPlace);
	
	/** \brief Adjustment time in seconds for turning in-place. */
	inline float GetAdjustTimeTurnIP() const{ return pAdjustTimeTurnIP; }
	
	/** \brief Set adjustment time in seconds for turning in-place. */
	void SetAdjustTimeTurnIP(float time);
	
	/** \brief Turn in place value. */
	inline float GetTurnIP() const{ return pTurnIP; }
	
	/** \brief Set turn in place value. */
	void SetTurnIP(float value);
	
	/** \brief Determine if the turn in place time linked controllers have to be reset. */
	inline bool GetResetTimeTurnIP() const{ return pResetTimeTurnIP; }
	
	/** \brief Set if the turn in place time linked controllers have to be reset. */
	void SetResetTimeTurnIP(bool reset);
	
	/** \brief Determine if the turn in place time linked controllers have to be reversed. */
	inline bool GetReverseTimeTurnIP() const{ return pReverseTimeTurnIP; }
	
	/** \brief Set if the turn in place time linked controllers have to be reversed. */
	void SetReverseTimeTurnIP(bool reverse);
	
	
	
	/** \brief Actor can turn in-place if looking beyond limits. */
	inline bool GetCanTurnInPlace() const{ return pCanTurnInPlace; }
	
	/** \brief Set if actor can turn in-place if looking beyond limits. */
	void SetCanTurnInPlace(bool canTurnInPlace);
	
	/** \brief Looking left limit in degrees before turning in-place. */
	inline float GetLimitTurnInPlaceLeft() const{ return pLimitTurnInPlaceLeft; }
	
	/** \brief Set looking left limit in degrees before turning in-place. */
	void SetLimitTurnInPlaceLeft(float limit);
	
	/** \brief Looking right limit in degrees before turning in-place. */
	inline float GetLimitTurnInPlaceRight() const{ return pLimitTurnInPlaceRight; }
	
	/** \brief Set looking right limit in degrees before turning in-place. */
	void SetLimitTurnInPlaceRight(float limit);
	
	
	
	/** \brief Determine if the walk/run time linked controllers have to be reset. */
	inline bool GetResetTimeWalk() const{ return pResetTimeWalk; }
	
	/** \brief Set if the walk/run time linked controllers have to be reset. */
	void SetResetTimeWalk(bool reset);
	
	
	
	/**
	 * \brief Determine if the actor can tilt the body.
	 * \details If disabled no tilt calculation is done.
	 */
	inline bool GetCanTilt() const{ return pCanTilt; }
 	
	/**
	 * \brief Set if the actor can tilt the body.
	 * \details If disabled no tilt calculation is done.
	 */
	void SetCanTilt(bool canTilt);
	
	/** \brief Tilt mode. */
	inline eTiltModes GetTiltMode() const{ return pTiltMode; }
	
	/** \brief Set tilt mode. */
	void SetTiltMode(eTiltModes mode);
	
	/** \brief Single collider collision test for tilt calculation or NULL if not set. */
	inline deColliderCollisionTest *GetCCTTiltSingle() const{ return pCCTTiltSingle; }
	
	/** \brief Set single collider collision test for tilt calculation or NULL if not set. */
	void SetCCTTiltSingle(deColliderCollisionTest *collisionTest);
	
	/** \brief Front left collider collision test for tilt calculation or NULL if not set. */
	inline deColliderCollisionTest *GetCCTTiltFrontLeft() const{ return pCCTTiltFrontLeft; }
	
	/** \brief Set front left collider collision test for tilt calculation or NULL if not set. */
	void SetCCTTiltFrontLeft(deColliderCollisionTest *collisionTest);
	
	/** \brief Front right collider collision test for tilt calculation or NULL if not set. */
	inline deColliderCollisionTest *GetCCTTiltFrontRight() const{ return pCCTTiltFrontRight; }
	
	/** \brief Set front right collider collision test for tilt calculation or NULL if not set. */
	void SetCCTTiltFrontRight(deColliderCollisionTest *collisionTest);
	
	/** \brief Back left collider collision test for tilt calculation or NULL if not set. */
	inline deColliderCollisionTest *GetCCTTiltBackLeft() const{ return pCCTTiltBackLeft; }
	
	/** \brief Set back left collider collision test for tilt calculation or NULL if not set. */
	void SetCCTTiltBackLeft(deColliderCollisionTest *collisionTest);
	
	/** \brief Back right collider collision test for tilt calculation or NULL if not set. */
	inline deColliderCollisionTest *GetCCTTiltBackRight() const{ return pCCTTiltBackRight; }
	
	/** \brief Set back right collider collision test for tilt calculation or NULL if not set. */
	void SetCCTTiltBackRight(deColliderCollisionTest *collisionTest);
	
	
	
	/** \brief Tilt up limit in degrees. */
	inline float GetLimitTiltUp() const{ return pLimitTiltUp; }
	
	/** \brief Set tilt up limit in degrees. */
	void SetLimitTiltUp(float limit);
	
	/** \brief Tilt down limit in degrees. */
	inline float GetLimitTiltDown() const{ return pLimitTiltDown; }
	
	/** \brief Set tilt down limit in degrees. */
	void SetLimitTiltDown(float limit);
	
	/** \brief Tilt up/down angle in degrees. */
	inline decSmoothFloat &GetTiltVertical(){ return pTiltVertical; }
	inline const decSmoothFloat &GetTiltVertical() const{ return pTiltVertical; }
	
	/** \brief Set current tilt up/down angle in degrees clamped to range. */
	void SetTiltVertical(float tiltVertical);
	
	/** \brief Set goal tilt up/down angle in degrees clamped to range. */
	void SetTiltVerticalGoal(float tiltVertical);
	
	
	
	/** \brief Tilt left limit in degrees. */
	inline float GetLimitTiltLeft() const{ return pLimitTiltLeft; }
	
	/** \brief Set tilt left limit in degrees. */
	void SetLimitTiltLeft(float limit);
	
	/** \brief Tilt right limit in degrees. */
	inline float GetLimitTiltRight() const{ return pLimitTiltRight; }
	
	/** \brief Set Tilt right limit in degrees. */
	void SetLimitTiltRight(float limit);
	
	/** \brief Tilt left/right angle in degrees. */
	inline decSmoothFloat &GetTiltHorizontal(){ return pTiltHorizontal; }
	inline const decSmoothFloat &GetTiltHorizontal() const{ return pTiltHorizontal; }
	
	/** \brief Set current tilt left/right angle in degrees clamped to range. */
	void SetTiltHorizontal(float tiltHorizontal);
	
	/** \brief Set goal tilt left/right angle in degrees clamped to range. */
	void SetTiltHorizontalGoal(float tiltHorizontal);
	
	
	
	/** \brief Tilt offset in meters. */
	inline float GetTiltOffset() const{ return pTiltOffset; }
	
	/** \brief Set tilt offset in meters. */
	void SetTiltOffset(float offset);
	
	
	/** \brief Number of controller mappings. */
	inline int GetControllerMappingCount() const{ return pControllerMappingCount; }
	
	/** \brief Controller mapping by index. */
	const dedsLControllerMapping &GetControllerMappingAt(int index) const;
	
	/** \brief Index of mapping for a controller or -1 if not present. */
	int IndexOfControllerMappingWith(deAnimatorInstance *animatorInstance, int controller) const;
	
	/** \brief Add or change controller mapping. */
	void AddControllerMapping(deAnimatorInstance *animatorInstance, int controller,
		dedsLControllerMapping::eAttributes attribute);
	
	/** \brief Remove controller mapping if present. */
	void RemoveControllerMapping(deAnimatorInstance *animatorInstance, int controller);
	
	/** \brief Remove all controller mappings. */
	void RemoveAllControllerMappings();
	
	
	
	/** \brief AI Collider to update or NULL if not used. */
	inline deCollider *GetAICollider() const{ return pAICollider; }
	
	/** \brief Set AI Collider to update or NULL if not used. */
	void SetAICollider(deCollider *collider);
	
	/** \brief Determine if the angular velocity of the AI collider is updated. */
	inline bool GetUpdateAIColliderAngularVelocity() const{ return pUpdateAIColliderAngularVelocity; }
 	
	/** \brief Set if the angular velocity of the AI collider is updated. */
	void SetUpdateAIColliderAngularVelocity(bool updateAngularVelocity);
	
	
	
	/** \brief Cancel Movement. */
	void CancelMovement();
	
	/** \brief Cancel inpurt. */
	void CancelInput();
	
	/** \brief Cancel motion. */
	void CancelMotion();
	
	/** \brief Cancel turning in place. */
	void CancelTurnInPlace();
	
	/** \brief Force body adjustment. */
	void ForceBodyAdjustment();
	
	
	
	/** \brief Update looking. */
	void UpdateLooking(float elapsed);
	
	/** \brief Apply states without interpolation or smoothing. */
	void ApplyStates();
	
	/** \brief Update locomotion. */
	void UpdateLocomotion(float elapsed);
	
	/** \brief Update is moving. */
	void UpdateIsMoving();
	
	/** \brief Update orientation. */
	void UpdateOrientation(float elapsed);
	
	/** \brief Update orientation for moving. */
	void UpdateOrientationMoving(float elapsed, float &adjustOrientation);
	
	/** \brief Update orientation for not moving. */
	void UpdateOrientationNotMoving(float elapsed, float &adjustOrientation);
	
	/** \brief Check for looking leaving sane ranges and apply fixes if required. */
	void CheckLookingRangeViolation(float &adjustOrientation);
	
	/** \brief Update linear velocity. */
	void UpdateLinearVelocity(float elapsed);
	
	/** \brief Update stance. */
	void UpdateStance(float elapsed);
	
	/** \brief Update locomotion after physics calculations. */
	void UpdatePostLocomotion(float elapsed);
	
	/** \brief Update tilt. */
	void UpdateTilt(float elapsed);
	
	/** \brief Update tilt using single cast mode. */
	void UpdateTiltSingleCast(float elapsed);
	
	/** \brief Update tilt using weighted cast mode. */
	void UpdateTiltWeightCast(float elapsed);
	
	/** \brief Update animator instance controllers if set with locomotion parameters. */
	void UpdateAnimatorInstance(float elapsed);
	
	/** \brief Update AI collider linear and angular velocity. */
	void UpdateAICollider();
	
	/** \brief Adjust orientation. */
	void AdjustOrientation(float angle);
	
	
	
	/** \brief Read from file reader. */
	void ReadFromFile(decBaseFileReader &reader);
	
	/** \brief Read from file reader. */
	void ReadFromFileV1(decBaseFileReader &reader);
	
	/** \brief Write to file reader. */
	void WriteToFile(decBaseFileWriter &writer) const;
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
