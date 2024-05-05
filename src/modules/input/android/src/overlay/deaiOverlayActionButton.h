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
