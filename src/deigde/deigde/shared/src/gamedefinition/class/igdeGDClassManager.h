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

#ifndef _IGDEGDCLASSMANAGER_H_
#define _IGDEGDCLASSMANAGER_H_

#include "../igdeTagManager.h"
#include "../igdeGDCategoryReference.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringSet.h>

class igdeGDClass;
class igdeGDVisitor;
class deLogger;



/**
 * \brief Game Class Manager.
 * 
 * Manages a list of game classes found in the game directory or elsewhere.
 * Game classes only contain the name of the class file and some classification
 * information useful searching for the right class to use.
 */
class igdeGDClassManager{
private:
	decObjectOrderedSet pClasses;
	
	igdeGDCategoryReference pCategories;
	
	igdeTagManager pHideTags;
	igdeTagManager pPartialHideTags;
	decStringSet pAutoFindPath;
	
	decString pDefaultClassName;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game class manager. */
	igdeGDClassManager();
	
	/** \brief Clean up game class manager. */
	~igdeGDClassManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Top level category object. */
	inline igdeGDCategory *GetCategories() const{ return pCategories; }
	
	/** \brief Number of game classes. */
	int GetCount() const;
	
	/** \brief Determines if the given class exists. */
	bool Has( igdeGDClass *gdClass ) const;
	
	/** \brief Determines if a class with the given name exists. */
	bool HasNamed( const char *name ) const;
	
	/** \brief Index of the given class or -1 if not found. */
	int IndexOf( igdeGDClass *gdClass ) const;
	
	/** \brief Index of the class with the given name or -1 if not found. */
	int IndexOfNamed( const char *name ) const;
	
	/** \brief Game class at the given index. */
	igdeGDClass *GetAt( int index ) const;
	
	/** \brief Game class with the given name or NULL if not found. */
	igdeGDClass *GetNamed( const char *name ) const;
	
	/** \brief Adds a new game class. */
	void Add( igdeGDClass *gdClass );
	
	/** \brief Removes the given class. */
	void Remove( igdeGDClass *gdClass );
	
	/** \brief Removes all classes. */
	void RemoveAll();
	
	/** \brief Hide tags. */
	inline igdeTagManager &GetHideTags(){ return pHideTags; }
	inline const igdeTagManager &GetHideTags() const{ return pHideTags; }
	
	/** \brief Partial hide tags. */
	inline igdeTagManager &GetPartialHideTags(){ return pPartialHideTags; }
	inline const igdeTagManager &GetPartialHideTags() const{ return pPartialHideTags; }
	
	/** \brief Updates the tags. */
	void UpdateTags();
	
	/** \brief Name of the default class. */
	inline const decString &GetDefaultClassName() const{ return pDefaultClassName; }
	
	/** \brief Set name of the default class. */
	void SetDefaultClassName( const char *defaultClassName );
	
	/** \brief Set of path to use to auto-find objects. */
	inline decStringSet &GetAutoFindPath(){ return pAutoFindPath; }
	inline const decStringSet &GetAutoFindPath() const{ return pAutoFindPath; }
	
	/** \brief Resolves inherit classes. */
	void ResolveInheritClasses();
	
	/** \brief Visit classes matching the given category. */
	void VisitClassesMatchingCategory( igdeGDVisitor &visitor, const igdeGDCategory *category ) const;
	
	/** \brief Visit classes matching filter. */
	void VisitMatchingFilter( igdeGDVisitor &visitor, const decString &filter ) const;
	
	/**
	 * \brief Updates the class manager using another class manager.
	 * 
	 * Adds copies of classes in the given class manager . If the class exists already it is
	 * replaced. \ref UpdateTags and \ref ResolveLinks are not called automatically.
	 */
	void UpdateWith( const igdeGDClassManager &classManager );
	
	/**
	 * \brief Update with element classes.
	 * \see igdeGameDefinition::UpdateWithElementClasses().
	 */
	void UpdateWithElementClasses( const igdeGDClassManager &classManager );
	/*@}*/
};

#endif
