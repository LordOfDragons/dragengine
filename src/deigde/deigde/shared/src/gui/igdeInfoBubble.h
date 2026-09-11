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

#ifndef _IGDEINFOBUBBLE_H_
#define _IGDEINFOBUBBLE_H_

#include "resources/igdeFont.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/canvas/deCanvasPaint.h>
#include <dragengine/resources/canvas/deCanvasText.h>

class igdeViewRenderWindow;


/**
 * \brief Information bubble.
 */
class igdeInfoBubble : public deObject{
public:
	/** \brief Strong reference. */
	using Ref = deTObjectReference<igdeInfoBubble>;
	
	
	/** \brief Placement. */
	enum class Placement{
		/** \brief Top-left corner. */
		topLeft,
		
		/** \brief Top-right corner. */
		topRight,
		
		/** \brief Bottom-left corner. */
		bottomLeft,
		
		/** \brief Bottom-right corner. */
		bottomRight
	};
	
	
private:
	igdeViewRenderWindow &pView;
	
	deCanvasView::Ref pCanvasBubble, pCanvasContent;
	deCanvasPaint::Ref pCanvasBorder;
	deCanvasText::Ref pText;
	igdeFont::Ref pFont;
	decBoundary pPadding;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create information bubble. */
	explicit igdeInfoBubble(igdeViewRenderWindow &view, float order = 1000.0f);
	
protected:
	/** \brief Clean up information bubble. */
	~igdeInfoBubble() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief View. */
	inline igdeViewRenderWindow &GetView() const{ return pView; }
	
	/** \brief Content view. */
	inline const deCanvasView::Ref &GetCanvasContent() const{ return pCanvasContent; }
	
	/** \brief Font. */
	inline const igdeFont::Ref &GetFont() const{ return pFont; }
	
	/** \brief Set font. */
	void SetFont(const igdeFont::Ref &font);
	
	/** \brief Text color. */
	const decColor &GetTextColor() const;
	
	/** \brief Set text color. */
	void SetTextColor(const decColor &color);
	
	/** \brief Text. */
	const decString &GetText() const;
	
	/** \brief Set text. */
	void SetText(const decString &text);
	
	/** \brief Transparency. */
	float GetTransparency() const;
	
	/** \brief Set transparency. */
	void SetTransparency(float transparency);
	
	/** \brief Padding. */
	inline const decBoundary &GetPadding() const{ return pPadding; }

	/** \brief Set padding. */
	void SetPadding(const decBoundary &padding);
	
	
	/**
	 * \brief Calculate position and placement around mouse cursor edge.
	 * \param[in] position Target position.
	 * \param[out] showPosition Position to use for ShowAt() call.
	 * \param[out] placement Placement to use for ShowAt() call.
	 */
	void PositionAroundMouse(const decPoint &position, decPoint &showPosition, Placement &placement) const;
	
	/** \brief Show bubble at position. */
	void ShowAt(const decPoint &position, Placement placement);
	
	/** \brief Show bubble around edge of mouse cursor. */
	void ShowAroundMouse(const decPoint &position);
	
	/** \brief Show centered. */
	void ShowCentered();
	
	/** \brief Show centered with relative placement. */
	void ShowCentered(float x, float y);
	
	/** \brief Hide bubble. */
	void Hide();
	/*@}*/
	
	
protected:
	void pResize();
	decPoint pPlace(const decPoint &position, Placement placement);
};

#endif
