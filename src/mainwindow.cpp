#include "../include/mainwindow.h"


int Colors::id_counters=1;

typedef std::shared_ptr<std::pair<int,int>> spairPos;

void clearWindowDisplay(WINDOW *window){
    wclear(window);
    wrefresh(window);
}

SMainWindow basicWindowInit(string &&title){
    auto window= initscr();
    //check that terminal supports color, if not return a null
    //if so then continue
    if(has_colors()==false){
        return nullptr;
    }else{
        //enable colors
        start_color();
    }

    //setup the title of the application

    raw();
    nonl();
    
    noecho();
    keypad(window,true);

    printw("STARTING");
    refresh();

    curs_set(false);

    MainWindow *main=new MainWindow(title,window);
    SMainWindow mainWindow(main);
    return mainWindow;
}

void colorOn(WINDOW *window,Colors &colors){
    wattron(window,COLOR_PAIR(colors.getID()));
}

void colorOff(WINDOW *window,Colors &colors){
    wattroff(window,COLOR_PAIR(colors.getID()));
}

spairPos getScreenCenter(string &text,int width,int height){
    spairPos position(new std::pair<int,int>());

    //get size of text
    auto size=text.size();
    //get starting center based on width
    position->first=(width-size)/2;
    //get starting center based on height
    position->second=(height)/2;

    return position;
}

void MainWindow::start(){
    clearWindowDisplay(m_window);

    //create a new temporary window here
    WINDOW *tempForTitle=newwin(height-4,width-4,2,2);
    colorOn(tempForTitle,purple);
    wbkgd(tempForTitle,COLOR_PAIR(purple.getID()));
    box(tempForTitle,' ',' ');
    wrefresh(tempForTitle);

    //move to the center and show title for 4 seconds
    spairPos centerPosition=getScreenCenter(m_title,width-4,height-4);
    x=centerPosition->first;
    y=centerPosition->second;

    //clear everything and draw to the tempForTitle window
    //now display the title here in the new window, that will timout
    mvwprintw(tempForTitle,y,x,m_title.c_str());
    wrefresh(tempForTitle);
    
    //time out after 4 seconds and delete window
    napms(4000);
    
    //readFiles currently in directory
    this->getFileExtentions();
    this->extensionType=this->selectExtentionTypeFromMenu(tempForTitle);
    //filter to get the files that match selected extention type
    this->filterFileList();

    //clear screen
    clearWindowDisplay(m_window);

    //Now start the main loop, where we go through displaying everything, and playing from the playlist in folder
    this->beginPlayer();
}

void MainWindow::beginPlayer(){

    //set the main window color to purple
    wbkgd(m_window,COLOR_PAIR(purple.getID()));
    colorOn(m_window,purple);
    wattron(m_window,A_BOLD);
    string infoTitle("PRESS [ENTER] or [SPACE] to begin playing list");
    string infoTitle2("PRESS [R] to resume from last played position automatically");
    auto centerPosition=getScreenCenter(infoTitle,getmaxx(m_window),getmaxy(m_window));
    wmove(m_window,0,centerPosition->first);
    wprintw(m_window,infoTitle.c_str());
    centerPosition=getScreenCenter(infoTitle2,getmaxx(m_window),getmaxy(m_window));
    wmove(m_window,1,centerPosition->first);
    wprintw(m_window,infoTitle2.c_str());
    wrefresh(m_window);
    
    //create a new window here
    WINDOW *playListShadow=newwin(getmaxy(m_window)-4,getmaxx(m_window)-4,3,1);
    WINDOW *playList=newwin(getmaxy(m_window)-4,getmaxx(m_window)-4,2,2);
    //set shadow color to BLACK
    wbkgd(playListShadow,COLOR_PAIR(blackRed.getID()));
    //set playlist window background color to red
    wbkgd(playList,COLOR_PAIR(redBlack.getID()));
    //box the views
    box(playListShadow,' ',' ');
    box(playList,' ',' ');
    //refresh views
    wrefresh(m_window);
    wrefresh(playListShadow);
    wrefresh(playList);

    //set text color to black background and purple text
    colorOn(playList,redBlack);

    //enter title  here
    wmove(playList,0,0);
    wprintw(playList,"LIST OF PLAYLSIT:");
    wmove(playList,0,0);
    wchgat(playList,-1,A_BOLD|A_UNDERLINE,blueBlack.getID(),NULL);
    wmove(playList,1,0);

    int starting=1;
    int position=1;
    int prev_position=position;
    //populate view with playable extention files as per users choice
    for(auto &item:*this->fileListWithExtentions){
        bfs::path file(item.c_str());
        wprintw(playList,file.filename().string().c_str());
        wmove(playList,++position,0);
        wrefresh(playList);
    }
    //reset position
    position=1;

    //hold function for coloring rows once switched
    auto colorRows=[&](){
        //move to previous position
        wmove(playList,prev_position,0);
        //color it to unselect color aka red black
        wchgat(playList,-1,A_NORMAL,redBlack.getID(),NULL);
        //move to current location
        wmove(playList,position,0);
        //color it to black red
        wchgat(playList,-1,A_NORMAL,blackRed.getID(),NULL);
    };
    //highlight position 1
    colorRows();

    keypad(playList,true);

    int ch=-1;
    //keep on getting input, until user presses x or X which should close program
    while(ch!='x'){
        ch=wgetch(playList);
        switch(ch){
            //for closing the application
            case 'x':
            case 'q':
            case 'Q':
            case 'X':ch='x';break;
            //when scrolling down
            case 'j':
            case 'J':
            case KEY_DOWN:this->scrolling(this->fileListWithExtentions->size(),
                                starting,prev_position,position,SCROLLDOWN);
                        colorRows();
                        break;
            //when scrolling up
            case 'k':
            case 'K':
            case KEY_UP:this->scrolling(this->fileListWithExtentions->size(),
                                starting,prev_position,position,SCROLLUP);
                        colorRows();
                        break;
            //to auto start playing from last played position
            case 'r':
            case 'R': cout<<"Nigger clicked R"<<endl;break;
            //to start playing from selected file
            case ' ':
            case '\n':
            case '\r': cout<<"Play from current position"<<endl;break;
        }
    }
}

