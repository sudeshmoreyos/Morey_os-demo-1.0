#include "morey_os.h"

void setup(void);
extern struct process * autostart_processes[];
#ifdef DISABLE_OS
	void loop(void);
#endif

int main(void)
{
    setup();
    #ifdef DISABLE_OS
        while(1)
        {
            loop();
        }
    #else
        OS_INIT();
    #endif
}
