//
//  ZomDrawPanel.cpp
//  zombiewx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#include "ZomDrawPanel.h"
#include <wx/dcclient.h>
#include <wx/sizer.h>
#include "machine.h"
#include "world.h"

BEGIN_EVENT_TABLE(ZomDrawPanel, wxPanel)
    EVT_PAINT(ZomDrawPanel::paintEvent)
    EVT_MOTION(ZomDrawPanel::mouseMoved)
END_EVENT_TABLE()

ZomDrawPanel::ZomDrawPanel(wxFrame* parent)
: wxPanel(parent)
{
	
}

void ZomDrawPanel::mouseMoved(wxMouseEvent &event)
{
    //if( x > 10 && x < 610){
        x = event.m_x;
    //}
    //if( y > 10 && y < 610){
        y = event.m_y;
    //}
    
}

void ZomDrawPanel::paintEvent(wxPaintEvent & evt)
{
	wxPaintDC dc(this);
	render(dc);
}

void ZomDrawPanel::paintNow()
{
	wxClientDC dc(this);
	render(dc);
}

void ZomDrawPanel::render(wxDC& dc)
{
	// Clear
	dc.SetBackground(*wxWHITE_BRUSH);
	dc.Clear();
	
	// Draw the grid
	drawGrid(dc);
}

void ZomDrawPanel::drawGrid(wxDC& dc)
{
	dc.SetPen(*wxBLACK_PEN);
	dc.DrawRectangle(10, 10, 600, 600);
    
    // Draw the Grid
    for(int i = 0; i < 19; ++i){
        
        int n = 30*i + 40;
        
        dc.DrawLine(10, n, 610, n);
        dc.DrawLine(n, 10, n, 610);
        
    }
    
    // Figure out where the mouse is hovering
    int xI = (x - 10)/30;
    int yI = (y - 10)/30;
    
    if( xI < 0 || xI > 19){
        
        xI = 0;
        
    }
    if( yI < 0 || yI > 19 ){
        yI = 0;
    }
    
    // Separation of Stats on the Stats Board
    int n = 50;
    
    if(World::get().board[xI][yI] != nullptr){
        
        //std::cout << "Found at: x-index: " << xI << " and y-index: " << yI <<std::endl;
        
        if( World::get().board[xI][yI]->GetInfect() == false ){// (World::get().board[xI][yI] != nullptr) &&
            // human is occupying location
            
            std::string query("Mem[0]: " + std::to_string(World::get().board[xI][yI]->GetMemory(0)));
            query += "\nMem[1]:" + std::to_string(World::get().board[xI][yI]->GetMemory(1));
            
            wxString details = query;
            wxPoint wordPlace(620, 2*n);
            
            dc.DrawText(details, wordPlace);
            
        }
        
    }
    
    // Draw Current Month
    wxString month("Month Number: " + std::to_string(World::get().monthCount));
    wxPoint monthPlace(620, 3*n);
    dc.DrawText(month, monthPlace);
    
    // Number of Zombies Alive
    wxString zombieCount("Zombies: " + std::to_string(World::get().Zombies.size()));
    wxPoint zombiePlace(620, 4*n);
    dc.DrawText(zombieCount, zombiePlace);
    // Current Zombie file loaded
    wxPoint zombieFile(620, 5*n);
    dc.DrawText("Zombie File: " + World::get().zombieInputPath, zombieFile);
    
    // Number of Humans Alive
    wxString humanCount("Humans: " + std::to_string(World::get().Humans.size()));
    wxPoint humanPlace(720, 4*n);
    dc.DrawText(humanCount, humanPlace);
    // Current Human file loaded
    wxPoint humanFile(620, 6*n);
    dc.DrawText("Human File: " + World::get().humanInputPath, humanFile);
    
    
    // Draw the Zombies
    dc.SetBrush(*wxRED_BRUSH);
    for(auto it = World::get().Zombies.begin(); it != World::get().Zombies.end(); it++){
    
        wxPoint Point[3] = {};
        int x = 30*(**it).xlocation + 10;
        int y = 30*(**it).ylocation + 10;
        
        switch ((**it).m_Facing) {
            case (MachineState::UP):
                
                Point[0] = wxPoint(x , y + 30);
                Point[1] = wxPoint(x + 15, y);
                Point[2] = wxPoint(x + 30, y + 30);
                
                break;
            case (MachineState::RIGHT) :
                
                Point[0] = wxPoint(x , y);
                Point[1] = wxPoint(x + 30, y + 15);
                Point[2] = wxPoint(x, y + 30);
                
                break;
            case (MachineState::DOWN) :
                
                Point[0] = wxPoint(x + 30, y);
                Point[1] = wxPoint(x + 15, y + 30);
                Point[2] = wxPoint(x, y);
                
                break;
                
            default:
            case (MachineState::LEFT) :
                
                Point[0] = wxPoint(x + 30, y + 30);
                Point[1] = wxPoint(x, y + 15);
                Point[2] = wxPoint(x + 30, y);
                
                break;
        }
        
        dc.DrawPolygon(3, Point);

    }
    
    // Draw the Humans
    dc.SetBrush(*wxGREEN_BRUSH);
    for(auto it = World::get().Humans.begin(); it != World::get().Humans.end(); it++){
        
        wxPoint Point[3] = {};
        int x = 30*(**it).xlocation + 10;
        int y = 30*(**it).ylocation + 10;
        
        switch ((**it).m_Facing) {
            case (MachineState::UP):
                
                Point[0] = wxPoint(x , y + 30);
                Point[1] = wxPoint(x + 15, y);
                Point[2] = wxPoint(x + 30, y + 30);
                
                break;
            case (MachineState::RIGHT) :
                
                Point[0] = wxPoint(x , y);
                Point[1] = wxPoint(x + 30, y + 15);
                Point[2] = wxPoint(x, y + 30);
                
                break;
            case (MachineState::DOWN) :
                
                Point[0] = wxPoint(x + 30, y);
                Point[1] = wxPoint(x + 15, y + 30);
                Point[2] = wxPoint(x, y);
                
                break;
                
            default:
            case (MachineState::LEFT) :
                
                Point[0] = wxPoint(x + 30, y + 30);
                Point[1] = wxPoint(x, y + 15);
                Point[2] = wxPoint(x + 30, y);
                
                break;
        }
        
        dc.DrawPolygon(3, Point);
        
    }

}





