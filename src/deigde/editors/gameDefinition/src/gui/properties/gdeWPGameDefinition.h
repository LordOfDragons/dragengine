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
