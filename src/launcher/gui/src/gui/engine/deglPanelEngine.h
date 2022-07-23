/* 
 * Drag[en]gine GUI Launcher
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

#ifndef _DEGLPANELENGINE_H_
#define _DEGLPANELENGINE_H_

#include "../foxtoolkit.h"

class deglWindowMain;
class delEngineModule;



/**
 * Engine Panel.
 */
class deglPanelEngine : public FXVerticalFrame{
	FXDECLARE( deglPanelEngine )
protected:
	deglPanelEngine();
	
public:
	/** Icon list extended to send header click to enable sorting. */
	class ExtIconList : public FXIconList{
		FXDECLARE( ExtIconList )
	protected:
		ExtIconList();
		
	public:
		enum eFoxSelectors{
			SEL_HEADER_CLICKED = SEL_LAST
		};
		
		ExtIconList( FXComposite *p, FXObject *tgt = nullptr, FXSelector sel = 0,
			FXuint opts = ICONLIST_NORMAL, FXint x = 0, FXint y = 0,
			FXint w = 0, FXint h = 0 );
		
		~ExtIconList();
		
		long onHeaderClicked( FXObject *sender, FXSelector selector, void *data );
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
	deglPanelEngine( deglWindowMain *windowMain, FXComposite *container );
	
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
	static FXint SortModulesByNameAsc( const FXIconItem *item1, const FXIconItem *item2 );
	static FXint SortModulesByNameDesc( const FXIconItem *item1, const FXIconItem *item2 );
	
	/** Sorts module list by the type. */
	static FXint SortModulesByTypeAsc( const FXIconItem *item1, const FXIconItem *item2 );
	static FXint SortModulesByTypeDesc( const FXIconItem *item1, const FXIconItem *item2 );
	
	/** Sorts module list by the version. */
	static FXint SortModulesByVersionAsc( const FXIconItem *item1, const FXIconItem *item2 );
	static FXint SortModulesByVersionDesc( const FXIconItem *item1, const FXIconItem *item2 );
	
	/** Sorts module list by the status. */
	static FXint SortModulesByStatusAsc( const FXIconItem *item1, const FXIconItem *item2 );
	static FXint SortModulesByStatusDesc( const FXIconItem *item1, const FXIconItem *item2 );
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onListModulesChanged( FXObject *sender, FXSelector selector, void *data );
	long onListModulesRDown( FXObject *sender, FXSelector selector, void *data );
	long onListModulesRUp( FXObject *sender, FXSelector selector, void *data );
	long onListModulesDblClick( FXObject *sender, FXSelector selector, void *data );
	long onListModulesHeaderClicked( FXObject *sender, FXSelector selector, void *data );
	
	long onPUModuleProps( FXObject *sender, FXSelector selector, void *data );
	/*@}*/
};

#endif
