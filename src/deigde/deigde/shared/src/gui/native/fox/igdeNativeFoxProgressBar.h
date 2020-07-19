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

#ifndef _IGDENATIVEFOXPROGRESSBAR_H_
#define _IGDENATIVEFOXPROGRESSBAR_H_

#include "foxtoolkit.h"


class igdeProgressBar;
class igdeGuiTheme;
class igdeFont;


/**
 * \brief FOX toolkit Native Text Area.
 */
class igdeNativeFoxProgressBar : public FXProgressBar{
	FXDECLARE( igdeNativeFoxProgressBar )
protected:
	   igdeNativeFoxProgressBar();
	
public:
	enum eFoxIDs{
		ID_SELF = FXProgressBar::ID_LAST,
		ID_RESIZER,
		ID_LAST
	};
	
private:
	igdeProgressBar *pOwner;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create text widget. */
	igdeNativeFoxProgressBar( igdeProgressBar &owner, FXComposite *parent,
		const igdeUIFoxHelper::sChildLayoutFlags &layoutFlags, const igdeGuiTheme &guitheme );
	
	/** \brief Clean up text widget. */
	virtual ~igdeNativeFoxProgressBar();
	
	/** \brief Create native widget. */
	static igdeNativeFoxProgressBar* CreateNativeWidget( igdeProgressBar &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual void UpdateRange();
	virtual void UpdateValue();
	virtual void UpdateEnabled();
	virtual void UpdateDescription();
	
	static int ProgressBarFlags( const igdeProgressBar &owner );
	static igdeFont *ProgressBarFont( const igdeProgressBar &owner, const igdeGuiTheme &guitheme );
	static int ProgressBarPadLeft( const igdeGuiTheme &guitheme );
	static int ProgressBarPadRight( const igdeGuiTheme &guitheme );
	static int ProgressBarPadTop( const igdeGuiTheme &guitheme );
	static int ProgressBarPadBottom( const igdeGuiTheme &guitheme );
	/*@}*/
};

typedef igdeNativeFoxProgressBar igdeNativeProgressBar;

#endif
