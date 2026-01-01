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

#ifndef _SYNECONTROLLERTARGET_H_
#define _SYNECONTROLLERTARGET_H_

#include "../link/syneLink.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTOrderedSet.h>

class syneSynthesizer;
class deSynthesizerControllerTarget;



/**
 * Links for controller target.
 */
class syneControllerTarget : public deObject{
public:
	/** Type holding strong reference. */
	typedef deTObjectReference<syneControllerTarget> Ref;
	
	/** Type holding list of objects. */
	typedef decTObjectOrderedSet<syneControllerTarget> List;


private:
	syneLink::List pLinks;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create controller target. */
	syneControllerTarget();
	
	/** Create copy of controller target. */
	syneControllerTarget(const syneControllerTarget &copy);
	
	/** Clean up controller target. */
protected:
	~syneControllerTarget();
public:
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Links. */
	const syneLink::List &GetLinks() const{ return pLinks; }
	
	/** Add link. */
	void AddLink(syneLink *link);
	
	/** Remove link. */
	void RemoveLink(syneLink *link);
	
	/** Remove all links. */
	void RemoveAllLinks();
	
	/** Update engine target. */
	void UpdateEngineTarget(const syneSynthesizer &synthesizer, deSynthesizerControllerTarget &target) const;
	
	/** Add links to list of links. */
	void AddLinksToList(syneLink::List &list);
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** Set from another controller target. */
	syneControllerTarget &operator=(const syneControllerTarget &copy);
	/*@}*/
};

#endif
