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

#ifndef _DESYNTHESIZERLINK_H_
#define _DESYNTHESIZERLINK_H_

#include "../../common/math/decMath.h"
#include "../../common/curve/decCurveBezier.h"
#include "../../deObject.h"


// Definitions
////////////////

#define DEAL_THRESHOLD		CURVE_THRESHOLD


/**
 * \brief Synthesizer link.
 * 
 * Links controller value to source targets. Maps controller values using a bezier curve.
 * Using repeat values larger than 1 the curve is repeated along the Y axis.
 * 
 */
class DE_DLL_EXPORT deSynthesizerLink : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deSynthesizerLink> Ref;
	
	
private:
	int pController;
	decCurveBezier pCurve;
	int pRepeat;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create link. */
	deSynthesizerLink();
	
	/** \brief Create copy of link. */
	deSynthesizerLink(const deSynthesizerLink &copy);
	
protected:
	/**
	 * \brief Clean up link.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deSynthesizerLink();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Controller or -1 if not set. */
	inline int GetController() const{ return pController; }
	
	/** \brief Controller is set. */
	inline bool HasController() const{ return pController != -1; }
	
	/**
	 * \brief Set controller or -1 to not set.
	 * \throws deeInvalidParam \em controller is less than -1.
	 */
	void SetController(int controller);
	
	/** \brief Curve. */
	inline decCurveBezier &GetCurve(){ return pCurve; }
	inline const decCurveBezier &GetCurve() const{ return pCurve; }
	
	/** \brief Repeat count of input value. */
	inline int GetRepeat() const{ return pRepeat; }
	
	/**
	 * \brief Set repeat count of input value.
	 * \throws deeInvalidParam \em repeat is less than 1.
	 */
	void SetRepeat(int repeat);
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy link to this link. */
	deSynthesizerLink &operator=(const deSynthesizerLink &copy);
	/*@}*/
};

#endif
