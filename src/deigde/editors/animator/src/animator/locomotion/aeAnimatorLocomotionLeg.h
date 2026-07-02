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
#ifndef _AEANIMATORLOCOMOTIONLEG_H_
#define _AEANIMATORLOCOMOTIONLEG_H_

// includes
#include <deigde/gui/wrapper/debugdrawer/igdeWDebugDrawerShape.h>
#include <deigde/gui/wrapper/debugdrawer/igdeWCoordSysArrows.h>
#include <deigde/meta/igdeMetaContext.h>
#include <deigde/meta/property/igdeMetaPropertyFloat.h>
#include <deigde/meta/property/igdeMetaPropertyVector.h>

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/math/decMath.h>

class aeAnimatorLocomotion;
class aeWindowMain;

/**
 * Animator Locomotion Leg.
 *
 * Keeps track of a leg in an animator locomotion. A leg stores various
 * parameters required for properly animating a leg on uneven terrain.
 */
class aeAnimatorLocomotionLeg : public deObject{
public:
	using Ref = deTObjectReference<aeAnimatorLocomotionLeg>;
	
	using MetaContext = igdeMetaContextType<aeAnimatorLocomotionLeg>;
	static MetaContext::Ref CreateMetaContext(aeWindowMain &windowMain, aeAnimatorLocomotionLeg *leg);
	
	template<typename T>
	using MetaProperty = igdeMetaPropertyMCT<T, MetaContext>;
	
private:
	aeAnimatorLocomotion *pLocomotion;
	MetaContext::Ref pMetaContext;
	
	igdeMetaPropertyFloatStorage::Storage pMPLiftOffTime;
	igdeMetaPropertyFloatStorage::Storage pMPPutDownTime;
	igdeMetaPropertyVectorStorage::Storage pMPPutDownPosStand;
	igdeMetaPropertyVectorStorage::Storage pMPPutDownPosWalk;
	igdeMetaPropertyVectorStorage::Storage pMPPutDownPosRun;
	
	igdeWDebugDrawerShape::Ref pDDSLocked;
	igdeWCoordSysArrows pDDSLockedCF;
	igdeWDebugDrawerShape::Ref pDDSPredict;
	
	decString pName;
	
	bool pPositionLocked;
	decDVector pLockedPosition;
	decDVector pLockedRotation;
	decVector pLockedNormal;
	
	decDVector pPredictPosition;
	decDVector pPredictRotation;
	decVector pPredictNormal;
	
	decVector pGroundPosition;
	decVector pGroundNormal;
	
	float pIKInfluence;
	decVector pIKPosition;
	decVector pIKOrientation;
	
	decString pVisBoneName;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a object. */
	explicit aeAnimatorLocomotionLeg(aeAnimatorLocomotion *locomotion);
	
protected:
	/** Cleans up the object. */
	~aeAnimatorLocomotionLeg() override;
	
public:
	/*@}*/
	
	/** \name Management */
	/*@{*/
	inline const MetaContext::Ref &GetMetaContext() const{ return pMetaContext; }
	
	inline igdeMetaPropertyFloatStorage::Storage &GetMPLiftOffTime(){ return pMPLiftOffTime; }
	inline igdeMetaPropertyFloatStorage::Storage &GetMPPutDownTime(){ return pMPPutDownTime; }
	inline igdeMetaPropertyVectorStorage::Storage &GetMPPutDownPosStand(){ return pMPPutDownPosStand; }
	inline igdeMetaPropertyVectorStorage::Storage &GetMPPutDownPosWalk(){ return pMPPutDownPosWalk; }
	inline igdeMetaPropertyVectorStorage::Storage &GetMPPutDownPosRun(){ return pMPPutDownPosRun; }
	
	
	/** Locomotion */
	inline aeAnimatorLocomotion *GetLocomotion() const{ return pLocomotion; }
	aeAnimatorLocomotion &GetLocomotionRef() const;
	
	igdeEnvironment &GetEnvironment() const;
	igdeUndoSystem *GetUndoSystem() const;
	
	/** Retrieves the name. */
	inline const decString &GetName() const{ return pName; }
	/** Sets the name. */
	void SetName(const char *name);
	
	/** Retrieves the stand put down position. */
	inline const decVector &GetPutDownPositionStand() const{ return pMPPutDownPosStand; }
	/** Sets the stand put down position. */
	void SetPutDownPositionStand(const decVector &position);
	/** Retrieves the walk put down position. */
	inline const decVector &GetPutDownPositionWalk() const{ return pMPPutDownPosWalk; }
	/** Sets the walk put down position. */
	void SetPutDownPositionWalk(const decVector &position);
	/** Retrieves the run put down position. */
	inline const decVector &GetPutDownPositionRun() const{ return pMPPutDownPosRun; }
	/** Sets the run put down position. */
	void SetPutDownPositionRun(const decVector &position);
	
	/** Retrieves the lift off time. */
	inline float GetLiftOffTime() const{ return pMPLiftOffTime; }
	/** Sets the lift off time. */
	void SetLiftOffTime(float time);
	/** Retrieves the put down time. */
	inline float GetPutDownTime() const{ return pMPPutDownTime; }
	/** Sets the put down time. */
	void SetPutDownTime(float time);
	
	/** Retrieves the ground position. */
	inline const decVector &GetGroundPosition() const{ return pGroundPosition; }
	/** Retrieves the ground normal. */
	inline const decVector &GetGroundNormal() const{ return pGroundNormal; }
	
	/** Retrieves the ik influence value. */
	inline float GetIKInfluence() const{ return pIKInfluence; }
	/** Retrieves the ik position. */
	inline const decVector &GetIKPosition() const{ return pIKPosition; }
	/** Retrieves the ik orientation. */
	inline const decVector &GetIKOrientation() const{ return pIKOrientation; }
	
	/** Retrieves the name of the visualization bone. */
	inline const decString &GetVisBoneName() const{ return pVisBoneName; }
	/** Sets the name of the visualization bone. */
	void SetVisBoneName(const char *name);
	
	/** Resets leg. */
	void Reset();
	/** Update leg. */
	void Update(float elapsed);
	/** Post update after the animator has been applied. */
	void PostUpdate();
	/** Update debug drawers before rendering. */
	void UpdateDebugDrawers();
	/** Updates the shapes. */
	void UpdateShapes();
	/** Reposition shapes. */
	void RepositionShapes();
	
	/** Sets the shapes visible or not. */
	void SetShapesVisible(bool visible);
	/*@}*/
};

// end of include only once
#endif