void MainWindow::filterFileList(){
    if(this->fileListWithExtentions==nullptr){
        this->fileListWithExtentions=std::make_shared<std::vector<string>>();
    }
    this->fileListWithExtentions->clear();
    for(auto item:*this->filesList){
        if(item.find(this->extensionType)!=string::npos){
            //meaning contains the string, then add it to the fileLIstWithExtention vector
            this->fileListWithExtentions->push_back(item);
        }
    }
    //order the filtered list
    std::sort(fileListWithExtentions->begin(),fileListWithExtentions->end());
}

const string &MainWindow::selectExtentionTypeFromMenu(WINDOW *window){
    //get coordinates to center this menu
    string menuSelectionTitle("Select Extention type to play with:\n");
    int y=getcury(window)+3;
    int x=(getmaxx(window)-menuSelectionTitle.size()+1)/2;
    WINDOW *menuSelection=newwin(this->fileExtentions->size()+3,menuSelectionTitle.size(),y,x);
    wbkgd(menuSelection,COLOR_PAIR(blackRed.getID()));
    box(menuSelection,' ',' ');
    colorOn(menuSelection,blackRed);
    wrefresh(menuSelection);

    //set header to prompt user to select type
    wattron(menuSelection,A_BOLD);
    colorOn(menuSelection,blackBlue);
    wmove(menuSelection,0,0);
    wprintw(menuSelection,menuSelectionTitle.c_str());
    colorOff(menuSelection,blackBlue);
    wattroff(menuSelection,A_BOLD);
    wrefresh(menuSelection);

    colorOn(menuSelection,blackRed);
    //add the options for the user to select here
    for(auto &type:*this->fileExtentions){
        string temp=type+"\n";
        wprintw(menuSelection,temp.c_str());
    }
    wrefresh(menuSelection);
    
    //hold position of cursor starting point
    int starting=1;
    //hold current position, being at starting
    int current_position=1;
    //to hold previous position
    int prev=current_position;
    wmove(menuSelection,starting,0);
    keypad(menuSelection,true);

    //now start a loop to select the data to use
    int ch;
    do{
        //get input
        ch=wgetch(menuSelection);

        switch(ch){
            case KEY_UP:
            case 'k':
            case 'K':
                scrolling(this->fileExtentions->size(),starting,prev,current_position,SCROLLUP);
                wmove(menuSelection,prev,0);
                wchgat(menuSelection,-1,A_NORMAL,blackRed.getID(),NULL);
                wmove(menuSelection,current_position,0);
                wchgat(menuSelection,-1,A_NORMAL,redBlack.getID(),NULL);break;
            case 'j':
            case 'J':
            case KEY_DOWN:
                scrolling(this->fileExtentions->size(),starting,prev,current_position,SCROLLDOWN);
                wmove(menuSelection,prev,0);
                wchgat(menuSelection,-1,A_NORMAL,blackRed.getID(),NULL);
                wmove(menuSelection,current_position,0);
                wchgat(menuSelection,-1,A_NORMAL,redBlack.getID(),NULL);break;
            case '\n':
            case '\r':
                ch='\n';wprintw(menuSelection,"ENTER PRESSED");break;
        } 
    }while(ch!='\n');

    delwin(menuSelection);
    delwin(window);
    wrefresh(m_window);

    return [&]()->const string &{
      int i=0;
      auto iterator=this->fileExtentions->begin();
      while(i!=(current_position-1)){
        i++;
        iterator++;
      }
      return *iterator;
    }();
}

int &MainWindow::scrolling(int listSize,int starting_position,int &prev_position,int &current_position,int direction){
    //move according to direction
    prev_position=current_position;
    if(direction==MainWindow::SCROLLUP){
        --current_position;
        //loop back to bottom if reached the top
        if(current_position<starting_position){
            current_position=listSize;
        }
    }else if(direction==MainWindow::SCROLLDOWN){
        ++current_position;
        //loop back to top if reached bottom
        if(current_position>listSize){
            current_position=starting_position;
        }
    }
    
    return current_position;
}

sSstring MainWindow::getFileExtentions(){
    sSstring extentions(new std::set<string>);

    auto filesRead=this->readFiles();
    for(auto &file:*filesRead){
        bfs::path p(file);
        if(p.has_extension()){
            extentions->insert(p.extension().string());
        }
    }
    this->fileExtentions=extentions;

    return extentions;
}

sVstring MainWindow::readFiles(){
    //to hold the file names
    sVstring files(new std::vector<string>());

    bfs::directory_iterator currdir(bfs::current_path());
    for(auto &file:currdir){
        files->push_back(file.path().string());
    }

    this->filesList=files;

    return files;
}

MainWindow::MainWindow(const string &title,WINDOW *window){
    this->m_title=title;
    this->m_window=window;

    //fill in the locations
    x=getcurx(m_window);
    y=getcury(m_window);
    //get the size limits
    width=getmaxx(m_window);
    height=getmaxy(m_window);
}

MainWindow::~MainWindow(){
    endwin(); 
}
