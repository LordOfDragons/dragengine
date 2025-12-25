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

#ifndef _SEWPEFFECTLISTENER_H_
#define _SEWPEFFECTLISTENER_H_

#include "../../synthesizer/seSynthesizerNotifier.h"

class seWPEffect;



/**
 * \brief Source properties panel listener.
 */
class seWPEffectListener : public seSynthesizerNotifier{
public:
	typedef deTObjectReference<seWPEffectListener> Ref;
	
private:
	seWPEffect &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	seWPEffectListener(seWPEffect &panel);
	
	/** \brief Clean up listener. */
	~seWPEffectListener() override;
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Controller name changed. */
	void ControllerNameChanged(seSynthesizer *synthesizer, seController *controller) override;
	
	/** \brief Controller count or order changed. */
	void ControllerStructureChanged(seSynthesizer *synthesizer) override;
	
	
	
	/** \brief Link name changed. */
	void LinkNameChanged(seSynthesizer *synthesizer, seLink *link) override;
	
	/** \brief Link count or order changed. */
	void LinkStructureChanged(seSynthesizer *synthesizer) override;
	
	
	
	/** \brief Active source changed. */
	void ActiveSourceChanged(seSynthesizer *synthesizer, seSource *rule) override;
	
	/** \brief Active effect changed. */
	void ActiveEffectChanged(seSynthesizer *synthesizer, seSource *source) override;
	
	/** \brief Effect changed. */
	void EffectChanged(seSynthesizer *synthesizer, seSource *source, seEffect *effect) override;
	
	/** \brief Effect count or order changed. */
	void EffectStructureChanged(seSynthesizer *synthesizer, seSource *source) override;
	/*@}*/
};

#endif
