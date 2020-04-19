/* 
 * Drag[en]gine DragonScript Script Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
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

#ifndef _DECLASSSESTRETCH_H_
#define _DECLASSSESTRETCH_H_

#include <libdscript/libdscript.h>

class deSynthesizerSource;
class deSynthesizerEffectStretch;
class deScriptingDragonScript;



/**
 * \brief Class synthesizer effect stretch.
 * \details Script class wrapping an attached or detached synthesizer effect. Objects of
 *          this class hold a pointer to the effect and optional an synthesizer. If the
 *          synthesizer is non-NULL the effect pointer is held by the object. Otherwise
 *          only a reference to the synthesizer is held and the pointer to the effect
 *          is not held. In addition an index is stored used to update the effect in
 *          the synthesizer if existing. This index is not necessary the index of the
 *          effect itself but the index of the top most effect altered by changes made
 *          to this effect.
 */
class deClassSEStretch : public dsClass{
public:
	enum eTargets{
		etStrength,
		etTime,
		etPitch
	};
	
	
	
private:
	deScriptingDragonScript &pDS;
	dsClass *pClsSEStretchTarget;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create script class. */
	deClassSEStretch( deScriptingDragonScript &ds );
	
	/** \brief Clean up script class. */
	virtual ~deClassSEStretch();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Script module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Effect or \em NULL if deleted or myself is \em NULL. */
	deSynthesizerEffectStretch *GetEffect( dsRealObject *myself ) const;
	
	/** \brief Assigns synthesizer or \em NULL. */
	void AssignSynthesizer( dsRealObject *myself, deSynthesizerSource *source );
	
	/** \brief Pushes a effect. */
	void PushEffect( dsRunTime *rt, deSynthesizerSource *source, deSynthesizerEffectStretch *effect );
	
	inline dsClass *GetClassSEStretchTarget() const{ return pClsSEStretchTarget; }
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsSEStretch;
		
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsString;
		dsClass *clsInt;
		dsClass *clsFloat;
		dsClass *clsObject;
		dsClass *clsSEStretchTarget;
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
	
	DEF_NATFUNC( nfSetMinTime );
	DEF_NATFUNC( nfSetMaxTime );
	DEF_NATFUNC( nfSetMinPitch );
	DEF_NATFUNC( nfSetMaxPitch );
#undef DEF_NATFUNC
};

#endif
