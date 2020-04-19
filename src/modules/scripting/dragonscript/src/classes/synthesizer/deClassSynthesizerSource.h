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

#ifndef _DECLASSSYNTHESIZERSOURCE_H_
#define _DECLASSSYNTHESIZERSOURCE_H_

#include <libdscript/libdscript.h>

class deSynthesizer;
class deSynthesizerSource;
class deScriptingDragonScript;



/**
 * \brief Class Synthesizer source.
 * \details Script class wrapping an attached or detached synthesizer source. Objects of
 *          this class hold a pointer to the source and optional an synthesizer. If the
 *          synthesizer is non-NULL the source pointer is held by the object. Otherwise
 *          only a reference to the synthesizer is held and the pointer to the source
 *          is not held. In addition an index is stored used to update the source in
 *          the synthesizer if existing. This index is not necessary the index of the
 *          source itself but the index of the top most source altered by changes made
 *          to this source.
 */
class deClassSynthesizerSource : public dsClass{
private:
	deScriptingDragonScript &pDS;
	dsClass *pClsSynthesizerSourceMix;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create script class. */
	deClassSynthesizerSource( deScriptingDragonScript &ds );
	
	/** \brief Clean up script class. */
	virtual ~deClassSynthesizerSource();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Script module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Source or \em NULL if deleted or myself is \em NULL. */
	deSynthesizerSource *GetSource( dsRealObject *myself ) const;
	
	/** \brief Assigns source or \em NULL. */
	void AssignSource( dsRealObject *myself, deSynthesizerSource *source );
	
	/** \brief Assigns synthesizer or \em NULL. */
	void AssignSynthesizer( dsRealObject *myself, deSynthesizer *synthesizer );
	
	/**
	 * \brief Pushes a source.
	 * \details Delegates the call to the matching source script class.
	 */
	void PushSource( dsRunTime *rt, deSynthesizer *synthesizer, deSynthesizerSource *source );
	
	inline dsClass *GetClassSynthesizerSourceMix() const{ return pClsSynthesizerSourceMix; }
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsSynS;
		
		dsClass *clsSynEff;
		
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsString;
		dsClass *clsInt;
		dsClass *clsFloat;
		dsClass *clsObject;
		dsClass *clsSynthesizerSourceMix;
	};
	#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfSetEnabled );
	DEF_NATFUNC( nfSetMixMode );
	DEF_NATFUNC( nfSetBlendFactor );
	
	DEF_NATFUNC( nfSetMinVolume );
	DEF_NATFUNC( nfSetMaxVolume );
	DEF_NATFUNC( nfSetMinPanning );
	DEF_NATFUNC( nfSetMaxPanning );
	
	DEF_NATFUNC( nfAddEffect );
	DEF_NATFUNC( nfRemoveEffect );
	DEF_NATFUNC( nfRemoveAllEffects );
#undef DEF_NATFUNC
};

#endif
