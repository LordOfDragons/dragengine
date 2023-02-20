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

#ifndef _IGDEGDSKINMANAGER_H_
#define _IGDEGDSKINMANAGER_H_

#include "../igdeGDCategoryReference.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringSet.h>

class igdeGDSkin;
class igdeGDVisitor;
class deVirtualFileSystem;
class decPath;


/**
 * \brief Game Skin Manager.
 * 
 * Manages a list of game skins found in the game directory or elsewhere.
 * Game skins only contain the path of skin file and some classification
 * information useful searching for the right skin to use.
 */
class DE_DLL_EXPORT igdeGDSkinManager{
private:
	decObjectOrderedSet pSkins;
	igdeGDCategoryReference pCategories;
	decString pDefaultSkinPath;
	decStringSet pAutoFindPath;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game skin manager. */
	igdeGDSkinManager();
	
	/** \brief Clean up game skin manager. */
	~igdeGDSkinManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Top level category object. */
	inline igdeGDCategory *GetCategories() const{ return pCategories; }
	
	/** \brief Number of game skins. */
	int GetSkinCount() const;
	
	/** \brief Skin is present. */
	bool HasSkin( igdeGDSkin *skin ) const;
	
	/** \brief Skin with path is present. */
	bool HasSkinWithPath( const char *path ) const;
	
	/** \brief Named skin is present. */
	bool HasSkinWithName( const char *name ) const;
	
	/** \brief Skin with name or path is present. */
	bool HasSkinWithPathOrName( const char *path, const char *name ) const;
	
	/** \brief Index of skin or -1 if absent. */
	int IndexOfSkin( igdeGDSkin *skin ) const;
	
	/** \brief Index of skin with path or -1 if absent. */
	
	int IndexOfSkinWithPath( const char *path ) const;
	
	/** \brief Index of skin with name or -1 if absent. */
	int IndexOfSkinWithName( const char *name ) const;
	
	/** \brief Index of skin with name or -1 if absent. */
	int IndexOfSkinWithPathOrName( const char *path, const char *name ) const;
	
	/** \brief Game skin at the given index. */
	igdeGDSkin *GetSkinAt( int index ) const;
	
	/** \brief Game skin with path or NULL if absent. */
	igdeGDSkin *GetSkinWithPath( const char *path ) const;
	
	/** \brief Game skin with name or NULL if absent. */
	igdeGDSkin *GetSkinWithName( const char *name ) const;
	
	/** \brief Add game skin. */
	void AddSkin( igdeGDSkin *skin );
	
	/** \brief Remove skin. */
	void RemoveSkin( igdeGDSkin *skin );
	
	/** \brief Remove all skins. */
	void RemoveAllSkins();
	
	/** \brief Path of default skin. */
	inline const decString &GetDefaultSkinPath() const{ return pDefaultSkinPath; }
	
	/** \brief Set path of default skin. */
	void SetDefaultSkinPath( const char *defaultSkinPath );
	
	/** \brief Set of path to use to auto-find skins. */
	inline decStringSet &GetAutoFindPath(){ return pAutoFindPath; }
	inline const decStringSet &GetAutoFindPath() const{ return pAutoFindPath; }
	
	/** \brief Visit skins matching the given category. */
	void VisitSkinsMatchingCategory( igdeGDVisitor &visitor, const igdeGDCategory *category ) const;
	
	/** \brief Visit skins matching filter. */
	void VisitMatchingFilter( igdeGDVisitor &visitor, const decString &filter ) const;
	
	/**
	 * \brief Updates skin manager using another skin manager.
	 * 
	 * Adds copies of skins in the given skin manager . If skin exists it is replaced.
	 */
	void UpdateWith( const igdeGDSkinManager &skinManager );
	
	/**
	 * \brief Update skin manager with found skins.
	 */
	void UpdateWithFound( const igdeGDSkinManager &skinManager );
	
	/** \brief Find skins and add them. */
	void FindAndAddSkins( deVirtualFileSystem &vfs, const decPath &directory,
		const char *pattern = "*.deskin", bool recursive = true );
	/*@}*/
};

#endif
