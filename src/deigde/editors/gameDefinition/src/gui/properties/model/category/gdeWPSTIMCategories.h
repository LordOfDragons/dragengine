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
	gdeWPSTIMCategories( gdeWPSTreeModel &tree );
	
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
	virtual void OnContextMenu( igdeMenuCascade &contextMenu );
	/*@}*/
};

#endif
