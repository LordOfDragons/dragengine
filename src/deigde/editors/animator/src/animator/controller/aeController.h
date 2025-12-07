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

#include "../../gui/gizmo/aeGizmoControllerIKPosition.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/deObject.h>

#include <dragengine/common/string/decString.h>

class aeAnimator;
class deAnimatorController;


/**
 * Animator Controller.
 *
 * Working object for an animator controller.
 */
class aeController : public deObject{
public:
	typedef deTObjectReference<aeController> Ref;
	
	
	enum eVectorSimulation{
		evsNone,
		evsPosition,
		evsRotation
	};
	
	
private:
	aeAnimator *pAnimator;
	int pEngControllerIndex;
	
	decString pName;
	
	float pMinValue;
	float pMaxValue;
	float pCurValue;
	bool pClamp;
	bool pFrozen;
	decVector pVector;
	int pLocoAttr;
	int pLocoLeg;
	eVectorSimulation pVectorSimulation;
	float pDefaultValue;
	decVector pDefaultVector;
	
	aeGizmoControllerIKPosition::Ref pGizmoIKPosition;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create controller. */
	aeController(const char *name = "Controller");
	
	/** Create copy of controller. */
	aeController(const aeController &copy);
	
protected:
	/** Cleans up the animator controller. */
	virtual ~aeController() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Retrieves the parent animator. */
	inline aeAnimator *GetAnimator() const{return pAnimator;}
	/** Sets the parent animator. */
	void SetAnimator(aeAnimator *animator);
	
	/** Retrieves the index of the engine animator instance controller to update in the future. */
	inline int GetEngineControllerIndex() const{return pEngControllerIndex;}
	/** Sets the index of the engine animator instance controller to update in the future. */
	void SetEngineControllerIndex(int index);
	
	/** Retrieves the name. */
	inline const decString &GetName() const{return pName;}
	/** Sets the world filename. */
	void SetName(const char *filename);
	/** Updates the controller if required. */
	void UpdateValue(float elapsed);
	/** Resets the controller if linked to a locomotion attribute. */
	void ResetValue();
	/** Inverses the current value. */
	void InverseValue();
	
	/** Retrieves the minimum value. */
	inline float GetMinimumValue() const{return pMinValue;}
	/** Sets the minimum value. */
	void SetMinimumValue(float value);
	/** Retrieves the maximum value. */
	inline float GetMaximumValue() const{return pMaxValue;}
	/** Sets the maximum value. */
	void SetMaximumValue(float value);
	/** Retrieves the current value. */
	inline float GetCurrentValue() const{return pCurValue;}
	/** Sets the current value. */
	void SetCurrentValue(float value);
	/** Increments the current value. */
	void IncrementCurrentValue(float incrementBy);
	/** Determines if the controller is frozen. */
	inline bool GetFrozen() const{return pFrozen;}
	/** Sets if the controller is frozen. */
	void SetFrozen(bool frozen);
	/** Determines if values passed the range are clamped or wrapped around. */
	inline bool GetClamp() const{return pClamp;}
	/** Sets if values passed the range are clamped or wrapped around. */
	void SetClamp(bool clamp);
	
	/** Retrieves the controller vector. */
	inline const decVector &GetVector() const{return pVector;}
	/** Sets the controller vector. */
	void SetVector(const decVector &vector);
	
	/** Retrieves the locomotion attribute this target is controlled by. */
	inline int GetLocomotionAttribute() const{return pLocoAttr;}
	/** Sets the locomotion attribute this target is controlled by. */
	void SetLocomotionAttribute(int attribute);
	/** Retrieves the locomotion leg this target is controlled by. */
	inline int GetLocomotionLeg() const{return pLocoLeg;}
	/** Sets the locomotion leg this target is controlled by. */
	void SetLocomotionLeg(int leg);
	
	/** Vector simulation. */
	inline eVectorSimulation GetVectorSimulation() const{return pVectorSimulation;}
	
	/** Set vector simulation. */
	void SetVectorSimulation(eVectorSimulation simulation);
	
	/** Default value. */
	inline float GetDefaultValue() const{return pDefaultValue;}
	void SetDefaultValue(float value);
	
	/** Default vector. */
	inline const decVector &GetDefaultVector() const{return pDefaultVector;}
	void SetDefaultVector(const decVector &vector);
	
	inline const aeGizmoControllerIKPosition::Ref &GetGizmoIKPosition() const{return pGizmoIKPosition;}
	/*@}*/
	
	
	
private:
	void pCleanUp();
	float pCheckValue(float value);
	void pReleaseGizmos();
	void pCreateGizmos();
};

#endif
