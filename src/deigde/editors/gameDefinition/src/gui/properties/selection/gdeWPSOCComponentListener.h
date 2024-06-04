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

#ifndef _GDEWPSOCCOMPONENTLISTENER_H_
#define _GDEWPSOCCOMPONENTLISTENER_H_

#include "../../../gamedef/gdeGameDefinitionListener.h"

class gdeWPSOCComponent;



/**
 * \brief Object class component panel listener.
 */
class gdeWPSOCComponentListener : public gdeGameDefinitionListener{
private:
	gdeWPSOCComponent &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	gdeWPSOCComponentListener( gdeWPSOCComponent &panel );
	
	/** \brief Clean up listener. */
	virtual ~gdeWPSOCComponentListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Base path changed. */
	virtual void BasePathChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief Base game definitions changed. */
	virtual void BaseGameDefinitionsChanged( gdeGameDefinition *gameDefinition );
	
	
	
	/** \brief Object property name changed. */
	virtual void OCPropertyNameChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeProperty *property );
	
	/** \brief Object properties changed. */
	virtual void OCPropertiesChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** \brief Object texture property name changed. */
	virtual void OCTexturePropertyNameChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeProperty *property );
	
	/** \brief Object texture properties changed. */
	virtual void OCTexturePropertiesChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** \brief Active object class changed. */
	virtual void ActiveObjectClassChanged( gdeGameDefinition *gameDefinition );
	
	
	
	/** \brief Object class components changed. */
	virtual void OCComponentsChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** \brief Object class component changed. */
	virtual void OCComponentChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCComponent *component );
	
	/** \brief Object class component active texture changed. */
	virtual void OCComponentActiveTextureChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCComponent *component );
	
	/** \brief Object class component texture changed. */
	virtual void OCComponentTextureChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCComponent *component, gdeOCComponentTexture *texture );
	
	/** \brief Object class component texture name changed. */
	virtual void OCComponentTextureNameChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCComponent *component, gdeOCComponentTexture *texture );
	
	/** \brief Object class component texture properties changed. */
	virtual void OCComponentTexturePropertiesChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCComponent *component, gdeOCComponentTexture *texture );
	
	/** \brief Active object class component changed. */
	virtual void ActiveOCComponentChanged( gdeGameDefinition *gameDefinition );
	/*@}*/
};

#endif
