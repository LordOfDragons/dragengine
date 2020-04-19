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

#ifndef _SESYNTHESIZERNOTIFIER_H_
#define _SESYNTHESIZERNOTIFIER_H_

#include <dragengine/deObject.h>

class seEffect;
class seSynthesizer;
class seController;
class seLink;
class seSource;



/**
 * \brief Synthesizer notifier.
 */
class seSynthesizerNotifier : public deObject{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create synthesizer notifier. */
	seSynthesizerNotifier();
	
	/** \brief Clean up synthesizer notifier. */
	virtual ~seSynthesizerNotifier();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Changed or saved state changed. */
	virtual void StateChanged( seSynthesizer *synthesizer );
	
	/** \brief Undos changed. */
	virtual void UndoChanged( seSynthesizer *synthesizer );
	
	/** \brief Synthesizer changed. */
	virtual void SynthesizerChanged( seSynthesizer *synthesizer );
	
	/** \brief Playback properties changed. */
	virtual void PlaybackChanged( seSynthesizer *synthesizer );
	
	
	
	/** \brief Active controller changed. */
	virtual void ActiveControllerChanged( seSynthesizer *synthesizer, seController *controller );
	
	/** \brief Controller changed. */
	virtual void ControllerChanged( seSynthesizer *synthesizer, seController *controller );
	
	/** \brief Controller name changed. */
	virtual void ControllerNameChanged( seSynthesizer *synthesizer, seController *controller );
	
	/** \brief Controller range changed. */
	virtual void ControllerRangeChanged( seSynthesizer *synthesizer, seController *controller );
	
	/** \brief Controller curve changed. */
	virtual void ControllerCurveChanged( seSynthesizer *synthesizer, seController *controller );
	
	/** \brief Controller count or order changed. */
	virtual void ControllerStructureChanged( seSynthesizer *synthesizer );
	
	
	
	/** \brief Active link changed. */
	virtual void ActiveLinkChanged( seSynthesizer *synthesizer, seLink *link );
	
	/** \brief Link changed. */
	virtual void LinkChanged( seSynthesizer *synthesizer, seLink *link );
	
	/** \brief Link name changed. */
	virtual void LinkNameChanged( seSynthesizer *synthesizer, seLink *link );
	
	/** \brief Link count or order changed. */
	virtual void LinkStructureChanged( seSynthesizer *synthesizer );
	
	
	
	/** \brief Active source changed. */
	virtual void ActiveSourceChanged( seSynthesizer *synthesizer, seSource *source );
	
	/** \brief Source changed. */
	virtual void SourceChanged( seSynthesizer *synthesizer, seSource *source );
	
	/** \brief Source name changed. */
	virtual void SourceNameChanged( seSynthesizer *synthesizer, seSource *source );
	
	/** \brief Source count or order changed. */
	virtual void SourceStructureChanged( seSynthesizer *synthesizer );
	
	/** \brief Active effect changed. */
	virtual void ActiveEffectChanged( seSynthesizer *synthesizer, seSource *source );
	
	/** \brief Effect changed. */
	virtual void EffectChanged( seSynthesizer *synthesizer, seSource *source, seEffect *effect );
	
	/** \brief Effect count or order changed. */
	virtual void EffectStructureChanged( seSynthesizer *synthesizer, seSource *source );
	/*@}*/
};

#endif
