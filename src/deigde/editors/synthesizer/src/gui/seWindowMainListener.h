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

#ifndef _SEWINDOWMAINLISTENER_H_
#define _SEWINDOWMAINLISTENER_H_

#include "../synthesizer/seSynthesizerNotifier.h"

class seWindowMain;



/**
 * \brief Main window listener for synthesizer changes.
 */
class seWindowMainListener : public seSynthesizerNotifier{
public:
	typedef deTObjectReference<seWindowMainListener> Ref;
	
private:
	seWindowMain &pWindowMain;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	seWindowMainListener(seWindowMain &windowMain);
	
	/** \brief Clean up listener. */
	~seWindowMainListener() override;
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Changed or saved state changed. */
	void StateChanged(seSynthesizer *synthesizer) override;
	
	/** \brief Undos changed. */
	void UndoChanged(seSynthesizer *synthesizer) override;
	
	/** \brief Synthesizer changed. */
	void SynthesizerChanged(seSynthesizer *synthesizer) override;
	
	
	
	/** \brief Controller changed. */
	void ControllerChanged(seSynthesizer *synthesizer, seController *controller) override;
	
	/** \brief Controller count or order changed. */
	void ControllerStructureChanged(seSynthesizer *synthesizer) override;
	
	
	
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
