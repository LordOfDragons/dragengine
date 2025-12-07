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

#ifndef _SESOURCEGROUP_H_
#define _SESOURCEGROUP_H_

#include "seSource.h"
#include "seSourceList.h"

#include <dragengine/resources/synthesizer/source/deSynthesizerSourceGroup.h>



/**
 * \brief Synthesizer source group.
 */
class seSourceGroup : public seSource{
private:
	seSourceList pSources;
	
	deSynthesizerSourceGroup::eApplicationTypes pApplicationType;
	
	seControllerTarget pTargetSelect;
	
	bool pTreeListExpanded;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create source. */
	seSourceGroup();
	
	/** \brief Create copy of source. */
	seSourceGroup(const seSourceGroup &copy);
	
	/** \brief Clean up source. */
	virtual ~seSourceGroup();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Sources. */
	inline const seSourceList &GetSources() const{ return pSources; }
	
	/** \brief Add source. */
	void AddSource(seSource *source);
	
	/** \brief Insert source. */
	void InsertSourceAt(seSource *source, int index);
	
	/** \brief Move source. */
	void MoveSourceTo(seSource *source, int index);
	
	/** \brief Remove source. */
	void RemoveSource(seSource *source);
	
	/** \brief Remove all sources. */
	void RemoveAllSources();
	
	
	
	/** \brief Application type. */
	inline deSynthesizerSourceGroup::eApplicationTypes GetApplicationType() const{ return pApplicationType; }
	
	/** \brief Set application type. */
	void SetApplicationType(deSynthesizerSourceGroup::eApplicationTypes type);
	
	/** \brief Select target. */
	inline seControllerTarget &GetTargetSelect(){ return pTargetSelect; }
	inline const seControllerTarget &GetTargetSelect() const{ return pTargetSelect; }
	
	
	
	/** \brief Create an engine synthesizer source. */
	virtual deSynthesizerSource *CreateEngineSource();
	
	/** \brief Update targets. */
	virtual void UpdateTargets();
	
	/** \brief Retrieve the number of targets using a given link. */
	virtual int CountLinkUsage(seLink *link) const;
	
	/** \brief Removes a link from all targets using it. */
	virtual void RemoveLinkFromTargets(seLink *link);
	
	/** \brief Removes all links from all targets. */
	virtual void RemoveLinksFromAllTargets();
	
	/** \brief Create a copy of this source. */
	virtual seSource *CreateCopy() const;
	
	/** \brief List all links of all source targets. */
	virtual void ListLinks(seLinkList& list);
	
	
	
	/** \brief Tree list expanded state. */
	inline bool GetTreeListExpanded() const{ return pTreeListExpanded; }
	
	/** \brief Set tree list expanded state. */
	void SetTreeListExpanded(bool expanded);
	
	/** \brief Parent synthesizer changed. */
	virtual void SynthesizerChanged();
	
	/** \brief Synthesizer directory changed. */
	virtual void SynthesizerDirectoryChanged();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copies another group source to this group source. */
	virtual seSourceGroup &operator=(const seSourceGroup &copy);
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
