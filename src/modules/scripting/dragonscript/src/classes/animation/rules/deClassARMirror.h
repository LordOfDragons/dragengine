/* 
 * Drag[en]gine DragonScript Script Module
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

#ifndef _DECLASSARMIRROR_H_
#define _DECLASSARMIRROR_H_

#include <libdscript/libdscript.h>

class deAnimator;
class deAnimatorRuleMirror;
class deScriptingDragonScript;



/**
 * Class animator rule mirror.
 * 
 * Script class wrapping an attached or detached animator rule. Objects of
 * this class hold a pointer to the rule and optional an animator. If the
 * animator is non-NULL the rule pointer is held by the object. Otherwise
 * only a reference to the animator is held and the pointer to the rule
 * is not held. In addition an index is stored used to update the rule in
 * the animator if existing. This index is not necessary the index of the
 * rule itself but the index of the top most rule altered by changes made
 * to this rule.
 */
class deClassARMirror : public dsClass{
public:
	enum eTargets{
		etBlendFactor
	};
	
private:
	deScriptingDragonScript &pDS;
	dsClass *pClsARMirrorTarget;
	dsClass *pClsARMirrorMirrorAxis;
	dsClass *pClsARMirrorMatchNameType;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create script class. */
	deClassARMirror( deScriptingDragonScript &ds );
	
	/** Clean up script class. */
	virtual ~deClassARMirror();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Script module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** Create class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** Rule or nullptr if deleted or myself is nullptr. */
	deAnimatorRuleMirror *GetRule( dsRealObject *myself ) const;
	
	/** Assigns animator or nullptr. */
	void AssignAnimator( dsRealObject *myself, deAnimator *animator );
	
	/** Pushes a rule. */
	void PushRule( dsRunTime *rt, deAnimator *animator, deAnimatorRuleMirror *rule );
	
	inline dsClass *GetClassARMirrorTarget() const{ return pClsARMirrorTarget; }
	inline dsClass *GetClassARMirrorMirrorAxis() const{ return pClsARMirrorMirrorAxis; }
	inline dsClass *GetClassARMirrorMatchNameType() const{ return pClsARMirrorMatchNameType; }
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsARMirror;
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsStr;
		dsClass *clsInt;
		dsClass *clsFlt;
		dsClass *clsObj;
		dsClass *clsVec;
		dsClass *clsQuat;
		dsClass *clsVec2;
		dsClass *clsAr;
		dsClass *clsCol;
		dsClass *clsARMirrorMirrorAxis;
		dsClass *clsARMirrorMatchNameType;
		dsClass *clsARMirrorTarget;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfTargetAddLink );
	DEF_NATFUNC( nfTargetRemoveAllLinks );
	
	DEF_NATFUNC( nfSetMirrorMirrorAxis );
	DEF_NATFUNC( nfSetMirrorBone );
	DEF_NATFUNC( nfAddMatchName );
	
	DEF_NATFUNC( nfSetEnablePosition );
	DEF_NATFUNC( nfSetEnableOrientation );
	DEF_NATFUNC( nfSetEnableSize );
	DEF_NATFUNC( nfSetEnableVertexPositionSet );
#undef DEF_NATFUNC
};

#endif
