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

#ifndef _IGDEGDSKIN_H_
#define _IGDEGDSKIN_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/resources/image/deImageReference.h>



/**
 * \brief Game Skin.
 * 
 * Contains information about a game skin file. The path is the unique
 * identifier of a game skin. The name is an optional name used to display
 * the skin in listings. The description is an optional too giving some
 * more in depth description of the skin if required. Every skin has a
 * cathegory and optional tags. The cathegory is used to classify a skin
 * in a tree like list whereas tags are used to filter a flat list of
 * skins the user is interested in. The cathegory is a unix path like
 * string in absolute form.
 */
class DE_DLL_EXPORT igdeGDSkin : public deObject{
private:
	decString pPath;
	decString pName;
	decString pDescription;
	decString pCategory;
	deImageReference pPreviewImage;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game definition skin. */
	igdeGDSkin( const char *path, const char *name );
	
	/** \brief Create game definition skin as a copy of a another game definition skin. */
	igdeGDSkin( const igdeGDSkin &skin );
	
protected:
	/** \breif Clean up game definition skin. */
	virtual ~igdeGDSkin();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Path. */
	inline const decString &GetPath() const{ return pPath; }
	
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Description. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description. */
	void SetDescription( const char *description );
	
	/** \brief Category. */
	inline const decString &GetCategory() const{ return pCategory; }
	
	/** \brief Set category. */
	void SetCategory( const char *category );
	
	/** \brief Preview image or NULL if not created. */
	inline deImage *GetPreviewImage() const{ return pPreviewImage; }
	
	/** \brief Set preview image or NULL if not created. */
	void SetPreviewImage( deImage *image );
	/*@}*/
};

#endif
