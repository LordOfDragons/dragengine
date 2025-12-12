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

// include only once
#ifndef _IGDESMPROPFIELD_H_
#define _IGDESMPROPFIELD_H_

// includes
#include <dragengine/systems/modules/scripting/deBaseScriptingPropField.h>



/**
 * @brief IGDE Script Module Prop Field Peer.
 *
 * Prop Field peer for the IGDRE Script Module. If a delegee is assigned
 * all events are forwarded to the delegee.
 */
class igdeSMPropField : public deBaseScriptingPropField{
private:
	deBaseScriptingPropField *pDelegee;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new gui peer. */
	igdeSMPropField();
	/** Cleans up the peer. */
	virtual ~igdeSMPropField();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the delegee or nullptr. */
	inline deBaseScriptingPropField *GetDelegee() const{ return pDelegee; }
	/** Sets the delegee or nullptr. */
	void SetDelegee(deBaseScriptingPropField *delegee);
	/*@}*/
	
	/** @name Notifications */
	/*@{*/
	/**
	 * Requests the game scripts to fill the prop field with instances. The density
	 * indicates the percentage of instances to use compared to a fully populated
	 * prop field.
	 */
	virtual void CreateInstances(float density);
	/*@}*/
};

// end of include only once
#endif
