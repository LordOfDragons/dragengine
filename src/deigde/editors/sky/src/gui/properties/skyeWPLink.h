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

#ifndef _SKYEWPLINK_H_
#define _SKYEWPLINK_H_

#include "skyeWPLinkListener.h"
#include "../../sky/skyeSky.h"

#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/layout/igdeContainerScroll.h>
#include <deigde/gui/curveedit/igdeViewCurveBezier.h>
#include <deigde/gui/event/igdeAction.h>

class skyeLink;
class skyeWindowProperties;

class igdeViewCurveBezier;


/**
 * \brief Link Panel.
 */
class skyeWPLink : public igdeContainerScroll{
public:
	typedef deTObjectReference<skyeWPLink> Ref;
	
private:
	skyeWindowProperties &pWindowProperties;
	skyeWPLinkListener::Ref pListener;
	
	skyeSky::Ref pSky;
	
	igdeAction::Ref pActionLinkAdd;
	igdeAction::Ref pActionLinkRemove;
	
	igdeListBox::Ref pListLinks;
	
	igdeTextField::Ref pEditName;
	igdeComboBox::Ref pCBController;
	igdeViewCurveBezier::Ref pEditCurve;
	igdeTextField::Ref pEditRepeat;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	skyeWPLink(skyeWindowProperties &windowProperties);
	
protected:
	/** \brief Clean up panel. */
	virtual ~skyeWPLink();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Sky. */
	inline const skyeSky::Ref &GetSky() const{ return pSky; }
	
	/** \brief Set sky. */
	void SetSky(skyeSky *sky);
	
	/** \brief Active link or \em nullptr. */
	skyeLink *GetLink() const;
	
	/** \brief Update link list. */
	void UpdateLinkList();
	
	/** \brief Change selection to active link. */
	void SelectActiveLink();
	
	/** \brief Update link parameters. */
	void UpdateLink();
	
	/** \brief Update list of controller. */
	void UpdateControllerList();
	
	/** \brief Actions. */
	inline const igdeAction::Ref &GetActionLinkAdd() const{ return pActionLinkAdd; }
	inline const igdeAction::Ref &GetActionLinkRemove() const{ return pActionLinkRemove; }
	/*@}*/
};

#endif
