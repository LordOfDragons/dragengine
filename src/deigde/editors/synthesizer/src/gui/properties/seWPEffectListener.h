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

#ifndef _SEWPEFFECTLISTENER_H_
#define _SEWPEFFECTLISTENER_H_

#include "../../synthesizer/seSynthesizerNotifier.h"

class seWPEffect;



/**
 * \brief Source properties panel listener.
 */
class seWPEffectListener : public seSynthesizerNotifier{
private:
	seWPEffect &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	seWPEffectListener( seWPEffect &panel );
	
	/** \brief Clean up listener. */
	virtual ~seWPEffectListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Controller name changed. */
	virtual void ControllerNameChanged( seSynthesizer *synthesizer, seController *controller );
	
	/** \brief Controller count or order changed. */
	virtual void ControllerStructureChanged( seSynthesizer *synthesizer );
	
	
	
	/** \brief Link name changed. */
	virtual void LinkNameChanged( seSynthesizer *synthesizer, seLink *link );
	
	/** \brief Link count or order changed. */
	virtual void LinkStructureChanged( seSynthesizer *synthesizer );
	
	
	
	/** \brief Active source changed. */
	virtual void ActiveSourceChanged( seSynthesizer *synthesizer, seSource *rule );
	
	/** \brief Active effect changed. */
	virtual void ActiveEffectChanged( seSynthesizer *synthesizer, seSource *source );
	
	/** \brief Effect changed. */
	virtual void EffectChanged( seSynthesizer *synthesizer, seSource *source, seEffect *effect );
	
	/** \brief Effect count or order changed. */
	virtual void EffectStructureChanged( seSynthesizer *synthesizer, seSource *source );
	/*@}*/
};

#endif
