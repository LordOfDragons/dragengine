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

#ifndef _DEANIMATORCONTROLLER_H_
#define _DEANIMATORCONTROLLER_H_

#include "../../../common/math/decMath.h"
#include "../../../common/string/decString.h"


/**
 * \brief Animator Controller Class.
 *
 * Animator controllers provide the user with control over how the
 * individual rules of an animator produce an animation. Controllers
 * are written in a way that you can use them without having to do
 * scaling or transformation on your own. You can provide a minimum
 * and maximum value matching your setup. The controller takes care
 * of transforming the current value into the proper 0 to 1 range
 * required for rules to work with. The current value is set by the
 * user and is clamped to the range specified before. The real value
 * is in the range 0 to 1 and is used by the rules. The real value
 * can only be read. Furthermore a controller can be frozen. If a
 * controller is frozen no values can be changed. This is useful to
 * prevent a controller from changing without having to change all
 * code path to account for the frozen state. In addition a controller
 * can also contain a matrix instead of a ranged value. This can be
 * useful for certain specialized rules which need a position or even
 * orientations to do their work.
 */
class DE_DLL_EXPORT deAnimatorController{
private:
	decString pName;
	float pMinValue;
	float pMaxValue;
	float pCurValue;
	bool pClamp;
	bool pFrozen;
	decVector pVector;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create unnamed animator controller with range 0 to 1 and value 0. */
	deAnimatorController();
	
	/** \brief Clean up animator. */
	~deAnimatorController();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{return pName;}
	
	/** \brief Set name. */
	void SetName(const char *name);
	
	/** \brief Minimum value. */
	inline float GetMinimumValue() const{return pMinValue;}
	
	/** \brief Maximum value. */
	inline float GetMaximumValue() const{return pMaxValue;}
	
	/** \brief Set value range. */
	void SetValueRange(float minValue, float maxValue);
	
	/** \brief Current value. */
	inline float GetCurrentValue() const{return pCurValue;}
	
	/** \brief Set current value. */
	void SetCurrentValue(float value);
	
	/** \brief Increment current value. */
	void IncrementCurrentValue(float incrementBy);
	
	/** \brief Controller is frozen. */
	inline bool GetFrozen() const{return pFrozen;}
	
	/** \brief Set if controller is frozen. */
	void SetFrozen(bool frozen);
	
	/** \brief Values passing range are clamped instead of wrapped around. */
	inline bool GetClamp() const{return pClamp;}
	
	/** \brief Set if values passing range are clamped instead of wrapped around. */
	void SetClamp(bool clamp);
	
	/** \brief Vector. */
	inline const decVector &GetVector() const{return pVector;}
	
	/** \brief Set vector. */
	void SetVector(const decVector &vector);
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy controller parameters. */
	deAnimatorController &operator=(const deAnimatorController &controller);
	/*@}*/
	
	
	
private:
	void pUpdateValue();
};

#endif
