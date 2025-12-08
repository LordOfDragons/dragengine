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

#ifndef _DEALWIDGETIMAGE_H_
#define _DEALWIDGETIMAGE_H_

#include "dealWidget.h"

class dealImage;


/**
 * \brief Image widget.
 */
class dealWidgetImage : public dealWidget{
private:
	dealImage::Ref pImage;
	decColor pColorize;
	float pScaling;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create widget. */
	dealWidgetImage(dealDisplay &display, dealImage::Ref image = NULL);
	
	/** \brief Clean up widget. */
	~dealWidgetImage() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Image or \em NULL if not set. */
	inline const dealImage::Ref &GetImage() const{ return pImage; }
	
	/** \brief Set image or \em NULL if not set. */
	void SetImage(dealImage *image);
	
	/** \brief Colorize color. */
	inline const decColor &GetColorize() const{ return pColorize; }
	
	/** \brief Set colorize color. */
	void SetColorize(const decColor &colorize);
	
	/** \brief Scaling. */
	inline float GetScaling() const{ return pScaling; }
	
	/** \brief Set scaling. */
	void SetScaling(float scaling);
	
	
	
	/** \brief Minimum size of widget. */
	decPoint GetMinimumSize() override;
	
	
	/** \brief Render content. */
	void RenderContent(const sRenderContext &context) override;
	/*@}*/
};

#endif
