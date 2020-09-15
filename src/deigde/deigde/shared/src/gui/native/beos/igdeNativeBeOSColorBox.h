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

#ifndef _IGDENATIVEBEOSCOLORBOX_H_
#define _IGDENATIVEBEOSCOLORBOX_H_

#include "beostoolkit.h"
#include "../../resources/igdeFontReference.h"

class igdeColorBox;
class igdeEnvironment;
class igdeGuiTheme;
class igdeWidget;


/**
 * FOX Native colorBox.
 */
class igdeNativeBeOSColorBox : public BView{
private:
	igdeColorBox *pOwner;
	int pDefaultHeight;
	int pDefaultWidth;
	
	// Temporary
	static decColor vCopyColor;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeBeOSColorBox( igdeColorBox &owner, const igdeGuiTheme &guitheme );
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeBeOSColorBox();
	
	/** \brief Create native widget. */
	static igdeNativeBeOSColorBox* CreateNativeWidget( igdeColorBox &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual void GetPreferredSize( float *width, float *height );
	virtual void MouseUp( BPoint where );
	
	virtual void UpdateColor();
	virtual void UpdateDescription();
	virtual void UpdateEnabled();
	
	virtual void ClipboardPutColor( const decColor &color );
	virtual decColor ClipboardGetColor();
	
	static rgb_color ColorIgdeToBeOS( const decColor &color );
	static decColor ColorBeOSToIgde( rgb_color color );
	static int ColorBoxHeight( const igdeGuiTheme &guitheme );
	/*@}*/
};

typedef igdeNativeBeOSColorBox igdeNativeColorBox;

#endif
