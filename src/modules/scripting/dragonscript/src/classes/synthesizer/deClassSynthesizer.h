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

#ifndef _DECLASSSYNTHESIZER_H_
#define _DECLASSSYNTHESIZER_H_

#include <libdscript/libdscript.h>

class deEngine;
class deSynthesizer;
class deScriptingDragonScript;



/**
 * \brief Synthesizer class.
 */
class deClassSynthesizer : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create script class. */
	deClassSynthesizer( deScriptingDragonScript &ds );
	
	/** \brief Clean up script class. */
	virtual ~deClassSynthesizer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Create class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Scripting module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief synthesizer or \em NULL if object is \em NULL. */
	deSynthesizer *GetSynthesizer( dsRealObject *object ) const;
	
	/** \brief Push synthesizer on the stack (pushes \em nil if instance is \em NULL). */
	void PushSynthesizer( dsRunTime *rt, deSynthesizer *synthesizer );
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsSyn;
		dsClass *clsSynS;
		dsClass *clsSynController;
		dsClass *clsCurveBezier;
		
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsString;
		dsClass *clsInt;
		dsClass *clsFloat;
		dsClass *clsObject;
	};
	
	#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetChannelCount );
	DEF_NATFUNC( nfSetChannelCount );
	DEF_NATFUNC( nfGetSampleRate );
	DEF_NATFUNC( nfSetSampleRate );
	DEF_NATFUNC( nfGetBytesPerSample );
	DEF_NATFUNC( nfSetBytesPerSample );
	DEF_NATFUNC( nfGetSampleCount );
	DEF_NATFUNC( nfSetSampleCount );
	
	DEF_NATFUNC( nfGetControllerCount );
	DEF_NATFUNC( nfSetControllerCount );
	DEF_NATFUNC( nfGetControllerAt );
	DEF_NATFUNC( nfGetControllerNamed );
	DEF_NATFUNC( nfIndexOfControllerNamed );
	
	DEF_NATFUNC( nfGetLinkCount );
	DEF_NATFUNC( nfAddLink );
	DEF_NATFUNC( nfRemoveAllLinks );
	DEF_NATFUNC( nfLinkSetController );
	DEF_NATFUNC( nfLinkSetRepeat );
	DEF_NATFUNC( nfLinkSetCurve );
	
	DEF_NATFUNC( nfGetSourceCount );
	DEF_NATFUNC( nfAddSource );
	DEF_NATFUNC( nfGetSourceAt );
	DEF_NATFUNC( nfRemoveAllSources );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfEquals2 );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

#endif
