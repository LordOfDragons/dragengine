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

#ifndef _CECONTROLLERVALUE_H_
#define _CECONTROLLERVALUE_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/deObject.h>



/**
 * Controller Value.
 */
class ceControllerValue : public deObject{
private:
	int pControllerIndex;
	decString pController;
	float pValue;
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<ceControllerValue> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** Create controller value. */
	ceControllerValue(int controller, float value);
	ceControllerValue(const char *controller, float value);
	
	/** Cleans up the controller value. */
	virtual ~ceControllerValue();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Controller. */
	inline int GetControllerIndex() const{return pControllerIndex;}
	
	/** Set controller. */
	void SetControllerIndex(int controller);
	
	/** Controller. */
	inline const decString &GetController() const{return pController;}
	
	/** Set controller. */
	void SetController(const char *controller);
	
	/** Value. */
	inline float GetValue() const{return pValue;}
	
	/** Set value. */
	void SetValue(float value);
	/*@}*/
};

#endif
