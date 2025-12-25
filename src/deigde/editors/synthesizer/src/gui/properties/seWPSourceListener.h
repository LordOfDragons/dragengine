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

#ifndef _SEWPSOURCELISTENER_H_
#define _SEWPSOURCELISTENER_H_

#include "../../synthesizer/seSynthesizerNotifier.h"

class seWPSource;



/**
 * \brief Source properties panel listener
 */
class seWPSourceListener : public seSynthesizerNotifier{
public:
	typedef deTObjectReference<seWPSourceListener> Ref;
	
private:
	seWPSource &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	seWPSourceListener(seWPSource &panel);
	
	/** \brief Clean up listener. */
	~seWPSourceListener() override;
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Synthesizer changed. */
	void SynthesizerChanged(seSynthesizer *synthesizer) override;
	
	
	
	/** \brief Controller name changed. */
	void ControllerNameChanged(seSynthesizer *synthesizer, seController *controller) override;
	
	/** \brief Controller count or order changed. */
	void ControllerStructureChanged(seSynthesizer *synthesizer) override;
	
	
	
	/** \brief Link name changed. */
	void LinkNameChanged(seSynthesizer *synthesizer, seLink *link) override;
	
	/** \brief Link count or order changed. */
	void LinkStructureChanged(seSynthesizer *synthesizer) override;
	
	
	
	/** \brief Active source changed. */
	void ActiveSourceChanged(seSynthesizer *synthesizer, seSource *source) override;
	
	/** \brief Source changed. */
	void SourceChanged(seSynthesizer *synthesizer, seSource *source) override;
	
	/** \brief Source name changed. */
	void SourceNameChanged(seSynthesizer *synthesizer, seSource *source) override;
	
	/** \brief Source count or order changed. */
	void SourceStructureChanged(seSynthesizer *synthesizer) override;
	/*@}*/
};

#endif
