/*
 * SimpleBuffer.hpp
 *
 *  Created on: 2017年2月23日
 *      Author: cmst0us
 */

#ifndef SRC_VIDEO_SIMPLEBUFFER_HPP_
#define SRC_VIDEO_SIMPLEBUFFER_HPP_
#include "Frame.hpp"
namespace rov {
struct SimpleBuffer {
	void* start[MAX_PLANES];
	uint32_t length[MAX_PLANES];
};
typedef SimpleBuffer* SimpleBufferRef;
}
#endif /* SRC_VIDEO_SIMPLEBUFFER_HPP_ */
