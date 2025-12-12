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

#ifndef _IGDENATIVEFOXTEXTAREA_H_
#define _IGDENATIVEFOXTEXTAREA_H_

#include "foxtoolkit.h"
#include "../../resources/igdeFont.h"


class igdeTextArea;
class igdeGuiTheme;
class igdeNativeFoxResizer;


/**
 * \brief FOX toolkit Native Text Area.
 */
class igdeNativeFoxTextArea : public FXVerticalFrame{
	FXDECLARE(igdeNativeFoxTextArea)
protected:
	   igdeNativeFoxTextArea();
	
public:
	enum eFoxIDs{
		ID_SELF = FXVerticalFrame::ID_LAST,
		ID_RESIZER,
		ID_LAST
	};
	
private:
	igdeTextArea *pOwner;
	igdeFont::Ref pFont;
	FXText *pTextArea;
	FXHiliteStyle *pStyles;
	igdeNativeFoxResizer *pResizer;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create text widget. */
	igdeNativeFoxTextArea(igdeTextArea &owner, FXComposite *parent,
		const igdeUIFoxHelper::sChildLayoutFlags &layoutFlags, const igdeGuiTheme &guitheme);
	
	/** \brief Clean up text widget. */
	virtual ~igdeNativeFoxTextArea();
	
	/** \brief Create native widget. */
	static igdeNativeFoxTextArea* CreateNativeWidget(igdeTextArea &owner);
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Text area. */
	inline FXText &GetTextArea() const{ return *pTextArea; }
	
	/** \brief Resizer or \em nullptr. */
	inline igdeNativeFoxResizer *GetResizser() const{ return pResizer; }
	
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
	
	static int TextAreaFlagsBorder(const igdeTextArea &owner);
	static int TextAreaFlags(const igdeTextArea &owner);
	static igdeFont *TextAreaFont(const igdeTextArea &owner, const igdeGuiTheme &guitheme);
	static int TextAreaPadLeft(const igdeGuiTheme &guitheme);
	static int TextAreaPadRight(const igdeGuiTheme &guitheme);
	static int TextAreaPadTop(const igdeGuiTheme &guitheme);
	static int TextAreaPadBottom(const igdeGuiTheme &guitheme);
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onMouseLeftPress(FXObject*, FXSelector, void*);
	long onMouseLeftRelease(FXObject*, FXSelector, void*);
	long onCommand(FXObject*, FXSelector, void*);
	long onChanged(FXObject*, FXSelector, void*);
	
	long onResizerDrag(FXObject*, FXSelector, void*);
	/*@}*/
	
	
	
private:
	void pBuildStylesArray();
};

typedef igdeNativeFoxTextArea igdeNativeTextArea;

#endif
