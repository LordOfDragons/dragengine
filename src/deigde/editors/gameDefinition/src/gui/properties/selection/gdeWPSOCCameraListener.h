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

#ifndef _GDEWPSOCCAMERALISTENER_H_
#define _GDEWPSOCCAMERALISTENER_H_

#include "../../../gamedef/gdeGameDefinitionListener.h"

class gdeWPSOCCamera;



/**
 * \brief Object class camera panel listener.
 */
class gdeWPSOCCameraListener : public gdeGameDefinitionListener{
private:
	gdeWPSOCCamera &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	gdeWPSOCCameraListener(gdeWPSOCCamera &panel);
	
	/** \brief Clean up listener. */
	virtual ~gdeWPSOCCameraListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Object property name changed. */
	virtual void OCPropertyNameChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeProperty *property);
	
	/** \brief Object properties changed. */
	virtual void OCPropertiesChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass);
	
	/** \brief Active object class changed. */
	virtual void ActiveObjectClassChanged(gdeGameDefinition *gameDefinition);
	
	
	
	
	/** \brief Object class cameras changed. */
	virtual void OCCamerasChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass);
	
	/** \brief Object class camera changed. */
	virtual void OCCameraChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCCamera *camera);
	
	/** \brief Active object class camera changed. */
	virtual void ActiveOCCameraChanged(gdeGameDefinition *gameDefinition);
	/*@}*/
};

#endif
