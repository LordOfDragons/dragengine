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

#ifndef _AERULEMIRROR_H_
#define _AERULEMIRROR_H_

#include "aeRule.h"

#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleMirror.h>



/**
 * Mirror rule.
 */
class aeRuleMirror : public aeRule{
public:
	typedef deTObjectReference<aeRuleMirror> Ref;
	
	
	class MatchName : public deObject {
	public:
		typedef deTObjectReference<MatchName> Ref;
		typedef decTObjectOrderedSet<MatchName> List;
		
		const decString first;
		const decString second;
		const deAnimatorRuleMirror::eMatchNameType type;
		
		MatchName(const char *first, const char *second, deAnimatorRuleMirror::eMatchNameType type);
		
		bool operator==(const MatchName &matchName) const;
		bool operator!=(const MatchName &matchName) const;
	};
	
	
	
private:
	deAnimatorRuleMirror::eMirrorAxis pMirrorAxis;
	decString pMirrorBone;
	MatchName::List pMatchNames;
	bool pEnablePosition;
	bool pEnableOrientation;
	bool pEnableSize;
	bool pEnableVertexPositionSet;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create rule. */
	aeRuleMirror();
	
	/** Create copy of rule. */
	aeRuleMirror(const aeRuleMirror &copy);
	
	/** Create rule with default settings. */
	static aeRuleMirror::Ref CreateDefault();
	
protected:
	/** Clean up rule. */
	~aeRuleMirror() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Mirror axis. */
	inline deAnimatorRuleMirror::eMirrorAxis GetMirrorAxis() const{ return pMirrorAxis; }
	
	/** Set mirror axis. */
	void SetMirrorAxis(deAnimatorRuleMirror::eMirrorAxis axis);
	
	/** Name of mirror bone or empty string to use component. */
	inline const decString &GetMirrorBone() const{ return pMirrorBone; }
	
	/** Set name of mirror bone or empty string to use component. */
	void SetMirrorBone(const char *boneName);
	
	/** Match names. */
	const MatchName::List &GetMatchNames() const{ return pMatchNames; }
	
	/** Add match name. */
	void AddMatchName(aeRuleMirror::MatchName *matchName);
	
	/** Insert match name. */
	void InsertMatchName(aeRuleMirror::MatchName *matchName, int index);
	
	/** Set match name at index. */
	void SetMatchNameAt(int index, aeRuleMirror::MatchName *matchName);
	
	/** Remove match name. */
	void RemoveMatchName(MatchName *matchName);
	
	/** Remove all match names. */
	void RemoveAllMatchNames();
	
	/** Position manipulation is enabled. */
	inline bool GetEnablePosition() const{ return pEnablePosition; }
	
	/** Set if position manipulation is enabled. */
	void SetEnablePosition(bool enabled);
	
	/** Orientation manipulation is enabled. */
	inline bool GetEnableOrientation() const{ return pEnableOrientation; }
	
	/** Set if orientation manipulation is enabled. */
	void SetEnableOrientation(bool enabled);
	
	/** Size manipulation is enabled. */
	inline bool GetEnableSize() const{ return pEnableSize; }
	
	/** Set if size manipulation is enabled. */
	void SetEnableSize(bool enabled);
	
	/** Vertex position set manipulation is enabled. */
	inline bool GetEnableVertexPositionSet() const{ return pEnableVertexPositionSet; }
	
	/** Set if vertex position set manipulation is enabled. */
	void SetEnableVertexPositionSet(bool enabled);
	
	
	
	/** Create engine animator rule. */
	deAnimatorRule::Ref CreateEngineRule() override;
	
	/** Create copy of rule. */
	aeRule::Ref CreateCopy() const override;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** Copy from another rule. */
	virtual aeRuleMirror &operator=(const aeRuleMirror &copy);
	/*@}*/
	
	
	
private:
	void pUpdateMatchNames();
};

#endif
