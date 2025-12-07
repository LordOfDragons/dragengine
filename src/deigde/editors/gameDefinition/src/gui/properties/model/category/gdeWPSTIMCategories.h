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

#ifndef _GDEWPSTIMCATEGORIES_H_
#define _GDEWPSTIMCATEGORIES_H_

#include "../gdeWPSTreeItemModel.h"

class gdeWPSTIMCategoriesObjectClass;
class gdeWPSTIMCategoriesParticleEmitter;
class gdeWPSTIMCategoriesSkin;
class gdeWPSTIMCategoriesSky;


/**
 * \brief Group containing all category groups.
 */
class gdeWPSTIMCategories : public gdeWPSTreeItemModel{
private:
	gdeWPSTIMCategoriesObjectClass *pObjectClass;
	gdeWPSTIMCategoriesSkin *pSkin;
	gdeWPSTIMCategoriesSky *pSky;
	gdeWPSTIMCategoriesParticleEmitter *pParticleEmitter;
	
	
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new tree item model. */
	gdeWPSTIMCategories(gdeWPSTreeModel &tree);
	
protected:
	/** \brief Clean up tree item model. */
	virtual ~gdeWPSTIMCategories();
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Type object class. */
	inline gdeWPSTIMCategoriesObjectClass &GetObjectClass() const{ return *pObjectClass; }
	
	/** \brief Type skin. */
	inline gdeWPSTIMCategoriesSkin &GetSkin() const{ return *pSkin; }
	
	/** \brief Type sky. */
	inline gdeWPSTIMCategoriesSky &GetSky() const{ return *pSky; }
	
	/** \brief Type particle emitter. */
	inline gdeWPSTIMCategoriesParticleEmitter &GetParticleEmitter() const{ return *pParticleEmitter; }
	
	
	
	/** \brief Added to tree. */
	virtual void OnAddedToTree();
	
	/** \brief User requests context menu for selected item. */
	virtual void OnContextMenu(igdeMenuCascade &contextMenu);
	/*@}*/
};

#endif
