/* 
 * Drag[en]gine Android Input Module
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

#ifndef _DEAIOVERLAY_H_
#define _DEAIOVERLAY_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>

#include "../deaiLayout.h"


class deAndroidInput;
class deCanvasView;


/**
 * \brief Base overlay class.
 */
class deaiOverlay : public deObject{
private:
	deAndroidInput &pAndroidInput;
	
	deCanvasView *pCanvas;
	
	int pPointer;
	decPoint pPointerPosition;
	
	deaiLayout pLayoutHorizontal;
	deaiLayout pLayoutVertical;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create overlay.
	 * \details Initial size is 256x256 with 35% transparency.
	 */
	deaiOverlay( deAndroidInput &androidInput );
	
	/** \brief Clean up overlay. */
	virtual ~deaiOverlay();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Input module. */
	inline deAndroidInput &GetAndroidInput() const{ return pAndroidInput; }
	
	/** \brief Canvas for this overlay. */
	inline deCanvasView *GetCanvas() const{ return pCanvas; }
	
	
	
	/** \brief Canvas has pointer assigned. */
	bool HasPointer() const;
	
	/** \brief Tracked android pointer or -1 if not tracking a pointer. */
	inline int GetPointer() const{ return pPointer; }
	
	/** \brief Clear pointer. */
	void ClearPointer();
	
	/** \brief Set tracked android pointer or -1 if not tracking a pointer. */
	void SetPointer( int pointer, const decPoint &position );
	
	/** \brief Difference to last pointer position. */
	decPoint PointerMove( const decPoint &position );
	
	/** \brief Pointer position. */
	inline const decPoint &GetPointerPosition() const{ return pPointerPosition; }
	
	
	
	/** \brief Horizontal layout. */
	inline const deaiLayout &GetLayoutHorizontal() const{ return pLayoutHorizontal; }
	
	/** \brief Set horizontal layout. */
	void SetLayoutHorizontal( const deaiLayout &layout );
	
	/** \brief Vertical layout. */
	inline const deaiLayout &GetLayoutVertical() const{ return pLayoutVertical; }
	
	/** \brief Set vertical layout. */
	void SetLayoutVertical( const deaiLayout &layout );
	
	
	
	/** \brief Update from horizontal layout. */
	void UpdateFromHorizontalLayout();
	
	/** \brief Update from vertical layout. */
	void UpdateFromVerticalLayout();
	
	/** \brief Update from layout. */
	virtual void UpdateFromLayout( const deaiLayout &layout );
	
	
	
	/** \brief Update binding indices. */
	virtual void UpdateBindingIndices();
	
	/** \brief Update canvas content. */
	virtual void UpdateContent();
	
	/**
	 * \brief Touch event.
	 * \details Overlay receives upcoming move events until release if \em true is returned.
	 *          Default implementation returns \em false.
	 * \param[in] position Touch position.
	 * \returns \em true if event has been handled or \em false otherwise.
	 */
	virtual bool OnTouch( int pointerId, const decPoint &position );
	
	/**
	 * \brief Move event.
	 * \param[in] position Move distance since last move event.
	 */
	virtual void OnMove( const decPoint &position );
	
	/** \brief Release. */
	virtual void OnRelease();
	/*@}*/
};

#endif
