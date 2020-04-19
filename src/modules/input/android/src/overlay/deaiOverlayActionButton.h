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

#ifndef _DEAIOVERLAYACTIONBUTTON_H_
#define _DEAIOVERLAYACTIONBUTTON_H_

#include "deaiInputBinding.h"
#include "deaiOverlay.h"

class deCanvasImage;
class deCanvasText;
class deFont;


/**
 * \brief Action button overlay.
 */
class deaiOverlayActionButton : public deaiOverlay{
private:
	deaiInputBinding pBinding;
	
	int pRadius;
	decPoint pCenter;
	
	bool pPressed;
	
	deCanvasImage *pCImage;
	deCanvasText *pCLabel;
	
	bool pDirtyLabelSize;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create overlay action button. */
	deaiOverlayActionButton( deAndroidInput &androidInput );
	
	/** \brief Clean up overlay action button. */
	virtual ~deaiOverlayActionButton();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Binding. */
	inline const deaiInputBinding &GetBinding() const{ return pBinding; }
	
	/** \brief Set binding. */
	void SetBinding( const deaiInputBinding &binding );
	
	
	
	/** \brief Button radius in pixels. */
	inline int GetRadius() const{ return pRadius; }
	
	/** \brief Set button radius in pixels. */
	void SetRadius( int radius );
	
	/** \brief Center of button. */
	inline const decPoint &GetCenter() const{ return pCenter; }
	
	/** \brief Set center of button. */
	void SetCenter( const decPoint &center );
	
	
	
	/** \brief Button text. */
	const decString &GetText() const;
	
	/** \brief Set button text. */
	void SetText( const char *text );
	
	/** \brief Button text color. */
	const decColor &GetColor() const;
	
	/** \brief Set button text color. */
	void SetColor( const decColor &color );
	
	/** \brief Button text font or \em NULL if not set. */
	deFont *GetFont() const;
	
	/** \brief Set button text font or \em NULL if not set. */
	void SetFont( deFont *font );
	
	
	
	/** \brief Button is pressed. */
	inline bool GetPressed() const{ return pPressed; }
	
	/** \brief Set if button is pressed. */
	void SetPressed( bool pressed );
	
	
	
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
	
private:
	void pCleanUp();
	void pUpdateLabelSize();
};

#endif
