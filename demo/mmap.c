/*
 * Example of using commands - asynchronous input
 * Part of Comedilib
 *
 * Copyright (c) 1999,2000 David A. Schleef <ds@schleef.org>
 *
 * This file may be freely modified, distributed, and combined with
 * other software, as long as proper attribution is given in the
 * source code.
 */

/*
 * An example for directly using Comedi commands.  Comedi commands
 * are used for asynchronous acquisition, with the timing controlled
 * by on-board timers or external events.
 */

#include <stdio.h>
#include <comedilib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include "examples.h"

unsigned int chanlist[256];

void *map;

int prepare_cmd_lib(comedi_t *dev, int subdevice, int n_scan, int n_chan, unsigned period_nanosec, comedi_cmd *cmd);
int prepare_cmd(comedi_t *dev, int subdevice, int n_scan, int n_chan, unsigned period_nanosec, comedi_cmd *cmd);


int main(int argc, char *argv[])
{
	comedi_t *dev;
	comedi_cmd c,*cmd=&c;
	unsigned int bufsize;
	unsigned int front, back;
	int ret;
	unsigned int i;
	unsigned int bufpos;
	unsigned int subdev_flags;
	unsigned int sample_size;
	unsigned int nsamples;
	unsigned int col;
	struct parsed_options options;

	init_parsed_options(&options);
	parse_options(&options, argc, argv);

	dev = comedi_open(options.filename);
	if(!dev){
		comedi_perror(options.filename);
		exit(1);
	}

	ret = comedi_get_buffer_size(dev, options.subdevice);
	if(ret < 0){
		comedi_perror("comedi_get_buffer_size");
		exit(1);
	}
	bufsize = ret;
	fprintf(stderr,"buffer size is %u\n", bufsize);
	ret = comedi_get_subdevice_flags(dev, options.subdevice);
	if(ret < 0){
		comedi_perror("comedi_get_subdevice_flags");
	}
	subdev_flags = ret;
	sample_size = (subdev_flags & SDF_LSAMPL)
		? sizeof(lsampl_t) : sizeof(sampl_t);
	fprintf(stderr,"sample size is %u\n", sample_size);

	map = mmap(NULL,bufsize,PROT_READ,MAP_SHARED, comedi_fileno(dev), 0);
	fprintf(stderr, "map=%p\n", map);
	if( map == MAP_FAILED ){
		perror( "mmap" );
		exit(1);
	}

	for(i = 0; i < options.n_chan; i++){
		chanlist[i] = CR_PACK(options.channel + i, options.range, options.aref);
	}

	//prepare_cmd_lib(dev, options.subdevice, options.n_scan, options.n_chan, 1e9 / options.freq, cmd);
	prepare_cmd(dev, options.subdevice, options.n_scan, options.n_chan, 1e9 / options.freq, cmd);

	ret = comedi_command_test(dev, cmd);

	ret = comedi_command_test(dev, cmd);

	if(ret != 0){
		fprintf(stderr,"command_test failed\n");
		exit(1);
	}

	dump_cmd(stderr, cmd);

	ret = comedi_command(dev, cmd);
	if(ret < 0){
		comedi_perror("comedi_command");
		exit(1);
	}

	front = 0;
	back = 0;
	bufpos = 0;
	col = 0;
	while(1){
		ret = comedi_get_buffer_contents(dev, options.subdevice);
		if(ret < 0){
			comedi_perror("comedi_get_buffer_contents");
			break;
		}
		front += ret;
		nsamples = (front - back) / sample_size;
		front = back + nsamples * sample_size;
		if(options.verbose) fprintf(stderr, "front = %u, back = %u, samples = %u\n", front, back, nsamples);
		if(front == back){
			//comedi_poll(dev, options.subdevice);
			usleep(10000);
			continue;
		}
		for(i = 0; i < nsamples; i++){
			unsigned int sample;

			if(sample_size == sizeof(sampl_t))
				sample = *(sampl_t *)((char *)map + bufpos);
			else
				sample = *(lsampl_t *)((char *)map + bufpos);
			printf("%u ", sample);
			col++;
			if(col == options.n_chan){
				printf("\n");
				col = 0;
			}
			bufpos += sample_size;
			if(bufpos >= bufsize){
				bufpos = 0;
			}
		}

		ret = comedi_mark_buffer_read(dev, options.subdevice, front - back);
		if(ret < 0){
			comedi_perror("comedi_mark_buffer_read");
			break;
		}
		back = front;
	}

	return 0;
}

int prepare_cmd_lib(comedi_t *dev, int subdevice, int n_scan, int n_chan, unsigned scan_period_nanosec, comedi_cmd *cmd)
{
	int ret;

	ret = comedi_get_cmd_generic_timed(dev, subdevice, cmd, n_chan, scan_period_nanosec);
	if(ret<0){
		comedi_perror("comedi_get_cmd_generic_timed\n");
		return ret;
	}

	cmd->chanlist = chanlist;
	cmd->chanlist_len = n_chan;
	if(cmd->stop_src == TRIG_COUNT) cmd->stop_arg = n_scan;

	return 0;
}

int prepare_cmd(comedi_t *dev, int subdevice, int n_scan, int n_chan, unsigned period_nanosec, comedi_cmd *cmd)
{
	memset(cmd,0,sizeof(*cmd));

	cmd->subdev = subdevice;

	cmd->flags = 0;

	cmd->start_src = TRIG_NOW;
	cmd->start_arg = 0;

	cmd->scan_begin_src = TRIG_TIMER;
	cmd->scan_begin_arg = period_nanosec;

	cmd->convert_src = TRIG_TIMER;
	cmd->convert_arg = 1;

	cmd->scan_end_src = TRIG_COUNT;
	cmd->scan_end_arg = n_chan;

	cmd->stop_src = TRIG_NONE;
	cmd->stop_arg = 0;

	cmd->chanlist = chanlist;
	cmd->chanlist_len = n_chan;

	return 0;
}



