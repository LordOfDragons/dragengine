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

#ifndef _SEWPVIEWLISTENER_H_
#define _SEWPVIEWLISTENER_H_

#include "../../sky/seSkyListener.h"

class seWPView;



/**
 * \brief View panel listener.
 */
class seWPViewListener : public seSkyListener{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<seWPViewListener> Ref;
	
	

private:
	seWPView &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	seWPViewListener(seWPView &panel);
	
	/** \brief Clean up listener. */
	~seWPViewListener() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Environment component changed. */
	void EnvObjectChanged(seSky *sky) override;
	
	/** \brief View changed. */
	void ViewChanged(seSky *sky) override;
	
	/** \brief Camera changed. */
	void CameraChanged(seSky *sky) override;
	/*@}*/
};

#endif
