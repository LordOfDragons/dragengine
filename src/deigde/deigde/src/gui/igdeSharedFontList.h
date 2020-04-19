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

#ifndef _IGDESHAREDFONTSLIST_H_
#define _IGDESHAREDFONTSLIST_H_

#include <deigde/gui/resources/igdeFont.h>
#include <deigde/gui/resources/igdeFontReference.h>

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectList.h>

class igdeEnvironment;



/**
 * \brief IGDE Shared font list.
 */
class igdeSharedFontList{
private:
	class cFont : public deObject{
	private:
		igdeFont::sConfiguration pConfig;
		igdeFontReference pFont;
		
	public:
		cFont( const igdeFont::sConfiguration &config, igdeFont *font );
		virtual ~cFont();
		
		inline igdeFont *GetFont() const{ return pFont; }
		inline const igdeFont::sConfiguration &GetConfig() const{ return pConfig; }
	};
	
	igdeEnvironment &pEnvironment;
	decObjectList pFonts;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create shared font list. */
	igdeSharedFontList( igdeEnvironment &environment );
	
	/** \brief Clean up shared font list. */
	~igdeSharedFontList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Get font matching configuration. */
	igdeFont *GetFontWith( const igdeFont::sConfiguration &configuration );
	/*@}*/
};

#endif
