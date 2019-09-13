#ifndef VLCPLAYER_H 
#define VLCPLAYER_H 

#include <iostream>
#include <string>
#include <vlcpp/vlc.hpp>

typedef std::string string;

class VLCPlayer{
public:
	VLCPlayer();
	~VLCPlayer();

    void play(string &file);

private:
	//delete copy constructor by default
	VLCPlayer(const VLCPlayer &self)=delete;
	//delete copy operator
	VLCPlayer& operator=(const VLCPlayer &self)=delete;

    decltype(VLC::Instance()) vlc_instance;
    decltype(VLC::MediaPlayer()) mediaplayer;
};
#endif 
