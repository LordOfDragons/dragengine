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

#ifndef _GDEWPSTREEITEMMODEL_H_
#define _GDEWPSTREEITEMMODEL_H_

#include <deigde/gui/model/igdeTreeItem.h>

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>

class gdeGameDefinition;
class gdeWPSTreeItem;
class gdeWPSTreeModel;
class gdeWindowMain;
class igdeMenuCascade;


/**
 * \brief Base class for tree item models.
 * 
 * Tree item models are responsible to update the visual state and content
 * of an assigned tree item.
 */
class gdeWPSTreeItemModel : public igdeTreeItem{
public:
	/** \brief Tree item types. */
	enum eTypes{
		etCategories,
		etCategoriesObjectClass,
		etCategoriesSkin,
		etCategoriesSky,
		etCategoriesParticleEmitter,
		etCategoryObjectClass,
		etCategorySkin,
		etCategorySky,
		etCategoryParticleEmitter,
		etObjectClasses,
		etObjectClass,
		etObjectClassBillboard,
		etObjectClassCamera,
		etObjectClassComponent,
		etObjectClassEnvMapProbe,
		etObjectClassLight,
		etObjectClassNavigationBlocker,
		etObjectClassNavigationSpace,
		etObjectClassParticleEmitter,
		etObjectClassForceField,
		etObjectClassSnapPoint,
		etObjectClassSpeaker,
		etObjectClassWorld,
		etParticleEmitters,
		etParticleEmitter,
		etSkins,
		etSkin,
		etSkies,
		etSky
	};
	
	
	
private:
	gdeWPSTreeModel &pTree;
	const eTypes pType;
	
	
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new tree item model. */
	gdeWPSTreeItemModel(gdeWPSTreeModel &tree, eTypes type);
	
protected:
	/** \brief Clean up tree item model. */
	virtual ~gdeWPSTreeItemModel();
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Tree. */
	inline gdeWPSTreeModel &GetTree() const{return pTree;}
	
	/** \brief Window selection. */
	gdeWindowMain &GetWindowMain() const;
	
	/** \brief Game definition. */
	gdeGameDefinition &GetGameDefinition() const;
	
	/** \brief Tree item type. */
	inline eTypes GetType() const{return pType;}
	
	
	
	/** \brief Sort children. */
	void SortChildren();
	
	/** \brief Sort parent item. */
	void ParentSortItems();
	
	/** \brief Set item as current item and make it visible. */
	void SetAsCurrentItem();
	
	/** \brief Append model of type gdeWPSTreeItemModel and call OnAddedToTree(). */
	void AppendModel(igdeTreeItem *item);
	
	/** \brief Remove model of type gdeWPSTreeItemModel. */
	void RemoveModel(igdeTreeItem *item);
	
	/** \brief Remove all models of type gdeWPSTreeItemModel. */
	void RemoveAllModels();
	
	/** \brief Item changed. */
	void ItemChanged();
	
	
	
	/** \brief Added to tree. */
	virtual void OnAddedToTree();
	
	/**
	 * \brief Compare this item with another for sorting.
	 * 
	 * Default implementation returns 0 to keep same order.
	 */
	virtual int Compare(const gdeWPSTreeItemModel &item) const;
	
	/** \brief User selected item. */
	virtual void OnSelected();
	
	/** \brief User requests context menu for selected item. */
	virtual void OnContextMenu(igdeMenuCascade &contextMenu);
	
	/** \brief Select object mest matching name. */
	virtual void SelectBestMatching(const char *string);
	/*@}*/
};

#endif
 
 
