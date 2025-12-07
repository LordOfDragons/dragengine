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

#ifndef _DEBPDEBUGINFORMATION_H_
#define _DEBPDEBUGINFORMATION_H_

#include "debpDebugInformationList.h"

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>



/**
 * \brief Debug information for performance profiling.
 * 
 * Stores an elapsed time in seconds and a counter value. A name is used to display
 * the information on screen. The font and background color can be set to help identify
 * the information on screen quickly. Debug information can be disabled in which case
 * the font is half-way faded to the background and the values set to dashes. Information
 * can be groups containing child information. If child information exist and an elapsed
 * time is present the child information calculate a ratio of their elapsed time compared
 * to this elapsed time to find better bottle necks.
 * 
 * Debug information be be set invisible in which case they are not drawn. Avoids the
 * need to add and remove debug information all time.
 */
class debpDebugInformation : public deObject{
private:
	decString pName;
	float pElapsedTime;
	float pElapsedTimeAccum;
	int pCounter;
	bool pHasElapsedTime;
	bool pHasCounter;
	decString pText;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<debpDebugInformation> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create debug information. */
	debpDebugInformation(const char *name);
	
	/** \brief Clean up debug information. */
	virtual ~debpDebugInformation();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName(const char *name);
	
	/** \brief Clear elapsed time and counter. */
	void Clear();
	
	
	
	/** \brief Elapsed time. */
	inline float GetElapsedTime() const{ return pElapsedTime; }
	
	/** \brief Set elapsed time. */
	void SetElapsedTime(float time);
	
	/** \brief Increment elapsed time. */
	void IncrementElapsedTime(float time);
	
	/** \brief Clear elapsed time. */
	void ClearElapsedTime();
	
	/** \brief Has elapsed time. */
	inline bool HasElapsedTime() const{ return pHasElapsedTime; }
	
	
	
	/** \brief Counter. */
	inline int GetCounter() const{ return pCounter; }
	
	/** \brief Set counter. */
	void SetCounter(int counter);
	
	/** \brief Increment counter by one. */
	void IncrementCounter();
	
	/** \brief Increment counter. */
	void IncrementCounter(int count);
	
	/** \brief Clear counter. */
	void ClearCounter();
	
	/** \brief Has counter. */
	inline bool HasCounter() const{ return pHasCounter; }
	
	
	
	/** \brief Text. */
	inline const decString &GetText() const{ return pText; }
	
	/** \brief Set text. */
	void SetText(const char *text);
	/*@}*/
};

#endif
