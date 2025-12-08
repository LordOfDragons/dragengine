/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _IGDEGDSKINMANAGER_H_
#define _IGDEGDSKINMANAGER_H_

#include "../igdeGDCategory.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringList.h>

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
	igdeGDCategory::Ref pCategories;
	decString pDefaultSkinPath;
	decStringList pAutoFindPath;
	
	
	
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
	inline const igdeGDCategory::Ref &GetCategories() const{ return pCategories; }
	
	/** \brief Number of game skins. */
	int GetSkinCount() const;
	
	/** \brief Skin is present. */
	bool HasSkin(igdeGDSkin *skin) const;
	
	/** \brief Skin with path is present. */
	bool HasSkinWithPath(const char *path) const;
	
	/** \brief Named skin is present. */
	bool HasSkinWithName(const char *name) const;
	
	/** \brief Skin with name or path is present. */
	bool HasSkinWithPathOrName(const char *path, const char *name) const;
	
	/** \brief Index of skin or -1 if absent. */
	int IndexOfSkin(igdeGDSkin *skin) const;
	
	/** \brief Index of skin with path or -1 if absent. */
	
	int IndexOfSkinWithPath(const char *path) const;
	
	/** \brief Index of skin with name or -1 if absent. */
	int IndexOfSkinWithName(const char *name) const;
	
	/** \brief Index of skin with name or -1 if absent. */
	int IndexOfSkinWithPathOrName(const char *path, const char *name) const;
	
	/** \brief Game skin at the given index. */
	igdeGDSkin *GetSkinAt(int index) const;
	
	/** \brief Game skin with path or NULL if absent. */
	igdeGDSkin *GetSkinWithPath(const char *path) const;
	
	/** \brief Game skin with name or NULL if absent. */
	igdeGDSkin *GetSkinWithName(const char *name) const;
	
	/** \brief Add game skin. */
	void AddSkin(igdeGDSkin *skin);
	
	/** \brief Remove skin. */
	void RemoveSkin(igdeGDSkin *skin);
	
	/** \brief Remove all skins. */
	void RemoveAllSkins();
	
	/** \brief Path of default skin. */
	inline const decString &GetDefaultSkinPath() const{ return pDefaultSkinPath; }
	
	/** \brief Set path of default skin. */
	void SetDefaultSkinPath(const char *defaultSkinPath);
	
	/** \brief Set of path to use to auto-find skins. */
	inline decStringList &GetAutoFindPath(){ return pAutoFindPath; }
	inline const decStringList &GetAutoFindPath() const{ return pAutoFindPath; }
	
	/** \brief Visit skins matching the given category. */
	void VisitSkinsMatchingCategory(igdeGDVisitor &visitor, const igdeGDCategory *category) const;
	
	/** \brief Visit skins matching filter. */
	void VisitMatchingFilter(igdeGDVisitor &visitor, const decString &filter) const;
	
	/**
	 * \brief Updates skin manager using another skin manager.
	 * 
	 * Adds copies of skins in the given skin manager . If skin exists it is replaced.
	 */
	void UpdateWith(const igdeGDSkinManager &skinManager);
	
	/**
	 * \brief Update skin manager with found skins.
	 */
	void UpdateWithFound(const igdeGDSkinManager &skinManager);
	
	/** \brief Find skins and add them. */
	void FindAndAddSkins(deVirtualFileSystem &vfs, const decPath &directory,
		const char *pattern = "*.deskin", bool recursive = true);
	/*@}*/
};

#endif
