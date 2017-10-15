/*
 * Camera.hpp
 *
 *  Created on: 2017年1月10日
 *      Author: cmst0us
 */

#ifndef SRC_VIDEO_CAMERA_HPP_
#define SRC_VIDEO_CAMERA_HPP_

#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <linux/videodev2.h>
//#include <functional>
#include "utils.h"
#include <sys/ioctl.h>

#include "SimpleBuffer.hpp"
#include "Frame.hpp"
namespace rov {

class Camera{
private:
	int _deviceFd;
	uint32_t _frameWidth;
	uint32_t _frameHeight;
	uint32_t _frameFormat;
	uint32_t _frameField;
	uint32_t _frameBufferCount;
	uint32_t _frameBufferType;

	uint8_t _fps;

	uint32_t _bytesPerLine;


	SimpleBufferRef _buffer;
//	bool _multiPlane;
//	uint8_t _planeCount;
public:
	Camera(const char* device, int options);
	virtual ~Camera();
	uint32_t getDeviceFd(){return this->_deviceFd;}
	uint32_t getFrameWidth(){return this->_frameWidth;}
	uint32_t getFrameHeight(){return this->_frameHeight;}
	uint32_t getFrameFormat(){return this->_frameFormat;}
	uint32_t getFrameFidld(){return this->_frameField;}
	uint32_t getFrameBufferCount(){return this->_frameBufferCount;}
	uint32_t getFrameBUfferType(){return this->_frameBufferType;}
	uint32_t getFps(){return this->_fps;}
	uint32_t getBytesPerLine(){return this->_bytesPerLine;}
//	BufferRef getBuffer(){return this->_buffer;}

//    void setDeviceFd(int_t deviceFd){this->_deviceFd = deviceFd;}
    void setFrameWidth(uint32_t frameWidth){this->_frameWidth = frameWidth;}
    void setFrameHeight(uint32_t frameHeight){this->_frameHeight = frameHeight;}
    void setFrameFormat(uint32_t frameFormat){this->_frameFormat = frameFormat;}
    void setFrameField(uint32_t frameField){this->_frameField = frameField;}
    void setFrameBufferCount(uint32_t frameBufferCount){this->_frameBufferCount = frameBufferCount;}
    void setFrameBufferType(uint32_t frameBufferType){this->_frameBufferType = frameBufferType;}
    void setFps(uint8_t fps){this->_fps = fps;}
//    void setBytesPerLine(uint32_t bytesPerLine){this->_bytesPerLine = bytesPerLine;}
//    void setBuffer(BufferRef buffer){this->_buffer = buffer;}


public:

	static int xioctl(int fd, unsigned long int id, void* config) {
    	int ret = ioctl(fd, id, config);
    	if (ret != 0) {
    		perror("ioctl err");

    		return -1;
    	}
    	return 0;
    }
    /*
     * @brief 将配置传递给设备
     * @param id     配置id(参考V4L2 ioctl配置项)
     * 		  config 用户配置，默认使用系统的配置。
     * @return int 配置是否成功 返回ioctl的值。
     * 			   成功为0
     */
	virtual int configure(unsigned long int id, void* config = nullptr);

	/*
	 * @brief 质询每一个缓冲区
	 */
	virtual int queryBuffer();

	/*
	 * @brief 读取一帧
	 * @param c lambda表达式回调。当一帧从FIFO出列时，调用，传递参数
	 * 			1. void* data 数据指针
	 * 			2. int length 数据大小
	 * @return 成功 0
	 * 		   失败 -1
	 */
	virtual FrameRef readFrame();

};

} /* namespace rov */

#endif /* SRC_VIDEO_CAMERA_HPP_ */
