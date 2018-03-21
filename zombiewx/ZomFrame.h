//
//  ZomFrame.h
//  zombiewx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#pragma once
#include <wx/frame.h>
#include <wx/timer.h>
#include <wx/menu.h>
#include "machine.h"
#include "traits.h"

class ZomFrame : public wxFrame
{
public:
	ZomFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	~ZomFrame();
private:
	void onExit(wxCommandEvent& event);
	void onNew(wxCommandEvent& event);
	void onSimStart(wxCommandEvent& event);
	void onTurnTimer(wxTimerEvent& event);
    void onLoadZombie(wxCommandEvent& event);//wxCommandEvent& event
    void onLoadSurvivor(wxCommandEvent& event);//wxCommandEvent& event
    void onRandomize(wxCommandEvent& event);//wxCommandEvent& event
    void onReset(wxCommandEvent& event);
	wxDECLARE_EVENT_TABLE();
private:
	// Panel for drawing
	class ZomDrawPanel* mPanel;
	
	// Simulation menu
	wxMenu* mSimMenu;
	
	// Turn timer
	wxTimer* mTurnTimer;
	
	bool mIsActive;
    int loadFlag = 0;
    
};




