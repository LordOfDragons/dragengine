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
#include <deigde/gui/composed/igdeEditVectorReference.h>
#include <deigde/gui/event/igdeActionReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class meWindowProperties;
class meWPWorldListener;
class meWorld;
class mePathFindTestType;



/**
 * \brief Property window displaying the properties of the world.
 */
class meWPWorld : public igdeContainerScroll{
private:
	meWindowProperties &pWindowProperties;
	meWPWorldListener *pListener;
	meWorld *pWorld;
	
	igdeActionReference pActionPFTTypeAdd;
	igdeActionReference pActionPFTTypeRemove;
	igdeActionReference pActionPFTTypeClear;
	
	
	igdeWidgetReference pEditProperties;
	
	igdeEditVectorReference pEditPFTStartPosition;
	igdeButtonReference pBtnPFTStartPosFromCamera;
	igdeEditVectorReference pEditPFTGoalPosition;
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
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	meWPWorld( meWindowProperties &windowProperties );
	
protected:
	/** \brief Clean up . */
	virtual ~meWPWorld();
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Properties window. */
	inline meWindowProperties &GetWindowProperties() const{ return pWindowProperties; }
	
	/** \brief World. */
	inline meWorld *GetWorld() const{ return pWorld; }
	
	/** \brief Set world. */
	void SetWorld( meWorld *world );
	
	/** \brief Update world. */
	void UpdateWorld();
	
	/** \brief Update path find test. */
	void UpdatePathFindTest();
	
	/** \brief Active path find test type or NULL if there is none. */
	mePathFindTestType *GetActivePathFindTestType() const;
	
	/** \brief Update path find test type list. */
	void UpdatePathFindTestTypeList();
	
	/** \brief Update path find test type. */
	void UpdatePathFindTestType();
	
	
	
	/** \brief Active property or an empty string if there is none. */
	const decString &GetActiveProperty() const;
	
	/** \brief Select active property. */
	void SelectActiveProperty();
	
	/** \brief Update property keys. */
	void UpdatePropertyKeys();
	
	/** \brief Update properties. */
	void UpdateProperties();
	
	/** \brief Update identifier list. */
	void UpdateIdentifierLists();
	
	/** \brief Select PFType. */
	void SelectPFType( mePathFindTestType *type );
	
	
	
	/** \brief Game project game definition changed. */
	void OnGameDefinitionChanged();
	
	
	
	inline igdeAction* GetActionPFTTypeAdd() const{ return pActionPFTTypeAdd; }
	inline igdeAction* GetActionPFTTypeRemove() const{ return pActionPFTTypeRemove; }
	inline igdeAction* GetActionPFTTypeClear() const{ return pActionPFTTypeClear; }
};

#endif
