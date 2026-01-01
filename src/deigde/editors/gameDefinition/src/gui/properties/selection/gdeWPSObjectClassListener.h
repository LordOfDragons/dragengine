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
	typedef deTObjectReference<gdeWPSObjectClassListener> Ref;
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	gdeWPSObjectClassListener(gdeWPSObjectClass &panel);
	
	/** \brief Clean up listener. */
protected:
	~gdeWPSObjectClassListener() override;
public:
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Object class used tags changed. */
	void ObjectClassUsedTagsChanged(gdeGameDefinition *gameDefinition) override;
	
	/** \brief Object class categories changed. */
	void ObjectClassCategoriesChanged(gdeGameDefinition *gameDefinition) override;
	
	/** \brief Base path changed. */
	void BasePathChanged(gdeGameDefinition *gameDefinition) override;
	
	/** \brief Base game definitions changed. */
	void BaseGameDefinitionsChanged(gdeGameDefinition *gameDefinition) override;
	
	/** \brief Defined and used identifiers changed. */
	void DefinedUsedIDsChanged(gdeGameDefinition *gameDefinition) override;
	
	
	
	/** Object class count or order changed. */
	void ObjectClassStructureChanged(gdeGameDefinition *gameDefinition) override;
	
	/** \brief Object class changed. */
	void ObjectClassChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass) override;
	
	/** \brief Object class name changed. */
	void ObjectClassNameChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass) override;
	
	/** \brief Object property changed. */
	void OCPropertyChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeProperty *property) override;
	
	/** \brief Object property name changed. */
	void OCPropertyNameChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeProperty *property) override;
	
	/** \brief Object properties changed. */
	void OCPropertiesChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass) override;
	
	/** \brief Object property values changed. */
	void OCPropertyValuesChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass) override;
	
	/** \brief Object texture property changed. */
	void OCTexturePropertyChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeProperty *property) override;
	
	/** \brief Object texture property name changed. */
	void OCTexturePropertyNameChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeProperty *property) override;
	
	/** \brief Object texture properties changed. */
	void OCTexturePropertiesChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass) override;
	
	/** \brief Active object class changed. */
	void ActiveObjectClassChanged(gdeGameDefinition *gameDefinition) override;
	
	/** \brief Object class inherits changed. */
	void OCInheritsChanged(gdeGameDefinition *gameDefinition, gdeObjectClass *objectClass) override;
	
	/** \brief Object class inherit changed. */
	void OCInheritChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCInherit *inherit) override;
	
	/** Object class textures changed. */
	void OCTexturesChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass) override;
	
	/** Object class tzexture changed. */
	void OCTextureChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCComponentTexture *texture) override;
	
	/** Object class active texture changed. */
	void OCActiveTextureChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass) override;
	/*@}*/
};

#endif
