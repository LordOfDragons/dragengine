/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#ifndef _GDEWPSOSPEAKER_H_
#define _GDEWPSOSPEAKER_H_

#include "../../../gamedef/objectClass/speaker/gdeOCSpeaker.h"

#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeComboBoxFilterReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/composed/igdeEditPathReference.h>
#include <deigde/gui/composed/igdeEditVectorReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class gdeGameDefinition;
class gdeObjectClass;
class gdeOCSpeaker;
class gdeWindowProperties;
class gdeWPSOCSpeakerListener;



/**
 * \brief Object class speaker property panel.
 */
class gdeWPSOCSpeaker : public igdeContainerScroll{
private:
	gdeWindowProperties &pWindowProperties;
	gdeWPSOCSpeakerListener *pListener;
	
	gdeGameDefinition *pGameDefinition;
	
	igdeEditPathReference pEditPathSound;
	igdeEditVectorReference pEditPosition;
	igdeEditVectorReference pEditRotation;
	igdeTextFieldReference pEditBoneName;
	igdeCheckBoxReference pChkLooping;
	igdeCheckBoxReference pChkPlaying;
	igdeTextFieldReference pEditVolume;
	igdeTextFieldReference pEditRange;
	igdeTextFieldReference pEditRollOff;
	igdeTextFieldReference pEditPlaySpeed;
	
	igdeComboBoxReference pCBPropertyNames;
	igdeComboBoxFilterReference pCBPropertyNameTarget;
	
	igdeComboBoxReference pCBTriggerNames;
	igdeComboBoxFilterReference pCBTriggerNameTarget;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	gdeWPSOCSpeaker( gdeWindowProperties &windowMain );
	
	/** \brief Clean up panel. */
	virtual ~gdeWPSOCSpeaker();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Game definition or \em NULL if not set. */
	inline gdeGameDefinition *GetGameDefinition() const{ return pGameDefinition; }
	
	/** \brief Set game definition or \em NULL if not set. */
	void SetGameDefinition( gdeGameDefinition *gameDefinition );
	
	
	
	/** \brief Active object class or \em NULL if absent. */
	gdeObjectClass *GetObjectClass() const;
	
	/** \brief Active object class speaker or \em NULL if not set. */
	gdeOCSpeaker *GetSpeaker() const;
	
	/** \brief Selected property name. */
	const gdeOCSpeaker::eProperties GetPropertyName() const;
	
	/** \brief Selected property name. */
	const gdeOCSpeaker::eTriggers GetTriggerName() const;
	
	
	
	/** \brief Update object class property list. */
	void UpdatePropertyList();
	
	
	
	/** \brief Update speaker. */
	void UpdateSpeaker();
	
	/** \brief Update speaker property name. */
	void UpdatePropertyName();
	
	/** \brief Update speaker trigger name. */
	void UpdateTriggerName();
	/*@}*/
};

#endif
