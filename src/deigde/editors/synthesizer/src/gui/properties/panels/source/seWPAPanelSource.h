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

#ifndef _SEWPAPANELSOURCE_H_
#define _SEWPAPANELSOURCE_H_

#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeListBoxReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/event/igdeActionReference.h>
#include <deigde/gui/layout/igdeContainerFlow.h>

#include <dragengine/resources/synthesizer/source/deSynthesizerSourceVisitorIdentify.h>

class seLink;
class seSynthesizer;
class seSource;
class seWPEffect;
class seWPSource;
class seWindowMain;
class seControllerTarget;



/**
 * \brief Base source option panel.
 */
class seWPAPanelSource : public igdeContainerFlow{
private:
	seWPSource &pWPSource;
	deSynthesizerSourceVisitorIdentify::eSourceTypes pRequiredType;
	
	igdeActionReference pActionLinkAdd;
	igdeActionReference pActionLinkRemove;
	
	igdeTextFieldReference pEditName;
	igdeComboBoxReference pCBMixMode;
	igdeTextFieldReference pEditBlendFactor;
	igdeTextFieldReference pEditMinVolume;
	igdeTextFieldReference pEditMaxVolume;
	igdeTextFieldReference pEditMinPanning;
	igdeTextFieldReference pEditMaxPanning;
	igdeCheckBoxReference pChkEnabled;
	
	igdeComboBoxReference pCBTarget;
	igdeListBoxReference pListLinks;
	igdeComboBoxReference pCBLinks;
	igdeButtonReference pBtnLinkAdd;
	
	seWPEffect *pWPEffect;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	seWPAPanelSource( seWPSource &wpSource, deSynthesizerSourceVisitorIdentify::eSourceTypes requiredType );
	
protected:
	/** \brief Clean up panel. */
	virtual ~seWPAPanelSource();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Parent window. */
	inline seWPSource &GetWPSource() const{ return pWPSource; }
	
	/** \brief Required source type. */
	inline deSynthesizerSourceVisitorIdentify::eSourceTypes GetRequiredType() const{ return pRequiredType; }
	
	/** \brief Main window. */
	seWindowMain &GetWindowMain() const;
	
	/** \brief Synthesizer or \em NULL if not set. */
	seSynthesizer *GetSynthesizer() const;
	
	/** \brief Source or \em NULL if not set. */
	seSource *GetSource() const;
	
	/** \brief Target or \em NULL if not set. */
	seControllerTarget *GetTarget() const;
	
	/** \brief Selected link in combo box or \em NULL if not set. */
	seLink *GetCBLink() const;
	
	/** \brief Selected link in list box or \em NULL if not set. */
	seLink *GetListLink() const;
	
	
	
	/** \brief Synthesizer changed. */
	virtual void SetSynthesizer( seSynthesizer *synthesizer );
	
	/** \brief Synthesizer path changed. */
	virtual void OnSynthesizerPathChanged();
	
	/** \brief Panel has been activated. */
	virtual void OnActivated();
	
	/** \brief Update synthesizer. */
	virtual void UpdateSynthesizer();
	
	/** \brief Update controller list. */
	virtual void UpdateControllerList();
	
	/** \brief Update link list. */
	virtual void UpdateLinkList();
	
	/** \brief Update source. */
	virtual void UpdateSource();
	
	/** \brief Update target list. */
	virtual void UpdateTargetList();
	
	/** \brief Update target. */
	void UpdateTarget();
	
	/** \brief Remove all targets. */
	void RemoveAllTargets();
	
	/** \brief Add target. */
	void AddTarget( const char *name, seControllerTarget *target );
	
	
	
	/** \brief Actions. */
	inline igdeAction *GetActionLinkAdd() const{ return pActionLinkAdd; }
	inline igdeAction *GetActionLinkRemove() const{ return pActionLinkRemove; }
	/*@}*/
};

#endif
