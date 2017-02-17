#include "util.h"
void mySleep(double minutes) {
#if defined WIN32 || defined WIN64
	Sleep((unsigned long)(minutes * 60 * 1000));
#else 
	sleep((unsigned long)(minutes * 60));
#endif
}
