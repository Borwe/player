#ifndef MAINWINDOW_H 
#define MAINWINDOW_H 

#include <iostream>
#include <string>
#include <cstring>
#include <memory>
#include <thread>
#include <curses.h>
#include <panel.h>
#include <vector>
#include <set>
#include <boost/filesystem.hpp>

using std::cout; using std::endl;
using std::cerr;

class MainWindow;
class Colors;

typedef std::string string;
typedef std::shared_ptr<string> sstring;
typedef std::shared_ptr<std::vector<string>> sVstring;
typedef std::shared_ptr<std::set<string>> sSstring;
namespace bfs=boost::filesystem;
typedef std::shared_ptr<MainWindow> SMainWindow;


SMainWindow basicWindowInit(string &&);
void clearWindowDisplay(WINDOW *window);

//for getting color id from color types directly
void colorOn(WINDOW *window,const Colors &colors);
void colorOff(WINDOW *window,const Colors &colors);

class Colors{
public:
    static int id_counters;
    int id;
    Colors(int foreground,int background){
        id=id_counters;
        init_pair(id,foreground,background);
        id_counters++;
    }
    int &getID(){
        return id;
    }
    Colors(const Colors &color){
        this->id=color.id;
    }
    const Colors &operator=(const Colors &color){
        return color;
    }
};

class MainWindow{
public:
    static const int SCROLLUP=1;
    static const int SCROLLDOWN=-1;

	~MainWindow();
    friend SMainWindow basicWindowInit(string &&);

    void start();
    /*
     * for displaying a menu where the user selects the type of extention they want to use
     * when playing with vlc
     */
    const string &selectExtentionTypeFromMenu(WINDOW *window);

    sVstring readFiles();
    sSstring getFileExtentions(); //hold file extentions
    int &scrolling(int listSize,int starting_position,int &prev_position,int &current_position,int direction);

private:
    //begin playing
    void beginPlayer();

    //narrow the selection to the list of the ones that match the extention type
    void filterFileList();

    //main costructor, private cause not supposed to be initialized by anybody
    MainWindow(const string &title,WINDOW *window);
    
	//delete copy constructor by default
	MainWindow(const MainWindow &self)=delete;
	//delete copy operator
	MainWindow& operator=(const MainWindow &self)=delete;

    //hold title of the window
    string m_title;
    //hold window pointer
    WINDOW *m_window;

    int x;//location at x
    int y;//location at y
    int width;//width of window
    int height;//height of window
    
    //purple background, black foreground
    Colors purple=Colors(COLOR_BLACK,COLOR_MAGENTA);
    Colors blackPurple=Colors(COLOR_MAGENTA,COLOR_BLACK);
    Colors blackRed=Colors(COLOR_RED,COLOR_BLACK);
    Colors blackBlue=Colors(COLOR_CYAN,COLOR_BLACK);
    Colors blueBlack=Colors(COLOR_BLACK,COLOR_CYAN);
    Colors redBlack=Colors(COLOR_BLACK,COLOR_RED);

    //for holding items/videos to be played by vlc
    sVstring filesList;
    //for holding the fileList with extentions to play
    sVstring fileListWithExtentions;
    //for holding the types currently in this directory
    sSstring fileExtentions;
    //for holding the selected type for videos to pick from
    string extensionType;
};


#endif 
