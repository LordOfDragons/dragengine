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

#ifndef _GDECATEGORY_H_
#define _GDECATEGORY_H_

#include "gdeCategoryList.h"

#include <dragengine/deObject.h>
#include <dragengine/common/string/decStringSet.h>


/**
 * \brief Category.
 */
class gdeCategory : public deObject{
public:
	typedef deTObjectReference<gdeCategory> Ref;
	
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
	explicit gdeCategory(const char *name);
	
	/** \brief Create copy of category. */
	gdeCategory(const gdeCategory &category);
	
	/** \brief Clean up category. */
	virtual ~gdeCategory();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName(const char *name);
	
	/** \brief Description. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description. */
	void SetDescription(const char *description);
	
	/** \brief Parent category or \em nullptr if top level. */
	inline gdeCategory *GetParent() const{ return pParent; }
	
	/** \brief Set parent category or \em nullptr if top level. */
	void SetParent(gdeCategory *parent);
	
	/** \brief Get path. */
	decString GetPath() const;
	
	/** \brief List of patterns to match to add. */
	inline const decStringSet &GetAutoCategorizePattern() const{ return pAutoCategorizePattern; }
	
	/** \brief Set list of patterns to match to add. */
	void SetAutoCategorizePattern(const decStringSet &patternList);
	
	/** \brief Hidden in browser. */
	inline bool GetHidden() const{ return pHidden; }
	
	/** \brief Set if hidden in browser. */
	void SetHidden(bool hidden);
	
	
	
	/** \brief Sub categories. */
	inline const gdeCategoryList &GetCategories() const{ return pCategories; }
	
	/** \brief Add category. */
	void AddCategory(gdeCategory *category);
	
	/** \brief Remove category. */
	void RemoveCategory(gdeCategory *category);
	
	/** \brief Remove all categories. */
	void RemoveAllCategories();
	
	
	
	/** \brief Set from another property. */
	gdeCategory &operator=(const gdeCategory &category);
	/*@}*/
};

#endif
