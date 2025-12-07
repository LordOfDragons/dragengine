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
	gdeWPSOCComponentListener(gdeWPSOCComponent &panel);
	
	/** \brief Clean up listener. */
	~gdeWPSOCComponentListener() override;
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Base path changed. */
	void BasePathChanged(gdeGameDefinition *gameDefinition) override;
	
	/** \brief Base game definitions changed. */
	void BaseGameDefinitionsChanged(gdeGameDefinition *gameDefinition) override;
	
	
	
	/** \brief Object property name changed. */
	void OCPropertyNameChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeProperty *property) override;
	
	/** \brief Object properties changed. */
	void OCPropertiesChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass) override;
	
	/** \brief Object texture property name changed. */
	void OCTexturePropertyNameChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeProperty *property) override;
	
	/** \brief Object texture properties changed. */
	void OCTexturePropertiesChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass) override;
	
	/** \brief Active object class changed. */
	void ActiveObjectClassChanged(gdeGameDefinition *gameDefinition) override;
	
	
	
	/** \brief Object class components changed. */
	void OCComponentsChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass) override;
	
	/** \brief Object class component changed. */
	void OCComponentChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCComponent *component) override;
	
	/** \brief Object class component active texture changed. */
	void OCComponentActiveTextureChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCComponent *component) override;
	
	/** \brief Object class component texture changed. */
	void OCComponentTextureChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCComponent *component, gdeOCComponentTexture *texture) override;
	
	/** \brief Object class component texture name changed. */
	void OCComponentTextureNameChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCComponent *component, gdeOCComponentTexture *texture) override;
	
	/** \brief Object class component texture properties changed. */
	void OCComponentTexturePropertiesChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCComponent *component, gdeOCComponentTexture *texture) override;
	
	/** \brief Active object class component changed. */
	void ActiveOCComponentChanged(gdeGameDefinition *gameDefinition) override;
	/*@}*/
};

#endif
