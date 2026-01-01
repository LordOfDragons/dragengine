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

#ifndef _GDEWPSOCFORCEFIELDLISTENER_H_
#define _GDEWPSOCFORCEFIELDLISTENER_H_

#include "../../../gamedef/gdeGameDefinitionListener.h"

class gdeWPSOCForceField;



/**
 * \brief Object class force field panel listener.
 */
class gdeWPSOCForceFieldListener : public gdeGameDefinitionListener{
private:
	gdeWPSOCForceField &pPanel;
	
	
	
public:
	typedef deTObjectReference<gdeWPSOCForceFieldListener> Ref;
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	gdeWPSOCForceFieldListener(gdeWPSOCForceField &panel);
	
	/** \brief Clean up listener. */
protected:
	~gdeWPSOCForceFieldListener() override;
public:
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Object property name changed. */
	void OCPropertyNameChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeProperty *property) override;
	
	/** \brief Object properties changed. */
	void OCPropertiesChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass) override;
	
	/** \brief Active object class changed. */
	void ActiveObjectClassChanged(gdeGameDefinition *gameDefinition) override;
	
	
	
	/** \brief Object class force fields changed. */
	void OCForceFieldsChanged(gdeGameDefinition *gameDefinition, gdeObjectClass *objectClass) override;
	
	/** \brief Object class force field changed. */
	void OCForceFieldChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCForceField *field) override;
	
	/** \brief Active object class force field changed. */
	void ActiveOCForceFieldChanged(gdeGameDefinition *gameDefinition) override;
	/*@}*/
};

#endif
