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

#ifndef _SEWPSOURCE_H_
#define _SEWPSOURCE_H_

#include <deigde/gui/igdeTreeList.h>
#include <deigde/gui/igdeSwitcher.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/layout/igdeContainerScroll.h>



class seSynthesizer;
class seSource;
class seViewSynthesizer;
class seWPSourceListener;
class seWPAPanelSourceSound;
class seWPAPanelSourceWave;
class seWPAPanelSourceChain;
class seWPAPanelSourceGroup;
class seWPAPanelSourceSynthesizer;
class seWPAPanelSource;

class igdeTreeItem;


/**
 * \brief Base synthesizer source properties panel.
 */
class seWPSource : public igdeContainerScroll{
private:
	seViewSynthesizer &pViewSynthesizer;
	seWPSourceListener::Ref pListener;
	
	seSynthesizer::Ref pSynthesizer;
	
	igdeTreeList::Ref pTreeSource;
	
	igdeSwitcher::Ref pSwitcher;
	seWPAPanelSourceSound *pPanelSound;
	seWPAPanelSourceWave *pPanelWave;
	seWPAPanelSourceChain *pPanelChain;
	seWPAPanelSourceGroup *pPanelGroup;
	seWPAPanelSourceSynthesizer *pPanelSynthesizer;
	seWPAPanelSource *pActivePanel;
	
	igdeAction::Ref pActionSourceCut;
	igdeAction::Ref pActionSourceCopy;
	igdeAction::Ref pActionSourcePaste;
	igdeAction::Ref pActionSourcePasteIntoGroup;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	seWPSource(seViewSynthesizer &viewSynthesizer);
	
protected:
	/** \brief Clean up panel. */
	virtual ~seWPSource();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief View synthesizer. */
	inline seViewSynthesizer &GetViewSynthesizer() const{ return pViewSynthesizer; }
	
	/** \brief Synthesizer or \em NULL if not set. */
	inline seSynthesizer *GetSynthesizer() const{ return pSynthesizer; }
	
	/** \brief Set synthesizer or \em NULL if not set. */
	void SetSynthesizer(seSynthesizer::Ref synthesizer);
	
	/** \brief Source or \em NULL if not set. */
	seSource *GetSource() const;
	
	
	
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
	void UpdateSourceTreeItem(igdeTreeItem *item, seSource *source);
	
	/** \brief Select active source. */
	void SelectActiveSource();
	
	/** \brief Show panel matching active source. */
	void ShowActiveSourcePanel();
	
	/** \brief Update the active source. */
	void UpdateSource();
	
	
	
	/** \brief Actions. */
	inline igdeAction *GetActionSourceCut() const{ return pActionSourceCut; }
	inline igdeAction *GetActionSourceCopy() const{ return pActionSourceCopy; }
	inline igdeAction *GetActionSourcePaste() const{ return pActionSourcePaste; }
	inline igdeAction *GetActionSourcePasteIntoGroup() const{ return pActionSourcePasteIntoGroup; }
	/*@}*/
};

#endif
