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

#ifndef _IGDENATIVEFOXLABEL_H_
#define _IGDENATIVEFOXLABEL_H_

#include "foxtoolkit.h"
#include "../../resources/igdeFontReference.h"

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
	igdeFontReference pFont;
	
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
