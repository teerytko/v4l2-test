/*
 * v4l-test: Test environment for Video For Linux Two API
 *
 *  3 Apr 2009  0.2  Added camera enabling through
 *                   /sys/devices/platform/eeepc/camera
 * 18 Dec 2008  0.1  First release
 *
 * Written by Márton Németh <nm127@freemail.hu>
 * Released under GPL
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "dev_video.h"

#define DEV_VIDEO_STATE_NORMAL		0
#define DEV_VIDEO_STATE_ASUS		1
#define DEV_VIDEO_STATE_EEEPC		2

static int f;
static int dev_video_state;

static int try_camera(char *path) {
	f = open(path, O_RDWR);
	if (f < 0) {
		printf("Cannot open %s", path);
		return f;
	}

	return f;
}

int open_video() {
	int error = 0;

	fflush(stdout);

	f = try_camera("/dev/video0");
	if (f < 0) {
		error = 1;
	} else {
		dev_video_state = DEV_VIDEO_STATE_NORMAL;
	}
	return error;
}

int close_video() {
	int ret;

	fflush(stdout);

	ret = close(f);
	f = 0;
	if (ret < 0) {
		perror("Cannot close");
		return 1;
	}
	switch (dev_video_state) {
		case DEV_VIDEO_STATE_NORMAL:
			break;
	}

	return 0;
}

int get_video_fd() {
	return f;
}
