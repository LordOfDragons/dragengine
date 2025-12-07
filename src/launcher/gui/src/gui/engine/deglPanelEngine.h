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

#ifndef _DEGLPANELENGINE_H_
#define _DEGLPANELENGINE_H_

#include "../foxtoolkit.h"

class deglWindowMain;
class delEngineModule;



/**
 * Engine Panel.
 */
class deglPanelEngine : public FXVerticalFrame{
	FXDECLARE(deglPanelEngine)
protected:
	deglPanelEngine();
	
public:
	/** Icon list extended to send header click to enable sorting. */
	class ExtIconList : public FXIconList{
		FXDECLARE(ExtIconList)
	protected:
		ExtIconList();
		
	public:
		enum eFoxSelectors{
			SEL_HEADER_CLICKED = SEL_LAST
		};
		
		ExtIconList(FXComposite *p, FXObject *tgt = nullptr, FXSelector sel = 0,
			FXuint opts = ICONLIST_NORMAL, FXint x = 0, FXint y = 0,
			FXint w = 0, FXint h = 0);
		
		~ExtIconList();
		
		long onHeaderClicked(FXObject *sender, FXSelector selector, void *data);
	};
	
	
	
public:
	enum eFoxIDs{
		ID_SELF = FXVerticalFrame::ID_LAST,
		ID_LIST_MODULES,
		ID_PU_MODULE_PROPS,
		ID_LAST
	};
	
	enum eListModuleSorting{
		elmsNameAsc,
		elmsNameDesc,
		elmsTypeAsc,
		elmsTypeDesc,
		elmsVersionAsc,
		elmsVersionDesc,
		elmsStatusAsc,
		elmsStatusDesc,
	};
	
	
	
private:
	deglWindowMain *pWindowMain;
	
	FXIconList *pListModules;
	eListModuleSorting pSortListModules;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create panel. */
	deglPanelEngine(deglWindowMain *windowMain, FXComposite *container);
	
	/** Clean up panel. */
	virtual ~deglPanelEngine();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Main window. */
	inline deglWindowMain *GetWindowMain() const{ return pWindowMain; }
	
	/** Selected module or nullptr. */
	delEngineModule *GetSelectedModule() const;
	
	/** Update module list. */
	void UpdateModuleList();
	
	/** Sorts module list by the name. */
	static FXint SortModulesByNameAsc(const FXIconItem *item1, const FXIconItem *item2);
	static FXint SortModulesByNameDesc(const FXIconItem *item1, const FXIconItem *item2);
	
	/** Sorts module list by the type. */
	static FXint SortModulesByTypeAsc(const FXIconItem *item1, const FXIconItem *item2);
	static FXint SortModulesByTypeDesc(const FXIconItem *item1, const FXIconItem *item2);
	
	/** Sorts module list by the version. */
	static FXint SortModulesByVersionAsc(const FXIconItem *item1, const FXIconItem *item2);
	static FXint SortModulesByVersionDesc(const FXIconItem *item1, const FXIconItem *item2);
	
	/** Sorts module list by the status. */
	static FXint SortModulesByStatusAsc(const FXIconItem *item1, const FXIconItem *item2);
	static FXint SortModulesByStatusDesc(const FXIconItem *item1, const FXIconItem *item2);
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onListModulesChanged(FXObject *sender, FXSelector selector, void *data);
	long onListModulesRDown(FXObject *sender, FXSelector selector, void *data);
	long onListModulesRUp(FXObject *sender, FXSelector selector, void *data);
	long onListModulesDblClick(FXObject *sender, FXSelector selector, void *data);
	long onListModulesHeaderClicked(FXObject *sender, FXSelector selector, void *data);
	
	long onPUModuleProps(FXObject *sender, FXSelector selector, void *data);
	/*@}*/
};

#endif
