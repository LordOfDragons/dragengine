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

#ifndef _DEAIOVERLAYCIRCLEPAD_H_
#define _DEAIOVERLAYCIRCLEPAD_H_

#include "deaiInputBinding.h"
#include "deaiOverlay.h"

class deCanvasImage;


/**
 * \brief Circle pad overlay.
 */
class deaiOverlayCirclePad : public deaiOverlay{
private:
	deaiInputBinding pAxisLeftRight;
	deaiInputBinding pAxisUpDown;
	
	deaiInputBinding pKeyCodeLeft;
	deaiInputBinding pKeyCodeUp;
	deaiInputBinding pKeyCodeRight;
	deaiInputBinding pKeyCodeDown;
	
	deaiInputBinding pKeyCodeUpLeft;
	deaiInputBinding pKeyCodeUpRight;
	deaiInputBinding pKeyCodeDownLeft;
	deaiInputBinding pKeyCodeDownRight;
	
	bool pPressedLeft;
	bool pPressedUp;
	bool pPressedRight;
	bool pPressedDown;
	
	bool pPressedUpLeft;
	bool pPressedUpRight;
	bool pPressedDownLeft;
	bool pPressedDownRight;
	
	int pPadRadius;
	float pOuterRadius;
	float pInnerRadius;
	decPoint pCenter;
	
	float pLeftRightPerSecond;
	float pUpDownPerSecond;
	
	deCanvasView *pCanvas;
	deCanvasImage *pCImage;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create overlay circle pad. */
	deaiOverlayCirclePad( deAndroidInput &androidInput );
	
	/** \brief Clean up overlay circle pad. */
	virtual ~deaiOverlayCirclePad();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Left-right axis. */
	inline const deaiInputBinding &GetAxisLeftRight() const{ return pAxisLeftRight; }
	
	/** \brief Set left-right axis. */
	void SetAxisLeftRight( const deaiInputBinding &axis );
	
	/** \brief Up-down axis. */
	inline const deaiInputBinding &GetAxisUpDown() const{ return pAxisUpDown; }
	
	/** \brief Set up-down axis. */
	void SetAxisUpDown( const deaiInputBinding &axis );
	
	
	
	/** \brief Key binding for left pad press. */
	inline const deaiInputBinding &GetKeyCodeLeft() const{ return pKeyCodeLeft; }
	
	/** \brief Set key binding for left pad press. */
	void SetKeyCodeLeft( const deaiInputBinding &binding );
	
	/** \brief Key binding for up pad press. */
	inline const deaiInputBinding &GetKeyCodeUp() const{ return pKeyCodeUp; }
	
	/** \brief Set key binding for up pad press. */
	void SetKeyCodeUp( const deaiInputBinding &binding );
	
	/** \brief Key binding for right pad press. */
	inline const deaiInputBinding &GetKeyCodeRight() const{ return pKeyCodeRight; }
	
	/** \brief Set key binding for right pad press. */
	void SetKeyCodeRight( const deaiInputBinding &binding );
	
	/** \brief Key binding for down pad press. */
	inline const deaiInputBinding &GetKeyCodeDown() const{ return pKeyCodeDown; }
	
	/** \brief Set key binding for down pad press. */
	void SetKeyCodeDown( const deaiInputBinding &binding );
	
	
	
	/** \brief Key binding for up-left pad press or empty to activate up and left binding together. */
	inline const deaiInputBinding &GetKeyCodeUpLeft() const{ return pKeyCodeUpLeft; }
	
	/** \brief Set key binding for up-left pad press or empty to activate up and left binding together. */
	void SetKeyCodeUpLeft( const deaiInputBinding &binding );
	
	/** \brief Key binding for up-right pad press or empty to activate up and right binding together. */
	inline const deaiInputBinding &GetKeyCodeUpRight() const{ return pKeyCodeUpRight; }
	
	/** \brief Set key binding for up-right pad press or empty to activate up and right binding together. */
	void SetKeyCodeUpRight( const deaiInputBinding &binding );
	
	/** \brief Key binding for down-left pad press or empty to activate down and left binding together. */
	inline const deaiInputBinding &GetKeyCodeDownLeft() const{ return pKeyCodeDownLeft; }
	
