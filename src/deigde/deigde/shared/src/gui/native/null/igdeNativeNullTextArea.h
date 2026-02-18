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

#ifndef _IGDENATIVENULLTEXTAREA_H_
#define _IGDENATIVENULLTEXTAREA_H_

#include "../../igdeTextArea.h"

class igdeTextArea;


/**
 * Null Text Area.
 */
class igdeNativeNullTextArea : public igdeTextArea::cNativeTextArea{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create text widget. */
	igdeNativeNullTextArea();
	
	/** \brief Clean up text widget. */
	virtual ~igdeNativeNullTextArea();
	
	/** \brief Create native widget. */
	static igdeNativeNullTextArea* CreateNativeWidget(igdeTextArea &owner);
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	void UpdateStyles();
	void ApplyStyles();
	void UpdateText() override;
	void UpdateEnabled() override;
	void UpdateDescription() override;
	void UpdateEditable() override;
	void Focus() override;
	virtual int GetCursorPosition() const;
	void SetCursorPosition(int position) override;
	virtual int GetCursorColumn() const;
	virtual int GetCursorRow() const;
	void SetCursorColumn(int column) override;
	void SetCursorRow(int row) override;
	virtual int GetTopLine() const;
	void SetTopLine(int line) override;
	virtual int GetBottomLine() const;
	void SetBottomLine(int line) override;
	virtual int GetLineCount() const;
	void UpdateColumns() override;
	void UpdateRows() override;
	/*@}*/
};

typedef igdeNativeNullTextArea igdeNativeTextArea;

#endif
