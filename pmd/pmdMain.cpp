#include "core.hpp"
#include "pmd.hpp"
#include "pmdOptions.hpp"
#include "pd.hpp"

static int pmdResolveArguments(int argc, char **argv)
{
	int rc = EDB_OK;

	pmdOptions options;
	rc = options.init(argc, argv);
	if(rc)
	{
		if(EDB_PMD_HELP_ONLY != rc)//因为配置参数有-help或者-h时，rc也不为0
		{
			PD_LOG(PDERROR, "Failed to init options, rc = %d", rc);
			goto error;
		}
	}

	rc = pmdGetKRCB()->init(&options);//把options中的参数传入内核控制块
	if(rc)
	{
		PD_LOG(PDERROR, "failed to init krcb, rc = %d", rc);
		goto error;
	}

done: 
	return rc;
error:
	goto done;
	
}

struct _signalInfo
{
	const char *name;
	int handle;
};

typedef struct _signalInfo _signalInfo;

static _signalInfo signalHandleMap [] = {//主要定义收到哪些信号（handle==1）后需要关闭数据库
       { "Unknow", 0 },
       { "SIGHUP", 1 },     //1
       { "SIGINT", 1 },     //2
       { "SIGQUIT", 1 },    //3
       { "SIGILL", 1 },     //4
       { "SIGTRAP", 1 },    //5
       { "SIGABRT", 1 },    //6
       { "SIGBUS", 1 },     //7
       { "SIGFPE", 1 },     //8
       { "SIGKILL", 1 },    //9
       { "SIGUSR1", 0 },    //10
       { "SIGSEGV", 1 },    //11
       { "SIGUSR2", 0 },    //12
       { "SIGPIPE", 1 },    //13
       { "SIGALRM", 0 },    //14
       { "SIGTERM", 1 },    //15
       { "SIGSTKFLT", 0 },  //16
       { "SIGCHLD", 0 },    //17
       { "SIGCONT", 0 },    //18
       { "SIGSTOP", 1 },    //19
       { "SIGTSTP", 0 },    //20
       { "SIGTTIN", 0 },    //21
       { "SIGTTOU", 0 },    //22
       { "SIGURG", 0 },     //23
       { "SIGXCPU", 0 },    //24
       { "SIGXFSZ", 0 },    //25
       { "SIGVTALRM", 0 },  //26
       { "SIGPROF", 0 },    //27
       { "SIGWINCH", 0 },   //28
       { "SIGIO", 0 },      //29
       { "SIGPWR", 1 },     //30
       { "SIGSYS", 1 },     //31
       { "UNKNOW", 0 },     //32
       { "UNKNOW", 0 },     //33
       { "SIGRTMIN", 0 },   //34
       { "SIGRTMIN+1", 0 }, //35
       { "SIGRTMIN+2", 0 }, //36
       { "SIGRTMIN+3", 0 }, //37
       { "SIGRTMIN+4", 0 }, //38
       { "SIGTTMIN+5", 0 }, //39
       { "SIGRTMIN+6", 0 }, //40
       { "SIGRTMIN+7", 0 }, //41
       { "SIGTTMIN+8", 0 }, //42
       { "SIGRTMIN+9", 0 }, //43
       { "SIGRTMIN+10", 0 },//44
       { "SIGRTMIN+11", 0 },//45
       { "SIGRTMIN+12", 0 },//46
       { "SIGRTMIN+13", 0 },//47
       { "SIGRTMIN+14", 0 },//48
       { "SIGRTMIN+15", 0 },//49
       { "SIGRTMAX-14", 0 },//50
       { "SIGRTMAX-13", 0 },//51
       { "SIGRTMAX-12", 0 },//52
       { "SIGRTMAX-11", 0 },//53
       { "SIGRTMAX-10", 0 },//54
       { "SIGRTMAX-9", 0 }, //55
       { "SIGRTMAX-8", 0 }, //56
       { "SIGRTMAX-7", 0 }, //57
       { "SIGRTMAX-6", 0 }, //58
       { "SIGRTMAX-5", 0 }, //59
       { "SIGRTMAX-4", 0 }, //60
       { "SIGRTMAX-3", 0 }, //61
       { "SIGRTMAX-2", 0 }, //62
       { "SIGRTMAX-1", 0 }, //63
       { "SIGRTMAX", 0 },   //64
};    

#define PMD_MAX_SIGNALS 64
static void pmdSignalHandler(int sigNum)//信号处理函数
{
	if(sigNum > 0 && sigNum <= PMD_MAX_SIGNALS)
	{
		if(signalHandleMap[sigNum].handle)
		{
			EDB_SHUTDOWN_DB;
		}
	}
}

static int pmdSetupSignalHandler()
{
	int rc = EDB_OK;
	struct sigaction newact;
	memset(&newact, 0, sizeof(newact));
	sigemptyset(&newact.sa_mask);

	newact.sa_flags = 0;
	newact.sa_handler = (__sighandler_t)pmdSignalHandler;
	for(int i = 0; i < PMD_MAX_SIGNALS; ++i)
	{
		sigaction(i+1, &newact, NULL);
	}

	return rc;
}

int pmdMasterThreadMain(int argc, char **argv)
{
	int rc = EDB_OK;
    EDB_KRCB *krcb = pmdGetKRCB();

	//signal handler
	rc = pmdSetupSignalHandler();
	PD_RC_CHECK(rc, PDERROR, "Failed to setup signal handler, rc = %d", rc);

	//arguments
	rc = pmdResolveArguments(argc, argv);
	if(EDB_PMD_HELP_ONLY == rc)
	{
		goto done;
	}

	PD_RC_CHECK(rc, PDERROR, "Failed to resolve argument, rc = %d", rc);
	while(EDB_IS_DB_UP)
	{
		sleep(1);
	}
done:
	return rc;
error:
	goto done;
}


int main(int argc, char **argv)
{   
    return pmdMasterThreadMain(argc, argv);
}
