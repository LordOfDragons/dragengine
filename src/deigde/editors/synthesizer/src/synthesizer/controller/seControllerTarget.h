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

#ifndef _SECONTROLLERTARGET_H_
#define _SECONTROLLERTARGET_H_

#include "../link/seLink.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTOrderedSet.h>

class seSynthesizer;
class deSynthesizerControllerTarget;



/**
 * Links for controller target.
 */
class seControllerTarget : public deObject{
public:
	/** Type holding strong reference. */
	typedef deTObjectReference<seControllerTarget> Ref;
	
	/** Type holding list of objects. */
	typedef decTObjectOrderedSet<seControllerTarget> List;


private:
	seLink::List pLinks;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create controller target. */
	seControllerTarget();
	
	/** Create copy of controller target. */
	seControllerTarget(const seControllerTarget &copy);
	
	/** Clean up controller target. */
	~seControllerTarget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Links. */
	const seLink::List &GetLinks() const{ return pLinks; }
	
	/** Add link. */
	void AddLink(seLink *link);
	
	/** Remove link. */
	void RemoveLink(seLink *link);
	
	/** Remove all links. */
	void RemoveAllLinks();
	
	/** Update engine target. */
	void UpdateEngineTarget(const seSynthesizer &synthesizer, deSynthesizerControllerTarget &target) const;
	
	/** Add links to list of links. */
	void AddLinksToList(seLink::List &list);
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** Set from another controller target. */
	seControllerTarget &operator=(const seControllerTarget &copy);
	/*@}*/
};

#endif
