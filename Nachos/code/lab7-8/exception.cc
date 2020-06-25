// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

void advancePC()
{
    //更新相关寄存器，具体定义在machine.h
    machine->WriteRegister(PrevPCReg,machine->ReadRegister(PCReg));
    machine->WriteRegister(PCReg,machine->ReadRegister(PCReg)+4);
    machine->WriteRegister(NextPCReg,machine->ReadRegister(NextPCReg)+4);
}
extern void StartProcess(int id);
extern int returnedSpaceID;
void ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);

    if ((which == SyscallException) && (type == SC_Halt)) 
    {
	    DEBUG('a', "Shutdown, initiated by user program.\n");
   	    interrupt->Halt();
    }
    else if((which == SyscallException) && (type == SC_Exec))
    {
        //读取参数
        int regArg = machine->ReadRegister(4);
        char name[128];
        int i = 0;
        do{//读取文件名
            machine->ReadMem(regArg+i,1,(int*)&name[i]);
        }while(name[i++]!='\0');
	printf("preapare to execute %s\n",name);
        OpenFile* executable = fileSystem->Open(name);
	if(executable==NULL)
	{
	    printf("Unable to open file %s\n",name);
	    return ;
	}
	AddrSpace* space = new AddrSpace(executable);
	delete executable;
	
	Thread* thread = new Thread("forked thread");
	thread->Fork(StartProcess,space->getSpaceID());
        //寄存器写入返回值
        machine->WriteRegister(2,returnedSpaceID);
        //PC相关寄存器更新
        advancePC();
    }
    else if((which == SyscallException) && (type == SC_Exit))
    {
	int status = machine->ReadRegister(4);
	printf("thread %s calls Exit(),Exit status:%d\n",currentThread->getName(),status);
	delete currentThread->space;
        advancePC();
        currentThread->Finish();
    }
    else if((which == SyscallException) && (type == SC_Join))
    {
  	int argId = machine->ReadRegister(4);
	printf("prepare to join %d\n",argId);
	currentThread->Join(argId);
	printf("Join finished,waitProcessExitCode is %d\n",currentThread->waitProcessExitCode);
	machine->WriteRegister(2,currentThread->waitProcessExitCode);
        advancePC();
    }
	else if((which == SyscallException) && (type == SC_Write))
	{
		printf("system call write.\n");
		int address = machine->ReadRegister(4);
		int count = machine->ReadRegister(5);
		int fd = machine->ReadRegister(6);
		char data[count];
		int value;
		for(int i = 0; i < count; i++)
		{//从内存中读出buffer的内容
			machine->ReadMem(address+i,1,&value);
			data[i] = char(value);
		}
		if(fd != 1)
		{//不是1,不是输出到控制台，而是写入文件
			OpenFile *openfile = (OpenFile*)fd;
			openfile->Write(data,count);
		}
		else
		{//否则直接使用putchar输出到控制台
			for(int i = 0; i < count; i++)
				putchar(data[i]);
		}
		advancePC();
	}
	else if((which == SyscallException) && (type == SC_Read))
        {
		//printf("system call read.\n");
		int address = machine->ReadRegister(4);
		int count = machine->ReadRegister(5);
		int fd = machine->ReadRegister(6);
		char data[count];
		int result;
		if(fd != 0)
		{//从文件读出
			OpenFile *openfile = (OpenFile*)fd;
			
			result = openfile->Read(data,count);
			
		}
		else
		{//否则直接从控制台getchar
			for(int i = 0; i < count; i++)
				data[i] = getchar();
			result = count;
			
		}
		for(int i = 0; i < result; i++)
		{//写入到buffer
			machine->WriteMem(address+i,1,int(data[i]));
		}
		machine->WriteRegister(2,result);
		advancePC();
	}
    else 
    {
	printf("Unexpected user mode exception %d %d\n", which, type);
	ASSERT(FALSE);
    }
}

