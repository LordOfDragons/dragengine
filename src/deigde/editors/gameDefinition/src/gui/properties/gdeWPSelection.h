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

#ifndef _GDEWPSELECTION_H_
#define _GDEWPSELECTION_H_

#include <dragengine/common/collection/decObjectList.h>

#include <deigde/gui/igdeTreeList.h>
#include <deigde/gui/igdeSwitcher.h>
#include <deigde/gui/igdeWidget.h>
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
	
	igdeSwitcher::Ref pSwitcher;
	igdeTreeList::Ref pTreeObjects;
	gdeWPSTreeModel *pModelTreeObjects;
	
	igdeWidget::Ref pPanelCategory;
	igdeWidget::Ref pPanelObjectClass;
	igdeWidget::Ref pPanelOCBillboard;
	igdeWidget::Ref pPanelOCCamera;
	igdeWidget::Ref pPanelOCComponent;
	igdeWidget::Ref pPanelOCEnvMapProbe;
	igdeWidget::Ref pPanelOCLight;
	igdeWidget::Ref pPanelOCNavigationBlocker;
	igdeWidget::Ref pPanelOCNavigationSpace;
	igdeWidget::Ref pPanelOCParticleEmitter;
	igdeWidget::Ref pPanelOCForceField;
	igdeWidget::Ref pPanelOCSnapPoint;
	igdeWidget::Ref pPanelOCSpeaker;
	igdeWidget::Ref pPanelOCWorld;
	igdeWidget::Ref pPanelParticleEmitter;
	igdeWidget::Ref pPanelSkin;
	igdeWidget::Ref pPanelSky;
	
	
	
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
