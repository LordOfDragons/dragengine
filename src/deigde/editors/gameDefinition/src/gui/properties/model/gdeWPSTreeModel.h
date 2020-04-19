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

#ifndef _GDEWPSTREEMODEL_H_
#define _GDEWPSTREEMODEL_H_

#include <deigde/gui/igdeTreeList.h>

class gdeGameDefinition;
class gdeWPSTIMCategories;
class gdeWPSTIMObjectClasses;
class gdeWPSTIMParticleEmitters;
class gdeWPSTIMSkins;
class gdeWPSTIMSkies;
class gdeWPSTreeModelListener;
class gdeWindowMain;

class igdeMenuCascade;


/**
 * \brief Tree model.
 * 
 * Tree model is responsible to update the visual state and
 * content of assigned tree list.
 */
class gdeWPSTreeModel{
private:
	igdeTreeList &pTreeList;
	gdeWindowMain &pWindowMain;
	gdeGameDefinition *pGameDefinition;
	gdeWPSTreeModelListener *pListener;
	
	gdeWPSTIMCategories *pCategories;
	gdeWPSTIMObjectClasses *pObjectClasses;
	gdeWPSTIMParticleEmitters *pParticleEmitters;
	gdeWPSTIMSkins *pSkins;
	gdeWPSTIMSkies *pSkies;
	
	bool pIgnoreSelectionChange;
	
	
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new tree model. */
	gdeWPSTreeModel( igdeTreeList &treeList, gdeWindowMain &windowMain, gdeGameDefinition *gameDefinition );
	
	/** \brief Clean up tree model. */
	~gdeWPSTreeModel();
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Window main. */
	inline gdeWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Game definition. */
	inline gdeGameDefinition *GetGameDefinition() const{ return pGameDefinition; }
	
	/** \brief Categories model. */
	inline gdeWPSTIMCategories &GetCategories() const{ return *pCategories; }
	
	/** \brief Object classes model. */
	inline gdeWPSTIMObjectClasses &GetObjectClasses() const{ return *pObjectClasses; }
	
	/** \brief Particle emitters model. */
	inline gdeWPSTIMParticleEmitters &GetParticleEmitters() const{ return *pParticleEmitters; }
	
	/** \brief Skins model. */
	inline gdeWPSTIMSkins &GetSkins() const{ return *pSkins; }
	
	/** \brief Skies model. */
	inline gdeWPSTIMSkies &GetSkies() const{ return *pSkies; }
	
	
	
	/** \brief Tree list. */
	inline igdeTreeList &GetTreeList() const{ return pTreeList; }
	
	/** \brief Clear current item. */
	void ClearCurrentItem();
	
	/** \brief Set current item from game definition selected object. */
	void SetCurrentItemFromGameDef();
	
	
	
	/** \brief User requests context menu for selected item. */
	void OnContextMenu( igdeMenuCascade &contextMenu );
	
	/** \brief Select object mest matching name. */
	void SelectBestMatching( const char *string );
	
	
	
	/** \brief Ignore selection change in tree list. */
	inline bool GetIgnoreSelectionChange() const{ return pIgnoreSelectionChange; }
	
	/** \brief Set ignore selection change in tree list. */
	void SetIgnoreSelectionChange( bool ignore );
	
	/** \brief Helper for ignore selection. */
	class HelperIgnoreSelection{
		gdeWPSTreeModel &pTree;
	public:
		HelperIgnoreSelection( gdeWPSTreeModel &tree );
		~HelperIgnoreSelection();
	};
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
