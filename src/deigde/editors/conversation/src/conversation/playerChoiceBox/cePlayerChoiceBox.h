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

#ifndef _CEPLAYERCHOICEBOX_H_
#define _CEPLAYERCHOICEBOX_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/font/deFontSize.h>

#include "cePCBOptionList.h"

class deCanvasView;
class deGraphicContext;
class ceConversation;



/**
 * \brief Player choice box in a conversation.
 */
class cePlayerChoiceBox{
private:
	ceConversation &pConversation;
	
	deFont::Ref pEngFont;
	deFontSize::Ref pEngFontSize;
	
	decString pPathFont;
	decColor pBackgroundColor;
	decColor pTextColor;
	decColor pSelectedBackgroundColor;
	decColor pSelectedTextColor;
	int pTextSize;
	int pPadding;
	int pPlaybackStackDepth;
	
	cePCBOptionList pOptions;
	int pSelectedOption;
	
	deCanvasView *pCanvasView;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create text box. */
	cePlayerChoiceBox(ceConversation &conversation);
	
	/** \brief Clean up text box. */
	~cePlayerChoiceBox();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Conversation. */
	inline ceConversation &GetConversation() const{ return pConversation; }
	
	/** \brief Engine font. */
	inline const deFont::Ref &GetFont() const{ return pEngFont; }
	
	/** \brief Engine font size. */
	inline const deFontSize::Ref &GetFontSize() const{ return pEngFontSize; }
	
	/** \brief Canvas view. */
	inline deCanvasView *GetCanvasView() const{ return pCanvasView; }
	
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
	
	/** \brief Selected background color. */
	inline const decColor &GetSelectedBackgroundColor() const{ return pSelectedBackgroundColor; }
	
	/** \brief Set selected background color. */
	void SetSelectedBackgroundColor(const decColor &color);
	
	/** \brief Selected text color. */
	inline const decColor &GetSelectedTextColor() const{ return pSelectedTextColor; }
	
	/** \brief Set selected text color. */
	void SetSelectedTextColor(const decColor &color);
	
	/** \brief Text size. */
	inline int GetTextSize() const{ return pTextSize; }
	
	/** \brief Set text size. */
	void SetTextSize(int size);
	
	/** \brief Text padding. */
	inline int GetPadding() const{ return pPadding; }
	
	/** \brief Set text padding. */
	void SetPadding(int padding);
	
	/** \brief Playback stack depth to return to after selecting an option. */
	inline int GetPlaybackStackDepth() const{ return pPlaybackStackDepth; }
	
	/** \brief Set playback stack depth to return to after selecting an option. */
	void SetPlaybackStackDepth(int depth);
	
	/** \brief List of options. */
	inline cePCBOptionList &GetOptionList(){ return pOptions; }
	inline const cePCBOptionList &GetOptionList() const{ return pOptions; }
	
	/** \brief Index of the selected option or -1 if there is none. */
	inline int GetSelectedOption() const{ return pSelectedOption; }
	
	/** \brief Set index of the selected option or -1 if there is none. */
	void SetSelectedOption(int index);
	
	/** \brief Index of the option under the given cursor position or -1 if there is none. */
	int IndexOfOptionAt(int x, int y) const;
	
	/** \brief Select option under the given cursor position if located over an option. */
	void SelectOptionAt(int x, int y);
	
	/** \brief Clear player choice box. */
	void Clear();
	
	/**
	 * \brief Update canvas.
	 * \details Call if:
	 *          - Text changed
	 *          - Selection changed
	 *          - Parent size changed
	 *          - Canvas has been added to a parent.
	 */
	void UpdateCanvas();
	/*@}*/
	
private:
	void pCleanUp();
	
	void pUpdateFont();
	void pUpdateFontSize();
};

#endif
