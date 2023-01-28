#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <spawn.h>
#include <notify.h>
#include <unistd.h>
#include <crt_externs.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <dlfcn.h>
#include <objc/objc.h>
#include <mach-o/dyld.h>
#define environ (*_NSGetEnviron())

static void reportActualStackSize (void)
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

static void * threadFunc(void * pdesiredstacksize)
{
	printf("Just entered threadFunc and have not fork()ed.\n");
	reportActualStackSize();
	if(fork()==0)	{
		printf("fork() is done and we are in the child process.\n");
	} else {
		printf("fork() is done and we are in the parent process.\n");
	}

	//if(actualStackSize < 8388608)
	{
	//   exit(0);
	}
	reportActualStackSize();
   //getchar();
   //execl("/system/library/privateframeworks/printkit.framework/printd", NULL);

   //execl("./exe2", NULL);
   //execl("./exe",NULL); //Don't uncomment this line as it will lead to a fork bomb!

   //int argc=2;
   //const char *argv[2] = {"/system/library/privateframeworks/printkit.framework/printd", NULL};
   //callentrypointofimage(argv[0], argc, (char *const*) argv);
   getchar();
   
   return NULL;
}

static void testCustomStackSize(size_t desiredStackSizeBytes)
{
   pthread_attr_t attr;
   if (pthread_attr_init(&attr) != 0)
   {
      perror("pthread_attr_init failed");

      return;
   }

   int r = pthread_attr_setstacksize(&attr, desiredStackSizeBytes);
   if (r == 0)
   {
      pthread_t thread;
      if (pthread_create(&thread, &attr, threadFunc, &desiredStackSizeBytes) == 0)
      {
         if (pthread_join(thread, NULL) != 0) perror("pthread_join failed");
      }
      else perror("pthread_create failed");
   }
   else
   {
      perror("pthread_attr_setstacksize failed");
      printf("pthread_attr_setstacksize failed and returned %i\n", r);
   }
}

int main(int argv, char ** argc)
{
   //testCustomStackSize(16384);
   printf("Now we are in main() proper. The stack size is now ");
   reportActualStackSize();
   printf("Also, sanity check: we have %d parameter, the 0-th being %s.\n", argv, argc[0]);
   return 0;
}
