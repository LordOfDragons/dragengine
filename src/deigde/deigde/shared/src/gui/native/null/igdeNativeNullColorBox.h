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

#ifndef _IGDENATIVENULLCOLORBOX_H_
#define _IGDENATIVENULLCOLORBOX_H_

#include "../../igdeColorBox.h"
#include <dragengine/common/math/decMath.h>

class igdeColorBox;


/**
 * Null colorBox.
 */
class igdeNativeNullColorBox : public igdeColorBox::cNativeColorBox{
private:
	static decColor vCopyColor;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeNullColorBox();
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeNullColorBox();
	
	/** \brief Create native widget. */
	static igdeNativeNullColorBox* CreateNativeWidget(igdeColorBox &owner);
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	void UpdateColor() override;
	void UpdateDescription() override;
	void UpdateEnabled() override;
	
	void ClipboardPutColor(const decColor &color) override;
	decColor ClipboardGetColor() override;
	/*@}*/
};

typedef igdeNativeNullColorBox igdeNativeColorBox;

#endif
