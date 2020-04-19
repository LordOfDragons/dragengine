/* 
 * Drag[en]gine IGDE Language Pack Editor
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

#ifndef _LPEWPLANGPACK_H_
#define _LPEWPLANGPACK_H_

#include <deigde/gui/igdeTextAreaReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class lpeLangPack;
class lpeWindowProperties;
class lpeWPLangPackListener;



/**
 * \brief Language pack properties panel.
 */
class lpeWPLangPack : public igdeContainerScroll{
private:
	lpeWindowProperties &pWindowProperties;
	lpeWPLangPackListener *pListener;
	
	lpeLangPack *pLangPack;
	
	igdeTextFieldReference pEditName;
	igdeTextAreaReference pEditDescription;
	igdeTextFieldReference pEditMissingText;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	lpeWPLangPack( lpeWindowProperties &windowProperties );
	
protected:
	/** \brief Clean up panel. */
	virtual ~lpeWPLangPack();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Properties window. */
	inline lpeWindowProperties &GetWindowProperties() const{ return pWindowProperties; }
	
	/** \brief Language pack or NULL. */
	inline lpeLangPack *GetLangPack() const{ return pLangPack; }
	
	/** \brief Set language pack or NULL. */
	void SetLangPack( lpeLangPack *langpack );
	
	/** \brief Update language pack. */
	void UpdateLangPack();
	/*@}*/
};

#endif
