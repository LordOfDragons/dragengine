/* 
 * Drag[en]gine OpenGL Graphic Module
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
#include <string.h>

#include "deoglSharedVBO.h"
#include "deoglSharedVBOBlock.h"
#include "deoglSharedVBOList.h"
#include "deoglVBOAttribute.h"
#include "../extensions/deoglExtensions.h"
#include "../memory/deoglMemoryManager.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTChoices.h"
#include "../vao/deoglVAO.h"

#include <dragengine/deObjectReference.h>
#include <dragengine/common/exceptions.h>



// Class deoglSharedVBO
/////////////////////////

// Constructor, destructor
////////////////////////////

deoglSharedVBO::deoglSharedVBO( deoglSharedVBOList *parentList, int size, int indexSize ) :
pMemUseVBO( parentList->GetRenderThread().GetMemoryManager().GetConsumption().bufferObject.vboShared ),
pMemUseIBO( parentList->GetRenderThread().GetMemoryManager().GetConsumption().bufferObject.iboShared )
{
	if( ! parentList || size < 1 || indexSize < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	const deoglVBOLayout &layout = parentList->GetLayout();
	const int attributeCount = layout.GetAttributeCount();
	deoglRenderThread &renderThread = parentList->GetRenderThread();
	int i;
	
	pParentList = parentList;
	pVBO = 0;
	pIBO = 0;
	pVAO = NULL;
	pUsedSize = 0;
	pSize = size;
	pIndexSize = indexSize;
	pIndexUsedSize = 0;
	pDirty = true;
	
	try{
		// create vbo
		OGL_CHECK( renderThread, pglGenBuffers( 1, &pVBO ) );
		if( ! pVBO ){
			DETHROW( deeOutOfMemory );
		}
		
		// create ibo if indices are used
		if( indexSize > 0 ){
			OGL_CHECK( renderThread, pglGenBuffers( 1, &pIBO ) );
			if( ! pIBO ){
				DETHROW( deeOutOfMemory );
			}
		}
		
		// create vao
		pVAO = new deoglVAO( parentList->GetRenderThread() );
		pVAO->SetIndexType( parentList->GetLayout().GetIndexType() );
		OGL_CHECK( renderThread, pglBindVertexArray( pVAO->GetVAO() ) );
		OGL_CHECK( renderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBO ) );
		for( i=0; i<attributeCount; i++ ){
			layout.SetVAOAttributeAt( renderThread, i, i );
		}
		
		if( pIBO ){
			OGL_CHECK( renderThread, pglBindBuffer( GL_ELEMENT_ARRAY_BUFFER, pIBO ) );
		}
		
		OGL_CHECK( renderThread, pglBindBuffer( GL_ARRAY_BUFFER, 0 ) );
		OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
		
		// add empty block for the entire vbo space
		deObjectReference block;
		block.TakeOver( new deoglSharedVBOBlock( this, 0, size, 0, indexSize ) );
		pBlocks.Add( block );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglSharedVBO::~deoglSharedVBO(){
	pCleanUp();
}



// Management
///////////////

void deoglSharedVBO::Prepare(){
	if( ! pDirty ){
		return;
	}
	
	deoglRenderThread &renderThread = pParentList->GetRenderThread();
	const int indexSize = pParentList->GetLayout().GetIndexSize();
	const int stride = pParentList->GetLayout().GetStride();
	const GLenum drawType = pParentList->GetDrawType();
	const int blockCount = pBlocks.GetCount();
	unsigned char *vboData = NULL;
	int i;
	
	// update vertex buffer
	OGL_CHECK( renderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBO ) );
	OGL_CHECK( renderThread, pglBufferData( GL_ARRAY_BUFFER, stride * pUsedSize, NULL, drawType ) );
	
	if( pUsedSize > 0 ){
		// another way which does not require a memory copy is to write the data blocks in ascending
		// order using glBufferSubData. according to the Internet this should not be slower than
		// glBufferData for a newly created buffer.
		try{
			vboData = new unsigned char[ stride * pUsedSize ];
			
			for( i=0; i<blockCount; i++ ){
				const deoglSharedVBOBlock &block = *( ( deoglSharedVBOBlock* )pBlocks.GetAt( i ) );
				if( ! block.GetEmpty() ){
					memcpy( vboData + stride * block.GetOffset(), block.GetData(), stride * block.GetSize() );
				}
			}
			
			OGL_CHECK( renderThread, pglBufferData( GL_ARRAY_BUFFER, stride * pUsedSize, vboData, drawType ) );
			delete [] vboData;
			
		}catch( const deException & ){
			if( vboData ){
				delete [] vboData;
			}
			throw;
		}
	}
	
	// update index buffer. works differently depending on the presence of base-vertex support
	if( pIBO ){
		OGL_CHECK( renderThread, pglBindBuffer( GL_ARRAY_BUFFER, pIBO ) );
		OGL_CHECK( renderThread, pglBufferData( GL_ARRAY_BUFFER, indexSize * pIndexUsedSize, NULL, drawType ) );
		
		if( pIndexUsedSize > 0 ){
			const bool useBaseVertex = renderThread.GetChoices().GetSharedVBOUseBaseVertex();

			// another way which does not require a memory copy is to write the data blocks in ascending
			// order using glBufferSubData. according to the Internet this should not be slower than
			// glBufferData for a newly created buffer.
			vboData = NULL;
			
			try{
				vboData = new unsigned char[ indexSize * pIndexUsedSize ];
				
				for( i=0; i<blockCount; i++ ){
					const deoglSharedVBOBlock &block = *( ( deoglSharedVBOBlock* )pBlocks.GetAt( i ) );
					if( block.GetEmpty() || block.GetIndexCount() == 0 ){
						continue;
					}
					
					if( useBaseVertex ){
						memcpy( vboData + indexSize * block.GetIndexOffset(),
							block.GetIndexData(), indexSize * block.GetIndexCount() );
						
					}else{
						if( pParentList->GetLayout().GetIndexType() != deoglVBOLayout::eitUnsignedInt ){
							DETHROW( deeInvalidParam );
						}
						
						const int indexCount = block.GetIndexCount();
						const int firstPoint = block.GetOffset();
						int j;
						
						unsigned int *dest = ( unsigned int * )( vboData + indexSize * block.GetIndexOffset() );
						unsigned int *source = ( unsigned int * )block.GetIndexData();
						for( j=0; j<indexCount; j++ ){
							dest[ j ] = ( unsigned int )( source[ j ] + firstPoint );
						}
					}
				}
				
				OGL_CHECK( renderThread, pglBufferData( GL_ARRAY_BUFFER, indexSize * pIndexUsedSize, vboData, drawType ) );
				delete [] vboData;
				
			}catch( const deException & ){
				if( vboData ){
					delete [] vboData;
				}
				throw;
			}
		}
	}
	
	// done
	OGL_CHECK( renderThread, pglBindBuffer( GL_ARRAY_BUFFER, 0 ) );
	pDirty = false;
	
	pMemUseVBO = stride * pUsedSize;
	pMemUseIBO = indexSize * pIndexUsedSize;
}

void deoglSharedVBO::UpdateUsedSizes(){
	const int count = pBlocks.GetCount();
	int i;
	
	// calculate the required size for the vertex buffer
	pUsedSize = 0;
	
	for( i=count-1; i>=0; i-- ){
		const deoglSharedVBOBlock &block = *( ( deoglSharedVBOBlock* )pBlocks.GetAt( i ) );
		if( ! block.GetEmpty() ){
			pUsedSize = block.GetOffset() + block.GetSize();
			break;
		}
	}
	
	// calculate the required size for the index buffer
	pIndexUsedSize = 0;
	
	for( i=count-1; i>=0; i-- ){
		deoglSharedVBOBlock &block = *( ( deoglSharedVBOBlock* )pBlocks.GetAt( i ) );
		if( ! block.GetEmpty() ){
			pIndexUsedSize = block.GetIndexOffset() + block.GetIndexCount();
			break;
		}
	}
}

void deoglSharedVBO::MarkDirty(){
	pDirty = true;
}



// Data Management
////////////////////

int deoglSharedVBO::GetBlockCount() const{
	return pBlocks.GetCount();
}

deoglSharedVBOBlock *deoglSharedVBO::GetBlockAt( int index ) const{
	return ( deoglSharedVBOBlock* )pBlocks.GetAt( index );
}

deoglSharedVBOBlock *deoglSharedVBO::AddBlock( int size, int indexCount ){
	if( size < 1 || size > pSize || indexCount < 0 || indexCount > pIndexSize ){
		return NULL;
	}
	
	const int index = IndexOfEmptyBlockWithMinSize( size, indexCount );
	deoglSharedVBOBlock *block = NULL;
	
	if( index != -1 ){
		block = ( deoglSharedVBOBlock* )pBlocks.GetAt( index );
		
		// if empty block is larger than requested size add empty block with remaining empty
		// space right after this block
		if( block->GetSize() > size || block->GetIndexCount() > indexCount ){
			deObjectReference emptyBlock;
			emptyBlock.TakeOver( new deoglSharedVBOBlock( this,
				block->GetOffset() + size, block->GetSize() - size,
				block->GetIndexOffset() + indexCount, block->GetIndexCount() - indexCount ) );
			pBlocks.Insert( emptyBlock, index + 1 );
		}
		
		// turn the block into a non-empty block with the requested size
		block->SetSize( size );
		block->SetIndexCount( indexCount );
		block->SetEmpty( false );
		
		UpdateUsedSizes();
		pDirty = true;
	}
	
	if( block ){
		block->AddReference();
	}
	
	return block;
}

void deoglSharedVBO::RemoveBlock( deoglSharedVBOBlock *block ){
	int index = pBlocks.IndexOf( block );
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	// turn the block into an empty block
	block->SetEmpty( true );
	
	// if the previous block is empty merge this block with the previous block
	if( index > 0 ){
		deoglSharedVBOBlock * const mergeBlock = ( deoglSharedVBOBlock* )pBlocks.GetAt( index - 1 );
		if( mergeBlock->GetEmpty() ){
			mergeBlock->SetSize( mergeBlock->GetSize() + block->GetSize() );
			mergeBlock->SetIndexCount( mergeBlock->GetIndexCount() + block->GetIndexCount() );
			
			block->DropVBO();
			pBlocks.RemoveFrom( index );
			
			block = mergeBlock;
			index--;
		}
	}
	
	// if the next block is empty merge the next block with this block
	if( index < pBlocks.GetCount() - 1 ){
		deoglSharedVBOBlock * const mergeBlock = ( deoglSharedVBOBlock* )pBlocks.GetAt( index + 1 );
		if( mergeBlock->GetEmpty() ){
			block->SetSize( block->GetSize() + mergeBlock->GetSize() );
			block->SetIndexCount( block->GetIndexCount() + mergeBlock->GetIndexCount() );
			
			mergeBlock->DropVBO();
			pBlocks.RemoveFrom( index + 1 );
		}
	}
	
	UpdateUsedSizes();
	pDirty = true;
}

int deoglSharedVBO::IndexOfEmptyBlockWithMinSize( int size, int indexCount ){
	const int count = pBlocks.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglSharedVBOBlock * const block = ( deoglSharedVBOBlock* )pBlocks.GetAt( i );
		if( block->GetEmpty() && block->GetSize() >= size && block->GetIndexCount() >= indexCount ){
			return i;
		}
	}
	
	return -1;
}



// Private Functions
//////////////////////

void deoglSharedVBO::pCleanUp(){
	const int count = pBlocks.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( deoglSharedVBOBlock* )pBlocks.GetAt( i ) )->DropVBO();
	}
	pBlocks.RemoveAll();
	
	if( pVAO ){
		delete pVAO;
	}
	if( pVBO ){
		pglDeleteBuffers( 1, &pVBO );
	}
	if( pIBO ){
		pglDeleteBuffers( 1, &pIBO );
	}
}
