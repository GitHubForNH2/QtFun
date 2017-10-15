/*
 * Camera.cpp
 *
 *  Created on: 2017年1月10日
 *      Author: cmst0us
 */

#include "Camera.hpp"
//#include "../utils.h"
#include <sys/ioctl.h>
#include <stdio.h>
#include <linux/videodev2.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
namespace rov {

Camera::Camera(const char* device, int options) {
	this->_deviceFd = open(device, options);
	this->_frameWidth = 0;
	this->_frameHeight = 0;
	this->_frameFormat = 0;
	this->_frameField = 0;
	this->_frameBufferCount = 0;
	this->_frameBufferType = 0;
	this->_fps = 0;
	this->_bytesPerLine = 0;

	this->_buffer = nullptr;
}

Camera::~Camera() {
	int ret;
	for(int i = 0; i < this->_frameBufferCount; ++i){
		ret = munmap(this->_buffer[i].start[0], this->_buffer[i].length[0]);
		if(ret < 0) ERR_LOG("munmap err, continue");
	}
	delete this->_buffer;
	close(this->_deviceFd);
}

int Camera::configure(unsigned long int id, void* config) {
	switch(id) {
		case VIDIOC_S_PARM: {
			struct v4l2_streamparm parm = {0};
			if (config != nullptr) {
				return xioctl(this->_deviceFd, VIDIOC_S_PARM, config);
			}
			parm.type = this->_frameBufferType;
			xioctl(this->_deviceFd, VIDIOC_G_PARM, &parm);

			parm.parm.capture.timeperframe.denominator = 1;
			parm.parm.capture.timeperframe.numerator = this->_fps;

			return xioctl(this->_deviceFd, VIDIOC_S_PARM, &parm);
		}
		break;
		case VIDIOC_G_PARM: {
			//TODO 计算fps 下面的有点问题，非30帧的时候

			struct v4l2_streamparm parm = {0};
			int ret;
			parm.type = this->_frameBufferType;
			ret = xioctl(this->_deviceFd, VIDIOC_G_PARM, &parm);

			this->_fps = (uint32_t)parm.parm.capture.timeperframe.numerator / parm.parm.capture.timeperframe.denominator;
			return ret;
		}
		break;
		case VIDIOC_S_FMT: {
			int ret;
			v4l2_format fmt = {0};
			if (config != nullptr) {
				return xioctl(this->_deviceFd, VIDIOC_S_PARM, config);
			}

			fmt.type = this->_frameBufferType;
			fmt.fmt.pix.width = this->_frameWidth;
			fmt.fmt.pix.height = this->_frameHeight;
			fmt.fmt.pix.pixelformat = this->_frameFormat;
			fmt.fmt.pix.field = this->_frameField;

			ret = xioctl(this->_deviceFd, VIDIOC_S_FMT, &fmt);
			this->_bytesPerLine = fmt.fmt.pix.bytesperline;
		}
		break;
		case VIDIOC_REQBUFS: {
			struct v4l2_requestbuffers req = {0};

			if (config != nullptr) {
				return xioctl(this->_deviceFd, VIDIOC_REQBUFS, config);
			}

			req.count = this->_frameBufferCount;
			req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			req.memory = V4L2_MEMORY_MMAP;

			return xioctl(this->_deviceFd, VIDIOC_REQBUFS, &req);
		}
		break;
		case VIDIOC_STREAMON: {
			if (config != nullptr) {
				return xioctl(this->_deviceFd, VIDIOC_STREAMON, config);
			}
			enum v4l2_buf_type t = static_cast<v4l2_buf_type>(this->_frameBufferType);
			return xioctl(this->_deviceFd, VIDIOC_STREAMON, &t);
		}
		break;
		case VIDIOC_STREAMOFF: {
			if (config != nullptr) {
				return xioctl(this->_deviceFd, VIDIOC_STREAMOFF, config);
			}
			enum v4l2_buf_type t = static_cast<v4l2_buf_type>(this->_frameBufferType);
			return xioctl(this->_deviceFd, VIDIOC_STREAMOFF, &t);
		}
		break;
		default:
			return xioctl(this->_deviceFd, id, config);
	}
	return -1;
}

int Camera::queryBuffer() {
	int ret = 0;

	this->_buffer = new rov::SimpleBuffer[this->_frameBufferCount];

	if(this->_buffer == nullptr) return -1;

	for (int i = 0; i < this->_frameBufferCount; ++i) {
		struct v4l2_buffer buf = {0};
		buf.type = 	this->_frameBufferType;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;

		ret |= xioctl(this->_deviceFd, VIDIOC_QUERYBUF, &buf);
		void* mmapBase = mmap(nullptr, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, this->_deviceFd, buf.m.offset);
		if (mmapBase == MAP_FAILED) {
			ERR_LOG("mmap error");
			return -1;
		} else {
			this->_buffer[i].start[0] = mmapBase;
			this->_buffer[i].length[0] = buf.length;
		}
		buf.type = this->_frameBufferType;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;
		//申请到的缓冲进入列队
		ret |= xioctl(this->_deviceFd, VIDIOC_QBUF, &buf);

		if (ret < 0) return ret;
	}
	return 0;
}
FrameRef Camera::readFrame() {
	struct v4l2_buffer buf = {0};
	buf.type = this->_frameBufferType;
	buf.memory = V4L2_MEMORY_MMAP;

	int ret = xioctl(this->_deviceFd, VIDIOC_DQBUF, &buf);
	if (ret < 0) return nullptr;

	FrameRef frame = new Frame;
	frame->setPlane(this->_buffer[buf.index].start[0], buf.bytesused, 0);
	ret = xioctl(this->_deviceFd, VIDIOC_QBUF, &buf);

	if (ret < 0) {
		delete frame;
		return nullptr;
	}
	return frame;
}

} /* namespace rov */
