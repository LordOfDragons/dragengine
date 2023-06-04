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

#ifndef _IGDEGDCATEGORY_H_
#define _IGDEGDCATEGORY_H_

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringSet.h>


class decPath;


/**
 * \brief Category.
 * 
 * Cathegories are used for sorting objects into a hierarchical list.
 * Every category contains a name unique in the scope of the parent
 * category and an optional list of children cathegories.
 */
class DE_DLL_EXPORT igdeGDCategory : public deObject{
private:
	decString pName;
	decString pDescription;
	decObjectOrderedSet pCategories;
	decStringSet pAutoCategorizePattern;
	bool pHidden;
	
	igdeGDCategory *pParent;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create category. */
	igdeGDCategory( const char *name );
	
	
	
protected:
	/** \brief Clean up category. */
	virtual ~igdeGDCategory();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Description. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description. */
	void SetDescription( const char *description );
	
	/** \brief Parent category or NULL if this is a top category. */
	inline igdeGDCategory *GetParent() const{ return pParent; }
	
	/** \brief Set parent category or NULL if this is a top category. */
	void SetParent( igdeGDCategory *category );
	
	/** \brief Patterns to match to add. */
	inline const decStringSet &GetAutoCategorizePattern() const{ return pAutoCategorizePattern; }
	
	/** \brief Set patterns to match to add. */
	void SetAutoCategorizePattern( const decStringSet &patternList );
	
	/** \brief Get category matching path using auto categorize. */
	igdeGDCategory *AutoCategorize( const decString &path ) const;
	
	/** \brief Hidden in browser. */
	inline bool GetHidden() const{ return pHidden; }
	
	/** \brief Set if hidden in browser. */
	void SetHidden( bool hidden );
	
	
	
	/**
	 * \brief Get full path.
	 * 
	 * Only the category names of all categories with a parent are added to the path.
	 * Hence the root category is not included in the path.
	 */
	void GetFullPath( decPath &path ) const;
	
	/** \brief Full path string. */
	decString GetFullPathString() const;
	/*@}*/
	
	
	
	/** \name Children Categories */
	/*@{*/
	/** \brief Count of categories. */
	int GetCategoryCount() const;
	
	/** \brief Category is present. */
	bool HasCategory( igdeGDCategory *category ) const;
	
	/** \brief Named category is present. */
	bool HasCategoryNamed( const char *name ) const;
	
	/** \brief Index of category or -1 if absent. */
	int IndexOfCategory( igdeGDCategory *category ) const;
	
	/** \brief Index of named category or -1 if absent. */
	int IndexOfCategoryNamed( const char *name ) const;
	
	/** \brief Category at index. */
	igdeGDCategory *GetCategoryAt( int index ) const;
	
	/** \brief Named category or NULL if absent. */
	igdeGDCategory *GetCategoryNamed( const char *name ) const;
	
	/** \brief Category by path or NULL if absent. */
	igdeGDCategory *GetCategoryWithPath( const decPath &path ) const;
	
	/** \brief Add category. */
	void AddCategory( igdeGDCategory *category );
	
	/** \brief Remove category. */
	void RemoveCategory( igdeGDCategory *category );
	
	/** \brief Remove all categories. */
	void RemoveAllCategories();
	
	/**
	 * \brief Update category using another category.
	 * 
	 * Adds copies of categories in the given category. If the category
	 * exists already only the description is updated. Children Categories
	 * are added to already existing categories not shadowing them.
	 */
	void UpdateWith( const igdeGDCategory &category );
	/*@}*/
	
	
	
private:
	void pAutoCategorize( const decString &path, int &longestMatch, igdeGDCategory *&category ) const;
};

#endif
