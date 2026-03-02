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

#ifndef _DEGLB_ICONTABVIEW_H_
#define _DEGLB_ICONTABVIEW_H_

#include <TabView.h>


/**
 * \brief Icon tab view.
 * \note Bitmap is not shared and has to stay valid as long as the widget exists.
 */
class deglbIconTabView : public BTab{
private:
	BBitmap *pBitmap;
	BString pLabel;
	float pPadding, pIconPadding;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create view. */
	deglbIconTabView(const char *label, BView *contentView);
	deglbIconTabView(BBitmap *bitmap, const char *label, BView *contentView);
	
	/** \brief Clean up dialog. */
	~deglbIconTabView() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Icon or nullptr. */
	inline BBitmap *GetIcon() const{ return pBitmap; }
	
	/** \brief Set icon or nullptr. */
	void SetIcon(BBitmap *bitmap);
	/*@}*/
	
	
	
	/** \name BView */
	void DrawLabel(BView *owner, BRect frame) override;
	/*@}*/
	
	
private:
	void pInitLabel(const char *label, BView *contentView);
};

#endif
