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

#ifndef _DESYNTHESIZERCONTROLLER_H_
#define _DESYNTHESIZERCONTROLLER_H_

#include "../../deObject.h"
#include "../../common/curve/decCurveBezier.h"
#include "../../common/math/decMath.h"
#include "../../common/string/decString.h"


/**
 * \brief Synthesizer controller.
 * 
 * Controllers provide the user with control over how the individual sounds of a synthesizer
 * produce the final sound. Controllers are written in a way that you can use them without
 * having to do scaling or transformation on your own. You can provide a minimum and maximum
 * value matching your setup. The controller takes care of transforming the current value
 * into the proper 0 to 1 range required for sounds to work with. The current value is
 * calculated using an value curve set by the user. The play position is used to sample the
 * curve producing the controller values over time. 
 */
class DE_DLL_EXPORT deSynthesizerController : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deSynthesizerController> Ref;
	
	
private:
	decString pName;
	float pMinValue;
	float pMaxValue;
	bool pClamp;
	decCurveBezier pCurve;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create controller. */
	deSynthesizerController();
	
	/** \brief Create copy of controller. */
	deSynthesizerController(const deSynthesizerController &controller);
	
protected:
	/**
	 * \brief Clean up animator.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deSynthesizerController();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName(const char *name);
	
	/** \brief Minimum value. */
	inline float GetMinimumValue() const{ return pMinValue; }
	
	/** \brief Maximum value. */
	inline float GetMaximumValue() const{ return pMaxValue; }
	
	/** \brief Set value range. */
	void SetValueRange(float minValue, float maxValue);
	
	/** \brief Clamp current value to range instead of wrapping around at the end points. */
	inline bool GetClamp() const{ return pClamp; }
	
	/** \brief Set if current value is clamped to range instead of wrapping around at the end points. */
	void SetClamp(bool clamp);
	
	/** \brief Value curve. */
	inline const decCurveBezier &GetCurve() const{ return pCurve; }
	
	/** \brief Set value curve. */
	void SetCurve(const decCurveBezier &curve);
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy controller parameters. */
	deSynthesizerController &operator=(const deSynthesizerController &controller);
	/*@}*/
};

#endif
