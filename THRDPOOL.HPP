/************************************************************************

      File Name..........: THRDPOOL.HPP
      File Description...: Thread Pool Class Header
      Date Written.......: 14-Nov-1991
      Written By.........: Steve Horne / CoralSoft, Inc.
                           305 Judson Dr. E.
                           Mobile, AL  36608
                           (205) 344-2251

-------------------------------------------------------------------------

   Description
   -----------
   See the description in THRDPOOL.CPP

   $Log:   E:/CSICLASS/INCLUDE/VCS/THRDPOOL.HVP  $
   
      Rev 1.0   02 Feb 1992 18:52:48
   Initial revision.

-------------------------------------------------------------------------

   The C and C++ structures and code in this document have been created
   by CoralSoft, Inc. and are considered proprietary and confidential.
   This information may not be distributed by any means, electronic or
   mechanical, without the prior written consent of CoralSoft, Inc.
   No warranties are either EXPRESSED or IMPLIED.
   Copyright (c) 1991, CoralSoft, Inc.

************************************************************************/
#define   _MT   1
const USHORT TP_MINIMUM_STACK =         2048;

//      Thread Pool Result Codes...
const USHORT TP_OK =                     0;
const USHORT TP_NOTHREADS =            1;
const USHORT TP_NOMEMORY =               2;

//   Thread commands to execute when pSignals[] is clear
enum TP_THREAD_COMMAND {
   TPC_INITIALIZE,
   TPC_DISPATCH,
   TPC_TERMINATE
};

enum TP_THREAD_STATE {
   TPS_WAITING,                     // Waiting for work
   TPS_USERFUNC,                     // Executing a user function
   TPS_STOPPED                        // Terminated by killAll
};

class ThreadPool {
   TID *pTIDs;                     
   RESULTCODES *pResults;          
   ULONG *pSignals;                
   TP_THREAD_STATE *pStates;       
   int thread_count;               
   USHORT stack_size;              
   USHORT lastError;
   ULONG poolSem;                  
   int active_threads;              
   int current_index;               
   HSEM current_notify;             
   TP_THREAD_COMMAND current_cmd;   
   void (*current_function)(PVOID args);
   PVOID current_argument;          
public:
   VOID FAR internal_thread(void);   // Thread function
   ThreadPool(int count, USHORT stack = 2048);
   ~ThreadPool(void);
   USHORT start(void (*func)(PVOID), PVOID arg = NULL, HSEM notify = NULL);
   USHORT error(void);
   void killAll(void);

};

extern "C" void C_thread(ThreadPool *tp);

