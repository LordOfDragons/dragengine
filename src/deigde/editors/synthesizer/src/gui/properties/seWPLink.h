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

#ifndef _SEWPLINK_H_
#define _SEWPLINK_H_

#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeSpinTextField.h>
#include <deigde/gui/curveedit/igdeViewCurveBezier.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class seSynthesizer;
class seLink;
class seViewSynthesizer;
class seWPLinkListener;



/**
 * \brief Link properties panel.
 */
class seWPLink : public igdeContainerScroll{
private:
	seViewSynthesizer &pViewSynthesizer;
	seWPLinkListener::Ref pListener;
	
	seSynthesizer::Ref pSynthesizer;
	
	igdeAction::Ref pActionLinkAdd;
	igdeAction::Ref pActionLinkRemove;
	
	igdeListBox::Ref pListLink;
	
	igdeTextField::Ref pEditName;
	igdeComboBox::Ref pCBController;
	igdeSpinTextField::Ref pSpinRepeat;
	igdeViewCurveBezier::Ref pEditCurve;
	
	bool pPreventUpdate;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create properties panel. */
	seWPLink(seViewSynthesizer &viewSynthesizer);
	
protected:
	/** \brief Clean up properties panel. */
	virtual ~seWPLink();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief View. */
	inline seViewSynthesizer &GetViewSynthesizer() const{ return pViewSynthesizer; }
	
	/** \brief Synthesizer or \em NULL if not set. */
	inline const seSynthesizer::Ref &GetSynthesizer() const{ return pSynthesizer; }
	
	/** \brief Set synthesizer or \em NULL if not set. */
	void SetSynthesizer(seSynthesizer *synthesizer);
	
	/** \brief Link or \em NULL if not set. */
	seLink *GetLink() const;
	
	
	
	/** \brief Update link list. */
	void UpdateLinkList();
	
	/** \brief Select active link. */
	void SelectActiveLink();
	
	/** \brief Update link. */
	void UpdateLink();
	
	/** \brief Update controller list. */
	void UpdateControllerList();
	
	/** \brief Prevent update for listener use only. */
	inline bool GetPreventUpdate() const{ return pPreventUpdate; }
	
	
	
	/** \brief Actions. */
	inline const igdeAction::Ref &GetActionLinkAdd() const{ return pActionLinkAdd; }
	inline const igdeAction::Ref &GetActionLinkRemove() const{ return pActionLinkRemove; }
	/*@}*/
};

#endif
