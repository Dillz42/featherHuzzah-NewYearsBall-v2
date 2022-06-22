#ifndef _NYE_NEOPIXEL_TIME_H
#define _NYE_NEOPIXEL_TIME_H

class NewYearsBall_Time
{
public:
	static String uptime()
	{
		int sec = millis() / 1000;
		int min = sec / 60;
		int hr = min / 60;

		char uptime[15];
		sprintf(uptime, "%02d:%02d:%02d.%03d", hr, min % 60, sec % 60, millis() - (sec * 1000));
		//00:00:00.000

		return String(uptime);
	}
};

#endif