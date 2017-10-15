/*
 * Frame.hpp
 *
 *  Created on: 2017年2月22日
 *      Author: cmst0us
 */
#ifndef SRC_VIDEO_FRAME_HPP_
#define SRC_VIDEO_FRAME_HPP_
#include <iostream>
#include <string.h>
#include <set>

#define MAX_PLANES 10
namespace rov {
class Frame {
private:
	/*
	 * 注意，对象拷贝至Frame持有！！！！
	 *
	 *
	 */
	void** _data;
	uint32_t* _length;
	std::set<int> _planeSet;
	int _maxPlane;

public:
	Frame() {
		this->_data = nullptr;
		this->_length = 0;
		this->_maxPlane = MAX_PLANES;
		this->_planeSet.empty();
		this->_data = (void**)calloc(this->_maxPlane, sizeof(void*));
		memset(this->_data, 0 , this->_maxPlane * sizeof(void*));
		this->_length = (uint32_t*)calloc(this->_maxPlane, sizeof(uint32_t));
		memset(this->_length, 0, this->_maxPlane * sizeof(uint32_t));

	};
	Frame(int maxPlane) {
		this->_data = nullptr;
		this->_length = 0;
		this->_maxPlane = maxPlane;
		this->_planeSet.empty();
		this->_data = (void**)calloc(this->_maxPlane, sizeof(void*));
		memset(this->_data, 0 , this->_maxPlane * sizeof(void*));
		this->_length = (uint32_t*)calloc(this->_maxPlane, sizeof(uint32_t));
		memset(this->_length, 0, this->_maxPlane * sizeof(uint32_t));
	}
	int setPlane(void* data, uint32_t length, int index) {
		if(index > this->_maxPlane) {
			return -1;
		}

		if (this->_planeSet.count(index) == 0) {
			//平面不存在
			this->_data[index] = malloc(length);
			memset(this->_data[index], 0, length);
			memcpy(this->_data[index], data, length);
			this->_planeSet.insert(index);
		}else{
			//平面存在
			free(this->_data[index]);

			this->_data[index] = malloc(length);
			memset(this->_data[index], 0, length);
			memcpy(this->_data[index], data, length);
		}
		this->_length[index] = length;
		return 0;
	}
	
	
	int getPlaneLength(int index) {
        if (this->_planeSet.count(index) == 0) {
            return -1;
        }
        return this->_length[index];
    }
    
    
	
	//TODO:In order to get correct length, pls do getPlaneLength first
        int getPlane(int index, void* buf, uint32_t* len) {
		if (this->_planeSet.count(index) == 0) {
			//平面不存在
			return -1;
		}
		memcpy(buf, this->_data[index], this->_length[index]);
		return 0;
	}

	~Frame() {
		std::set<int>::iterator set_iter = this->_planeSet.begin();
		for(;set_iter != this->_planeSet.end(); set_iter++) {
			int planeIndex = *set_iter;
			free(this->_data[planeIndex]);
		}
		free(this->_data);
		free(this->_length);
	}
};
typedef Frame* FrameRef;
}



#endif /* SRC_VIDEO_FRAME_HPP_ */
