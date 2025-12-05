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
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<seSynthesizerNotifier> Ref;


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
