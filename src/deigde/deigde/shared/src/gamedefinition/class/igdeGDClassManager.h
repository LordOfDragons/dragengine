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

#ifndef _IGDEGDCLASSMANAGER_H_
#define _IGDEGDCLASSMANAGER_H_

#include "../igdeTagManager.h"
#include "../igdeGDCategory.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringList.h>

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
class DE_DLL_EXPORT igdeGDClassManager{
private:
	decObjectOrderedSet pClasses;
	
	igdeGDCategory::Ref pCategories;
	
	igdeTagManager pHideTags;
	igdeTagManager pPartialHideTags;
	decStringList pAutoFindPath;
	
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
	inline const igdeGDCategory::Ref &GetCategories() const{ return pCategories; }
	
	/** \brief Number of game classes. */
	int GetCount() const;
	
	/** \brief Determines if the given class exists. */
	bool Has(igdeGDClass *gdClass) const;
	
	/** \brief Determines if a class with the given name exists. */
	bool HasNamed(const char *name) const;
	
	/** \brief Index of the given class or -1 if not found. */
	int IndexOf(igdeGDClass *gdClass) const;
	
	/** \brief Index of the class with the given name or -1 if not found. */
	int IndexOfNamed(const char *name) const;
	
	/** \brief Game class at the given index. */
	igdeGDClass *GetAt(int index) const;
	
	/** \brief Game class with the given name or nullptr if not found. */
	igdeGDClass *GetNamed(const char *name) const;
	
	/** \brief Adds a new game class. */
	void Add(igdeGDClass *gdClass);
	
	/** \brief Removes the given class. */
	void Remove(igdeGDClass *gdClass);
	
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
	void SetDefaultClassName(const char *defaultClassName);
	
	/** \brief Set of path to use to auto-find objects. */
	inline decStringList &GetAutoFindPath(){ return pAutoFindPath; }
	inline const decStringList &GetAutoFindPath() const{ return pAutoFindPath; }
	
	/** \brief Resolves inherit classes. */
	void ResolveInheritClasses();
	
	/** \brief Visit classes matching the given category. */
	void VisitClassesMatchingCategory(igdeGDVisitor &visitor, const igdeGDCategory *category) const;
	
	/** \brief Visit classes matching filter. */
	void VisitMatchingFilter(igdeGDVisitor &visitor, const decString &filter) const;
	
	/**
	 * \brief Updates the class manager using another class manager.
	 * 
	 * Adds copies of classes in the given class manager . If the class exists already it is
	 * replaced. \ref UpdateTags and \ref ResolveLinks are not called automatically.
	 */
	void UpdateWith(const igdeGDClassManager &classManager);
	
	/**
	 * \brief Update with element classes.
	 * \see igdeGameDefinition::UpdateWithElementClasses().
	 */
	void UpdateWithElementClasses(const igdeGDClassManager &classManager);
	/*@}*/
};

#endif
