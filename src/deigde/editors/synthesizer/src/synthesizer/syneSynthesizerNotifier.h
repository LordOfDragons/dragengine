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

#ifndef _SYNESYNTHESIZERNOTIFIER_H_
#define _SYNESYNTHESIZERNOTIFIER_H_

#include <dragengine/deObject.h>

class syneEffect;
class syneSynthesizer;
class syneController;
class syneLink;
class syneSource;



/**
 * \brief Synthesizer notifier.
 */
class syneSynthesizerNotifier : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<syneSynthesizerNotifier> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create synthesizer notifier. */
	syneSynthesizerNotifier();
	
	/** \brief Clean up synthesizer notifier. */
protected:
	virtual ~syneSynthesizerNotifier();
public:
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Changed or saved state changed. */
	virtual void StateChanged(syneSynthesizer *synthesizer);
	
	/** \brief Undos changed. */
	virtual void UndoChanged(syneSynthesizer *synthesizer);
	
	/** \brief Synthesizer changed. */
	virtual void SynthesizerChanged(syneSynthesizer *synthesizer);
	
	/** \brief Playback properties changed. */
	virtual void PlaybackChanged(syneSynthesizer *synthesizer);
	
	
	
	/** \brief Active controller changed. */
	virtual void ActiveControllerChanged(syneSynthesizer *synthesizer, syneController *controller);
	
	/** \brief Controller changed. */
	virtual void ControllerChanged(syneSynthesizer *synthesizer, syneController *controller);
	
	/** \brief Controller name changed. */
	virtual void ControllerNameChanged(syneSynthesizer *synthesizer, syneController *controller);
	
	/** \brief Controller range changed. */
	virtual void ControllerRangeChanged(syneSynthesizer *synthesizer, syneController *controller);
	
	/** \brief Controller curve changed. */
	virtual void ControllerCurveChanged(syneSynthesizer *synthesizer, syneController *controller);
	
	/** \brief Controller count or order changed. */
	virtual void ControllerStructureChanged(syneSynthesizer *synthesizer);
	
	
	
	/** \brief Active link changed. */
	virtual void ActiveLinkChanged(syneSynthesizer *synthesizer, syneLink *link);
	
	/** \brief Link changed. */
	virtual void LinkChanged(syneSynthesizer *synthesizer, syneLink *link);
	
	/** \brief Link name changed. */
	virtual void LinkNameChanged(syneSynthesizer *synthesizer, syneLink *link);
	
	/** \brief Link count or order changed. */
	virtual void LinkStructureChanged(syneSynthesizer *synthesizer);
	
	
	
	/** \brief Active source changed. */
	virtual void ActiveSourceChanged(syneSynthesizer *synthesizer, syneSource *source);
	
	/** \brief Source changed. */
	virtual void SourceChanged(syneSynthesizer *synthesizer, syneSource *source);
	
	/** \brief Source name changed. */
	virtual void SourceNameChanged(syneSynthesizer *synthesizer, syneSource *source);
	
	/** \brief Source count or order changed. */
	virtual void SourceStructureChanged(syneSynthesizer *synthesizer);
	
	/** \brief Active effect changed. */
	virtual void ActiveEffectChanged(syneSynthesizer *synthesizer, syneSource *source);
	
	/** \brief Effect changed. */
	virtual void EffectChanged(syneSynthesizer *synthesizer, syneSource *source, syneEffect *effect);
	
	/** \brief Effect count or order changed. */
	virtual void EffectStructureChanged(syneSynthesizer *synthesizer, syneSource *source);
	/*@}*/
};

#endif
