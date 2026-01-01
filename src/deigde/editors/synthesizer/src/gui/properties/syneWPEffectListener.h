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

#ifndef _SYNEWPEFFECTLISTENER_H_
#define _SYNEWPEFFECTLISTENER_H_

#include "../../synthesizer/syneSynthesizerNotifier.h"

class syneWPEffect;



/**
 * \brief Source properties panel listener.
 */
class syneWPEffectListener : public syneSynthesizerNotifier{
public:
	typedef deTObjectReference<syneWPEffectListener> Ref;
	
private:
	syneWPEffect &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	syneWPEffectListener(syneWPEffect &panel);
	
	/** \brief Clean up listener. */
protected:
	~syneWPEffectListener() override;
public:
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Controller name changed. */
	void ControllerNameChanged(syneSynthesizer *synthesizer, syneController *controller) override;
	
	/** \brief Controller count or order changed. */
	void ControllerStructureChanged(syneSynthesizer *synthesizer) override;
	
	
	
	/** \brief Link name changed. */
	void LinkNameChanged(syneSynthesizer *synthesizer, syneLink *link) override;
	
	/** \brief Link count or order changed. */
	void LinkStructureChanged(syneSynthesizer *synthesizer) override;
	
	
	
	/** \brief Active source changed. */
	void ActiveSourceChanged(syneSynthesizer *synthesizer, syneSource *rule) override;
	
	/** \brief Active effect changed. */
	void ActiveEffectChanged(syneSynthesizer *synthesizer, syneSource *source) override;
	
	/** \brief Effect changed. */
	void EffectChanged(syneSynthesizer *synthesizer, syneSource *source, syneEffect *effect) override;
	
	/** \brief Effect count or order changed. */
	void EffectStructureChanged(syneSynthesizer *synthesizer, syneSource *source) override;
	/*@}*/
};

#endif
