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

#ifndef _DECLASSSSWAVE_H_
#define _DECLASSSSWAVE_H_

#include <libdscript/libdscript.h>

class deSynthesizer;
class deSynthesizerSourceWave;
class deScriptingDragonScript;



/**
 * \brief Class synthesizer source wave.
 * \details Script class wrapping an attached or detached synthesizer source. Objects of
 *          this class hold a pointer to the source and optional an synthesizer. If the
 *          synthesizer is non-NULL the source pointer is held by the object. Otherwise
 *          only a reference to the synthesizer is held and the pointer to the source
 *          is not held. In addition an index is stored used to update the source in
 *          the synthesizer if existing. This index is not necessary the index of the
 *          source itself but the index of the top most source altered by changes made
 *          to this source.
 */
class deClassSSWave : public dsClass{
public:
	enum eTargets{
		etBlendFactor,
		etVolume,
		etPanning,
		etFrequency
	};
	
	
	
private:
	deScriptingDragonScript &pDS;
	dsClass *pClsSSWaveTarget;
	dsClass *pClsSSWaveType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create script class. */
	deClassSSWave( deScriptingDragonScript &ds );
	
	/** \brief Clean up script class. */
	virtual ~deClassSSWave();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Script module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Source or \em NULL if deleted or myself is \em NULL. */
	deSynthesizerSourceWave *GetSource( dsRealObject *myself ) const;
	
	/** \brief Assigns synthesizer or \em NULL. */
	void AssignSynthesizer( dsRealObject *myself, deSynthesizer *synthesizer );
	
	/** \brief Pushes a source. */
	void PushSource( dsRunTime *rt, deSynthesizer *synthesizer, deSynthesizerSourceWave *source );
	
	inline dsClass *GetClassSSWaveTarget() const{ return pClsSSWaveTarget; }
	inline dsClass *GetClassSSWaveType() const{ return pClsSSWaveType; }
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsSSWave;
		
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsString;
		dsClass *clsInt;
		dsClass *clsFloat;
		dsClass *clsObject;
		dsClass *clsSSWaveTarget;
		dsClass *clsSSWaveType;
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
	
	DEF_NATFUNC( nfSetType );
	DEF_NATFUNC( nfSetMinFrequency );
	DEF_NATFUNC( nfSetMaxFrequency );
#undef DEF_NATFUNC
};

#endif
