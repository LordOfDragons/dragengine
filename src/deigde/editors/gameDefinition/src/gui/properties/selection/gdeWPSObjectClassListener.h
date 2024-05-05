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

#ifndef _GDEWPSOBJECTLISTENER_H_
#define _GDEWPSOBJECTLISTENER_H_

#include "../../../gamedef/gdeGameDefinitionListener.h"

class gdeWPSObjectClass;



/**
 * \brief Object class panel listener.
 */
class gdeWPSObjectClassListener : public gdeGameDefinitionListener{
private:
	gdeWPSObjectClass &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	gdeWPSObjectClassListener( gdeWPSObjectClass &panel );
	
	/** \brief Clean up listener. */
	virtual ~gdeWPSObjectClassListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Object class used tags changed. */
	virtual void ObjectClassUsedTagsChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief Object class categories changed. */
	virtual void ObjectClassCategoriesChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief Base path changed. */
	virtual void BasePathChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief Base game definitions changed. */
	virtual void BaseGameDefinitionsChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief Defined and used identifiers changed. */
	virtual void DefinedUsedIDsChanged( gdeGameDefinition *gameDefinition );
	
	
	
	/** Object class count or order changed. */
	virtual void ObjectClassStructureChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief Object class changed. */
	virtual void ObjectClassChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** \brief Object class name changed. */
	virtual void ObjectClassNameChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** \brief Object property changed. */
	virtual void OCPropertyChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeProperty *property );
	
	/** \brief Object property name changed. */
	virtual void OCPropertyNameChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeProperty *property );
	
	/** \brief Object properties changed. */
	virtual void OCPropertiesChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** \brief Object property values changed. */
	virtual void OCPropertyValuesChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** \brief Object texture property changed. */
	virtual void OCTexturePropertyChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeProperty *property );
	
	/** \brief Object texture property name changed. */
	virtual void OCTexturePropertyNameChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeProperty *property );
	
	/** \brief Object texture properties changed. */
	virtual void OCTexturePropertiesChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** \brief Active object class changed. */
	virtual void ActiveObjectClassChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief Object class inherits changed. */
	virtual void OCInheritsChanged( gdeGameDefinition *gameDefinition, gdeObjectClass *objectClass );
	
	/** \brief Object class inherit changed. */
	virtual void OCInheritChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCInherit *inherit );
	
	/** Object class textures changed. */
	virtual void OCTexturesChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** Object class tzexture changed. */
	virtual void OCTextureChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCComponentTexture *texture );
	
	/** Object class active texture changed. */
	virtual void OCActiveTextureChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	/*@}*/
};

#endif
