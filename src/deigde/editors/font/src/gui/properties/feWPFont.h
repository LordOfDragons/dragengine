/* 
 * Drag[en]gine IGDE Font Editor
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

#ifndef _FEWPFONT_H_
#define _FEWPFONT_H_

#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class feFont;
class feWindowProperties;
class feWPFontListener;



/**
 * \brief Font Panel.
 */
class feWPFont : public igdeContainerScroll{
private:
	feWindowProperties &pWindowProperties;
	feFont *pFont;
	feWPFontListener *pListener;
	
	igdeTextFieldReference pEditImagePath;
	igdeButtonReference pBtnImagePath;
	igdeTextFieldReference pEditLineHeight;
	igdeCheckBoxReference pChkColorFont;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	feWPFont( feWindowProperties &windowProperties );
	
	/** \brief Clean up panel. */
	virtual ~feWPFont();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Properties window. */
	inline feWindowProperties &GetWindowProperties() const{ return pWindowProperties; }
	
	/** \brief Font. */
	inline feFont *GetFont() const{ return pFont; }
	
	/** \brief Set font. */
	void SetFont( feFont *font );
	
	/** \brief Update font. */
	void UpdateFont();
	/*@}*/
};

#endif
