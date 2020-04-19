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

#ifndef _GDEWPGAMEDEFINITION_H_
#define _GDEWPGAMEDEFINITION_H_

#include <deigde/gui/igdeWidgetReference.h>
#include <deigde/gui/igdeTextAreaReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/event/igdeActionReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class gdeGameDefinition;
class gdeWindowProperties;
class gdeWindowMain;
class gdeWPGameDefinitionListener;
class gdeWPPropertyList;
class gdeProperty;



/**
 * \brief Property window game definition panel.
 */
class gdeWPGameDefinition : public igdeContainerScroll{
private:
	gdeWindowProperties &pWindowProperties;
	gdeGameDefinition *pGameDefinition;
	gdeWPGameDefinitionListener *pListener;
	
	igdeActionReference pActionBasePath;
	
	igdeTextFieldReference pEditID;
	igdeTextAreaReference pEditDescription;
	igdeTextFieldReference pEditBasePath;
	igdeTextFieldReference pEditVFSPath;
	igdeTextFieldReference pEditScriptModule;
	igdeTextFieldReference pEditUseBasePath;
	igdeCheckBoxReference pChkIsProjectGameDef;
	
// 	igdeWidgetReference pEditBaseGameDefIDs;
	
	igdeWidgetReference pEditWorldProperties;
	igdeWidgetReference pEditDecalProperties;
	
	igdeWidgetReference pEditAutoFindPathObjectClasses;
	igdeWidgetReference pEditAutoFindPathSkins;
	igdeWidgetReference pEditAutoFindPathSkies;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create property panel. */
	gdeWPGameDefinition( gdeWindowProperties &windowProperties );
	
protected:
	/** \brief Clean up property panel. */
	virtual ~gdeWPGameDefinition();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Properties window. */
	inline gdeWindowProperties &GetWindowProperties() const{ return pWindowProperties; }
	
	/** \brief Game definition or \em NULL. */
	inline gdeGameDefinition *GetGameDefinition() const{ return pGameDefinition; }
	
	/** \brief Set game definition or \em NULL. */
	void SetGameDefinition( gdeGameDefinition *gameDefinition );
	
	/** \brief Active world property. */
	gdeProperty *GetActiveWorldProperty() const;
	
	/** \brief Active decal property. */
	gdeProperty *GetActiveDecalProperty() const;
	
	
	
	/** \brief Update world. */
	void UpdateWorld();
	
	
	
	/** \brief Update world properties. */
	void UpdateWorldProperties();
	
	/** \brief Update world property. */
	void UpdateWorldProperty();
	
	/** \brief Update world property custom pattern list. */
	void UpdateWorldPropertyCustomPatternList();
	
	/** \brief Update world property custom pattern. */
	void UpdateWorldPropertyCustomPattern();
	
	
	
	/** \brief Update auto find path object classes. */
	void UpdateAutoFindPathObjectClasses();
	
	/** \brief Update auto find path skins. */
	void UpdateAutoFindPathSkins();
	
	/** \brief Update auto find path skies. */
	void UpdateAutoFindPathSkies();
	
	
	
	/** \brief Update decal properties. */
	void UpdateDecalProperties();
	
	/** \brief Update decal property. */
	void UpdateDecalProperty();
	
	/** \brief Update decal property custom pattern list. */
	void UpdateDecalPropertyCustomPatternList();
	
	/** \brief Update decal property custom pattern. */
	void UpdateDecalPropertyCustomPattern();
	/*@}*/
};

#endif
