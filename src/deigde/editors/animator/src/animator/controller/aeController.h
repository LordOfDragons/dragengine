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

#ifndef _AECONTROLLER_H_
#define _AECONTROLLER_H_

#include "../locomotion/aeAnimatorLocomotion.h"
#include "../../gui/gizmo/aeGizmoControllerIKPosition.h"
#include "../../meta/animator/aeMCController.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTOrderedSet.h>

#include <dragengine/common/string/decString.h>

#include <deigde/meta/igdeMetaContext.h>
#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyFloat.h>
#include <deigde/meta/property/igdeMetaPropertyInteger.h>
#include <deigde/meta/property/igdeMetaPropertySelection.h>
#include <deigde/meta/property/igdeMetaPropertyString.h>
#include <deigde/meta/property/igdeMetaPropertyVector.h>

class aeWindowMain;
class aeAnimator;
class deAnimatorController;


/**
 * Animator Controller.
 *
 * Working object for an animator controller.
 */
class aeController : public deObject{
public:
	using Ref = deTObjectReference<aeController>;
	using List = decTCollectionQueryByName<decTObjectOrderedSet<aeController>,aeController>;
	
	
	enum eVectorSimulation{
		evsNone,
		evsPosition,
		evsRotation
	};
	
	
private:
	aeMCController::Ref pMetaContext;
	aeAnimator *pAnimator;
	int pIndex;
	
	aeGizmoControllerIKPosition::Ref pGizmoIKPosition;
	
	igdeMetaPropertyStringStorage::Storage pMPName;
	igdeMetaPropertyFloatStorage::Storage pMPMinimumValue;
	igdeMetaPropertyFloatStorage::Storage pMPMaximumValue;
	igdeMetaPropertyFloatStorage::Storage pMPCurrentValue;
	igdeMetaPropertyBooleanStorage::Storage pMPClamp;
	igdeMetaPropertyBooleanStorage::Storage pMPFrozen;
	igdeMetaPropertyVectorStorage::Storage pMPVector;
	igdeMetaPropertySelectionEnumStorage<aeAnimatorLocomotion::eAttributes>::Storage pMPLocomotionAttribute;
	igdeMetaPropertyIntegerStorage::Storage pMPLocomotionLeg;
	igdeMetaPropertySelectionEnumStorage<eVectorSimulation>::Storage pMPVectorSimulation;
	igdeMetaPropertyFloatStorage::Storage pMPDefaultValue;
	igdeMetaPropertyVectorStorage::Storage pMPDefaultVector;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create controller. */
	aeController(aeWindowMain &windowMain, const char *name = "Controller");
	
	/** Create copy of controller. */
	aeController(aeWindowMain &windowMain, const aeController &copy);
	
	aeController(const aeController&) = delete;
	aeController(aeController&&) = delete;
	aeController& operator=(const aeController&) = delete;
	aeController& operator=(aeController&&) = delete;
	
protected:
	/** Cleans up the animator controller. */
	~aeController() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	inline igdeMetaPropertyStringStorage::Storage &GetMPName(){ return pMPName; }
	inline igdeMetaPropertyFloatStorage::Storage &GetMPMinimumValue(){ return pMPMinimumValue; }
	inline igdeMetaPropertyFloatStorage::Storage &GetMPMaximumValue(){ return pMPMaximumValue; }
	inline igdeMetaPropertyFloatStorage::Storage &GetMPCurrentValue(){ return pMPCurrentValue; }
	inline igdeMetaPropertyBooleanStorage::Storage &GetMPClamp(){ return pMPClamp; }
	inline igdeMetaPropertyBooleanStorage::Storage &GetMPFrozen(){ return pMPFrozen; }
	inline igdeMetaPropertyVectorStorage::Storage &GetMPVector(){ return pMPVector; }
	inline igdeMetaPropertySelectionEnumStorage<aeAnimatorLocomotion::eAttributes>::Storage &GetMPLocomotionAttribute(){ return pMPLocomotionAttribute; }
	inline igdeMetaPropertyIntegerStorage::Storage &GetMPLocomotionLeg(){ return pMPLocomotionLeg; }
	inline igdeMetaPropertySelectionEnumStorage<eVectorSimulation>::Storage &GetMPVectorSimulation(){ return pMPVectorSimulation; }
	inline igdeMetaPropertyFloatStorage::Storage &GetMPDefaultValue(){ return pMPDefaultValue; }
	inline igdeMetaPropertyVectorStorage::Storage &GetMPDefaultVector(){ return pMPDefaultVector; }
	
	
	/** Retrieves the parent animator. */
	inline aeAnimator *GetAnimator() const{ return pAnimator; }
	/** Sets the parent animator. */
	void SetAnimator(aeAnimator *animator);
	
