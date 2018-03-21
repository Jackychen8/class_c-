//
//  ZomDrawPanel.h
//  zombiewx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#pragma once
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/frame.h>
#include <string>

class ZomDrawPanel : public wxPanel
{
 
public:
	ZomDrawPanel(wxFrame* parent);
	void paintNow();
    
protected:
    void mouseMoved(wxMouseEvent& event);
	void paintEvent(wxPaintEvent & evt);
	void render(wxDC& dc);
	void drawGrid(wxDC& dc);
 
	DECLARE_EVENT_TABLE()
	
public:
	// Variables here
    int x;
    int y;
};

