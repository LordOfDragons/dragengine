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

#ifndef _DEGLB_ICONVIEW_H_
#define _DEGLB_ICONVIEW_H_

#include <View.h>


/**
 * \brief Icon view.
 * \note Bitmap is not shared and has to stay valid as long as the widget exists.
 */
class deglbIconView : public BView{
private:
	BBitmap *pBitmap;
	int pMinSize;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create view. */
	deglbIconView(BBitmap *bitmap = nullptr, int minSize = 0);
	
	/** \brief Clean up dialog. */
	~deglbIconView() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Icon or nullptr. */
	inline BBitmap *GetIcon() const{ return pBitmap; }
	
	/** \brief Set icon or nullptr. */
	void SetIcon(BBitmap *bitmap);
	/*@}*/
	
	
	
	/** \name BView */
	BSize MinSize() override;
	BSize PreferredSize() override;
	void Draw(BRect updateRect) override;
	/*@}*/
};

#endif
