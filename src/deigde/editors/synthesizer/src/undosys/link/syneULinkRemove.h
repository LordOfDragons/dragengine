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

#ifndef _SYNEULINKREMOVE_H_
#define _SYNEULINKREMOVE_H_

#include "../../synthesizer/link/syneLink.h"
#include "../../synthesizer/source/syneSource.h"
#include "../../synthesizer/syneSynthesizer.h"

#include <deigde/undo/igdeUndo.h>

class syneControllerTarget;



/**
 * \brief Undo action remove link.
 */
class syneULinkRemove : public igdeUndo{
public:
	typedef deTObjectReference<syneULinkRemove> Ref;
	
	
private:
	class cTarget : public deObject{
	public:
		typedef deTObjectReference<cTarget> Ref;
		typedef decTObjectOrderedSet<cTarget> List;
		
		syneSource::Ref source;
		syneControllerTarget::Ref target;
		
		cTarget() = default;
	};
	
	syneSynthesizer::Ref pSynthesizer;
	syneLink::Ref pLink;
	cTarget::List pTargets;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo action. */
	explicit syneULinkRemove(syneLink *link);
	
protected:
	/** \brief Clean up undo action. */
	virtual ~syneULinkRemove();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Undo action. */
	virtual void Undo();
	
	/** \brief Redo action. */
	virtual void Redo();
	/*@}*/
	
	
	
private:
	void pAddTargetsForSource(syneLink *link, int targetCount, const syneSource::Ref &source);
	void pAddTarget(int targetCount, syneSource *source, syneControllerTarget *target);
};

#endif
