//
//  ZomFrame.cpp
//  zombiewx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#include "ZomFrame.h"
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/log.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/filedlg.h>
#include "ZomDrawPanel.h"
#include "singleton.h"
#include "World.h"
#include "machine.h"

enum
{
	ID_SIM_START=1000,
	ID_TURN_TIMER,
    /////////////////////////////////
    ID_LoadZombie,
    ID_LoadSurvivor,
    ID_Randomize,
    ID_RESET,
    /////////////////////////////////
};

wxBEGIN_EVENT_TABLE(ZomFrame, wxFrame)
	EVT_MENU(wxID_EXIT, ZomFrame::onExit)
	EVT_MENU(wxID_NEW, ZomFrame::onNew)
	EVT_MENU(ID_SIM_START, ZomFrame::onSimStart)
	EVT_TIMER(ID_TURN_TIMER, ZomFrame::onTurnTimer)
    ///////////////////////////////////////////////////////
    EVT_MENU(ID_LoadZombie, ZomFrame::onLoadZombie)
    EVT_MENU(ID_LoadSurvivor, ZomFrame::onLoadSurvivor)
    EVT_MENU(ID_Randomize, ZomFrame::onRandomize)
    EVT_MENU(ID_RESET, ZomFrame::onReset)
    ///////////////////////////////////////////////////////
wxEND_EVENT_TABLE()

ZomFrame::ZomFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame(NULL, wxID_ANY, title, pos, size)
, mIsActive(false)
{
	// File menu
	wxMenu* menuFile = new wxMenu;
	menuFile->Append(wxID_NEW);
	menuFile->Append(wxID_EXIT);
	
	// Simulation menu
	mSimMenu = new wxMenu;
	mSimMenu->Append(ID_SIM_START, "Start/stop\tSpace", "Start or stop the simulation");
    
    ///////////////////////////////////////////////////////
    mSimMenu->Append(ID_LoadZombie, "Load Zombie", "Load Zombie... ");
	mSimMenu->Append(ID_LoadSurvivor, "Load Survivor", "Load Survivor... ");
    mSimMenu->Append(ID_Randomize, "Randomize", "Randomize... ");
    mSimMenu->Append(ID_RESET, "Reset", "Deletes all Humans and Zombies, Sets Month to 0");
    ///////////////////////////////////////////////////////
    mSimMenu->Enable(ID_SIM_START, false);
    mSimMenu->Enable(ID_Randomize, false);
    mSimMenu->Enable(ID_RESET, false);
    
	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	menuBar->Append(mSimMenu, "&Simulation");
	SetMenuBar(menuBar);
	CreateStatusBar();
	
	// Prepare the draw panel and show this frame
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	mPanel = new ZomDrawPanel(this);
	sizer->Add(mPanel, 1, wxEXPAND);
	
	SetSizer(sizer);
	SetAutoLayout(true);
	Show(true);
	
	mTurnTimer = new wxTimer(this, ID_TURN_TIMER);

}

ZomFrame::~ZomFrame()
{
	delete mTurnTimer;
}

void ZomFrame::onExit(wxCommandEvent& event)
{
	Close(true);
}

void ZomFrame::onNew(wxCommandEvent& event)
{
    mTurnTimer->Stop();
    mIsActive = false;
    // Delete Zombies and Humans from Board
    for(int x = 0; x < 20; x++){
        
        for(int y = 0; y < 20; y++){
            
            if(World::get().board[x][y] != nullptr){
                
                World::get().board[x][y] = nullptr;
            }
        }
    }
    
    // and delete from Vectors
    for(long i = World::get().Zombies.size(); i > 0; i--){
        
        delete World::get().Zombies[i-1];
        World::get().Zombies.erase( World::get().Zombies.begin()+i-1 );
        
    }
    for (long i = World::get().Humans.size(); i > 0; i--){
        
        delete World::get().Humans[i-1];
        World::get().Humans.erase( World::get().Humans.begin()+i-1 );
    }
    
    World::get().monthCount = 0;
    
    World::get().zombieMachine.~Machine();
    World::get().humanMachine.~Machine();
    
    World::get().zombieInputPath = "";
    World::get().humanInputPath = "";
    
    mPanel->paintNow();
    
    loadFlag = 0;
    mSimMenu->Enable(ID_SIM_START, false);
    mSimMenu->Enable(ID_Randomize, false);
    mSimMenu->Enable(ID_RESET, false);
}

void ZomFrame::onReset(wxCommandEvent &event){

    mTurnTimer->Stop();
    // Delete Zombies and Humans from Board
    
    for(int x = 0; x < 20; x++){
        
        for(int y = 0; y < 20; y++){
            
            if(World::get().board[x][y] != nullptr){
                
                World::get().board[x][y] = nullptr;
                
            }
        }
    }
    
    // and delete from Vectors
     for(long i = World::get().Zombies.size(); i > 0; i--){
     
         delete World::get().Zombies[i-1];
         World::get().Zombies.erase( World::get().Zombies.begin()+i-1 );

     }
     for (long i = World::get().Humans.size(); i > 0; i--){

             delete World::get().Humans[i-1];
             World::get().Humans.erase( World::get().Humans.begin()+i-1 );
     }
    
    World::get().monthCount = 0;
    mPanel->paintNow();
}



void ZomFrame::onSimStart(wxCommandEvent& event)
{
	if (!mIsActive)
	{
		// Add timer to run once per second
		mTurnTimer->Start(1000);
		mIsActive = true;
	}
	else
	{
		mTurnTimer->Stop();
		mIsActive = false;
	}
}

