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
#ifndef _RERIGBUILDER_H_
#define _RERIGBUILDER_H_

// includes
#include "dragengine/resources/rig/deRigBuilder.h"

// predefinitions
class reRig;
class reRigBone;



/**
 * @brief Rig Builder.
 * Builds an engine rig from an editor rig.
 */
class reRigBuilder : public deRigBuilder{
private:
	reRig *pRig;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new rig builder. */
	reRigBuilder( reRig *rig );
	/** Cleans up the rig. */
	virtual ~reRigBuilder();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Build rig into the provided rig object. */
	virtual void BuildRig( deRig *engRig );
	/** Build rig bone. */
	void BuildRigBone( deRig *engRig, reRigBone *rigBone );
	/*@}*/
};

// end of include only once
#endif
