#include "Frame.h"

Frame::Frame()
{
}

Frame::~Frame()
{
	for (auto item = functions.begin(); item != functions.end(); ++item) {
		delete* item;
	}
}