void ZomFrame::onTurnTimer(wxTimerEvent& event)
{
    std::vector<MachineState*>::iterator it;
    
    // All the Zombies move first
    for (it = World::get().Zombies.begin(); it != World::get().Zombies.end(); it++){
        
        if((**it).died == 0){
            World::get().zombieMachine.TakeTurn(**it);
        }
        
    }
    
    // All the Humans move next
    for (it = World::get().Humans.begin(); it != World::get().Humans.end(); it++){
        
        if((**it).died == 0){
            World::get().humanMachine.TakeTurn(**it);
        }

    }

    // Zombies/Humans are immediately removed from the board on death
    // but they cannot be removed from the vector as we are iterating through it
    // Humans can be immediately turned into zombies.
    for(long i = World::get().Zombies.size(); i > 0; i--){
        
        if( World::get().Zombies[i-1]->died == true ){
            
            delete World::get().Zombies[i-1];
            
            World::get().Zombies.erase( World::get().Zombies.begin()+i-1 );

         };
    }
    for (long i = World::get().Humans.size(); i > 0; i--){

        if( World::get().Humans[i-1]->died == true){
            
            //machineState* var = World::get().Humans[i-1];
            delete World::get().Humans[i-1];
            
            World::get().Humans.erase( World::get().Humans.begin()+i-1 );
            
        };
    }
    
    mPanel->paintNow();
    ++World::get().monthCount;
    if( World::get().Humans.size() == 0 ){
        wxMessageBox("The Zombies have won!", "Game Over", wxOK | wxICON_ERROR);
        mTurnTimer->Stop();
        //mIsActive = false;
    }
    if( World::get().Zombies.size() == 0 ){
        wxMessageBox("The Humans have won!", "Game Over", wxOK | wxICON_ERROR);
        mTurnTimer->Stop();
        //mIsActive = false;
    }
}

////////////////////////////////////////////
void ZomFrame::onLoadZombie(wxCommandEvent& event){
    
    wxFileDialog fileDlg(this, "Load a ZOM file...",
                         "./zom", "", "ZOM Files|*.zom",
                         wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    
    if (fileDlg.ShowModal() == wxID_OK)
    {
        World::get().zombieInputPath = fileDlg.GetPath();
        World::get().zombieMachine.LoadMachine( World::get().zombieInputPath );
    }
    
    if( loadFlag == 1 ){
        
        mSimMenu->Enable(ID_SIM_START, true);
        mSimMenu->Enable(ID_Randomize, true);
        mSimMenu->Enable(ID_RESET, true);
        
    }

    ++loadFlag;
}

void ZomFrame::onLoadSurvivor(wxCommandEvent& event){
    
    wxFileDialog fileDlg(this, "Load a ZOM file...",
                         "./zom", "", "ZOM Files|*.zom",
                         wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    
    if (fileDlg.ShowModal() == wxID_OK)
    {
        
        World::get().humanInputPath = fileDlg.GetPath();
        World::get().humanMachine.LoadMachine( World::get().humanInputPath );
        
    }
    
    if( loadFlag == 1 ){
        
        mSimMenu->Enable(ID_SIM_START, true);
        mSimMenu->Enable(ID_Randomize, true);
        mSimMenu->Enable(ID_RESET, true);
        
    }
    
    ++loadFlag;
}

void ZomFrame::onRandomize(wxCommandEvent &event)
{
    int i = 0;
    if( World::get().zombieMachine.GetOps() != 0 ){
        
        while (i < 20) {//Create Zombies
            
            // Add a zombie to the vector
            MachineState* newZombie = new MachineState;
            World::get().zombieMachine.BindState(*newZombie);
            
            World::get().Zombies.push_back(newZombie);
            
            int x;
            int y;
            int dir = rand()%4;
            
            //Randomize Starting Direction
            if(dir == 0){
                newZombie->m_Facing = MachineState::UP;
            }
            else if(dir == 1){
                newZombie->m_Facing = MachineState::RIGHT;
            }
            else if(dir == 2){
                newZombie->m_Facing = MachineState::DOWN;
            }
            else{
                newZombie->m_Facing = MachineState::LEFT;
            }
            
            bool spotEmpty = 0;
            // Randomize Starting Location
            // Check that the spot is empty
            while(spotEmpty == 0){
                
                x = rand()%20;
                y = rand()%20;
                
                if(World::get().board[x][y] == nullptr){
                    
                    spotEmpty = 1;
                    World::get().board[x][y] = newZombie;
                    newZombie->xlocation = x;
                    newZombie->ylocation = y;
                    
                }
            }
            ++i;
        }
        
    }
    
    
    i = 0;
    if( World::get().humanMachine.GetOps() != 0 ){
        while (i < 10) {//Create Humans
            
            // Add a zombie to the vector
            MachineState* newHuman = new MachineState;
            World::get().humanMachine.BindState(*newHuman);
            
            World::get().Humans.push_back(newHuman);
            
            int x;
            int y;
            int dir = rand()%4;
            
            //Randomize Starting Direction
            if(dir == 0){
                newHuman->m_Facing = MachineState::UP;
            }
            else if(dir == 1){
                newHuman->m_Facing = MachineState::RIGHT;
            }
            else if(dir == 2){
                newHuman->m_Facing = MachineState::DOWN;
            }
            else{
                newHuman->m_Facing = MachineState::LEFT;
            }
            
            bool spotEmpty = 0;
            // Randomize Starting Location
            // Check that the spot is empty
            while(spotEmpty == 0){
                
                x = rand()%20;
                y = rand()%20;
                
                if(World::get().board[x][y] == nullptr){
                    
                    spotEmpty = 1;
                    World::get().board[x][y] = newHuman;
                    newHuman->xlocation = x;
                    newHuman->ylocation = y;
                    
                }
            }
            ++i;
        }
    }
    mPanel->paintNow();
}


