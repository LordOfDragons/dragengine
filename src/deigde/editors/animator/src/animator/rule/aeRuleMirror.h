/* 
 * Drag[en]gine IGDE Animator Editor
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _AERULEMIRROR_H_
#define _AERULEMIRROR_H_

#include "aeRule.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleMirror.h>



/**
 * Mirror rule.
 */
class aeRuleMirror : public aeRule{
public:
	typedef deTObjectReference<aeRuleMirror> Ref;
	
	class cMatchName : public deObject {
	public:
		typedef deTObjectReference<cMatchName> Ref;
		
		cMatchName( const char *first, const char *second, deAnimatorRuleMirror::eMatchNameType type );
		
		inline const decString &GetFirst() const{ return pFirst; }
		inline const decString &GetSecond() const{ return pSecond; }
		inline deAnimatorRuleMirror::eMatchNameType GetType() const{ return pType; }
		
		bool operator==( const cMatchName &matchName ) const;
		bool operator!=( const cMatchName &matchName ) const;
		
	private:
		const decString pFirst;
		const decString pSecond;
		const deAnimatorRuleMirror::eMatchNameType pType;
	};
	
	
	
private:
	deAnimatorRuleMirror::eMirrorAxis pMirrorAxis;
	decString pMirrorBone;
	decObjectOrderedSet pMatchNames;
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
	aeRuleMirror( const aeRuleMirror &copy );
	
	/** Create rule with default settings. */
	static aeRuleMirror *CreateDefault();
	
protected:
	/** Clean up rule. */
	virtual ~aeRuleMirror();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Mirror axis. */
	inline deAnimatorRuleMirror::eMirrorAxis GetMirrorAxis() const{ return pMirrorAxis; }
	
	/** Set mirror axis. */
	void SetMirrorAxis( deAnimatorRuleMirror::eMirrorAxis axis );
	
	/** Name of mirror bone or empty string to use component. */
	inline const decString &GetMirrorBone() const{ return pMirrorBone; }
	
	/** Set name of mirror bone or empty string to use component. */
	void SetMirrorBone( const char *boneName );
	
	/** Count of match names. */
	int GetMatchNameCount() const;
	
	/** Match name at index. */
	cMatchName *GetMatchNameAt( int index ) const;
	
	/** Match name is present. */
	bool HasMatchName( cMatchName *matchName ) const;
	
	/** Index of match name or -1 if not found. */
	int IndexOfMatchName( cMatchName *matchName ) const;
	
	/** Add match name. */
	void AddMatchName( aeRuleMirror::cMatchName *matchName );
	
	/** Insert match name. */
	void InsertMatchName( aeRuleMirror::cMatchName *matchName, int index );
	
	/** Set match name at index. */
	void SetMatchNameAt( int index, aeRuleMirror::cMatchName *matchName );
	
	/** Remove match name. */
	void RemoveMatchName( cMatchName *matchName );
	
	/** Remove all match names. */
	void RemoveAllMatchNames();
	
	/** Position manipulation is enabled. */
	inline bool GetEnablePosition() const{ return pEnablePosition; }
	
	/** Set if position manipulation is enabled. */
	void SetEnablePosition( bool enabled );
	
	/** Orientation manipulation is enabled. */
	inline bool GetEnableOrientation() const{ return pEnableOrientation; }
	
	/** Set if orientation manipulation is enabled. */
	void SetEnableOrientation( bool enabled );
	
	/** Size manipulation is enabled. */
	inline bool GetEnableSize() const{ return pEnableSize; }
	
	/** Set if size manipulation is enabled. */
	void SetEnableSize( bool enabled );
	
	/** Vertex position set manipulation is enabled. */
	inline bool GetEnableVertexPositionSet() const{ return pEnableVertexPositionSet; }
	
	/** Set if vertex position set manipulation is enabled. */
	void SetEnableVertexPositionSet( bool enabled );
	
	
	
	/** Create engine animator rule. */
	virtual deAnimatorRule *CreateEngineRule();
	
	/** Create copy of rule. */
	virtual aeRule *CreateCopy() const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** Copy from another rule. */
	virtual aeRuleMirror &operator=( const aeRuleMirror &copy );
	/*@}*/
	
	
	
private:
	void pUpdateMatchNames();
};

#endif
