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

#ifndef _GDEWPSTIMCATEGORIESOBJECTCLASS_H_
#define _GDEWPSTIMCATEGORIESOBJECTCLASS_H_

#include "../gdeWPSTreeItemModel.h"

class gdeWPSTIMCategoryObjectClass;
class gdeCategory;


/**
 * \brief Object class categories.
 */
class gdeWPSTIMCategoriesObjectClass : public gdeWPSTreeItemModel{
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new tree item model. */
	gdeWPSTIMCategoriesObjectClass( gdeWPSTreeModel &tree );
	
protected:
	/** \brief Clean up tree item model. */
	virtual ~gdeWPSTIMCategoriesObjectClass();
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Model with category or \em NULL. */
	gdeWPSTIMCategoryObjectClass *GetChildWith( gdeCategory* category, bool deep ) const;
	
	/** \brief Categories changed. */
	void CategoriesChanged();
	
	
	
	/** \brief Added to tree. */
	virtual void OnAddedToTree();
	
	/** \brief User requests context menu for selected item. */
	virtual void OnContextMenu( igdeMenuCascade &contextMenu );
	
	/** \brief Select object mest matching name. */
	virtual void SelectBestMatching( const char *string );
	/*@}*/
};

#endif
