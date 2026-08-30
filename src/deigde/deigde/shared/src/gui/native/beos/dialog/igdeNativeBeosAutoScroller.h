/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _IGDENATIVEBEOSAUTOSCROLLER_H_
#define _IGDENATIVEBEOSAUTOSCROLLER_H_

#include <interface/View.h>

/**
 * \brief Auto-scroller helper for drag-and-drop operations.
 */
class igdeNativeBeosAutoScroller{
private:
	BView *pView;
	BPoint pLastMousePos;
	uint32 pScrollDirection;
	int32 pScrollSpeed;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create auto-scroller. */
	explicit igdeNativeBeosAutoScroller(BView *view);
	
	/** \brief Clean up auto-scroller. */
	~igdeNativeBeosAutoScroller();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Get associated view. */
	inline BView *GetView() const{ return pView; }
	
	/** \brief Set target view. */
	void SetView(BView *view);
	
	/** \brief Update scroll position based on mouse movement. */
	void UpdateScroll(BPoint mousePos, BRect scrollArea);
	
	/** \brief Start auto-scrolling. */
	void StartAutoScroll();
	
	/** \brief Stop auto-scrolling. */
	void StopAutoScroll();
	
	/** \brief Get scroll direction. */
	inline uint32 GetScrollDirection() const{ return pScrollDirection; }
	
	/** \brief Get scroll speed. */
	inline int32 GetScrollSpeed() const{ return pScrollSpeed; }
	
	/** \brief Set scroll speed. */
	void SetScrollSpeed(int32 speed);
	/*@}*/
};

#endif
