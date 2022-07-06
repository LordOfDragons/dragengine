/* 
 * Drag[en]gine Basic Network Module
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

#include "debnPConnectResendInterval.h"
#include "../../deNetworkBasic.h"
#include "../../configuration/debnConfiguration.h"

#include <dragengine/common/exceptions.h>


// Class debnPConnectResendInterval
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

debnPConnectResendInterval::debnPConnectResendInterval( deNetworkBasic &network ) :
debnParameterFloat( network )
{
	SetName( "connectResendInterval" );
	SetDescription( "Interval in seconds to keep resending connect request until connected or timed out." );
	SetCategory( ecExpert );
	SetDisplayName( "Connect Resend Interval" );
	SetDefaultValue( "1" );
}


// Parameter Value
////////////////////

float debnPConnectResendInterval::GetParameterFloat(){
	return pNetwork.GetConfiguration().GetConnectResendInterval();
}

void debnPConnectResendInterval::SetParameterFloat( float value ){
	pNetwork.GetConfiguration().SetConnectResendInterval( value );
}
