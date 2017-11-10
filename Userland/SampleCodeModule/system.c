#include <syscall.h>
#include <string.h>

#define	TIME_FMT_LONG		9	//	HH:MM:SS0
#define	DATE_FMT_LONG		9	//	DD/MM/AA0
#define	TIMEDATE_FMT_LONG	20	//	HH:MM:SS DD/MM/AA0


char*	getTimeString(){
	char time[TIME_FMT_LONG];
	int sec = getTime(0);
	int min = getTime(1);
	int hour = getTime(2);
	time[0] = '0' + hour/10;
	time[1] = '0' + hour%10;
	time[2] = ":";
	time[3] = '0' + min/10;
	time[4] = '0' + min%10;
	time[5]	= ":";
	time[6] = '0' + sec/10;
	time[7] = '0' + sec%10;
	time[TIME_FMT_LONG - 1] = 0;
	printf("%s\n",time);
	return time;
}

char*	getDateString(){
	char date[DATE_FMT_LONG];
	int day = getTime(3);
	int month = getTime(4);
	int year = getTime(5);
	date[0] = (char) '0' + day/10;
	date[1] = (char) '0' + day%10;
	date[2] = "/";
	date[3] = (char) '0' + month/10;
	date[4] = (char) '0' + month%10;
	date[5] = "/";
	date[6] = (char) '0' + year/10;
	date[7] = (char) '0' + year%10;
	date[DATE_FMT_LONG - 1] = 0; 
	printf("%s\n",date);
	return date;
}

char*	getTimeDateString(){
	char timeDate[TIMEDATE_FMT_LONG];
	char* time = getTimeString();
	char* date = getDateString();
	strcpy(timeDate,time);
	strcpy(timeDate[TIME_FMT_LONG]," ");
	strcpy(timeDate[TIME_FMT_LONG + 1],date);
	timeDate[TIMEDATE_FMT_LONG - 1] = 0;
	return timeDate;
}