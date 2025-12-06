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

#ifndef _IGDENATIVEFOXLABEL_H_
#define _IGDENATIVEFOXLABEL_H_

#include "foxtoolkit.h"
#include "../../resources/igdeFont.h"

class igdeLabel;
class igdeEnvironment;
class igdeGuiTheme;
class igdeWidget;


/**
 * FOX Native label.
 */
class igdeNativeFoxLabel : public FXLabel{
	FXDECLARE( igdeNativeFoxLabel )
	
protected:
	igdeNativeFoxLabel();
	
public:
	enum eFoxIDs{
		ID_SELF = FXLabel::ID_LAST,
	};
	
private:
	igdeLabel *pOwner;
	igdeFont::Ref pFont;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeFoxLabel( igdeLabel &owner, FXComposite *parent, int layoutFlags,
		const igdeGuiTheme &guitheme );
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeFoxLabel();
	
	/** \brief Create native widget. */
	static igdeNativeFoxLabel* CreateNativeWidget( igdeLabel &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual void UpdateText();
	virtual void UpdateAlignment();
	virtual void UpdateDescription();
	virtual void UpdateIcon();
	
	static FXIcon *LabelIcon( const igdeLabel &owner );
	static int LabelFlags( const igdeLabel &owner );
	static igdeFont *LabelFont( const igdeLabel &owner, const igdeGuiTheme &guitheme );
	/*@}*/
};

typedef igdeNativeFoxLabel igdeNativeLabel;

#endif
