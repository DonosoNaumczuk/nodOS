#ifndef	_system_h_
#define	_system_h_

#define	TIME_FMT_LONG		9	//	HH:MM:SS0
#define	DATE_FMT_LONG		9	//	DD/MM/AA0
#define	TIMEDATE_FMT_LONG	20	//	HH:MM:SS DD/MM/AA0

void getTimeString(unsigned char *);		//HH:MM:SS
void getDateString(unsigned char *);		//DD/MM/AA
void getTimeDateString(unsigned char *);	//HH:MM:SS DD/MM/AA

#endif