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

#ifndef _IGDENATIVEBEOSRESIZER_H_
#define _IGDENATIVEBEOSRESIZER_H_

#include <interface/View.h>
#include <support/SupportDefs.h>

/**
 * \brief Resize handle widget for window resizing.
 */
class igdeNativeBeosResizer : public BView {
public:
	/** \brief Resize direction flags. */
	enum eResizeDirection {
		erdNone = 0,
		erdLeft = 1,
		erdRight = 2,
		erdTop = 4,
		erdBottom = 8,
		erdTopLeft = erdLeft | erdTop,
		erdTopRight = erdRight | erdTop,
		erdBottomLeft = erdLeft | erdBottom,
		erdBottomRight = erdRight | erdBottom
	};
	
private:
	eResizeDirection pResizeDirection;
	BPoint pLastMousePos;
	bool pResizing;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create resizer. */
	igdeNativeBeosResizer(BRect frame, eResizeDirection direction = erdBottomRight);
	
	/** \brief Clean up resizer. */
	~igdeNativeBeosResizer() override;
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Get resize direction. */
	inline eResizeDirection GetResizeDirection() const{ return pResizeDirection; }
	
	/** \brief Set resize direction. */
	void SetResizeDirection(eResizeDirection direction);
	
	/** \brief Get whether currently resizing. */
	inline bool IsResizing() const{ return pResizing; }
	
	/** \brief BView overrides */
	void Draw(BRect updateRect) override;
	void MouseDown(BPoint where) override;
	void MouseUp(BPoint where) override;
	void MouseMoved(BPoint where, uint32 code, const BMessage *dragMessage) override;
	/*@}*/
};

#endif
