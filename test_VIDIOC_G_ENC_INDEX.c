/*
 * v4l-test: Test environment for Video For Linux Two API
 *
 * 25 Jul 2009  0.1  First release
 *
 * Written by Márton Németh <nm127@freemail.hu>
 * Released under GPL
 */

#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>

#include <linux/videodev2.h>
#include <linux/errno.h>

#include <CUnit/CUnit.h>

#include "v4l2_test.h"
#include "dev_video.h"
#include "video_limits.h"

#include "test_VIDIOC_G_ENC_INDEX.h"

static int valid_enc_idx_entry_flags(__u32 flags) {
	int valid = 0;

	switch (flags) {
	case V4L2_ENC_IDX_FRAME_I:
	case V4L2_ENC_IDX_FRAME_P:
	case V4L2_ENC_IDX_FRAME_B:
		valid = 1;
		break;
	default:
		valid = 0;
	}

	return valid;
}

void test_VIDIOC_G_ENC_INDEX() {
	struct v4l2_enc_idx enc_index;
	int ret_get, errno_get;
	int i;

	memset(&enc_index, 0xff, sizeof(enc_index));
	ret_get = ioctl(get_video_fd(), VIDIOC_G_ENC_INDEX, &enc_index);
	errno_get = errno;

	dprintf("\t%s:%u: VIDIOC_G_ENC_INDEX, ret_get=%i, errno_get=%i\n",
		__FILE__, __LINE__, ret_get, errno_get);

	if (ret_get == 0) {
		CU_ASSERT_EQUAL(ret_get, 0);

		CU_ASSERT(enc_index.entries <= enc_index.entries_cap);
		CU_ASSERT(0 < enc_index.entries_cap);
		CU_ASSERT_EQUAL(enc_index.reserved[0], 0);
		CU_ASSERT_EQUAL(enc_index.reserved[1], 0);
		CU_ASSERT_EQUAL(enc_index.reserved[2], 0);
		CU_ASSERT_EQUAL(enc_index.reserved[3], 0);

		for (i = 0; i < V4L2_ENC_IDX_ENTRIES; i++) {
			//CU_ASSERT_EQUAL(enc_index.entry[i].offset, ???);
			//CU_ASSERT_EQUAL(enc_index.entry[i].pts, ???);
			//CU_ASSERT_EQUAL(enc_index.entry[i].length, ???);
			CU_ASSERT(valid_enc_idx_entry_flags(enc_index.entry[i].flags));
			CU_ASSERT_EQUAL(enc_index.entry[i].reserved[0], 0);
			CU_ASSERT_EQUAL(enc_index.entry[i].reserved[1], 0);
		}

	} else {
		CU_ASSERT_EQUAL(ret_get, -1);
		CU_ASSERT_EQUAL(errno_get, EINVAL);
	}

}

void test_VIDIOC_G_ENC_INDEX_NULL() {
	struct v4l2_enc_idx enc_index;
	int ret_get, errno_get;
	int ret_null, errno_null;

	memset(&enc_index, 0, sizeof(enc_index));
	ret_get = ioctl(get_video_fd(), VIDIOC_G_ENC_INDEX, &enc_index);
	errno_get = errno;

	dprintf("\t%s:%u: VIDIOC_G_ENC_INDEX, ret_get=%i, errno_get=%i\n",
		__FILE__, __LINE__, ret_get, errno_get);

	ret_null = ioctl(get_video_fd(), VIDIOC_G_ENC_INDEX, NULL);
	errno_null = errno;

	dprintf("\t%s:%u: VIDIOC_G_ENC_INDEX, ret_null=%i, errno_null=%i\n",
		__FILE__, __LINE__, ret_null, errno_null);

	if (ret_get == 0) {
		CU_ASSERT_EQUAL(ret_get, 0);
		CU_ASSERT_EQUAL(ret_null, -1);
		CU_ASSERT_EQUAL(errno_null, EFAULT);
	} else {
		CU_ASSERT_EQUAL(ret_get, -1);
		CU_ASSERT_EQUAL(errno_get, EINVAL);
		CU_ASSERT_EQUAL(ret_null, -1);
		CU_ASSERT_EQUAL(errno_null, EINVAL);
	}

}
