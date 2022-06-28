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

#ifndef _SEWPAPANELSOURCESYNTHESIZER_H_
#define _SEWPAPANELSOURCESYNTHESIZER_H_

#include "seWPAPanelSource.h"

#include <deigde/gui/composed/igdeEditPathReference.h>



/**
 * \brief Synthesizer source panel.
 */
class seWPAPanelSourceSynthesizer : public seWPAPanelSource{
private:
	igdeEditPathReference pEditPathSynthesizer;
	igdeComboBoxReference pCBConTarget;
	igdeButtonReference pBtnConTargetMore;
	igdeButtonReference pBtnConTargetLess;
	igdeComboBoxReference pCBConController;
	bool pPreventUpdate;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	seWPAPanelSourceSynthesizer( seWPSource &wpSources );
	
	/** \brief Clean up panel. */
	virtual ~seWPAPanelSourceSynthesizer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Update connection target list. */
	void UpdateConTargetList();
	
	/** \brief Selected controller target. */
	int GetCBControllerTarget() const;
	
	/** \brief Synthesizer path changed. */
	virtual void OnSynthesizerPathChanged();
	
	/** \brief Update controller list. */
	virtual void UpdateControllerList();
	
	/** \brief Update source. */
	virtual void UpdateSource();
	
	/** \brief Update connection. */
	virtual void UpdateConnection();
	
	/** \brief Prevent update for listener use only. */
	inline bool GetPreventUpdate() const{ return pPreventUpdate; }
	/*@}*/
};

#endif
