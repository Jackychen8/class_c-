//
//  DNADrawPanel.h
//  DNAwx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#pragma once
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/frame.h>
#include <string>
#include <memory>

class DNADrawPanel : public wxPanel
{
public:
	DNADrawPanel(wxFrame* parent);
	void paintNow();

protected:
	void paintEvent(wxPaintEvent & evt);
	void render(wxDC& dc);
 
	DECLARE_EVENT_TABLE()
	
public:
	// Variables here
    float* histogram;
    int* counts;
    std::string* title;
    
    void setHistogram(float* hist);
    void setCounts(int* count);
    void setTitle(std::string* header);
    
    std::string names[20] = {
        "Alanine",
        "Cysteine",
        "Aspartic Acid",
        "Glutamic Acid",
        "Phenylalanine",
        "Glycine",
        "Histidine",
        "Isoleucine",
        "Lysine",
        "Leucine",
        "Methionine",
        "Asparagine",
        "Proline",
        "Glutamine",
        "Arginine",
        "Serine",
        "Threonine",
        "Valine",
        "Tryptophan",
        "Tyrosine",
    };
    
};

