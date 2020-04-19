/* 
 * Drag[en]gine Game Engine
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

#include "deBaseGraphicPropField.h"



// Class deBaseGraphicPropField
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deBaseGraphicPropField::deBaseGraphicPropField(){ }
deBaseGraphicPropField::~deBaseGraphicPropField(){ }



// Management
///////////////

void deBaseGraphicPropField::GroundChanged(){ }

void deBaseGraphicPropField::PositionChanged(){ }

void deBaseGraphicPropField::TypeAdded( int index, dePropFieldType *type ){ }
void deBaseGraphicPropField::TypeRemoved( int index, dePropFieldType *type ){ }
void deBaseGraphicPropField::AllTypesRemoved(){ }
void deBaseGraphicPropField::TypeChanged( int index, dePropFieldType *type ){ }
void deBaseGraphicPropField::InstancesChanged( int index, dePropFieldType *type ){ }
void deBaseGraphicPropField::AssignmentsChanged( int index, dePropFieldType *type ){ }
void deBaseGraphicPropField::BendStatesChanged( int index, dePropFieldType *type ){ }
