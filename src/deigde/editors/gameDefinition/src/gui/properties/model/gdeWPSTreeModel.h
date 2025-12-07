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

#ifndef _GDEWPSTREEMODEL_H_
#define _GDEWPSTREEMODEL_H_

#include <deigde/gui/igdeTreeList.h>
#include "gdeGameDefinition.h"

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
	gdeGameDefinition::Ref pGameDefinition;
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
	gdeWPSTreeModel(igdeTreeList &treeList, gdeWindowMain &windowMain, gdeGameDefinition *gameDefinition);
	
	/** \brief Clean up tree model. */
	~gdeWPSTreeModel();
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Window main. */
	inline gdeWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Game definition. */
	inline const gdeGameDefinition::Ref &GetGameDefinition() const{ return pGameDefinition; }
	
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
	void OnContextMenu(igdeMenuCascade &contextMenu);
	
	/** \brief Select object mest matching name. */
	void SelectBestMatching(const char *string);
	
	
	
	/** \brief Ignore selection change in tree list. */
	inline bool GetIgnoreSelectionChange() const{ return pIgnoreSelectionChange; }
	
	/** \brief Set ignore selection change in tree list. */
	void SetIgnoreSelectionChange(bool ignore);
	
	/** \brief Helper for ignore selection. */
	class HelperIgnoreSelection{
		gdeWPSTreeModel &pTree;
	public:
		HelperIgnoreSelection(gdeWPSTreeModel &tree);
		~HelperIgnoreSelection();
	};
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
