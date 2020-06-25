// filehdr.cc 
//	Routines for managing the disk file header (in UNIX, this
//	would be called the i-node).
//
//	The file header is used to locate where on disk the 
//	file's data is stored.  We implement this as a fixed size
//	table of pointers -- each entry in the table points to the 
//	disk sector containing that portion of the file data
//	(in other words, there are no indirect or doubly indirect 
//	blocks). The table size is chosen so that the file header
//	will be just big enough to fit in one disk sector, 
//
//      Unlike in a real system, we do not keep track of file permissions, 
//	ownership, last modification date, etc., in the file header. 
//
//	A file header can be initialized in two ways:
//	   for a new file, by modifying the in-memory data structure
//	     to point to the newly allocated data blocks
//	   for a file already on disk, by reading the file header from disk
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"

#include "system.h"
#include "filehdr.h"

//----------------------------------------------------------------------
// FileHeader::Allocate
// 	Initialize a fresh file header for a newly created file.
//	Allocate data blocks for the file out of the map of free disk blocks.
//	Return FALSE if there are not enough free blocks to accomodate
//	the new file.
//
//	"freeMap" is the bit map of free disk sectors
//	"fileSize" is the bit map of free disk sectors
//----------------------------------------------------------------------

bool
FileHeader::Allocate(BitMap *freeMap, int fileSize)
{ 
    numBytes = fileSize;
    numSectors  = divRoundUp(fileSize, SectorSize);
    if (freeMap->NumClear() < numSectors)
	return FALSE;		// not enough space
    //如果不需要二级索引
    if(numSectors<=NumDirect - 1)
    {
        for (int i = 0; i < numSectors; i++)
        {
            dataSectors[i] = freeMap->Find();
        }
        dataSectors[NumDirect - 1] = -1;
        return TRUE;
    }
    //如果需要二级索引，前29块不变，第30个存储二级索引
    else
    {
        for(int i = 0;i<NumDirect - 1;i++)
        {
            dataSectors[i] = freeMap->Find();
        }
        dataSectors[NumDirect - 1] = freeMap->Find();
        //申请第二级索引的扇区
        int dataSectors2[SecondNumDirect];
        for(int i = 0;i<numSectors - NumDirect + 1;i++)
        {
            dataSectors2[i] = freeMap->Find();
        }
        synchDisk->WriteSector(dataSectors[NumDirect - 1],(char*)dataSectors2);
    }
    return TRUE;
}

//----------------------------------------------------------------------
// FileHeader::Deallocate
// 	De-allocate all the space allocated for data blocks for this file.
//
//	"freeMap" is the bit map of free disk sectors
//----------------------------------------------------------------------

void 
FileHeader::Deallocate(BitMap *freeMap)
{
    //无二级索引
    if(dataSectors[NumDirect - 1]==-1)
    {
        for (int i = 0; i < numSectors; i++) 
        {
	        ASSERT(freeMap->Test((int) dataSectors[i]));  // ought to be marked!
	        freeMap->Clear((int) dataSectors[i]);
        }
    }
    else
    {//有二级索引，一级的直接释放空间,二级的读出之后再释放
        for(int i = 0;i<NumDirect - 1;i++)
        {
            ASSERT(freeMap->Test((int) dataSectors[i]));  // ought to be marked!
	        freeMap->Clear((int) dataSectors[i]);
        }
        int dataSectors2[SecondNumDirect];
        synchDisk->ReadSector(dataSectors[NumDirect - 1], (char *)dataSectors2);
        freeMap->Clear((int) dataSectors[NumDirect - 1]);

        for(int i = 0; i < numSectors - NumDirect; i++)
            freeMap->Clear((int) dataSectors2[i]);

    }
    
}

//----------------------------------------------------------------------
// FileHeader::FetchFrom
// 	Fetch contents of file header from disk. 
//
//	"sector" is the disk sector containing the file header
//----------------------------------------------------------------------

void
FileHeader::FetchFrom(int sector)
{
    synchDisk->ReadSector(sector, (char *)this);
}

//----------------------------------------------------------------------
// FileHeader::WriteBack
// 	Write the modified contents of the file header back to disk. 
//
//	"sector" is the disk sector to contain the file header
//----------------------------------------------------------------------

void
FileHeader::WriteBack(int sector)
{
    synchDisk->WriteSector(sector, (char *)this); 
}

//----------------------------------------------------------------------
// FileHeader::ByteToSector
// 	Return which disk sector is storing a particular byte within the file.
//      This is essentially a translation from a virtual address (the
//	offset in the file) to a physical address (the sector where the
//	data at the offset is stored).
//
//	"offset" is the location within the file of the byte in question
//----------------------------------------------------------------------

int
FileHeader::ByteToSector(int offset)
{
    if(offset/SectorSize < NumDirect - 1)
        return(dataSectors[offset / SectorSize]);
    else
    {
        int dataSectors2[SecondNumDirect];
        synchDisk->ReadSector(dataSectors[NumDirect - 1], (char *)dataSectors2);
        return (dataSectors2[offset / SectorSize - NumDirect + 1]);
    }
}

//----------------------------------------------------------------------
// FileHeader::FileLength
// 	Return the number of bytes in the file.
//----------------------------------------------------------------------

int
FileHeader::FileLength()
{
    return numBytes;
}

