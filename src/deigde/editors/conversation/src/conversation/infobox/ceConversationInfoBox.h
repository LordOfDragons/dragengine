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

#ifndef _CECONVERSATIONINFOBOX_H_
#define _CECONVERSATIONINFOBOX_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decTList.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/font/deFontSize.h>
#include <dragengine/resources/canvas/deCanvasView.h>

class deGraphicContext;
class ceConversation;



/**
 * \brief Shows an information box for special actions in a conversation playback.
 */
class ceConversationInfoBox : public deObject{
public:
	typedef deTObjectReference<ceConversationInfoBox> Ref;
	
private:
	ceConversation &pConversation;
	
	deFont::Ref pEngFont;
	deFontSize::Ref pEngFontSize;
	
	decString pPathFont;
	decColor pBackgroundColor;
	decColor pTextColor;
	int pTextSize;
	int pPadding;
	int pWidth;
	int pHeight;
	
	decString pText;
	
	decTList<decString> pLayoutTexts;
	decTList<int> pLayoutWidths;
	
	deCanvasView::Ref pCanvasView;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create info box. */
	ceConversationInfoBox(ceConversation &conversation);
	
	/** \brief Clean up text box. */
	~ceConversationInfoBox();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Conversation. */
	inline ceConversation &GetConversation() const{ return pConversation; }
	
	/** \brief Engine font. */
	inline const deFont::Ref &GetFont() const{ return pEngFont; }
	
	/** \brief Engine font size. */
	inline const deFontSize::Ref &GetFontSize() const{ return pEngFontSize; }
	
	/** \brief Font path. */
	inline const decString &GetPathFont() const{ return pPathFont; }
	
	/** \brief Set font path. */
	void SetPathFont(const char *path);
	
	/** \brief Background color. */
	inline const decColor &GetBackgroundColor() const{ return pBackgroundColor; }
	
	/** \brief Set background color. */
	void SetBackgroundColor(const decColor &color);
	
	/** \brief Text color. */
	inline const decColor &GetTextColor() const{ return pTextColor; }
	
	/** \brief Set text color. */
	void SetTextColor(const decColor &color);
	
	/** \brief Text size. */
	inline int GetTextSize() const{ return pTextSize; }
	
	/** \brief Set text size. */
	void SetTextSize(int size);
	
	/** \brief Text padding. */
	inline int GetPadding() const{ return pPadding; }
	
	/** \brief Set text padding. */
	void SetPadding(int padding);
	
	/** \brief Text or an empty string to hide the box. */
	inline const decString &GetText() const{ return pText; }
	
	/** \brief Set text or an empty string to hide the box. */
	void SetText(const char *text);
	
	/** \brief Clear info box. */
	void Clear();
	
	/** \brief Canvas view. */
	inline const deCanvasView::Ref &GetCanvasView() const{ return pCanvasView; }
	
	/**
	 * \brief Update canvas.
	 * \details Call if:
	 *          - Text changed
	 *          - Parent size changed
	 *          - Canvas has been added to a parent.
	 */
	void UpdateCanvas();
	/*@}*/
	
private:
	void pCleanUp();
	
	void pUpdateFont();
	void pUpdateFontSize();
	void pUpdateText();
};

#endif
