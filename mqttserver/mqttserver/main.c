#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include <signal.h>
#include"elog.h"
#include"sqlWrapper.h"
#include"mqttserver.h"

#define		LOG_TAG			"main"

#if 0
void Signal_Handler(int signal)
{
    int nptrs;
    void *buffer[256];
    char **strings;
    int i = 0;
    log_e("signal_handler  singnal is %d\n",signal);
    nptrs = backtrace(buffer, 256);
    strings = backtrace_symbols(buffer, nptrs);
    if (strings == NULL)
    {
        printf("##################End of stack trace#####################");
        return;
    }
    log_e("##################Start of stack trace#####################");
    for(i = 0; i < nptrs; ++i){
        printf("[BACKTRACE]%s\n",strings[i]);
    }
    printf("##################End of stack trace#####################");
    free(strings);

    printf("system recv err singal reset gateway\n");
    return ;
}

void InitSignalHandler()
{
        signal(SIGABRT, Signal_Handler);
        signal(SIGBUS, Signal_Handler);
        signal(SIGFPE, Signal_Handler);
        signal(SIGUSR1, Signal_Handler);
        signal(SIGSEGV, Signal_Handler);
        signal(SIGPIPE, Signal_Handler);
}
#endif

void loginit(void)
{
        /* close printf buffer */
    setbuf(stdout, NULL);
    /* initialize EasyLogger */
    elog_init();
    /* set EasyLogger log format */
    elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL);
    elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL & ~ELOG_FMT_FUNC);
    elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL & ~ELOG_FMT_FUNC);
#ifdef ELOG_COLOR_ENABLE
    elog_set_text_color_enabled(true);
#endif
    /* start EasyLogger */
    elog_start();

    log_i("init loginit function\n");
}

int main(void)
{
	//	InitSignalHandler();
	loginit();

	sqlWrapper_Init();

	MQTTSERVER_Init();

	while(1)
	{
		sleep(10);
	}

	return 0;
}


