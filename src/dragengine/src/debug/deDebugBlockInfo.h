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

#ifndef _DEDEBUGBLOCKINFO_H_
#define _DEDEBUGBLOCKINFO_H_

#include "../resources/canvas/deCanvasView.h"
#include "../resources/canvas/deCanvasText.h"
#include "../resources/canvas/deCanvasPaint.h"
#include "../resources/font/deFont.h"
#include "../common/collection/decPointerList.h"
#include "../common/math/decMath.h"
#include "../common/string/decString.h"


class deEngine;


/**
 * \brief Debug overlay block displaying vertically aligned list of key value pairs.
 * 
 * The individual entries can be color coded.
 */
class DE_DLL_EXPORT deDebugBlockInfo{
private:
	struct sEntry{
		decString label;
		decString text;
		decColor color;
		decColor bgColor;
		deCanvasView::Ref view;
		deCanvasPaint::Ref viewBg;
		deCanvasText::Ref viewLabel;
		deCanvasText::Ref viewText;
	};
	
	
	deEngine &pEngine;
	decPointerList pEntries;
	deCanvasView::Ref pView;
	deCanvasPaint::Ref pViewTitleBg;
	deCanvasText::Ref pViewTitle;
	deFont::Ref pFont;
	int pPadding;
	int pSpacing;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create debug overlay block info. */
	deDebugBlockInfo(deEngine &engine);
	
	/** \brief Clean up debug overlay block info. */
	~deDebugBlockInfo();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Canvas view to be added to debug overlay. */
	inline deCanvasView *GetView() const{ return pView; }
	
	/** \brief Font. */
	inline deFont *GetFont() const{ return pFont; }
	
	/** \brief Set font. */
	void SetFont(deFont *font);
	
	/** \brief Padding. */
	inline int GetPadding() const{ return pPadding; }
	
	/** \brief Set padding. */
	void SetPadding(int padding);
	
	/** \brief Spacing. */
	inline int GetSpacing() const{ return pSpacing; }
	
	/** \brief Set spacing. */
	void SetSpacing(int spacing);
	
	
	
	/** \brief Add to debug overlay. */
	void AddToOverlay();
	
	/** \brief Remove from debug overlay. */
	void RemoveFromOverlay();
	
	
	
	/** \brief Set title. */
	void SetTitle(const char *title);
	
	/** \brief Set title color. */
	void SetTitleColor(const decColor &color);
	
	/** \brief Set title background color. */
	void SetTitleBgColor(const decColor &color);
	
	
	
	/** \brief Number of entries. */
	int GetEntryCount() const;
	
	/** \brief Add entry. */
	void AddEntry(const char *label, const char *text,
		const decColor &color = decColor(1.0f, 1.0f, 1.0f), 
		const decColor &bgColor = decColor(0.0f, 0.0f, 0.0f, 0.5f));
	
	/** \brief Set entry label. */
	void SetEntryLabel(int index, const char *label);
	
	/** \brief Set entry text. */
	void SetEntryText(int index, const char *text);
	
	/** \brief Set entry text color. */
	void SetEntryColor(int index, const decColor &color);
	
	/** \brief Set entry background color. */
	void SetEntryBgColor(int index, const decColor &color);
	
	/** \brief Remove entry. */
	void RemoveEntry(int index);
	
	/** \brief Remove all entries. */
	void RemoveAllEntries();
	
	
	
	/**
	 * \brief Update view.
	 * 
	 * Call this after changing information.
	 */
	void UpdateView();
	
	
	
	/** \brief Text size. */
	static decPoint pTextSize(const deFont &font, const char *text);
	/*@}*/
};

#endif
