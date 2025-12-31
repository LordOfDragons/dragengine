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

#ifndef _SYNEWPSOURCE_H_
#define _SYNEWPSOURCE_H_

#include "syneWPSourceListener.h"
#include "panels/source/syneWPAPanelSourceSound.h"
#include "panels/source/syneWPAPanelSourceWave.h"
#include "panels/source/syneWPAPanelSourceChain.h"
#include "panels/source/syneWPAPanelSourceGroup.h"
#include "panels/source/syneWPAPanelSourceSynthesizer.h"
#include "../../synthesizer/syneSynthesizer.h"

#include <deigde/gui/igdeTreeList.h>
#include <deigde/gui/igdeSwitcher.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class syneSource;
class syneViewSynthesizer;
class syneWPAPanelSource;

class igdeTreeItem;


/**
 * \brief Base synthesizer source properties panel.
 */
class syneWPSource : public igdeContainerScroll{
public:
	typedef deTObjectReference<syneWPSource> Ref;
	
private:
	syneViewSynthesizer &pViewSynthesizer;
	syneWPSourceListener::Ref pListener;
	
	syneSynthesizer::Ref pSynthesizer;
	
	igdeTreeList::Ref pTreeSource;
	
	igdeSwitcher::Ref pSwitcher;
	syneWPAPanelSourceSound::Ref pPanelSound;
	syneWPAPanelSourceWave::Ref pPanelWave;
	syneWPAPanelSourceChain::Ref pPanelChain;
	syneWPAPanelSourceGroup::Ref pPanelGroup;
	syneWPAPanelSourceSynthesizer::Ref pPanelSynthesizer;
	syneWPAPanelSource *pActivePanel;
	
	igdeAction::Ref pActionSourceCut;
	igdeAction::Ref pActionSourceCopy;
	igdeAction::Ref pActionSourcePaste;
	igdeAction::Ref pActionSourcePasteIntoGroup;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	syneWPSource(syneViewSynthesizer &viewSynthesizer);
	
protected:
	/** \brief Clean up panel. */
	virtual ~syneWPSource();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief View synthesizer. */
	inline syneViewSynthesizer &GetViewSynthesizer() const{ return pViewSynthesizer; }
	
	/** \brief Synthesizer or \em nullptr if not set. */
	inline const syneSynthesizer::Ref &GetSynthesizer() const{ return pSynthesizer; }
	
	/** \brief Set synthesizer or \em nullptr if not set. */
	void SetSynthesizer(syneSynthesizer *synthesizer);
	
	/** \brief Source or \em nullptr if not set. */
	syneSource *GetSource() const;
	
	
	
	/** \brief Synthesizer path changed. */
	void OnSynthesizerPathChanged();
	
	/** \brief Update synthesizer. */
	void UpdateSynthesizer();
	
	/** \brief Update the controller list. */
	void UpdateControllerList();
	
	/** \brief Update the link list. */
	void UpdateLinkList();
	
	/** \brief Update the source tree. */
	void UpdateSourceTree();
	
	/** \brief Update item in the source tree. */
	void UpdateSourceTreeItem(igdeTreeItem *item, syneSource *source);
	
	/** \brief Select active source. */
	void SelectActiveSource();
	
	/** \brief Show panel matching active source. */
	void ShowActiveSourcePanel();
	
	/** \brief Update the active source. */
	void UpdateSource();
	
	
	
	/** \brief Actions. */
	inline const igdeAction::Ref &GetActionSourceCut() const{ return pActionSourceCut; }
	inline const igdeAction::Ref &GetActionSourceCopy() const{ return pActionSourceCopy; }
	inline const igdeAction::Ref &GetActionSourcePaste() const{ return pActionSourcePaste; }
	inline const igdeAction::Ref &GetActionSourcePasteIntoGroup() const{ return pActionSourcePasteIntoGroup; }
	/*@}*/
};

#endif
