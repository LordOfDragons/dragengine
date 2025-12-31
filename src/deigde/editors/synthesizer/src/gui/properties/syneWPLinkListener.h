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

#ifndef _SYNEWPLINKLISTENER_H_
#define _SYNEWPLINKLISTENER_H_

#include "../../synthesizer/syneSynthesizerNotifier.h"

class syneWPLink;



/**
 * \brief Link properties panel listener.
 */
class syneWPLinkListener : public syneSynthesizerNotifier{
public:
	typedef deTObjectReference<syneWPLinkListener> Ref;
	
private:
	syneWPLink &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	explicit syneWPLinkListener(syneWPLink &panel);
	
	/** \brief Clean up listener. */
protected:
	~syneWPLinkListener() override;
public:
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Controller name changed. */
	void ControllerNameChanged(syneSynthesizer *synthesizer, syneController *controller) override;
	
	/** \brief Controller count or order changed. */
	void ControllerStructureChanged(syneSynthesizer *synthesizer) override;
	
	
	
	/** \brief Active link changed. */
	void ActiveLinkChanged(syneSynthesizer *synthesizer, syneLink *link) override;
	
	/** \brief Link changed. */
	void LinkChanged(syneSynthesizer *synthesizer, syneLink *link) override;
	
	/** \brief Link name changed. */
	void LinkNameChanged(syneSynthesizer *synthesizer, syneLink *link) override;
	
	/** \brief Link count or order changed. */
	void LinkStructureChanged(syneSynthesizer *synthesizer) override;
	/*@}*/
};

#endif
