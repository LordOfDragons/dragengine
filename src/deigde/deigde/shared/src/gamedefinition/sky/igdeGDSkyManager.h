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

#ifndef _IGDEGDSKYMANAGER_H_
#define _IGDEGDSKYMANAGER_H_

#include "igdeGDSkyList.h"
#include "../igdeGDCategoryReference.h"

#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringList.h>

class igdeGDVisitor;
class deVirtualFileSystem;
class decPath;


/**
 * \brief Game definition sky manager.
 */
class DE_DLL_EXPORT igdeGDSkyManager{
private:
	igdeGDSkyList pSkyList;
	igdeGDCategoryReference pCategories;
	decString pDefaultPath;
	decStringList pAutoFindPath;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create sky manager. */
	igdeGDSkyManager();
	
	/** \brief Clean up sky manager. */
	~igdeGDSkyManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Sky list. */
	inline const igdeGDSkyList &GetSkyList() const{ return pSkyList; }
	
	/** \brief Add sky. */
	void AddSky( igdeGDSky *sky );
	
	/** \brief Remove sky. */
	void RemoveSky( igdeGDSky *sky );
	
	/** \brief Remove all skies. */
	void RemoveAllSkies();
	
	/** \brief Top level category object. */
	inline igdeGDCategory *GetCategories() const{ return pCategories; }
	
	/** \brief Path of default sky. */
	inline const decString &GetDefaultPath() const{ return pDefaultPath; }
	
	/** \brief Set path of default sky. */
	void SetDefaultPath( const char *path );
	
	/** \brief Set of path to use to auto-find skies. */
	inline decStringList &GetAutoFindPath(){ return pAutoFindPath; }
	inline const decStringList &GetAutoFindPath() const{ return pAutoFindPath; }
	
	/** \brief Visit skies matching the given category. */
	void VisitSkiesMatchingCategory( igdeGDVisitor &visitor, const igdeGDCategory *category ) const;
	
	/** \brief Visit skins matching filter. */
	void VisitMatchingFilter( igdeGDVisitor &visitor, const decString &filter ) const;
	
	/**
	 * \brief Updates the sky manager using another sky manager.
	 * 
	 * Adds copies of skies in the given sky manager .
	 * If the sky exists already it is replaced.
	 */
	void UpdateWith( const igdeGDSkyManager &manager );
	
	/**
	 * \brief Update skin manager with found skins.
	 */
	void UpdateWithFound( const igdeGDSkyManager &skyManager );
	
	/** \brief Find skies and add them. */
	void FindAndAddSkies( deVirtualFileSystem &vfs, const decPath &directory,
		const char *pattern = "*.desky", bool recursive = true );
	/*@}*/
};

#endif
