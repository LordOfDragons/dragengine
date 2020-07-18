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
	/** \brief Create debug information. */
	deoglDebugInformation( const char *name, const decColor &colorText, const decColor &colorBackground );
	
	/** \brief Clean up debug information. */
	virtual ~deoglDebugInformation();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName( const char *name );
	
	/** \brief Text color. */
	inline const decColor &GetColorText() const{ return pColorText; }
	
	/** \brief Set text color. */
	void SetColorText( const decColor &color );
	
	/** \brief Background color. */
	inline const decColor &GetColorBackground() const{ return pColorBackground; }
	
	/** \brief Set background color. */
	void SetColorBackground( const decColor &color );
	
	/** \brief Clear elapsed time and counter. */
	void Clear();
	
	/** \brief Visible. */
	inline bool GetVisible() const{ return pVisible; }
	
	/** \brief Set if visible. */
	void SetVisible( bool visible );
	
	/** \brief Children. */
	inline deoglDebugInformationList &GetChildren(){ return pChildren; }
	inline const deoglDebugInformationList &GetChildren() const{ return pChildren; }
	
	
	
	/** \brief Elapsed time. */
	inline float GetElapsedTime() const{ return pElapsedTime; }
	
	/** \brief Set elapsed time. */
	void SetElapsedTime( float time );
	
	/** \brief Increment elapsed time. */
	void IncrementElapsedTime( float time );
	
	/** \brief Clear elapsed time. */
	void ClearElapsedTime();
	
	/** \brief Has elapsed time. */
	inline bool HasElapsedTime() const{ return pHasElapsedTime; }
	
	/** \brief Sliding elapsed time factor. */
	inline float GetSlidingElapsedTimeFactor() const{ return pSlidingElapsedTimeFactor; }
	
	/** \brief Set sliding elapsed time factor. */
	void SetSlidingElapsedTimeFactor( float factor );
	
	
	
	/** \brief Counter. */
	inline int GetCounter() const{ return pCounter; }
	
	/** \brief Set counter. */
	void SetCounter( int counter );
	
	/** \brief Increment counter by one. */
	void IncrementCounter();
	
	/** \brief Increment counter. */
	void IncrementCounter( int count );
	
	/** \brief Clear counter. */
	void ClearCounter();
	
	/** \brief Has counter. */
	inline bool HasCounter() const{ return pHasCounter; }
	/*@}*/
	
	
	
	/** \name Rendering for use by deoglRenderDevMode only. */
	/*@{*/
	/** \brief Render text. */
	inline const decString &GetRenderText() const{ return pRenderText; }
	
	/** \brief Set render text. */
	void SetRenderText( const char *text );
	
	/** \brief Render size. */
	inline const decPoint &GetRenderSize() const{ return pRenderSize; }
	
	/** \brief Set render size. */
	void SetRenderSize( const decPoint &size );
	
	/** \brief Render position. */
	inline const decPoint &GetRenderPosition() const{ return pRenderPosition; }
	
	/** \brief Set render position. */
	void SetRenderPosition( const decPoint &position );
	/*@}*/
};

#endif
