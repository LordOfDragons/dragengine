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
	gdeWPSTreeItemModel( gdeWPSTreeModel &tree, eTypes type );
	
protected:
	/** \brief Clean up tree item model. */
	virtual ~gdeWPSTreeItemModel();
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Tree. */
	inline gdeWPSTreeModel &GetTree() const{ return pTree; }
	
	/** \brief Window selection. */
	gdeWindowMain &GetWindowMain() const;
	
	/** \brief Game definition. */
	gdeGameDefinition &GetGameDefinition() const;
	
	/** \brief Tree item type. */
	inline eTypes GetType() const{ return pType; }
	
	
	
	/** \brief Sort children. */
	void SortChildren();
	
	/** \brief Sort parent item. */
	void ParentSortItems();
	
	/** \brief Set item as current item and make it visible. */
	void SetAsCurrentItem();
	
	/** \brief Append model of type gdeWPSTreeItemModel and call OnAddedToTree(). */
	void AppendModel( igdeTreeItem *item );
	
	/** \brief Remove model of type gdeWPSTreeItemModel. */
	void RemoveModel( igdeTreeItem *item );
	
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
	virtual int Compare( const gdeWPSTreeItemModel &item ) const;
	
	/** \brief User selected item. */
	virtual void OnSelected();
	
	/** \brief User requests context menu for selected item. */
	virtual void OnContextMenu( igdeMenuCascade &contextMenu );
	
	/** \brief Select object mest matching name. */
	virtual void SelectBestMatching( const char *string );
	/*@}*/
};

#endif
 
 
