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

#ifndef _SYNESOURCEGROUP_H_
#define _SYNESOURCEGROUP_H_

#include "syneSource.h"

#include <dragengine/resources/synthesizer/source/deSynthesizerSourceGroup.h>



/**
 * \brief Synthesizer source group.
 */
class syneSourceGroup : public syneSource{
public:
	typedef deTObjectReference<syneSourceGroup> Ref;
	
private:
	syneSource::List pSources;
	
	deSynthesizerSourceGroup::eApplicationTypes pApplicationType;
	
	syneControllerTarget::Ref pTargetSelect;
	
	bool pTreeListExpanded;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create source. */
	syneSourceGroup();
	
	/** \brief Create copy of source. */
	syneSourceGroup(const syneSourceGroup &copy);
	
	/** \brief Clean up source. */
protected:
	~syneSourceGroup() override;
public:
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Sources. */
	inline const syneSource::List &GetSources() const{ return pSources; }
	
	/** \brief Add source. */
	void AddSource(syneSource *source);
	
	/** \brief Insert source. */
	void InsertSourceAt(syneSource *source, int index);
	
	/** \brief Move source. */
	void MoveSourceTo(syneSource *source, int index);
	
	/** \brief Remove source. */
	void RemoveSource(syneSource *source);
	
	/** \brief Remove all sources. */
	void RemoveAllSources();
	
	
	
	/** \brief Application type. */
	inline deSynthesizerSourceGroup::eApplicationTypes GetApplicationType() const{ return pApplicationType; }
	
	/** \brief Set application type. */
	void SetApplicationType(deSynthesizerSourceGroup::eApplicationTypes type);
	
	/** \brief Select target. */
	inline const syneControllerTarget::Ref &GetTargetSelect() const{ return pTargetSelect; }
	
	
	
	/** \brief Create an engine synthesizer source. */
	virtual deSynthesizerSource::Ref CreateEngineSource();
	
	/** \brief Update targets. */
	void UpdateTargets() override;
	
	/** \brief Retrieve the number of targets using a given link. */
	int CountLinkUsage(syneLink *link) const override;
	
	/** \brief Removes a link from all targets using it. */
	void RemoveLinkFromTargets(syneLink *link) override;
	
	/** \brief Removes all links from all targets. */
	void RemoveLinksFromAllTargets() override;
	
	/** \brief Create a copy of this source. */
	syneSource::Ref CreateCopy() const override;
	
	/** \brief List all links of all source targets. */
	void ListLinks(syneLink::List& list) override;
	
	
	
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
	virtual syneSourceGroup &operator=(const syneSourceGroup &copy);
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
