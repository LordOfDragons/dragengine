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

#ifndef _GDEMAOCFORCEFIELDCOPY_H_
#define _GDEMAOCFORCEFIELDCOPY_H_

#include "../gdeBaseMAOCSubObject.h"


/**
 * \brief Menu action copy object class.
 */
class gdeMAOCForceFieldCopy : public gdeBaseMAOCSubObject{
public:
	typedef deTObjectReference<gdeMAOCForceFieldCopy> Ref;
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create menu actions. */
	gdeMAOCForceFieldCopy(gdeWindowMain &windowMain);
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Run action if game definition is not nullptr. */
	igdeUndo::Ref OnActionSubObject(gdeGameDefinition &gameDefinition, gdeObjectClass &objectClass) override;
	
	/** \brief Request update of action parameters if required. */
	void Update() override;
	/*@}*/
};

#endif
