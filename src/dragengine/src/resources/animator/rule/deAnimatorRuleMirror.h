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

#ifndef _DEANIMATORRULEMIRROR_H_
#define _DEANIMATORRULEMIRROR_H_

#include "deAnimatorRule.h"
#include "../../../common/string/decString.h"


/**
 * \brief Animator Mirror Rule Class.
 * \version 1.9
 *
 * The mirror rule mirrors the state between two related bones. This is typically used to
 * mirror the left and right bone states to reuse animations. Bones not covered by the name
 * matching rules are mirrored in place. Multiple name matching rules can be defined each
 * applying to a group of bones. Typically one name matching rule is required but multiple
 * name matching rules can be added if required.
 */
class DE_DLL_EXPORT deAnimatorRuleMirror : public deAnimatorRule{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deAnimatorRuleMirror> Ref;
	
	
public:
	/** \brief Mirror axis. */
	enum eMirrorAxis{
		/** \brief Mirror along X axis. */
		emaX,
		
		/** \brief Mirror along Y axis. */
		emaY,
		
		/** \brief Mirror along Z axis. */
		emaZ
	};
	
	/** \brief Match name type. */
	enum eMatchNameType{
		/** \brief Match at begin of bone name. */
		emntFirst,
		
		/** \brief Match at end of bone name. */
		emntLast,
		
		/** \brief Match in the middle of bone name. */
		emntMiddle
	};
	
	/** \brief Match name. */
	struct sMatchName{
		/** \brief First bone name component to match. */
		decString first;
		
		/** \brief Second bone name component to match. */
		decString second;
		
		/** \brief Match type. */
		eMatchNameType type;
	};
	
	
	
private:
	eMirrorAxis pMirrorAxis;
	decString pMirrorBone;
	sMatchName *pMatchNames;
	int pMatchNameCount;
	bool pEnablePosition;
	bool pEnableOrientation;
	bool pEnableSize;
	bool pEnableVertexPositionSet;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new animator rule. */
	deAnimatorRuleMirror();
	
protected:
	/**
	 * \brief Clean up animator rule.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deAnimatorRuleMirror();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Mirror axis. */
	inline eMirrorAxis GetMirrorAxis() const{ return pMirrorAxis; }
	
	/** \brief Set mirror axis. */
	void SetMirrorAxis(eMirrorAxis axis);
	
	/** \brief Name of mirror bone or empty string to use component. */
	inline const decString &GetMirrorBone() const{ return pMirrorBone; }
	
	/** \brief Set name of mirror bone or empty string to use component. */
	void SetMirrorBone(const char *boneName);
	
	/** \brief Count of match names. */
	inline int GetMatchNameCount() const{ return pMatchNameCount; }
	
	/** \brief Match name at index. */
	const sMatchName &GetMatchNameAt(int index) const;
	
	/** \brief Add match name. */
	void AddMatchName(const char *first, const char *second, eMatchNameType type);
	
	/** \brief Remove all match names. */
	void RemoveAllMatchNames();
	
	/** \brief Determines if position manipulation is enabled. */
	inline bool GetEnablePosition() const{ return pEnablePosition; }
	
	/** \brief Sets if position manipulation is enabled. */
	void SetEnablePosition(bool enabled);
	
	/** \brief Determines if orientation manipulation is enabled. */
	inline bool GetEnableOrientation() const{ return pEnableOrientation; }
	
	/** \brief Sets if orientation manipulation is enabled. */
	void SetEnableOrientation(bool enabled);
	
	/** \brief Determines if size manipulation is enabled. */
	inline bool GetEnableSize() const{ return pEnableSize; }
	
	/** \brief Sets if size manipulation is enabled. */
	void SetEnableSize(bool enabled);
	
	/** \brief Vertex position sets are enabled. */
	inline bool GetEnableVertexPositionSet() const{ return pEnableVertexPositionSet; }
	
	/** \brief Set if vertex position sets are enabled. */
	void SetEnableVertexPositionSet(bool enabled);
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visits the rule. */
	virtual void Visit(deAnimatorRuleVisitor &visitor);
	/*@}*/
};

#endif
