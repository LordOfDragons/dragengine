/* 
 * Drag[en]gine OpenAL Audio Module
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

#ifndef _DEOALMODEL_H_
#define _DEOALMODEL_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/audio/deBaseAudioModel.h>

class deAudioOpenAL;
class deoalAModel;

class deModel;



/**
 * \brief Model resource peer.
 */
class deoalModel : public deBaseAudioModel{
private:
	deAudioOpenAL &pOal;
	const deModel &pModel;
	deoalAModel *pAModel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create model peer. */
	deoalModel( deAudioOpenAL &oal, const deModel &model );
	
	/** \brief Clean up model peer. */
	virtual ~deoalModel();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deAudioOpenAL &GetOal() const{ return pOal; }
	
	/** \brief Model. */
	inline const deModel &GetModel() const{ return pModel; }
	
	/** \brief Audio model. */
	inline deoalAModel *GetAModel() const{ return pAModel; }
	/*@}*/
};

#endif
