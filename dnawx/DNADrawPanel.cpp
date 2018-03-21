//
//  DNADrawPanel.cpp
//  DNAwx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#include "DNADrawPanel.h"
#include <wx/dcclient.h>
#include <wx/sizer.h>
#include <array>
#include <sstream>
#include <iomanip>
#include <string>
#include <iostream>
#include <algorithm>

BEGIN_EVENT_TABLE(DNADrawPanel, wxPanel)
	EVT_PAINT(DNADrawPanel::paintEvent)

END_EVENT_TABLE()

DNADrawPanel::DNADrawPanel(wxFrame* parent)
: wxPanel(parent)
{
    histogram = NULL;
}

void DNADrawPanel::paintEvent(wxPaintEvent & evt)
{
	wxPaintDC dc(this);
	render(dc);
}

void DNADrawPanel::paintNow()
{
	wxClientDC dc(this);
	render(dc);
}

void DNADrawPanel::render(wxDC& dc)
{
	// Clear
	dc.SetBackground(*wxWHITE_BRUSH);
	dc.Clear();
    
	// TODO: Draw histogram, if one has been generated
    
    int h = 100;
    std::string name;
    
    if(histogram != NULL){
        
        dc.DrawText(*title, 200, 50);
        
        for(int i = 0; i<20; ++i){
        
            if(i%3 == 0){
                dc.SetTextForeground(*wxRED);
                dc.SetPen(*wxBLACK_PEN);
                dc.SetBrush(*wxRED_BRUSH);
            }
            if(i%3 == 1){
                dc.SetTextForeground(*wxBLUE);
                dc.SetPen(*wxBLACK_PEN);
                dc.SetBrush(*wxBLUE_BRUSH);
            }
            if(i%3 == 2){
                dc.SetTextForeground(*wxGREEN);
                dc.SetPen(*wxBLACK_PEN);
                dc.SetBrush(*wxGREEN_BRUSH);
            }
            
            double maxNum = *std::max_element(histogram, histogram + 20);
            
            name = names[i] + ": " + std::to_string(histogram[i]) + "% (" + std::to_string(counts[i]) + ")";
            dc.DrawText(name, 50 , h);
            
            //
            
            dc.DrawRectangle(300, h - 10, 700*histogram[i]/maxNum, 25);
            
            h = h + 25;
        }
    }
}


void DNADrawPanel::setHistogram(float *hist){
    
    histogram = hist;
    
}

void DNADrawPanel::setCounts(int *count){
    
    counts = count;
    
}

void DNADrawPanel::setTitle(std::string* header){
    
    title = header;
    
};