	/** \brief Set key binding for down-left pad press or empty to activate down and left binding together. */
	void SetKeyCodeDownLeft( const deaiInputBinding &binding );
	
	/** \brief Key binding for down-right pad press or empty to activate down and right binding together. */
	inline const deaiInputBinding &GetKeyCodeDownRight() const{ return pKeyCodeDownRight; }
	
	/** \brief Set key binding for down-right pad press or empty to activate down and right binding together. */
	void SetKeyCodeDownRight( const deaiInputBinding &binding );
	
	
	
	/** \brief Left button is pressed. */
	inline bool GetPressedLeft() const{ return pPressedLeft; }
	
	/** \brief Set if left button is pressed. */
	void SetPressedLeft( bool pressed );
	
	/** \brief Up button is pressed. */
	inline bool GetPressedUp() const{ return pPressedUp; }
	
	/** \brief Set if up button is pressed. */
	void SetPressedUp( bool pressed );
	
	/** \brief Right button is pressed. */
	inline bool GetPressedRight() const{ return pPressedRight; }
	
	/** \brief Set if right button is pressed. */
	void SetPressedRight( bool pressed );
	
	/** \brief Down button is pressed. */
	inline bool GetPressedDown() const{ return pPressedDown; }
	
	/** \brief Set if down button is pressed. */
	void SetPressedDown( bool pressed );
	
	
	
	/** \brief Up-left button is pressed. */
	inline bool GetPressedUpLeft() const{ return pPressedUpLeft; }
	
	/** \brief Set up-left button is pressed. */
	void SetPressedUpLeft( bool pressed );
	
	/** \brief Up-right button is pressed. */
	inline bool GetPressedUpRight() const{ return pPressedUpRight; }
	
	/** \brief Set up-right button is pressed. */
	void SetPressedUpRight( bool pressed );
	
	/** \brief Down-left button is pressed. */
	inline bool GetPressedDownLeft() const{ return pPressedDownLeft; }
	
	/** \brief Set down-left button is pressed. */
	void SetPressedDownLeft( bool pressed );
	
	/** \brief Down-right button is pressed. */
	inline bool GetPressedDownRight() const{ return pPressedDownRight; }
	
	/** \brief Set down-right button is pressed. */
	void SetPressedDownRight( bool pressed );
	
	
	
	/** \brief Set all pressed states. */
	void SetPressedAll( bool left, bool up, bool right, bool down,
		bool upLeft, bool upRight, bool downLeft, bool downRight );
	
	
	/** \brief Pad radius in pixels. */
	inline int GetPadRadius() const{ return pPadRadius; }
	
	/** \brief Set pad radius in pixels. */
	void SetPadRadius( int radius );
	
	/** \brief Outer radius in percentage of pad radius. */
	inline float GetOuterRadius() const{ return pOuterRadius; }
	
	/** \brief Set outer radius in percentage of pad radius. */
	void SetOuterRadius( float radius );
	
	/** \brief Inner radius in percentage of pad radius. */
	inline float GetInnerRadius() const{ return pInnerRadius; }
	
	/** \brief Set inner radius in percentage of pad radius. */
	void SetInnerRadius( float radius );
	
	/** \brief Center of pad. */
	inline const decPoint &GetCenter() const{ return pCenter; }
	
	/** \brief Set center of pad. */
	void SetCenter( const decPoint &center );
	
	
	
	/** \brief Axis unit per second to move for left-right axis. */
	inline float GetLeftRightPerSecond() const{ return pLeftRightPerSecond; }
	
	/** \brief Set axis unit per second to move for left-right axis. */
	void SetLeftRightPerSecond( float unitsPerSecond );
	
	/** \brief Axis unit per second to move for up-down axis. */
	inline float GetUpDownPerSecond() const{ return pUpDownPerSecond; }
	
	/** \brief Set axis unit per second to move for up-down axis. */
	void SetUpDownPerSecond( float unitsPerSecond );
	
	
	
	/** \brief Generate input events if pointer is set. */
	void GenerateEvents();
	
	
	
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
};

#endif
