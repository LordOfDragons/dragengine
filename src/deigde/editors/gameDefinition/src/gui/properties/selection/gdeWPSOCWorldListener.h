/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _GDEWPSOCWORLDLISTENER_H_
#define _GDEWPSOCWORLDLISTENER_H_

#include "../../../gamedef/gdeGameDefinitionListener.h"

class gdeWPSOCWorld;


/**
 * \brief Object class world panel listener.
 */
class gdeWPSOCWorldListener : public gdeGameDefinitionListener{
private:
	gdeWPSOCWorld &pPanel;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	gdeWPSOCWorldListener(gdeWPSOCWorld &panel);
	
protected:
	/** \brief Clean up listener. */
	~gdeWPSOCWorldListener() override = default;
	/*@}*/
	
	
public:
	/** \name Notifications */
	/*@{*/
	/** \brief Object property name changed. */
	void OCPropertyNameChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeProperty *property) override;
	
	/** \brief Object properties changed. */
	void OCPropertiesChanged(gdeGameDefinition *gameDefinition, gdeObjectClass *objectClass ) override;
	
	/** \brief Active object class changed. */
	void ActiveObjectClassChanged(gdeGameDefinition *gameDefinition) override;
	
	
	/** \brief Object class worlds changed. */
	void OCWorldsChanged(gdeGameDefinition *gameDefinition, gdeObjectClass *objectClass) override;
	
	/** \brief Object class world changed. */
	void OCWorldChanged(gdeGameDefinition *gameDefinition, gdeObjectClass *objectClass,
		gdeOCWorld *world) override;
	
	/** \brief Active object class world changed. */
	void ActiveOCWorldChanged(gdeGameDefinition *gameDefinition) override;
	/*@}*/
};

#endif
