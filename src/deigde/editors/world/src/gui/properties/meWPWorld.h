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

#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeWidget.h>
#include <deigde/gui/composed/igdeEditDVector.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditSliderText.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionContextMenu.h>
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
	meWPWorldListener::Ref pListener;
	meWorld::Ref pWorld;
	
	igdeActionContextMenu::Ref pActionPFTTypes;
	igdeAction::Ref pActionPFTTypeAdd;
	igdeAction::Ref pActionPFTTypeRemove;
	igdeAction::Ref pActionPFTTypeClear;
	igdeAction::Ref pActionMusicPlay;
	igdeAction::Ref pActionMusicPause;
	igdeAction::Ref pActionMusicStop;
	
	
	igdeWidget::Ref pEditProperties;
	
	igdeEditDVector::Ref pEditSize;
	igdeEditVector::Ref pEditGravity;
	
	igdeEditDVector::Ref pEditPFTStartPosition;
	igdeButton::Ref pBtnPFTStartPosFromCamera;
	igdeEditDVector::Ref pEditPFTGoalPosition;
	igdeButton::Ref pBtnPFTGoalPosFromCamera;
	igdeTextField::Ref pEditPFTLayer;
	igdeComboBox::Ref pCBPFTSpaceType;
	igdeTextField::Ref pEditPFTBlockingCost;
	igdeCheckBox::Ref pChkPFTShowPath;
	igdeComboBox::Ref pCBPFTType;
	igdeButton::Ref pBtnPFTTypes;
	igdeTextField::Ref pEditPFTTypeNumber;
	igdeTextField::Ref pEditPFTTypeName;
	igdeTextField::Ref pEditPFTTypeFixCost;
	igdeTextField::Ref pEditPFTTypeCPM;
	
	
	igdeEditPath::Ref pEditMusicPath;
	igdeEditSliderText::Ref pEditMusicVolume;
	igdeButton::Ref pBtnMusicPlay;
	igdeButton::Ref pBtnMusicPause;
	igdeButton::Ref pBtnMusicStop;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create panel. */
	meWPWorld(meWindowProperties &windowProperties);
	
protected:
	/** Clean up . */
	virtual ~meWPWorld();
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Properties window. */
	inline meWindowProperties &GetWindowProperties() const{ return pWindowProperties; }
	
	/** World. */
	inline const meWorld::Ref &GetWorld() const{ return pWorld; }
	
	/** Set world. */
	void SetWorld(meWorld *world);
	
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
	void SelectPFType(mePathFindTestType *type);
	
	
	
	/** Game project game definition changed. */
	void OnGameDefinitionChanged();
	
	
	
	inline const igdeAction::Ref &GetActionPFTTypeAdd() const{ return pActionPFTTypeAdd; }
	inline const igdeAction::Ref &GetActionPFTTypeRemove() const{ return pActionPFTTypeRemove; }
	inline const igdeAction::Ref &GetActionPFTTypeClear() const{ return pActionPFTTypeClear; }
};

#endif
