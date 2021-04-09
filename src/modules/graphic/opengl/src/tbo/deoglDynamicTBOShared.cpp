/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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
#include <string.h>

#include "deoglDynamicTBO.h"
#include "deoglDynamicTBOShared.h"
#include "deoglDynamicTBOBlock.h"

#include <dragengine/deObjectReference.h>
#include <dragengine/common/exceptions.h>



// Class deoglDynamicTBOShared
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDynamicTBOShared::deoglDynamicTBOShared( deoglDynamicTBO *tbo ) :
pTBO( tbo ),
pUsedSize( 0 ),
pDirty( true )
{
	if( ! tbo ){
		DETHROW( deeInvalidParam );
	}
}

deoglDynamicTBOShared::~deoglDynamicTBOShared(){
	const int count = pBlocks.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( deoglDynamicTBOBlock* )pBlocks.GetAt( i ) )->DropSharedTBO();
	}
	pBlocks.RemoveAll();
}



// Management
///////////////

void deoglDynamicTBOShared::Prepare(){
	if( ! pDirty ){
		return;
	}
	
	deoglDynamicTBO &tbo = *GetTBO();
	if( pUsedSize > tbo.GetPixelCount() ){
		tbo.IncreasePixelCount( pUsedSize - tbo.GetPixelCount() );
	}
	
	//const int componentCount = pTBO->GetComponentCount();
	const int stride = tbo.GetComponentCount() * tbo.GetDataTypeSize();
	uint8_t * const tboData = tbo.GetData();
	const int count = pBlocks.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const deoglDynamicTBOBlock &block = *( ( deoglDynamicTBOBlock* )pBlocks.GetAt( i ) );
		if( block.GetEmpty() || ! block.GetData() ){
			continue;
		}
		
		//const int offset = block.GetOffset() * componentCount;
		//pTBO->SetTBO( offset, *block.GetData() );
		//pTBO->Update( offset, block.GetSize() );
		memcpy( tboData + block.GetOffset() * stride, block.GetData()->GetData(), block.GetSize() * stride );
	}
	
	pDirty = false;
}

void deoglDynamicTBOShared::UpdateUsedSizes(){
	const int count = pBlocks.GetCount();
	int i;
	
	pUsedSize = 0;
	
	for( i=count-1; i>=0; i-- ){
		const deoglDynamicTBOBlock &block = *( ( deoglDynamicTBOBlock* )pBlocks.GetAt( i ) );
		if( block.GetEmpty() ){
			continue;
		}
		
		pUsedSize = block.GetOffset() + block.GetSize();
		break;
	}
}

void deoglDynamicTBOShared::MarkDirty(){
	pDirty = true;
}



int deoglDynamicTBOShared::GetBlockCount() const{
	return pBlocks.GetCount();
}

deoglDynamicTBOBlock *deoglDynamicTBOShared::GetBlockAt( int index ) const{
	return ( deoglDynamicTBOBlock* )pBlocks.GetAt( index );
}

deoglDynamicTBOBlock *deoglDynamicTBOShared::AddBlock( deoglDynamicTBO *tbo ){
	if( ! tbo ){
		DETHROW( deeInvalidParam );
	}
	
	const int index = FirstMatchingBlock( tbo );
	const int tboSize = tbo->GetPixelCount();
	deoglDynamicTBOBlock *block = NULL;
	
	if( index != -1 ){
		block = ( deoglDynamicTBOBlock* )pBlocks.GetAt( index );
		
		// if the empty block is larger than the requested size add a new empty block with the
		// remaining empty space right after this block
		if( block->GetSize() > tboSize ){
			deObjectReference emptyBlock;
			emptyBlock.TakeOver( new deoglDynamicTBOBlock( this,
				block->GetOffset() + tboSize, block->GetSize() - tboSize ) );
			pBlocks.Insert( emptyBlock, index + 1 );
		}
		
		// turn the block into a non-empty block with the requested size
		block->SetSize( tboSize );
		block->SetData( tbo );
		block->SetEmpty( false );
		
		UpdateUsedSizes();
		pDirty = true;
	}
	
	if( block ){
		block->AddReference();
	}
	
	return block;
}

void deoglDynamicTBOShared::RemoveBlock( deoglDynamicTBOBlock *block ){
	int index = pBlocks.IndexOf( block );
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	// turn the block into an empty block
	block->SetData( NULL );
	block->SetEmpty( true );
	
	// if the previous block is empty merge this block with the previous block
	deoglDynamicTBOBlock *mergeBlock = NULL;
	
	if( index > 0 ){
		mergeBlock = ( deoglDynamicTBOBlock* )pBlocks.GetAt( index - 1 );
		
		if( mergeBlock->GetEmpty() ){
			mergeBlock->SetSize( mergeBlock->GetSize() + block->GetSize() );
			
			block->DropSharedTBO();
			pBlocks.RemoveFrom( index );
			
			block = mergeBlock;
			index--;
		}
	}
	
	// if the next block is empty merge the next block with this block
	if( index < pBlocks.GetCount() - 1 ){
		mergeBlock = ( deoglDynamicTBOBlock* )pBlocks.GetAt( index + 1 );
		
		if( mergeBlock->GetEmpty() ){
			block->SetSize( block->GetSize() + mergeBlock->GetSize() );
			
			mergeBlock->DropSharedTBO();
			pBlocks.RemoveFrom( index + 1 );
		}
	}
	
	UpdateUsedSizes();
	pDirty = true;
}

int deoglDynamicTBOShared::FirstMatchingBlock( deoglDynamicTBO *tbo ){
	const int tboSize = tbo->GetPixelCount();
	const int count = pBlocks.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const deoglDynamicTBOBlock &block = *( ( deoglDynamicTBOBlock* )pBlocks.GetAt( i ) );
		if( block.GetEmpty() && block.GetSize() >= tboSize ){
			return i;
		}
	}
	
	return -1;
}
