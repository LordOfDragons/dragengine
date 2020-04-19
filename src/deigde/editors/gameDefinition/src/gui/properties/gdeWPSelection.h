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

#ifndef _GDEWPSELECTION_H_
#define _GDEWPSELECTION_H_

#include <dragengine/common/collection/decObjectList.h>

#include <deigde/gui/igdeTreeListReference.h>
#include <deigde/gui/igdeSwitcherReference.h>
#include <deigde/gui/igdeWidgetReference.h>
#include <deigde/gui/layout/igdeContainerSplitted.h>

class gdeWPSelectionListener;
class gdeWPSObjectClass;
class gdeWindowProperties;
class gdeGameDefinition;

class gdeWPSTreeModel;


/**
 * \brief Selected object property panel.
 */
class gdeWPSelection : public igdeContainerSplitted{
private:
	gdeWindowProperties &pWindowProperties;
	gdeWPSelectionListener *pListener;
	
	gdeGameDefinition *pGameDefinition;
	
	igdeSwitcherReference pSwitcher;
	igdeTreeListReference pTreeObjects;
	gdeWPSTreeModel *pModelTreeObjects;
	
	igdeWidgetReference pPanelCategory;
	igdeWidgetReference pPanelObjectClass;
	igdeWidgetReference pPanelOCBillboard;
	igdeWidgetReference pPanelOCCamera;
	igdeWidgetReference pPanelOCComponent;
	igdeWidgetReference pPanelOCEnvMapProbe;
	igdeWidgetReference pPanelOCLight;
	igdeWidgetReference pPanelOCNavigationBlocker;
	igdeWidgetReference pPanelOCNavigationSpace;
	igdeWidgetReference pPanelOCParticleEmitter;
	igdeWidgetReference pPanelOCForceField;
	igdeWidgetReference pPanelOCSnapPoint;
	igdeWidgetReference pPanelOCSpeaker;
	igdeWidgetReference pPanelParticleEmitter;
	igdeWidgetReference pPanelSkin;
	igdeWidgetReference pPanelSky;
	
	
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new panel. */
	gdeWPSelection( gdeWindowProperties &windowProperties );
	
protected:
	/** \brief Clean up panel. */
	virtual ~gdeWPSelection();
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Properties window. */
	inline gdeWindowProperties &GetWindowProperties() const{ return pWindowProperties; }
	
	/** \brief Game definition or \em NULL if not present. */
	inline gdeGameDefinition *GetGameDefinition() const{ return pGameDefinition; }
	
	/** \brief Set game definition or \em NULL if not present. */
	void SetGameDefinition( gdeGameDefinition *gameDefinition );
	
	/** \brief Tree model or NULL. */
	inline gdeWPSTreeModel *GetModelTreeObjects() const{ return pModelTreeObjects; }
	
	
	
	/** \brief Selected object changed. */
	void SelectedObjectChanged();
	
	/** \brief Find object. */
	void Find( const char *text );
	/*@}*/
};

#endif
