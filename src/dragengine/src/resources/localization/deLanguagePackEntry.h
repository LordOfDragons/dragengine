/* 
 * Drag[en]gine Game Engine
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

#ifndef _DELANGUAGEPACKENTRY_H_
#define _DELANGUAGEPACKENTRY_H_

#include "../../common/string/decString.h"
#include "../../common/string/unicode/decUnicodeString.h"


/**
 * \brief Language pack translation entry.
 */
class DE_DLL_EXPORT deLanguagePackEntry{
private:
	decString pName;
	decUnicodeString pText;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create language pack entry. */
	deLanguagePackEntry();
	
	/** \brief Clean up language pack entry. */
	~deLanguagePackEntry();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName( const char *name );
	
	/** \brief Text. */
	inline const decUnicodeString &GetText() const{ return pText; }
	
	/** \brief Set text. */
	void SetText( const decUnicodeString &text );
	/*@}*/
};

#endif
