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

#ifndef _DEALWIDGETLABEL_H_
#define _DEALWIDGETLABEL_H_

#include "dealWidget.h"
#include "../../common/string/decString.h"


class dealFont;


/**
 * \brief Label widget.
 */
class dealWidgetLabel : public dealWidget{
public:
	/** \brief Alignment. */
	enum eAlignments{
		/** \brief Left alignment. */
		eaLeft = 0x01,
		
		/** \brief Center alignment. */
		eaCenter = 0x02,
		
		/** \brief Right alignment. */
		eaRight = 0x03,
		
		/** \brief Top alignment. */
		eaTop = 0x10,
		
		/** \brief Middle alignment. */
		eaMiddle = 0x20,
		
		/** \brief Bottom alignment. */
		eaBottom = 0x30
	};
	
private:
	dealFont::Ref pFont;
	int pFontSize;
	decString pText;
	decColor pColor;
	eAlignments pAlignment;
	int pMaxLineWidth;
	
	decPoint pTextSize;
	bool pDirtyTextSize;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create widget. */
	dealWidgetLabel(dealDisplay &display, const char *text);
	
	/** \brief Create widget. */
	dealWidgetLabel(dealDisplay &display, dealFont::Ref font, int fontSize, const char *text);
	
	/** \brief Clean up widget. */
	~dealWidgetLabel() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Font or \em NULL if not set. */
	inline dealFont *GetFont() const{ return pFont; }
	
	/** \brief Set font or \em NULL if not set. */
	void SetFont(dealFont::Ref font);
	
	/** \brief Font size in pixels. */
	inline int GetFontSize() const{
		return pFontSize;
	}
	
	/** \brief Set font size in pixels. */
	void SetFontSize(int fontSize);
	
	/** \brief Text. */
	inline const decString &GetText() const{ return pText; }
	
	/** \brief Set text. */
	void SetText(const char *text);
	
	/** \brief Text color. */
	inline const decColor &GetColor() const{ return pColor; }
	
	/** \brief Set text color. */
	void SetColor(const decColor &color);
	
	/** \brief Alignment. */
	inline eAlignments GetAlignment() const{ return pAlignment; }
	
	/** \brief Set alignment. */
	void SetAlignment(eAlignments alignment);
	
	/** \brief Maximum line width. */
	inline int GetMaxLineWidth() const{ return pMaxLineWidth; }
	
	/** \brief Set maximum line width. */
	void SetMaxLineWidth(int lineWidth);
	
	
	
	/** \brief Minimum size of widget. */
	decPoint GetMinimumSize() override;
	
	
	
	/** \brief Render content. */
	void RenderContent(const sRenderContext &context) override;
	
	
	
	/** \brief Update text size if dirty. */
	void UpdateTextSize();
	/*@}*/
};



/** \brief Helper for combining alignments. */
inline dealWidgetLabel::eAlignments operator|(dealWidgetLabel::eAlignments a, dealWidgetLabel::eAlignments b){
	return (dealWidgetLabel::eAlignments)((int)a | (int)b);
}

inline dealWidgetLabel::eAlignments operator&(dealWidgetLabel::eAlignments a, dealWidgetLabel::eAlignments b){
	return a = (dealWidgetLabel::eAlignments)((int)a & (int)b);
}

#endif
