/* 
 * Drag[en]gine DragonScript Script Module
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

#ifndef _DEDSSOUNDLEVELMETER_H_
#define _DEDSSOUNDLEVELMETER_H_

#include <dragengine/systems/modules/scripting/deBaseScriptingSoundLevelMeter.h>

class deScriptingDragonScript;
class dsValue;
class dsRealObject;



/**
 * \brief Sound level meter peer.
 */
class dedsSoundLevelMeter : public deBaseScriptingSoundLevelMeter{
private:
	deScriptingDragonScript &pDS;
	deSoundLevelMeter *pSoundLevelMeter;
	
	dsValue *pValCB;
	bool pHasCB;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	dedsSoundLevelMeter( deScriptingDragonScript &ds, deSoundLevelMeter *soundLevelMeter );
	
	/** \brief Clean up peer. */
	virtual ~dedsSoundLevelMeter();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Callback object or \em NULL. */
	dsRealObject *GetCallback() const;
	
	/** \brief Set callback object or \em NULL. */
	void SetCallback( dsRealObject *object );
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/**
	 * \brief Speaker became audible.
	 */
	virtual void SpeakerAudible( const deSoundLevelMeter::cAudibleSpeaker &speaker );
	
	/**
	 * \brief Speaker became inaudible.
	 */
	virtual void SpeakerInaudible( deSpeaker *speaker );
	/*@}*/
};

#endif
