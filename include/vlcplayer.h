#ifndef VLCPLAYER_H 
#define VLCPLAYER_H 

#include <string>

typedef std::string string;

class VLCPlayer{
public:
	VLCPlayer();
	~VLCPlayer();

private:
	//delete copy constructor by default
	VLCPlayer(const VLCPlayer &self)=delete;
	//delete copy operator
	VLCPlayer& operator=(const VLCPlayer &self)=delete;
};
#endif 
