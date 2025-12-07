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

public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeGDCategory> Ref;
	
	
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
	igdeGDCategory(const char *name);
	
	
	
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
	void SetDescription(const char *description);
	
	/** \brief Parent category or NULL if this is a top category. */
	inline igdeGDCategory *GetParent() const{ return pParent; }
	
	/** \brief Set parent category or NULL if this is a top category. */
	void SetParent(igdeGDCategory *category);
	
	/** \brief Patterns to match to add. */
	inline const decStringSet &GetAutoCategorizePattern() const{ return pAutoCategorizePattern; }
	
	/** \brief Set patterns to match to add. */
	void SetAutoCategorizePattern(const decStringSet &patternList);
	
	/** \brief Get category matching path using auto categorize. */
	igdeGDCategory *AutoCategorize(const decString &path) const;
	
	/** \brief Hidden in browser. */
	inline bool GetHidden() const{ return pHidden; }
	
	/** \brief Set if hidden in browser. */
	void SetHidden(bool hidden);
	
	
	
	/**
	 * \brief Get full path.
	 * 
	 * Only the category names of all categories with a parent are added to the path.
	 * Hence the root category is not included in the path.
	 */
	void GetFullPath(decPath &path) const;
	
	/** \brief Full path string. */
	decString GetFullPathString() const;
	/*@}*/
	
	
	
	/** \name Children Categories */
	/*@{*/
	/** \brief Count of categories. */
	int GetCategoryCount() const;
	
	/** \brief Category is present. */
	bool HasCategory(igdeGDCategory *category) const;
	
	/** \brief Named category is present. */
	bool HasCategoryNamed(const char *name) const;
	
	/** \brief Index of category or -1 if absent. */
	int IndexOfCategory(igdeGDCategory *category) const;
	
	/** \brief Index of named category or -1 if absent. */
	int IndexOfCategoryNamed(const char *name) const;
	
	/** \brief Category at index. */
	igdeGDCategory *GetCategoryAt(int index) const;
	
	/** \brief Named category or NULL if absent. */
	igdeGDCategory *GetCategoryNamed(const char *name) const;
	
	/** \brief Category by path or NULL if absent. */
	igdeGDCategory *GetCategoryWithPath(const decPath &path) const;
	
	/** \brief Add category. */
	void AddCategory(igdeGDCategory *category);
	
	/** \brief Remove category. */
	void RemoveCategory(igdeGDCategory *category);
	
	/** \brief Remove all categories. */
	void RemoveAllCategories();
	
	/**
	 * \brief Update category using another category.
	 * 
	 * Adds copies of categories in the given category. If the category
	 * exists already only the description is updated. Children Categories
	 * are added to already existing categories not shadowing them.
	 */
	void UpdateWith(const igdeGDCategory &category);
	/*@}*/
	
	
	
private:
	void pAutoCategorize(const decString &path, int &longestMatch, igdeGDCategory *&category) const;
};

#endif
