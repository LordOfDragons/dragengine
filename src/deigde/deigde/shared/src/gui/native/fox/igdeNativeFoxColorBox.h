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

#ifndef _IGDENATIVEFOXCOLORBOX_H_
#define _IGDENATIVEFOXCOLORBOX_H_

#include "foxtoolkit.h"
#include "../../resources/igdeFontReference.h"

class igdeColorBox;
class igdeEnvironment;
class igdeGuiTheme;
class igdeWidget;


/**
 * FOX Native colorBox.
 */
class igdeNativeFoxColorBox : public FXFrame{
	FXDECLARE( igdeNativeFoxColorBox )
	
protected:
	igdeNativeFoxColorBox();
	
public:
	enum eFoxIDs{
		ID_SELF = FXFrame::ID_LAST,
	};
	
private:
	igdeColorBox *pOwner;
	FXColor pColor;
	int pDefaultHeight;
	int pDefaultWidth;
	
	// Temporary FOX Bug hack
	static decColor vCopyColor;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeFoxColorBox( igdeColorBox &owner, FXComposite *parent,
		int layoutFlags, const igdeGuiTheme &guitheme );
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeFoxColorBox();
	
	/** \brief Create native widget. */
	static igdeNativeFoxColorBox* CreateNativeWidget( igdeColorBox &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual FXint getDefaultWidth();
	virtual FXint getDefaultHeight();
	
	virtual void UpdateColor();
	virtual void UpdateDescription();
	virtual void UpdateEnabled();
	
	virtual void ClipboardPutColor( const decColor &color );
	virtual decColor ClipboardGetColor();
	
	static FXColor ColorIgdeToFx( const decColor &color );
	static decColor ColorFxToIgde( FXColor color );
	static int ColorBoxFlags( const igdeColorBox &owner );
	static int ColorBoxHeight( const igdeGuiTheme &guitheme );
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onPaint( FXObject*, FXSelector, void* );
	long onClicked( FXObject*, FXSelector, void* );
	long onRClicked( FXObject*, FXSelector, void* );
	/*@}*/
};

typedef igdeNativeFoxColorBox igdeNativeColorBox;

#endif
