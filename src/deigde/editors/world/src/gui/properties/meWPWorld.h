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
#include <deigde/gui/event/igdeActionContextMenuReference.h>
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
	
	igdeActionContextMenuReference pActionPFTTypes;
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
