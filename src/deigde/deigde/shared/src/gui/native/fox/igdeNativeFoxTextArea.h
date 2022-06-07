/* 
 * Drag[en]gine IGDE
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _IGDENATIVEFOXTEXTAREA_H_
#define _IGDENATIVEFOXTEXTAREA_H_

#include "foxtoolkit.h"
#include "../../resources/igdeFontReference.h"


class igdeTextArea;
class igdeGuiTheme;
class igdeNativeFoxResizer;


/**
 * \brief FOX toolkit Native Text Area.
 */
class igdeNativeFoxTextArea : public FXVerticalFrame{
	FXDECLARE( igdeNativeFoxTextArea )
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
	igdeFontReference pFont;
	FXText *pTextArea;
	FXHiliteStyle *pStyles;
	igdeNativeFoxResizer *pResizer;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create text widget. */
	igdeNativeFoxTextArea( igdeTextArea &owner, FXComposite *parent,
		const igdeUIFoxHelper::sChildLayoutFlags &layoutFlags, const igdeGuiTheme &guitheme );
	
	/** \brief Clean up text widget. */
	virtual ~igdeNativeFoxTextArea();
	
	/** \brief Create native widget. */
	static igdeNativeFoxTextArea* CreateNativeWidget( igdeTextArea &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Text area. */
	inline FXText &GetTextArea() const{ return *pTextArea; }
	
	/** \brief Resizer or \em NULL. */
	inline igdeNativeFoxResizer *GetResizser() const{ return pResizer; }
	
	void UpdateStyles();
	void ApplyStyles();
	virtual void UpdateText();
	virtual void UpdateEnabled();
	virtual void UpdateDescription();
	virtual void UpdateEditable();
	virtual void Focus();
	virtual int GetCursorPosition() const;
	virtual void SetCursorPosition( int position );
	virtual int GetCursorColumn() const;
	virtual int GetCursorRow() const;
	virtual void SetCursorColumn( int column );
	virtual void SetCursorRow( int row );
	virtual int GetTopLine() const;
	virtual void SetTopLine( int line );
	virtual int GetBottomLine() const;
	virtual void SetBottomLine( int line );
	virtual int GetLineCount() const;
	virtual void UpdateColumns();
	virtual void UpdateRows();
	
	static int TextAreaFlagsBorder( const igdeTextArea &owner );
	static int TextAreaFlags( const igdeTextArea &owner );
	static igdeFont *TextAreaFont( const igdeTextArea &owner, const igdeGuiTheme &guitheme );
	static int TextAreaPadLeft( const igdeGuiTheme &guitheme );
	static int TextAreaPadRight( const igdeGuiTheme &guitheme );
	static int TextAreaPadTop( const igdeGuiTheme &guitheme );
	static int TextAreaPadBottom( const igdeGuiTheme &guitheme );
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onMouseLeftPress( FXObject*, FXSelector, void* );
	long onMouseLeftRelease( FXObject*, FXSelector, void* );
	long onCommand( FXObject*, FXSelector, void* );
	long onChanged( FXObject*, FXSelector, void* );
	
	long onResizerDrag( FXObject*, FXSelector, void* );
	/*@}*/
	
	
	
private:
	void pBuildStylesArray();
};

typedef igdeNativeFoxTextArea igdeNativeTextArea;

#endif
