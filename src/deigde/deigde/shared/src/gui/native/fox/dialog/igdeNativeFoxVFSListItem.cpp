/* 
 * Drag[en]gine IGDE
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

#include "igdeNativeFoxVFSListItem.h"

#include <dragengine/filesystem/dePathList.h>
#include <dragengine/filesystem/deVFSContainer.h>
#include <dragengine/common/exceptions.h>



// Class igdeNativeFoxVFSListItem
//////////////////////////

// Constructor, destructor
////////////////////////////

igdeNativeFoxVFSListItem::igdeNativeFoxVFSListItem( const decPath &path, const FXString &text, FXIcon *bi, FXIcon *mi ) :
FXIconItem( text, bi, mi ),
pPath( path ),
pFileType( deVFSContainer::eftRegularFile ),
pFileSize( 0 ),
pFileModificationTime( 0 ){
}

igdeNativeFoxVFSListItem::~igdeNativeFoxVFSListItem(){
}



// Management
///////////////

void igdeNativeFoxVFSListItem::SetFileName( const char *filename ){
	pFileName = filename;
}

void igdeNativeFoxVFSListItem::SetFileType( deVFSContainer::eFileTypes fileType ){
	pFileType = fileType;
}

void igdeNativeFoxVFSListItem::SetFileSize( int fileSize ){
	pFileSize = fileSize;
}

void igdeNativeFoxVFSListItem::SetFileModificationTime( TIME_SYSTEM time ){
	pFileModificationTime = time;
}

bool igdeNativeFoxVFSListItem::IsDirectory() const{
	return pFileType == deVFSContainer::eftDirectory;
}



void igdeNativeFoxVFSListItem::UpdateText( const char* pattern ){
	FXString text, format;
	
	while( *pattern ){
		if( ! text.empty() ){
			text.append( "\t" );
		}
		
		if( *pattern == 'n' ){
			text.append( pFileName );
			
		}else if( *pattern == 's' ){
			if( IsDirectory() ){
				text.append( "-" );
				
			}else{
				if( pFileSize < 1024 ){
					format.format( "%i B", pFileSize );
					text.append( format );
					
				}else if( pFileSize < 1024000 ){
					format.format( "%0.1f KiB", ( double )pFileSize / 1024.0 );
					text.append( format );
					
				}else if( pFileSize < 1024000000 ){
					format.format( "%0.1f MiB", ( double )pFileSize / 1024000.0 );
					text.append( format );
					
				}else{
					format.format( "%0.1f GiB", ( double )pFileSize / 1024000000.0 );
					text.append( format );
				}
			}
			
		}else if( *pattern == 'm' ){
			if( IsDirectory() ){
				text.append( "-" );
				
			}else{
				const decDateTime datetime( pFileModificationTime );
				format.format( "%02i:%02i %02i.%02i.%04i", datetime.GetHour(), datetime.GetMinute(),
					datetime.GetDay(), datetime.GetMonth(), datetime.GetYear() );
				text.append( format );
			}
			
		}else if( *pattern == 't' ){
			if( IsDirectory() ){
				text.append( "Directory" );
				
			}else{
				text.append( "File" );
			}
		}
		
		pattern++;
	}
	
	setText( text );
}



// Sorting
////////////

FXint igdeNativeFoxVFSListItem::fSortNameAsc( const FXIconItem *item1, const FXIconItem *item2 ){
	const igdeNativeFoxVFSListItem &file1 = *( ( igdeNativeFoxVFSListItem* )item1 );
	const igdeNativeFoxVFSListItem &file2 = *( ( igdeNativeFoxVFSListItem* )item2 );
	const bool isDir1 = file1.IsDirectory();
	const bool isDir2 = file2.IsDirectory();
	
	if( isDir1 == isDir2 ){
		return compare( file1.GetFileName(), file2.GetFileName() );
		
	}else{
		if( isDir1 && ! isDir2 ){
			return -1;
			
		}else{
			return 1;
		}
	}
}

FXint igdeNativeFoxVFSListItem::fSortNameDesc( const FXIconItem *item1, const FXIconItem *item2 ){
	const igdeNativeFoxVFSListItem &file1 = *( ( igdeNativeFoxVFSListItem* )item1 );
	const igdeNativeFoxVFSListItem &file2 = *( ( igdeNativeFoxVFSListItem* )item2 );
	const bool isDir1 = file1.IsDirectory();
	const bool isDir2 = file2.IsDirectory();
	
	if( isDir1 == isDir2 ){
		return compare( file2.GetFileName(), file1.GetFileName() );
		
	}else{
		if( isDir1 && ! isDir2 ){
			return -1;
			
		}else{
			return 1;
		}
	}
}

FXint igdeNativeFoxVFSListItem::fSortSizeAsc( const FXIconItem *item1, const FXIconItem *item2 ){
	const igdeNativeFoxVFSListItem &file1 = *( ( igdeNativeFoxVFSListItem* )item1 );
	const igdeNativeFoxVFSListItem &file2 = *( ( igdeNativeFoxVFSListItem* )item2 );
	const bool isDir1 = file1.IsDirectory();
	const bool isDir2 = file2.IsDirectory();
	
	if( isDir1 == isDir2 ){
		FXint result = ( FXint )( file1.GetFileSize() - file2.GetFileSize() );
		if( result == 0 ){ // same size, sort by name instead
			result = compare( file1.GetFileName(), file2.GetFileName() );
		}
		return result;
		
	}else{
		if( isDir1 && ! isDir2 ){
			return -1;
			
		}else{
			return 1;
		}
	}
}

FXint igdeNativeFoxVFSListItem::fSortSizeDesc( const FXIconItem *item1, const FXIconItem *item2 ){
	const igdeNativeFoxVFSListItem &file1 = *( ( igdeNativeFoxVFSListItem* )item1 );
	const igdeNativeFoxVFSListItem &file2 = *( ( igdeNativeFoxVFSListItem* )item2 );
	const bool isDir1 = file1.IsDirectory();
	const bool isDir2 = file2.IsDirectory();
	
	if( isDir1 == isDir2 ){
		FXint result = ( FXint )( file2.GetFileSize() - file1.GetFileSize() );
		if( result == 0 ){ // same size, sort by name instead
			result = compare( file2.GetFileName(), file1.GetFileName() );
		}
		return result;
		
	}else{
		if( isDir1 && ! isDir2 ){
			return -1;
			
		}else{
			return 1;
		}
	}
}

FXint igdeNativeFoxVFSListItem::fSortModTimeAsc( const FXIconItem *item1, const FXIconItem *item2 ){
	const igdeNativeFoxVFSListItem &file1 = *( ( igdeNativeFoxVFSListItem* )item1 );
	const igdeNativeFoxVFSListItem &file2 = *( ( igdeNativeFoxVFSListItem* )item2 );
	const bool isDir1 = file1.IsDirectory();
	const bool isDir2 = file2.IsDirectory();
	
	if( isDir1 == isDir2 ){
		FXint result = ( FXint )( file2.GetFileModificationTime() - file1.GetFileModificationTime() );
		if( result == 0 ){ // same size, sort by name instead
			result = compare( file1.GetFileName(), file2.GetFileName() );
		}
		return result;
		
	}else{
		if( isDir1 && ! isDir2 ){
			return -1;
			
		}else{
			return 1;
		}
	}
}

FXint igdeNativeFoxVFSListItem::fSortModTimeDesc( const FXIconItem *item1, const FXIconItem *item2 ){
	const igdeNativeFoxVFSListItem &file1 = *( ( igdeNativeFoxVFSListItem* )item1 );
	const igdeNativeFoxVFSListItem &file2 = *( ( igdeNativeFoxVFSListItem* )item2 );
	const bool isDir1 = file1.IsDirectory();
	const bool isDir2 = file2.IsDirectory();
	
	if( isDir1 == isDir2 ){
		FXint result = ( FXint )( file1.GetFileModificationTime() - file2.GetFileModificationTime() );
		if( result == 0 ){ // same size, sort by name instead
			result = compare( file2.GetFileName(), file1.GetFileName() );
		}
		return result;
		
	}else{
		if( isDir1 && ! isDir2 ){
			return -1;
			
		}else{
			return 1;
		}
	}
}
