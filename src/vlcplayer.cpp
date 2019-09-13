#include <vlcplayer.h>

//constructor
VLCPlayer::VLCPlayer(){
    //create vlc instance
    try{
        this->vlc_instance=VLC::Instance(0,nullptr);
    }catch(std::runtime_error &err){
        std::cout<<"ERROR: "<<err.what()<<"\n";
        throw err;
    }
    if(vlc_instance==nullptr){
        std::cout<<"Couldn't create vlc_instance"<<std::endl;
    }
    this->mediaplayer=VLC::MediaPlayer();
}

//to begin playing a video/file with vlc
void VLCPlayer::play(string &file){
    //setup the player
    auto media=VLC::Media(vlc_instance,file,VLC::Media::FromPath);
    //put media into player
    mediaplayer.setMedia(media);

    mediaplayer.play();

    //now loop until the video stops playing
    while(mediaplayer.isPlaying()==true){
        
    }
    std::cout<<"DONE PLAYING: "<<file<<"\n";
}

VLCPlayer::~VLCPlayer(){
    if(mediaplayer.isPlaying()){
        mediaplayer.stop();
    }
}
