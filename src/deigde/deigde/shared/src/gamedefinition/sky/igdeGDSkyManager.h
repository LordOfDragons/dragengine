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

#ifndef _IGDEGDSKYMANAGER_H_
#define _IGDEGDSKYMANAGER_H_

#include "igdeGDSkyList.h"
#include "../igdeGDCategory.h"

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
	igdeGDCategory::Ref pCategories;
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
