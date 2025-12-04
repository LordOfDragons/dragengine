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

#ifndef _IGDENATIVEFOXCOLORBOX_H_
#define _IGDENATIVEFOXCOLORBOX_H_

#include "foxtoolkit.h"
#include "../../resources/igdeFont.h"

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
