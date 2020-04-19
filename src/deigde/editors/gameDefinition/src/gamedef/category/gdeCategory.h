/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#ifndef _GDECATEGORY_H_
#define _GDECATEGORY_H_

#include "gdeCategoryList.h"

#include <dragengine/deObject.h>
#include <dragengine/common/string/decStringSet.h>



/**
 * \brief Category.
 */
class gdeCategory : public deObject{
private:
	decString pName;
	decString pDescription;
	gdeCategoryList pCategories;
	gdeCategory *pParent;
	decStringSet pAutoCategorizePattern;
	bool pHidden;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create category. */
	gdeCategory();
	
	/** \brief Create category. */
	gdeCategory( const char *name );
	
	/** \brief Create copy of category. */
	gdeCategory( const gdeCategory &category );
	
	/** \brief Clean up category. */
	virtual ~gdeCategory();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName( const char *name );
	
	/** \brief Description. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description. */
	void SetDescription( const char *description );
	
	/** \brief Parent category or \em NULL if top level. */
	inline gdeCategory *GetParent() const{ return pParent; }
	
	/** \brief Set parent category or \em NULL if top level. */
	void SetParent( gdeCategory *parent );
	
	/** \brief Get path. */
	decString GetPath() const;
	
	/** \brief List of patterns to match to add. */
	inline const decStringSet &GetAutoCategorizePattern() const{ return pAutoCategorizePattern; }
	
	/** \brief Set list of patterns to match to add. */
	void SetAutoCategorizePattern( const decStringSet &patternList );
	
	/** \brief Hidden in browser. */
	inline bool GetHidden() const{ return pHidden; }
	
	/** \brief Set if hidden in browser. */
	void SetHidden( bool hidden );
	
	
	
	/** \brief Sub categories. */
	inline const gdeCategoryList &GetCategories() const{ return pCategories; }
	
	/** \brief Add category. */
	void AddCategory( gdeCategory *category );
	
	/** \brief Remove category. */
	void RemoveCategory( gdeCategory *category );
	
	/** \brief Remove all categories. */
	void RemoveAllCategories();
	
	
	
	/** \brief Set from another property. */
	gdeCategory &operator=( const gdeCategory &category );
	/*@}*/
};

#endif
