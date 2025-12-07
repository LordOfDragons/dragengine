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

#ifndef _DEDSPROPFIELD_H_
#define _DEDSPROPFIELD_H_

#include <dragengine/systems/modules/scripting/deBaseScriptingPropField.h>

class deScriptingDragonScript;
class dePropField;
class dsValue;
class dsRealObject;



/**
 * \brief Prop field peer.
 */
class dedsPropField : public deBaseScriptingPropField{
private:
	deScriptingDragonScript *pDS;
	dePropField *pPropField;
	dsValue *pValCB;
	bool pHasCB;
	deBaseScriptingPropField *pDelegee;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new peer. */
	dedsPropField(deScriptingDragonScript *ds, dePropField *propField);
	/** \brief Cleans up the peer. */
	virtual ~dedsPropField();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the callback object. */
	dsRealObject *GetCallback() const;
	/** \brief Sets the callback object. */
	void SetCallback(dsRealObject *object);
	/** \brief Retrieves the delegee or NULL. */
	inline deBaseScriptingPropField *GetDelegee() const{ return pDelegee; }
	/** \brief Sets the delegee or NULL. */
	void SetDelegee(deBaseScriptingPropField *delegee);
	/*@}*/
	
	/** \name Notifications */
	/*@{*/
	/**
	 * \brief Requests the game scripts to fill the prop field with instances.
	 * \details The density indicates the percentage of instances to use compared
	 *          to a fully populated prop field.
	 */
	virtual void CreateInstances(float density);
	/*@}*/
};

#endif
