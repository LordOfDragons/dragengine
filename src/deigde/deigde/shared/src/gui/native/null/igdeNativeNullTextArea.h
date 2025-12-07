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

class igdeTextArea;


/**
 * Null Text Area.
 */
class igdeNativeNullTextArea{
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
	virtual void UpdateText();
	virtual void UpdateEnabled();
	virtual void UpdateDescription();
	virtual void UpdateEditable();
	virtual void Focus();
	virtual int GetCursorPosition() const;
	virtual void SetCursorPosition(int position);
	virtual int GetCursorColumn() const;
	virtual int GetCursorRow() const;
	virtual void SetCursorColumn(int column);
	virtual void SetCursorRow(int row);
	virtual int GetTopLine() const;
	virtual void SetTopLine(int line);
	virtual int GetBottomLine() const;
	virtual void SetBottomLine(int line);
	virtual int GetLineCount() const;
	virtual void UpdateColumns();
	virtual void UpdateRows();
	/*@}*/
};

typedef igdeNativeNullTextArea igdeNativeTextArea;

#endif
