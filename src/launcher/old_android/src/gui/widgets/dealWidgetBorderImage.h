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

#ifndef _DEALWIDGETBORDERIMAGE_H_
#define _DEALWIDGETBORDERIMAGE_H_

#include "dealWidgetImage.h"


/**
 * \brief 9-part border image widget.
 */
class dealWidgetBorderImage : public dealWidgetImage{
private:
	int pBorderLeft;
	int pBorderTop;
	int pBorderRight;
	int pBorderBottom;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create widget. */
	dealWidgetBorderImage(dealDisplay &display, dealImage *image = NULL, int borderLeft = 0,
		int borderTop = 0, int borderRight = 0, int borderBottom = 0);
	
	/** \brief Clean up widget. */
	virtual ~dealWidgetBorderImage();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Left border size. */
	inline int getBorderLeft() const{return pBorderLeft;}
	
	/** \brief Set left border size. */
	void SetBorderLeft(int size);
	
	/** \brief Top border size. */
	inline int getBorderTop() const{return pBorderTop;}
	
	/** \brief Set top border size. */
	void SetBorderTop(int size);
	
	/** \brief Right border size. */
	inline int getBorderRight() const{return pBorderRight;}
	
	/** \brief Set right border size. */
	void SetBorderRight(int size);
	
	/** \brief Bottom border size. */
	inline int getBorderBottom() const{return pBorderBottom;}
	
	/** \brief Set bottom border size. */
	void SetBorderBottom(int size);
	
	/** \brief Set borders. */
	void SetBorders(int left, int top, int right, int bottom);
	
	
	
	/** \brief Render content. */
	virtual void RenderContent(const sRenderContext &context);
	/*@}*/
};

#endif
