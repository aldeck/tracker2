#ifndef CHRONO_H

#include <OS.h>


class Chrono {
public:
				Chrono()
				{
					fStartTime = system_time();
				}
				
	void		Stop(const char * name)
				{
					fEndTime = system_time();
					printf("%s %lliµs\n", name, fEndTime - fStartTime);
				}
				
				
protected:
	bigtime_t	fStartTime;
	bigtime_t	fEndTime;
};

#endif //	CHRONO_H
