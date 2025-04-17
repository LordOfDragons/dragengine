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

#ifndef _DEAINPOVERLAY_H_
#define _DEAINPOVERLAY_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>

#include "../deainpLayout.h"


class deAndroidInput;
class deCanvasView;


/**
 * \brief Base overlay class.
 */
class deainpOverlay : public deObject{
public:
	typedef deTObjectReference<deainpOverlay> Ref;
	
private:
	deAndroidInput &pAndroidInput;
	
	deCanvasView *pCanvas;
	
	int pPointer;
	decPoint pPointerPosition;
	
	deainpLayout pLayoutHorizontal;
	deainpLayout pLayoutVertical;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create overlay.
	 * \details Initial size is 256x256 with 35% transparency.
	 */
	deainpOverlay( deAndroidInput &androidInput );
	
	/** \brief Clean up overlay. */
	virtual ~deainpOverlay();
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
	inline const deainpLayout &GetLayoutHorizontal() const{ return pLayoutHorizontal; }
	
	/** \brief Set horizontal layout. */
	void SetLayoutHorizontal( const deainpLayout &layout );
	
	/** \brief Vertical layout. */
	inline const deainpLayout &GetLayoutVertical() const{ return pLayoutVertical; }
	
	/** \brief Set vertical layout. */
	void SetLayoutVertical( const deainpLayout &layout );
	
	
	
	/** \brief Update from horizontal layout. */
	void UpdateFromHorizontalLayout();
	
	/** \brief Update from vertical layout. */
	void UpdateFromVerticalLayout();
	
	/** \brief Update from layout. */
	virtual void UpdateFromLayout( const deainpLayout &layout );
	
	
	
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