	/** Index. */
	inline int GetIndex() const{ return pIndex; }
	
	/** Sets the index. */
	void SetIndex(int index);
	
	/** Meta context. */
	inline const aeMCController::Ref &GetMetaContext() const{ return pMetaContext; }
	
	/** Retrieves the name. */
	inline const decString &GetName() const{ return pMPName; }
	/** Sets the name. */
	void SetName(const char *name);
	/** Updates the controller if required. */
	void UpdateValue(float elapsed);
	/** Resets the controller if linked to a locomotion attribute. */
	void ResetValue(bool fullReset = false);
	/** Inverses the current value. */
	void InverseValue();
	
	/** Retrieves the minimum value. */
	inline float GetMinimumValue() const{ return pMPMinimumValue; }
	/** Sets the minimum value. */
	void SetMinimumValue(float value);
	/** Retrieves the maximum value. */
	inline float GetMaximumValue() const{ return pMPMaximumValue; }
	/** Sets the maximum value. */
	void SetMaximumValue(float value);
	/** Retrieves the current value. */
	inline float GetCurrentValue() const{ return pMPCurrentValue; }
	/** Sets the current value. */
	void SetCurrentValue(float value);
	/** Increments the current value. */
	void IncrementCurrentValue(float incrementBy);
	/** Determines if the controller is frozen. */
	inline bool GetFrozen() const{ return pMPFrozen; }
	/** Sets if the controller is frozen. */
	void SetFrozen(bool frozen);
	/** Determines if values passed the range are clamped or wrapped around. */
	inline bool GetClamp() const{ return pMPClamp; }
	/** Sets if values passed the range are clamped or wrapped around. */
	void SetClamp(bool clamp);
	
	/** Retrieves the controller vector. */
	inline const decVector &GetVector() const{ return pMPVector; }
	/** Sets the controller vector. */
	void SetVector(const decVector &vector);
	
	/** Retrieves the locomotion attribute this target is controlled by. */
	inline aeAnimatorLocomotion::eAttributes GetLocomotionAttribute() const{ return pMPLocomotionAttribute; }
	/** Sets the locomotion attribute this target is controlled by. */
	void SetLocomotionAttribute(aeAnimatorLocomotion::eAttributes attribute);
	/** Retrieves the locomotion leg this target is controlled by. */
	inline int GetLocomotionLeg() const{ return pMPLocomotionLeg; }
	/** Sets the locomotion leg this target is controlled by. */
	void SetLocomotionLeg(int leg);
	
	/** Vector simulation. */
	inline eVectorSimulation GetVectorSimulation() const{ return pMPVectorSimulation; }
	
	/** Set vector simulation. */
	void SetVectorSimulation(eVectorSimulation simulation);
	
	/** Default value. */
	inline float GetDefaultValue() const{ return pMPDefaultValue; }
	void SetDefaultValue(float value);
	
	/** Default vector. */
	inline const decVector &GetDefaultVector() const{ return pMPDefaultVector; }
	void SetDefaultVector(const decVector &vector);
	
	inline const aeGizmoControllerIKPosition::Ref &GetGizmoIKPosition() const{ return pGizmoIKPosition; }
	/*@}*/
	
	
	
private:
	float pCheckValue(float value);
	void pReleaseGizmos();
	void pCreateGizmos();
	
	void pNotifyControllerChanged();
	void pNotifyControllerValueChanged();
	void pOnLimitsChanged();
};

#endif
