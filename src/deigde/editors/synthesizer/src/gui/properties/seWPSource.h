/* 
 * Drag[en]gine IGDE Synthesizer Editor
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

#ifndef _SEWPSOURCE_H_
#define _SEWPSOURCE_H_

#include <deigde/gui/igdeTreeListReference.h>
#include <deigde/gui/igdeSwitcherReference.h>
#include <deigde/gui/event/igdeActionReference.h>
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
	seWPSourceListener *pListener;
	
	seSynthesizer *pSynthesizer;
	
	igdeTreeListReference pTreeSource;
	
	igdeSwitcherReference pSwitcher;
	seWPAPanelSourceSound *pPanelSound;
	seWPAPanelSourceWave *pPanelWave;
	seWPAPanelSourceChain *pPanelChain;
	seWPAPanelSourceGroup *pPanelGroup;
	seWPAPanelSourceSynthesizer *pPanelSynthesizer;
	seWPAPanelSource *pActivePanel;
	
	igdeActionReference pActionSourceCut;
	igdeActionReference pActionSourceCopy;
	igdeActionReference pActionSourcePaste;
	igdeActionReference pActionSourcePasteIntoGroup;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	seWPSource( seViewSynthesizer &viewSynthesizer );
	
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
	void SetSynthesizer( seSynthesizer *synthesizer );
	
	/** \brief Source or \em NULL if not set. */
	seSource *GetSource() const;
	
	
	
	/** \brief Update synthesizer. */
	void UpdateSynthesizer();
	
	/** \brief Update the controller list. */
	void UpdateControllerList();
	
	/** \brief Update the link list. */
	void UpdateLinkList();
	
	/** \brief Update the source tree. */
	void UpdateSourceTree();
	
	/** \brief Update item in the source tree. */
	void UpdateSourceTreeItem( igdeTreeItem *item, seSource *source );
	
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
