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

#ifndef _SEWPLINKLISTENER_H_
#define _SEWPLINKLISTENER_H_

#include "../../sky/seSkyListener.h"

class seWPLink;


/**
 * \brief Link properties panel listener.
 */
class seWPLinkListener : public seSkyListener{
private:
	seWPLink &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	seWPLinkListener(seWPLink &panel);
	
	/** \brief Clean up listener. */
	~seWPLinkListener() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Controller added or removed. */
	void ControllerStructureChanged (seSky *sky) override;
	
	/** \brief Controller name changed. */
	void ControllerNameChanged(seSky *sky, seController *controller) override;
	
	/** \brief Link added or removed. */
	void LinkStructureChanged (seSky *sky) override;
	
	/** \brief Link changed. */
	void LinkChanged(seSky *sky, seLink *link) override;
	
	/** \brief Link name changed. */
	void LinkNameChanged(seSky *sky, seLink *link) override;
	
	/** \brief Active link changed. */
	void ActiveLinkChanged(seSky *sky) override;
	/*@}*/
};

// end of include only once
#endif
