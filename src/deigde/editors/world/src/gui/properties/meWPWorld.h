/* 
 * Drag[en]gine IGDE World Editor
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

#ifndef _MEWNDPROPWORLD_H_
#define _MEWNDPROPWORLD_H_

#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/igdeWidgetReference.h>
#include <deigde/gui/composed/igdeEditDVectorReference.h>
#include <deigde/gui/composed/igdeEditVectorReference.h>
#include <deigde/gui/composed/igdeEditPathReference.h>
#include <deigde/gui/composed/igdeEditSliderTextReference.h>
#include <deigde/gui/event/igdeActionReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class meWindowProperties;
class meWPWorldListener;
class meWorld;
class mePathFindTestType;



/**
 * Property window displaying the properties of the world.
 */
class meWPWorld : public igdeContainerScroll{
private:
	meWindowProperties &pWindowProperties;
	meWPWorldListener *pListener;
	meWorld *pWorld;
	
	igdeActionReference pActionPFTTypeAdd;
	igdeActionReference pActionPFTTypeRemove;
	igdeActionReference pActionPFTTypeClear;
	igdeActionReference pActionMusicPlay;
	igdeActionReference pActionMusicPause;
	igdeActionReference pActionMusicStop;
	
	
	igdeWidgetReference pEditProperties;
	
	igdeEditDVectorReference pEditSize;
	igdeEditVectorReference pEditGravity;
	
	igdeEditDVectorReference pEditPFTStartPosition;
	igdeButtonReference pBtnPFTStartPosFromCamera;
	igdeEditDVectorReference pEditPFTGoalPosition;
	igdeButtonReference pBtnPFTGoalPosFromCamera;
	igdeTextFieldReference pEditPFTLayer;
	igdeComboBoxReference pCBPFTSpaceType;
	igdeTextFieldReference pEditPFTBlockingCost;
	igdeCheckBoxReference pChkPFTShowPath;
	igdeComboBoxReference pCBPFTType;
	igdeButtonReference pBtnPFTTypes;
	igdeTextFieldReference pEditPFTTypeNumber;
	igdeTextFieldReference pEditPFTTypeName;
	igdeTextFieldReference pEditPFTTypeFixCost;
	igdeTextFieldReference pEditPFTTypeCPM;
	
	
	igdeEditPathReference pEditMusicPath;
	igdeEditSliderTextReference pEditMusicVolume;
	igdeButtonReference pBtnMusicPlay;
	igdeButtonReference pBtnMusicPause;
	igdeButtonReference pBtnMusicStop;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create panel. */
	meWPWorld( meWindowProperties &windowProperties );
	
protected:
	/** Clean up . */
	virtual ~meWPWorld();
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Properties window. */
	inline meWindowProperties &GetWindowProperties() const{ return pWindowProperties; }
	
	/** World. */
	inline meWorld *GetWorld() const{ return pWorld; }
	
	/** Set world. */
	void SetWorld( meWorld *world );
	
	/** Update world. */
	void UpdateWorld();
	
	/** Update world parameters. */
	void UpdateWorldParameters();
	
	/** Update path find test. */
	void UpdatePathFindTest();
	
	/** Active path find test type or NULL if there is none. */
	mePathFindTestType *GetActivePathFindTestType() const;
	
	/** Update path find test type list. */
	void UpdatePathFindTestTypeList();
	
	/** Update path find test type. */
	void UpdatePathFindTestType();
	
	
	
	/** Update music. */
	void UpdateMusic();
	
	
	
	/** Active property or an empty string if there is none. */
	const decString &GetActiveProperty() const;
	
	/** Select active property. */
	void SelectActiveProperty();
	
	/** Update property keys. */
	void UpdatePropertyKeys();
	
	/** Update properties. */
	void UpdateProperties();
	
	/** Update identifier list. */
	void UpdateIdentifierLists();
	
	/** Select PFType. */
	void SelectPFType( mePathFindTestType *type );
	
	
	
	/** Game project game definition changed. */
	void OnGameDefinitionChanged();
	
	
	
	inline igdeAction* GetActionPFTTypeAdd() const{ return pActionPFTTypeAdd; }
	inline igdeAction* GetActionPFTTypeRemove() const{ return pActionPFTTypeRemove; }
	inline igdeAction* GetActionPFTTypeClear() const{ return pActionPFTTypeClear; }
};

#endif
