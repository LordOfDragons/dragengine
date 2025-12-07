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

#ifndef _GDEWPGAMEDEFINITIONLISTENER_H_
#define _GDEWPGAMEDEFINITIONLISTENER_H_

#include "../../gamedef/gdeGameDefinitionListener.h"

class gdeWPGameDefinition;



/**
 * \brief Properties window game definition panel listener.
 */
class gdeWPGameDefinitionListener : public gdeGameDefinitionListener{
private:
	gdeWPGameDefinition &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	gdeWPGameDefinitionListener(gdeWPGameDefinition &panel);
	
	/** \brief Clean up listener. */
	virtual ~gdeWPGameDefinitionListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Game definition changed. */
	virtual void GameDefinitionChanged(gdeGameDefinition *gameDefinition);
	
	/** \brief Base path changed. */
	virtual void BasePathChanged(gdeGameDefinition *gameDefinition);
	
	/** \brief Base game definitions changed. */
	virtual void BaseGameDefinitionsChanged(gdeGameDefinition *gameDefinition);
	
	
	
	/** \brief World properties changed. */
	virtual void WorldPropertiesChanged(gdeGameDefinition *gameDefinition);
	
	/** \brief World changed. */
	virtual void WorldPropertyChanged(gdeGameDefinition *gameDefinition,
		gdeProperty *property);
	
	/** \brief World name changed. */
	virtual void WorldPropertyNameChanged(gdeGameDefinition *gameDefinition,
		gdeProperty *property);
	
	
	
	/** \brief Decal properties changed. */
	virtual void DecalPropertiesChanged(gdeGameDefinition *gameDefinition);
	
	/** \brief Decal changed. */
	virtual void DecalPropertyChanged(gdeGameDefinition *gameDefinition,
		gdeProperty *property);
	
	/** \brief Decal name changed. */
	virtual void DecalPropertyNameChanged(gdeGameDefinition *gameDefinition,
		gdeProperty *property);
	
	
	
	/** \brief Auto find path object classes changed. */
	virtual void AutoFindPathObjectClassesChanged(gdeGameDefinition *gameDefinition);
	
	/** \brief Auto find path skins changed. */
	virtual void AutoFindPathSkinsChanged(gdeGameDefinition *gameDefinition);
	
	/** \brief Auto find path skies changed. */
	virtual void AutoFindPathSkiesChanged(gdeGameDefinition *gameDefinition);
	/*@}*/
};

#endif
