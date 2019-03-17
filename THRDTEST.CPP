/************************************************************************

      File Name..........: THRDPOOL.CPP
      File Description...: Thread Pool Test Program
      Date Written.......: 2-Feb-1992
      Written By.........: Steve Horne / CoralSoft, Inc.
                           305 Judson Dr. E.
                           Mobile, AL  36608
                           (205) 344-2251

-------------------------------------------------------------------------

   $Log:   E:/CSICLASS/SOURCE/VCS/THRDTEST.CVP  $
   
      Rev 1.0   02 Feb 1992 18:54:26
   Initial revision.

-------------------------------------------------------------------------

   The C and C++ structures and code in this document have been created
   by CoralSoft, Inc. and are considered proprietary and confidential.
   This information may not be distributed by any means, electronic or
   mechanical, without the prior written consent of CoralSoft, Inc.
   No warranties are either EXPRESSED or IMPLIED.
   Copyright (c) 1991, CoralSoft, Inc.

************************************************************************/
#define   INCL_BASE
#define   INCL_NOPM
#include   <os2.h>
#include   <stdlib.h>
#include <process.h>
#include <disp.h>
#include "thrdpool.hpp"

void test_thread(PVOID arg);

//   'global_sem' is used to coordinate use of the "DISP" package between
//   the threads.
ULONG global_sem = 0L;

const int thread_column = 22;
typedef struct _THREADDATA {
   int tid;
   int count;
   ULONG delay;
   int row;
} THREADDATA;

main()
{
   ULONG tt_sem1 = 0L;
   ULONG tt_sem2 = 0L;
   ULONG tt_sem3 = 0L;

   disp_open();                           // open display package
   disp_move(0,0);                        // and clear screen
   disp_eeop();
   disp_box(0, DISP_NORMAL, 1, 1, 7, 40);   // box around thread output
   disp_move(2, 5);
   disp_printf("Thread 1 output:");
   disp_move(4, 5);
   disp_printf("Thread 2 output:");
   disp_move(6, 5);
   disp_printf("Thread 3 output:");

//   Initialize the ThreadPool object
   ThreadPool pool(5);

//   Initialize the termination semaphores for all three threads
   DosSemSet(&tt_sem1);
   DosSemSet(&tt_sem2);
   DosSemSet(&tt_sem3);

//   Create & initialize THREADDATA structures for the threads to use
//   as arguments.
   THREADDATA data1 = {1,  5, 250L, 2};
   THREADDATA data2 = {2, 10, 250L, 4};
   THREADDATA data3 = {3, 15, 250L, 6};

//   Start thread 1
   USHORT rc= pool.start(test_thread, (PVOID) &data1, &tt_sem1);
   if (rc)
      disp_printf("\nERROR %d in pool.start()", rc);

//   Start thread 2
   rc= pool.start(test_thread, (PVOID) &data2, &tt_sem2);
   if (rc)
      disp_printf("\nERROR %d in pool.start()", rc);

//   Start thread 3
   rc= pool.start(test_thread, (PVOID) &data3, &tt_sem3);
   if (rc)
      disp_printf("\nERROR %d in pool.start()", rc);

   DosSemRequest(&global_sem, SEM_INDEFINITE_WAIT);
   disp_move(2, 45);
   disp_printf("MAIN waiting...");
   DosSemClear(&global_sem);
   DosSemWait(&tt_sem1, SEM_INDEFINITE_WAIT);
   DosSemRequest(&global_sem, SEM_INDEFINITE_WAIT);
   disp_move(2, 45);
   disp_printf("Finished.......");
   disp_move(4, 45);
   disp_printf("MAIN waiting...");
   DosSemClear(&global_sem);
   DosSemWait(&tt_sem2, SEM_INDEFINITE_WAIT);
   DosSemRequest(&global_sem, SEM_INDEFINITE_WAIT);
   disp_move(4, 45);
   disp_printf("Finished.......");
   disp_move(6, 45);
   disp_printf("MAIN waiting...");
   DosSemClear(&global_sem);
   DosSemWait(&tt_sem3, SEM_INDEFINITE_WAIT);
   disp_move(6, 45);
   disp_printf("Finished.......");
   disp_move(9, 0);
   disp_printf("Main has finished... All threads done.");
   disp_close();
   return(0);
}

//   'test_thread' is the function executed by each of the threads started...
//   Notice that this is a C++ function, not C.
//   The threads do different things depending on the argument passed, which
//   is actually a pointer to a THREADDATA structure.

void test_thread(PVOID arg)
{
THREADDATA *td;

   td= (THREADDATA *) arg;
   for (int cnt= 0; cnt < td->count; cnt++)
      {
      DosSleep(td->delay);
      DosSemRequest(&global_sem, SEM_INDEFINITE_WAIT);
      disp_move(td->row, thread_column + cnt);
      disp_printf("+");
      disp_flush();
      DosSemClear(&global_sem);
      }
   DosSemRequest(&global_sem, SEM_INDEFINITE_WAIT);
   disp_move(td->row, thread_column + cnt);
   disp_printf("*");
   disp_flush();
   DosSemClear(&global_sem);
}



