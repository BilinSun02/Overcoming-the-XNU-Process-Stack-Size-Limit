#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>
#include<dlfcn.h>

int mArgc;
char **mArgv;

static void dylibReportActualStackSize(void);
pthread_t runbeforemain;

static void threadFunc(void)
{
	pid_t myChild;
	
	printf("Entered threadFunc().\n");

	/*//Let's temporarily try doing everything just in the parent process without fork()ing.
	myChild=fork();
	if(myChild!=0) 
	{
		int childReturnVal=0;
		waitpid(myChild,&childReturnVal,0); //We only need one process in the end. The child will hopefully have only the desired thread and so will be useful; the parent should then just be killed.
		//Better use wait() or waitpid() so that the parent doesn't exit right away and leave the child a zombie.
		
		printf("Child process ended and returned %d. Now the parent process is ending...");
		exit(childReturnVal);
	}
	*/

	printf("We're now in threadFunc. The stack size is");
	dylibReportActualStackSize();

	printf("Now let's see if we ever go back to the hijacked workflow...\n");
	void *mainProgramHandle = dlopen(NULL, RTLD_NOW);
	int (*mainAddr)(int,char**) = dlsym(mainProgramHandle,"main");
	printf("Found the program entry at address %u\n",(unsigned long)mainAddr);
	exit((*mainAddr)(mArgc,mArgv));
}

static void dylibReportActualStackSize (void)
{
	size_t actualStackSize=pthread_get_stacksize_np(pthread_self());
//	pthread_attr_t tattr;
//	if (pthread_attr_init(&tattr) == 0)
//	{
//		pthread_getaddr_np(pthread_self(), &tattr);
//	 	if (pthread_attr_getstacksize(&tattr, &actualStackSize) == 0)
//	   	{
//			perror("pthread_attr_getstacksize failed");
//			return;
//		}
//	}
//	else
//	{
//		perror("pthread_attr_init(2) failed when trying to report actual stack size.");
//		return;
//	}
	printf("the current stack size is %ul\n", actualStackSize);
}

static void spawnThreadWithCustomStackSize(size_t desiredStackSizeBytes)
{
   int wentWrong=0;
   pthread_attr_t attr;
   if (pthread_attr_init(&attr) != 0)
   {
      perror("pthread_attr_init failed");
      wentWrong=1;
   }

   int r = pthread_attr_setstacksize(&attr, desiredStackSizeBytes);
   if (r == 0)
   {
      pthread_t thread;
      if (pthread_create(&thread, &attr, threadFunc, &desiredStackSizeBytes) == 0)
      {
         if (pthread_join(thread, NULL) != 0)
	 {
	     perror("pthread_join failed");
	     wentWrong=1;
	 }
      }
      else
      {
	      perror("pthread_create failed");
	      wentWrong=1;
      }
   }
   else
   {
      perror("pthread_attr_setstacksize failed");
      printf("pthread_attr_setstacksize failed and returned %i\n", r);
      wentWrong=1;
   }

   if(wentWrong==1)//If the program ever reaches here then it is probably not working already, but if we have wentWrong==1 then we know a little better about the exact problem...
   {
	   printf("One of the pthread functions failed.\n");
	   exit(1);
   }
   else
   {
	   printf("I don't know what has gone wrong but we shouldn't have reached here.\n");
	   exit(2);
   }
}

__attribute__((constructor))
	static void runBeforeMain(int argc,const char **argv)
{
	mArgc=argc; mArgv=argv;
	runbeforemain=pthread_self();
	printf("Hello from dylib!\n");
	spawnThreadWithCustomStackSize(6553600);
	//We just expect the new thread to do all the work. In the parent process the new thread will just kill the process. In the child process the old thread will not be present at all. In any case, we just let the old thread halt and wait for the things next, and the use of pthread_join() in spawnThreadWithCustomStackSize() already makes sure the old thread halts and waits.
}
