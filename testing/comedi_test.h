
#ifndef _COMEDI_TEST_H_
#define _COMEDI_TEST_H_

extern char *filename;
extern comedi_t *device;

extern int subdevice;

extern int verbose;

extern unsigned int capabilities;

extern int realtime;

extern int comedi_internal_trigger(comedi_t *device,unsigned int subdevice,
	unsigned int trignum);

#endif

