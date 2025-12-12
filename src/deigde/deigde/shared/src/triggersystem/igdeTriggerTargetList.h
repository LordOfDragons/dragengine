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

#ifndef _IGDETRIGGERTARGETLIST_H_
#define _IGDETRIGGERTARGETLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class igdeTriggerTarget;



/**
 * \brief Trigger Target List.
 */
class DE_DLL_EXPORT igdeTriggerTargetList{
private:
	decObjectOrderedSet pTargets;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list. */
	igdeTriggerTargetList();
	
	/** \brief Clean up list. */
	~igdeTriggerTargetList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of targets. */
	int GetCount() const;
	
	/** \brief Target at index. */
	igdeTriggerTarget *GetAt(int position) const;
	
	/** \brief Named target or nullptr if absent. */
	igdeTriggerTarget *GetNamed(const char *name) const;
	
	/** \brief Named target adding it if absent. */
	igdeTriggerTarget *GetNamedAddIfMissing(const char *name);
	
	/** \brief Index of target or -1 if absent. */
	int IndexOf(igdeTriggerTarget *target) const;
	
	/** \brief Target is present. */
	bool Has(igdeTriggerTarget *target) const;
	
	/** \brief Named target is present. */
	bool HasNamed(const char *name) const;
	
	/** \brief Add target throwing exception if present. */
	void Add(igdeTriggerTarget *target);
	
	/** \brief Remove target throwing exception if absent. */
	void Remove(igdeTriggerTarget *target);
	
	/** \brief Remove all targets. */
	void RemoveAll();
	
	/** \brief Remove unused targets which are all targets with reference count of 1. */
	void RemoveUnused();
	/*@}*/
};

#endif
