/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
	deoglDebugInformation( const char *name, const decColor &colorText, const decColor &colorBackground );
	
	/** Clean up debug information. */
	virtual ~deoglDebugInformation();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** Set name. */
	void SetName( const char *name );
	
	/** Text color. */
	inline const decColor &GetColorText() const{ return pColorText; }
	
	/** Set text color. */
	void SetColorText( const decColor &color );
	
	/** Background color. */
	inline const decColor &GetColorBackground() const{ return pColorBackground; }
	
	/** Set background color. */
	void SetColorBackground( const decColor &color );
	
	/** Clear elapsed time and counter. */
	void Clear();
	
	/** Visible. */
	inline bool GetVisible() const{ return pVisible; }
	
	/** Set if visible. */
	void SetVisible( bool visible );
	
	/** Children. */
	inline deoglDebugInformationList &GetChildren(){ return pChildren; }
	inline const deoglDebugInformationList &GetChildren() const{ return pChildren; }
	
	
	
	/** Elapsed time. */
	inline float GetElapsedTime() const{ return pElapsedTime; }
	
	/** Set elapsed time. */
	void SetElapsedTime( float time );
	
	/** Increment elapsed time. */
	void IncrementElapsedTime( float time );
	
	/** Clear elapsed time. */
	void ClearElapsedTime();
	
	/** Has elapsed time. */
	inline bool HasElapsedTime() const{ return pHasElapsedTime; }
	
	/** Sliding elapsed time factor. */
	inline float GetSlidingElapsedTimeFactor() const{ return pSlidingElapsedTimeFactor; }
	
	/** Set sliding elapsed time factor. */
	void SetSlidingElapsedTimeFactor( float factor );
	
	
	
	/** Counter. */
	inline int GetCounter() const{ return pCounter; }
	
	/** Set counter. */
	void SetCounter( int counter );
	
	/** Increment counter by one. */
	void IncrementCounter();
	
	/** Increment counter. */
	void IncrementCounter( int count );
	
	/** Clear counter. */
	void ClearCounter();
	
	/** Has counter. */
	inline bool HasCounter() const{ return pHasCounter; }
	
	/** Copy results from another debug information. */
	void CopyResults( const deoglDebugInformation &info );
	/*@}*/
	
	
	
	/** \name Rendering for use by deoglRenderDevMode only. */
	/*@{*/
	/** Render text. */
	inline const decString &GetRenderText() const{ return pRenderText; }
	
	/** Set render text. */
	void SetRenderText( const char *text );
	
	/** Render size. */
	inline const decPoint &GetRenderSize() const{ return pRenderSize; }
	
	/** Set render size. */
	void SetRenderSize( const decPoint &size );
	
	/** Render position. */
	inline const decPoint &GetRenderPosition() const{ return pRenderPosition; }
	
	/** Set render position. */
	void SetRenderPosition( const decPoint &position );
	/*@}*/
};

#endif
