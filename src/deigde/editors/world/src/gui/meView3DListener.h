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

#ifndef _MEVIEW3DLISTENER_H_
#define _MEVIEW3DLISTENER_H_

#include "../world/meWorldNotifier.h"

class meView3D;



/**
 * \brief View 3D Listener
 */
class meView3DListener : public meWorldNotifier{
public:
	typedef deTObjectReference<meView3DListener> Ref;
	
private:
	meView3D &pView3D;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	meView3DListener(meView3D &view3D);
	
protected:
	/** \brief Clean up listener. */
	~meView3DListener() override;
	
public:
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Element or work mode changed. */
	void ModeChanged(meWorld *world) override;
	
	/** \brief Active camera changed. */
	void ActiveCameraChanged(meWorld *world) override;
	/*@}*/
};

#endif
