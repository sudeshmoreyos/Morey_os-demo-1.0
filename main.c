#include "morey_os.h"

void setup(void);
extern struct task_struct_os * autostart_tasks[];
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
