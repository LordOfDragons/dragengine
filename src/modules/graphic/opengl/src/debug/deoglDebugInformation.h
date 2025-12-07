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

#ifndef _DEOGLDEBUGINFORMATION_H_
#define _DEOGLDEBUGINFORMATION_H_

#include "deoglDebugInformationList.h"

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>



/**
 * Debug information for performance profiling.
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
class deoglDebugInformation : public deObject{
public:
	typedef deTObjectReference<deoglDebugInformation> Ref;
	
	
private:
	decString pName;
	decColor pColorText;
	decColor pColorBackground;
	float pElapsedTime;
	float pElapsedTimeAccum;
	int pCounter;
	bool pHasElapsedTime;
	bool pHasCounter;
	bool pVisible;
	deoglDebugInformationList pChildren;
	
	float pLastElapsedTime;
	float pSlidingElapsedTimeFactor;
	
	decString pRenderText;
	decPoint pRenderSize;
	decPoint pRenderPosition;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create debug information. */
	deoglDebugInformation(const char *name, const decColor &colorText, const decColor &colorBackground);
	
	/** Clean up debug information. */
	virtual ~deoglDebugInformation();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Name. */
	inline const decString &GetName() const{return pName;}
	
	/** Set name. */
	void SetName(const char *name);
	
	/** Text color. */
	inline const decColor &GetColorText() const{return pColorText;}
	
	/** Set text color. */
	void SetColorText(const decColor &color);
	
	/** Background color. */
	inline const decColor &GetColorBackground() const{return pColorBackground;}
	
	/** Set background color. */
	void SetColorBackground(const decColor &color);
	
	/** Clear elapsed time and counter. */
	void Clear();
	
	/** Visible. */
	inline bool GetVisible() const{return pVisible;}
	
	/** Set if visible. */
	void SetVisible(bool visible);
	
	/** Children. */
	inline deoglDebugInformationList &GetChildren(){return pChildren;}
	inline const deoglDebugInformationList &GetChildren() const{return pChildren;}
	
	
	
	/** Elapsed time. */
	inline float GetElapsedTime() const{return pElapsedTime;}
	
	/** Accumulated elapsed time. */
	inline float GetElapsedTimeAccum() const{return pElapsedTimeAccum;}
	
	/** Set elapsed time. */
	void SetElapsedTime(float time);
	
	/** Increment elapsed time. */
	void IncrementElapsedTime(float time);
	
	/** Clear elapsed time. */
	void ClearElapsedTime();
	
	/** Has elapsed time. */
	inline bool HasElapsedTime() const{return pHasElapsedTime;}
	
	/** Sliding elapsed time factor. */
	inline float GetSlidingElapsedTimeFactor() const{return pSlidingElapsedTimeFactor;}
	
	/** Set sliding elapsed time factor. */
	void SetSlidingElapsedTimeFactor(float factor);
	
	
	
	/** Counter. */
	inline int GetCounter() const{return pCounter;}
	
	/** Set counter. */
	void SetCounter(int counter);
	
	/** Increment counter by one. */
	void IncrementCounter();
	
	/** Increment counter. */
	void IncrementCounter(int count);
	
	/** Clear counter. */
	void ClearCounter();
	
	/** Has counter. */
	inline bool HasCounter() const{return pHasCounter;}
	
	/** Copy results from another debug information. */
	void CopyResults(const deoglDebugInformation &info);
	/*@}*/
	
	
	
	/** \name Rendering for use by deoglRenderDevMode only. */
	/*@{*/
	/** Render text. */
	inline const decString &GetRenderText() const{return pRenderText;}
	
	/** Set render text. */
	void SetRenderText(const char *text);
	
	/** Render size. */
	inline const decPoint &GetRenderSize() const{return pRenderSize;}
	
	/** Set render size. */
	void SetRenderSize(const decPoint &size);
	
	/** Render position. */
	inline const decPoint &GetRenderPosition() const{return pRenderPosition;}
	
	/** Set render position. */
	void SetRenderPosition(const decPoint &position);
	/*@}*/
};

#endif
