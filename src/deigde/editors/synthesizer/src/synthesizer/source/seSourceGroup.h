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

#include <dragengine/resources/synthesizer/source/deSynthesizerSourceGroup.h>



/**
 * \brief Synthesizer source group.
 */
class seSourceGroup : public seSource{
public:
	typedef deTObjectReference<seSourceGroup> Ref;
	
private:
	seSource::List pSources;
	
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
	~seSourceGroup() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Sources. */
	inline const seSource::List &GetSources() const{ return pSources; }
	
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
	virtual deSynthesizerSource::Ref CreateEngineSource();
	
	/** \brief Update targets. */
	void UpdateTargets() override;
	
	/** \brief Retrieve the number of targets using a given link. */
	int CountLinkUsage(seLink *link) const override;
	
	/** \brief Removes a link from all targets using it. */
	void RemoveLinkFromTargets(seLink *link) override;
	
	/** \brief Removes all links from all targets. */
	void RemoveLinksFromAllTargets() override;
	
	/** \brief Create a copy of this source. */
	seSource::Ref CreateCopy() const override;
	
	/** \brief List all links of all source targets. */
	void ListLinks(seLink::List& list) override;
	
	
	
	/** \brief Tree list expanded state. */
	inline bool GetTreeListExpanded() const{ return pTreeListExpanded; }
	
	/** \brief Set tree list expanded state. */
	void SetTreeListExpanded(bool expanded);
	
	/** \brief Parent synthesizer changed. */
	void SynthesizerChanged() override;
	
	/** \brief Synthesizer directory changed. */
	void SynthesizerDirectoryChanged() override;
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
