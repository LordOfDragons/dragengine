/* 
 * Drag[en]gine IGDE Rig Editor
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

#include <stdio.h>
#include <stdlib.h>

#include "reUBoneImportFromFile.h"
#include "../../../rig/reRig.h"
#include "../../../rig/bone/reRigBone.h"
#include "../../../rig/bone/reSelectionBones.h"
#include "../../../rig/shape/reRigShape.h"
#include "../../../rig/shape/reRigShapeList.h"
#include "../../../rig/shape/reRigShapeSphere.h"
#include "../../../rig/shape/reRigShapeBox.h"
#include "../../../rig/shape/reRigShapeCylinder.h"
#include "../../../rig/shape/reRigShapeCapsule.h"
#include "../../../rig/constraint/reRigConstraint.h"
#include "../../../rig/constraint/reRigConstraintList.h"

#include <dragengine/common/exceptions.h>



// Class reUBoneImportFromFile
////////////////////////////////

// Constructor, destructor
////////////////////////////

reUBoneImportFromFile::reUBoneImportFromFile( reRig *rig, reRig *importedRig ){
	if( ! rig || ! importedRig ){
		DETHROW( deeInvalidParam );
	}
	
	const reSelectionBones &selection = *rig->GetSelectionBones();
	int b, boneCount = selection.GetBoneCount();
	int c, constraintCount;
	reRigBone *importBone;
	int s, shapeCount;
	reRigBone *bone;
	
	pRig = NULL;
	
	pBones = NULL;
	pBoneCount = 0;
	
	pScale = 1.0f;
	pImportBoneProperties = true;
	pImportShapes = true;
	pImportConstraints = true;
	
	SetShortInfo( "Import Bones" );
	
	try{
		if( boneCount > 0 ){
			// create bones array
			pBones = new sBone[ boneCount ];
			
			// store bones if found in the imported rig
			for( b=0; b<boneCount; b++ ){
				bone = selection.GetBoneAt( b );
				importBone = importedRig->GetBoneNamed( bone->GetName().GetString() );
				
				if( importBone ){
					sBone &ubone = pBones[ pBoneCount ];
					
					// clear the state before increasing the bone count
					ubone.bone = NULL;
					ubone.importBone = NULL;
					
					ubone.oldShapes = NULL;
					ubone.oldConstraints = NULL;
					
					pBoneCount++;
					
					// store the old state as well as the imported bone. no need
					// to copy the state since the imported bone has it all
					ubone.bone = bone;
					ubone.bone->AddReference();
					
					ubone.oldCMP = bone->GetCentralMassPoint();
					ubone.oldMass = bone->GetMass();
					ubone.oldDynamic = bone->GetDynamic();
					
					ubone.oldShapes = new reRigShapeList;
					
					shapeCount = bone->GetShapeCount();
					for( s=0; s<shapeCount; s++ ){
						ubone.oldShapes->AddShape( bone->GetShapeAt( s ) );
					}
					
					ubone.oldConstraints = new reRigConstraintList;
					
					constraintCount = bone->GetConstraintCount();
					for( c=0; c<constraintCount; c++ ){
						ubone.oldConstraints->AddConstraint( bone->GetConstraintAt( c ) );
					}
					
					ubone.importBone = importBone;
					ubone.importBone->AddReference();
				}
			}
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
	
	pRig = rig;
	rig->AddReference();
}

reUBoneImportFromFile::~reUBoneImportFromFile(){
	pCleanUp();
}



// Undo and Redo operations
/////////////////////////////

void reUBoneImportFromFile::SetScale( float scale ){
	pScale = scale;
}

void reUBoneImportFromFile::SetImportBoneProperties( bool import ){
	pImportBoneProperties = import;
}

void reUBoneImportFromFile::SetImportShapes( bool import ){
	pImportShapes = import;
}

void reUBoneImportFromFile::SetImportConstraints( bool import ){
	pImportConstraints = import;
}



void reUBoneImportFromFile::Undo(){
	int c, constraintCount;
	int s, shapeCount;
	reRigBone *bone;
	int b;
	
	for( b=0; b<pBoneCount; b++ ){
		sBone &ubone = pBones[ b ];
		bone = ubone.bone;
		
		if( pImportBoneProperties ){
			bone->SetCentralMassPoint( ubone.oldCMP );
			bone->SetMass( ubone.oldMass );
			bone->SetDynamic( ubone.oldDynamic );
		}
		
		if( pImportShapes ){
			bone->RemoveAllShapes();
			const reRigShapeList &oldShapes = *ubone.oldShapes;
			shapeCount = oldShapes.GetShapeCount();
			for( s=0; s<shapeCount; s++ ){
				bone->AddShape( oldShapes.GetShapeAt( s ) );
			}
		}
		
		if( pImportConstraints ){
			bone->RemoveAllConstraints();
			const reRigConstraintList &oldConstraints = *ubone.oldConstraints;
			constraintCount = oldConstraints.GetConstraintCount();
			for( c=0; c<constraintCount; c++ ){
				bone->AddConstraint( oldConstraints.GetConstraintAt( c ) );
			}
		}
	}
}

void reUBoneImportFromFile::Redo(){
	reRigConstraint *constraint = NULL;
	reRigShape *shape = NULL;
	int c, constraintCount;
	reRigBone *importBone;
	reRigBone *bone;
	int s, shapeCount;
	int b;
	
	try{
		for( b=0; b<pBoneCount; b++ ){
			sBone &ubone = pBones[ b ];
			importBone = ubone.importBone;
			bone = ubone.bone;
			
			if( pImportBoneProperties ){
				bone->SetCentralMassPoint( importBone->GetCentralMassPoint() * pScale );
				bone->SetMass( importBone->GetMass() );
				bone->SetDynamic( importBone->GetDynamic() );
			}
			
			if( pImportShapes ){
				bone->RemoveAllShapes();
				shapeCount = importBone->GetShapeCount();
				for( s=0; s<shapeCount; s++ ){
					shape = importBone->GetShapeAt( s )->Duplicate();
					shape->Scale( pScale );
					bone->AddShape( shape );
					shape->FreeReference();
					shape = NULL;
				}
			}
			
			if( pImportConstraints ){
				bone->RemoveAllConstraints();
				constraintCount = importBone->GetConstraintCount();
				for( c=0; c<constraintCount; c++ ){
					constraint = importBone->GetConstraintAt( c )->Duplicate();
					constraint->Scale( pScale );
					
					if( constraint->GetConstraintBone() ){
						constraint->SetConstraintBone( pRig->GetBoneNamed( constraint->GetConstraintBone()->GetName().GetString() ) );
					}
					
					bone->AddConstraint( constraint );
					constraint->FreeReference();
					constraint = NULL;
				}
			}
		}
		
	}catch( const deException & ){
		if( shape ){
			shape->FreeReference();
		}
		if( constraint ){
			constraint->FreeReference();
		}
		throw;
	}
}



// Private Functions
//////////////////////

void reUBoneImportFromFile::pCleanUp(){
	if( pBones ){
		while( pBoneCount > 0 ){
			pBoneCount--;
			sBone &ubone = pBones[ pBoneCount ];
			
			if( ubone.oldConstraints ){
				delete ubone.oldConstraints;
			}
			if( ubone.oldShapes ){
				delete ubone.oldShapes;
			}
			if( ubone.importBone ){
				ubone.importBone->FreeReference();
			}
			if( ubone.bone ){
				ubone.bone->FreeReference();
			}
		}
		
		delete [] pBones;
	}
	
	if( pRig ){
		pRig->FreeReference();
	}
}
