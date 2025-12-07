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

#ifndef _DESYNTHESIZERGROUPGROUP_H_
#define _DESYNTHESIZERGROUPGROUP_H_

#include "deSynthesizerSource.h"
#include "../../../common/collection/decObjectOrderedSet.h"


/**
 * \brief Synthesizer group source.
 */
class DE_DLL_EXPORT deSynthesizerSourceGroup : public deSynthesizerSource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deSynthesizerSourceGroup> Ref;
	
	
public:
	/** \brief Application types. */
	enum eApplicationTypes{
		/** \brief Output the mix of all sources in the group. */
		eatAll,
		
		/**
		 * \brief Use selection target to output blend between two selected sources.
		 * A target vlaue of 0 plays solo the first source in the group.
		 * A target value of 1 plays solo the last source in the group.
		 * Blends across all other sources in between in a linear way.
		 * To simulate blending over to the first source duplicate the
		 * first source also as last source.
		 */
		eatSelect,
		
		/**
		 * \brief Use selection target to output selected sources solo.
		 * Similar to eatSelect but does not blend between the two nearest
		 * sources but outputs only the nearest source. Source is chosen
		 * in a linear way.
		 */
		eatSolo
	};
	
	
	
private:
	decObjectOrderedSet pSources;
	
	eApplicationTypes pApplicationType;
	
	deSynthesizerControllerTarget pTargetSelect;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates synthesizer source. */
	deSynthesizerSourceGroup();
	
protected:
	/**
	 * \brief Clean up synthesizer source.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	~deSynthesizerSourceGroup() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Number of sources. */
	int GetSourceCount() const;
	
	/** \brief Source at index. */
	deSynthesizerSource *GetSourceAt(int index) const;
	
	/** \brief Index of source or -1 if absent. */
	int IndexOfSource(deSynthesizerSource *source) const;
	
	/** \brief Source is present. */
	bool HasSource(deSynthesizerSource *source) const;
	
	/** \brief Add source. */
	void AddSource(deSynthesizerSource *source);
	
	/** \brief Remove source. */
	void RemoveSource(deSynthesizerSource *source);
	
	/** \brief Remove all sources. */
	void RemoveAllSources();
	
	
	
	/** \brief Source application type. */
	inline eApplicationTypes GetApplicationType() const{ return pApplicationType; }
	
	/** \brief Set source application type. */
	void SetApplicationType(eApplicationTypes type);
	
	/** \brief Select target. */
	inline deSynthesizerControllerTarget &GetTargetSelect(){ return pTargetSelect; }
	inline const deSynthesizerControllerTarget &GetTargetSelect() const{ return pTargetSelect; }
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visits the source. */
	void Visit(deSynthesizerSourceVisitor &visitor) override;
	/*@}*/
};

#endif
