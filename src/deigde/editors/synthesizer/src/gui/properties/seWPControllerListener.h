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

#ifndef _SEWPCONTROLLERLISTENER_H_
#define _SEWPCONTROLLERLISTENER_H_

#include "../../synthesizer/seSynthesizerNotifier.h"

class seWPController;



/**
 * \brief Controller properties panel listener.
 */
class seWPControllerListener : public seSynthesizerNotifier{
private:
	seWPController &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	seWPControllerListener(seWPController &panel);
	
	/** \brief Clean up listener. */
	virtual ~seWPControllerListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Synthesizer changed. */
	virtual void SynthesizerChanged(seSynthesizer *synthesizer);
	
	
	
	/** \brief Active controller changed. */
	virtual void ActiveControllerChanged(seSynthesizer *synthesizer, seController *controller);
	
	/** \brief Controller changed. */
	virtual void ControllerChanged(seSynthesizer *synthesizer, seController *controller);
	
	/** \brief Controller name changed. */
	virtual void ControllerNameChanged(seSynthesizer *synthesizer, seController *controller);
	
	/** \brief Controller range changed. */
	virtual void ControllerRangeChanged(seSynthesizer *synthesizer, seController *controller);
	
	/** \brief Controller curve changed. */
	virtual void ControllerCurveChanged(seSynthesizer *synthesizer, seController *controller);
	
	/** \brief Controller count or order changed. */
	virtual void ControllerStructureChanged(seSynthesizer *synthesizer);
	/*@}*/
};

#endif
