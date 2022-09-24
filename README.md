# Morey_os

**What is Morey_os?**

Morey_os is an extremely light weight operating system for embedded controllers. Simply, Morey_os is designed as a **multi-tasking version** of popular **Arduino programming style**, with one **setup** function and multiple independently running **loop** functions named as **Tasks**. A Full version OS fits in minimum of 4K ROM and 256 bytes of RAM. Hence it can support even very low resource controllers like Atmega8 with 8K ROM and 1K RAM. 

**How is Morey_os built?**

Morey_os architechture broadly consists of three parts :
1. **Kernel** : Modified and trimmed down version of popular **contiki os kernel**, supporting only **cooperative threads**, implemented using files like process.h, ptimer.h (modified event timer) and clock.h. In future versions, this kernel can be replaced by more advance pre-emptive threads as well.
2. **OS drivers and libs** : Controller architechture independent Morey_os drivers and libraries like Serial.h , Digital.h, lcd.h, etc.
3. **Porting files** : Files to implement controller architechture dependent drivers and libraries. 

**Why Morey_OS?**

1. Arduino became very popular among hobbyist and electronic enthusiasts, due to easy programming interface and versatile embedded boards. Arduino however is not designed to work for custom embedded hardwares. 

2. Bare metal embedded C/C++ based programming do support custom embedded hardwares, but it is comparatively much difficult to code and do not support multi-tasking. Also C/C++ codes of one controller architecture like AVR is not compatible to other architechture like PIC, making it non portable.

3. Many popular Embedded OS are developed to support advance features like Multi-tasking, portability across different controller architecture, Queues, Mutexes, Semaphores, etc. Examples are Contiki-OS, FreeRtos, RIOT OS, Linux based OS. These OS mostly works well on resource heavy controllers like ARM7, ARM9, etc. Further due to complex concepts only expert programmer can work well with these OS. 

4. This creates a wide gap between Arduino or C/C++ based programming Vs traditional Embedded OS based programming. Morey_os tries to fill this gap by providing basic OS functionality to low resource controllers. It is also designed to provide support to all controllers of a particular architecture rather than supporting only a few limited boards. This feature makes it highly useful for custom designed Embedded hardwares. Like for example, if you wrote a code for atmega8 controller, simply by changing one MACRO line in Makefile, same code will work for Atmega16 or Atmega32 or Atmega328 etc. or even other architechtures like PIC or ARM.

**What are features of Morey_os?**

Morey_os is still under development. To know supported features and supported controllers/architectures of latest version please refer to "Current Version" section. This sections mentions all intended features of Morey_os :

1. Multi-Tasking capabilities
2. If multi-tasking is not required, i.e. a code has only one thread or process or loop, OS features can be removed to minimize code size. 
3. Should support all controllers of any selected architechture. For example, AVR 8 Bit controllers may be broadly classified into three series namely Mega series, Xmega series and Mega0 series. So for mega series, OS should provide support for all controllers like atmega8, atmega88, atmega16, atmega162, atmega164, atmega32, atmega328, etc.
4. 100% portable code across controllers and architectures, only limited by capabilities of controllers itself.
5. Should support coding of both controllers or boards. Means OS should support programming of atmega328p controller as well as Arduino UNO or Arduino Mega boards based on atmega328P or atmega2560 controllers

**What are not features of Morey_os**

1. Currently Morey_os do not support real time operations.
2. It do not support premptive multi-threading.
3. Most advance features of advance OS like FreeRtos is not supported, due to limited resources supported by low resource controllers.

**Current Version**

1. Latest version is named as **Morey_os-demo-1.0** where digit before dot represents main version number while digit after dot represent sub-versions.
2. Current version provides support for Arduino Uno. This version is launched as demo version just for demonstration purposes to relevant stakeholders.
3. A full version Morey_os is already in the intermediate stage of development and is expected to be launched within a few months.
