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

#ifndef _DEAINPOVERLAYSYSTEM_H_
#define _DEAINPOVERLAYSYSTEM_H_

#include <dragengine/common/math/decMath.h>

#include "deainpOverlayList.h"

class deAndroidInput;
class deCanvasView;


/**
 * \brief Overlays.
 */
class deainpOverlaySystem{
private:
	deAndroidInput &pAndroidInput;
	
	deainpOverlayList pOverlays;
	
	deCanvasView::Ref pCanvas;
	bool pInHorizontalMode;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create overlays. */
	deainpOverlaySystem(deAndroidInput &androidInput);
	
	/** \brief Clean up overlays. */
	virtual ~deainpOverlaySystem();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Overlay list. */
	deainpOverlayList GetOverlayList(){return pOverlays;}
	const deainpOverlayList &GetOverlayList() const{ return pOverlays; }
	
	/** \brief Overlays canvas. */
	inline const deCanvasView::Ref &GetCanvas() const{ return pCanvas; }
	
	/** \brief Update canvas content. */
	void UpdateContent();
	
	/** \brief Screen size changed. */
	void ScreenSizeChanged();
	
	/** \brief Update all binding indices. */
	void UpdateBindingIndices();
	
	/** \brief Overlay is in horizontal mode. */
	inline bool GetInHorizontalMode() const{ return pInHorizontalMode; }
	
	/** \brief Set if overlay is in horizontal mode. */
	void SetInHorizontalMode(bool inHorizontalMode);
	
	/** \brief Update overlays from active layout mode. */
	void UpdateFromLayout();
	
	/**
	 * \brief Touch event.
	 * \param[in] position Touch position.
	 * \returns \em true if event has been handled or \em false otherwise.
	 */
	bool OnTouch(int pointerId, const decPoint &position);
	
	/**
	 * \brief Move event.
	 * \param[in] position Touch position.
	 * \returns \em true if event has been handled or \em false otherwise.
	 */
	bool OnMove(int pointerId, const decPoint &position);
	
	/**
	 * \brief Release.
	 * \returns \em true if event has been handled or \em false otherwise.
	 */
	bool OnRelease(int pointerId);
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
