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
	
	
	using MetaContext = igdeMetaContextType<aeController>;
	static MetaContext::Ref CreateMetaContext(aeWindowMain &windowMain, aeController *controller);
	
	template<typename T>
	using MetaProperty = igdeMetaPropertyMCT<T, MetaContext>;
	
private:
	aeWindowMain &pWindowMain;
	MetaContext::Ref pMetaContext;
	aeAnimator *pAnimator;
	int pIndex;
	
	aeGizmoControllerIKPosition::Ref pGizmoIKPosition;
	
public:
	igdeMetaPropertyStringStorage::Storage mpName;
	igdeMetaPropertyFloatStorage::Storage mpMinimumValue;
	igdeMetaPropertyFloatStorage::Storage mpMaximumValue;
	igdeMetaPropertyFloatStorage::Storage mpCurrentValue;
	igdeMetaPropertyBooleanStorage::Storage mpClamp;
	igdeMetaPropertyBooleanStorage::Storage mpFrozen;
	igdeMetaPropertyVectorStorage::Storage mpVector;
	igdeMetaPropertySelectionEnumStorage<aeAnimatorLocomotion::eAttributes>::Storage mpLocomotionAttribute;
	igdeMetaPropertyIntegerStorage::Storage mpLocomotionLeg;
	igdeMetaPropertySelectionEnumStorage<eVectorSimulation>::Storage mpVectorSimulation;
	igdeMetaPropertyFloatStorage::Storage mpDefaultValue;
	igdeMetaPropertyVectorStorage::Storage mpDefaultVector;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create controller. */
	aeController(aeWindowMain &windowMain, const char *name = "Controller");
	
	/** Create copy of controller. */
	aeController(const aeController &copy);
	
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
	inline aeWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** Retrieves the parent animator. */
	inline aeAnimator *GetAnimator() const{ return pAnimator; }
	aeAnimator &GetAnimatorRef() const;
	
	/** Sets the parent animator. */
	void SetAnimator(aeAnimator *animator);
	
	igdeEnvironment &GetEnvironment() const;
	igdeUndoSystem *GetUndoSystem() const;
	
	/** Index. */
	inline int GetIndex() const{ return pIndex; }
	
	/** Sets the index. */
	void SetIndex(int index);
	
	/** Meta context. */
	inline const MetaContext::Ref &GetMetaContext() const{ return pMetaContext; }
	
	/** Retrieves the name. */
	inline const decString &GetName() const{ return mpName; }
	/** Sets the name. */
	void SetName(const char *name);
	/** Updates the controller if required. */
	void UpdateValue(float elapsed);
	/** Resets the controller if linked to a locomotion attribute. */
	void ResetValue(bool fullReset = false);
	/** Inverses the current value. */
	void InverseValue();
	
	/** Retrieves the minimum value. */
	inline float GetMinimumValue() const{ return mpMinimumValue; }
	/** Sets the minimum value. */
	void SetMinimumValue(float value);
	/** Retrieves the maximum value. */
	inline float GetMaximumValue() const{ return mpMaximumValue; }
	/** Sets the maximum value. */
	void SetMaximumValue(float value);
	/** Retrieves the current value. */
	inline float GetCurrentValue() const{ return mpCurrentValue; }
	/** Sets the current value. */
	void SetCurrentValue(float value);
	/** Increments the current value. */
	void IncrementCurrentValue(float incrementBy);
	/** Determines if the controller is frozen. */
	inline bool GetFrozen() const{ return mpFrozen; }
	/** Sets if the controller is frozen. */
	void SetFrozen(bool frozen);
	/** Determines if values passed the range are clamped or wrapped around. */
	inline bool GetClamp() const{ return mpClamp; }
	/** Sets if values passed the range are clamped or wrapped around. */
	void SetClamp(bool clamp);
	
	/** Retrieves the controller vector. */
	inline const decVector &GetVector() const{ return mpVector; }
	/** Sets the controller vector. */
	void SetVector(const decVector &vector);
	
	/** Retrieves the locomotion attribute this target is controlled by. */
	inline aeAnimatorLocomotion::eAttributes GetLocomotionAttribute() const{ return mpLocomotionAttribute; }
	/** Sets the locomotion attribute this target is controlled by. */
	void SetLocomotionAttribute(aeAnimatorLocomotion::eAttributes attribute);
	/** Retrieves the locomotion leg this target is controlled by. */
	inline int GetLocomotionLeg() const{ return mpLocomotionLeg; }
	/** Sets the locomotion leg this target is controlled by. */
	void SetLocomotionLeg(int leg);
	
	/** Vector simulation. */
	inline eVectorSimulation GetVectorSimulation() const{ return mpVectorSimulation; }
	
	/** Set vector simulation. */
	void SetVectorSimulation(eVectorSimulation simulation);
	
	/** Default value. */
	inline float GetDefaultValue() const{ return mpDefaultValue; }
	void SetDefaultValue(float value);
	
	/** Default vector. */
	inline const decVector &GetDefaultVector() const{ return mpDefaultVector; }
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