//----------------------------------------------------------------------
// FileHeader::Print
// 	Print the contents of the file header, and the contents of all
//	the data blocks pointed to by the file header.
//----------------------------------------------------------------------
void FileHeader::Print()
{
    int i, j, k;
    char *data = new char[SectorSize];
    if(dataSectors[NumDirect - 1] == -1)
    {
        printf("FileHeader contents. File size: %d. File blocks:\n", numBytes);
        for (i = 0; i < numSectors; i++)
            printf("%d ", dataSectors[i]);
        printf("\nFile contents:\n");

        for (i = k = 0; i < numSectors; i++) 
        {
            synchDisk->ReadSector(dataSectors[i], data);
            for (j = 0; (j < SectorSize) && (k < numBytes); j++, k++) 
            {
                if ('\040' <= data[j] && data[j] <= '\176')   // isprint(data[j])
                    printf("%c", data[j]);
                else
                    printf("\\%x", (unsigned char)data[j]);
            }
            printf("\n"); 
        }
    }
    else
    {
        int dataSectors2[SecondNumDirect];
        synchDisk->ReadSector(dataSectors[NumDirect - 1], (char *)dataSectors2);
        printf("FileHeader contents. File size: %d. File blocks:\n", numBytes);
        for (i = 0; i < NumDirect - 1; i++)
            printf("%d ", dataSectors[i]);
        for(; i < numSectors; i++)
            printf("%d ", dataSectors2[i - (NumDirect - 1)]);
        printf("\nFile contents:\n");
        for (i = k = 0; i < NumDirect - 1; i++) 
        {
            synchDisk->ReadSector(dataSectors[i], data);
            for (j = 0; (j < SectorSize) && (k < numBytes); j++, k++) 
            {
                if ('\040' <= data[j] && data[j] <= '\176')   // isprint(data[j])
                    printf("%c", data[j]);
                else
                    printf("\\%x", (unsigned char)data[j]);
            }
        printf("\n"); 
        }
        for( ; i < numSectors; i++) 
        {
            synchDisk->ReadSector(dataSectors2[i - NumDirect + 1], data);
            for (j = 0; (j < SectorSize) && (k < numBytes); j++, k++) 
            {
                if ('\040' <= data[j] && data[j] <= '\176')   // isprint(data[j])
                    printf("%c", data[j]);
                else
                    printf("\\%x", (unsigned char)data[j]);
            }
            printf("\n"); 
        }
    }
    delete [] data;
}

bool FileHeader::AddSectors(int appendSize)
{
    if(appendSize<0) return false;
    //参考filesys.cc中的写法，打开位示图文件
    OpenFile* bitmap = new OpenFile(0);
    BitMap* freeMap;
    freeMap = new BitMap(NumSectors);
    freeMap->FetchFrom(bitmap);
    //最大可容纳大小
    int maxAccom = SectorSize * numSectors;
    if(maxAccom - numBytes < appendSize)
    {//剩余空间小于扩展大小,需要分配新的扇区
        int newAllocateSize = appendSize - (maxAccom - numBytes);
        int appendSectors = divRoundUp(newAllocateSize,SectorSize);
        //检查当前剩余空闲扇区
        if(freeMap->NumClear() < appendSectors) return FALSE;
        else if(NumDirect + SecondNumDirect - 1 <= numSectors + appendSectors)
            return FALSE;
        //空扇区满足要求，分配扇区，修改文件头信息
       if(dataSectors[NumDirect - 1]==-1)
       {//原来无二级索引
            if(numSectors + appendSectors <= NumDirect - 1)
            {//之后依然不需要二级索引
                for(int i = numSectors;i < numSectors + appendSectors;i++)
                {
                    dataSectors[i] = freeMap->Find();
                }
                numBytes += appendSize;
                numSectors += appendSectors;
                //位示图写回
                freeMap->WriteBack(bitmap);
                delete bitmap;
                delete freeMap;
                return TRUE;
            }
            else
            {//否则
                for(int i = numSectors;i < NumDirect - 1;i++)
                {
                    dataSectors[i] = freeMap->Find();
                }
                dataSectors[NumDirect - 1]  = freeMap->Find();
                int dataSectors2[SecondNumDirect];
                for (int i = 0; i < appendSectors - (NumDirect - 1 - numSectors) ; i++)
                    dataSectors2[i] = freeMap->Find();
                synchDisk->WriteSector(dataSectors[NumDirect - 1], (char *)dataSectors2);
                numBytes += appendSize;
                numSectors += appendSectors;
                //位示图写回
                freeMap->WriteBack(bitmap);
                delete bitmap;
                delete freeMap;
                return TRUE;
            }       
       }
       else
       {//原来有二级索引
           int dataSectors2[SecondNumDirect];
            synchDisk->ReadSector(dataSectors[NumDirect - 1], (char *)dataSectors2);
            for(int i = numSectors - NumDirect + 1 ; i < numSectors - NumDirect + 1 + appendSectors; i++)
                dataSectors2[i] = freeMap -> Find();
            synchDisk->WriteSector(dataSectors[NumDirect - 1], (char *)dataSectors2);
            numBytes += appendSize;
            numSectors += appendSectors;
            //位示图写回
            freeMap->WriteBack(bitmap);
            delete bitmap;
            delete freeMap;
            return TRUE;
       }
 
    }
    else
    {
        numBytes += appendSize;
        freeMap->WriteBack(bitmap);
        delete bitmap;
        delete freeMap;
        return TRUE;
    }
}
