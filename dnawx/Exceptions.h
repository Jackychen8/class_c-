//
//  Exceptions.h
//  DNAwx
//
//  Created by Sanjay Madhav on 12/29/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#pragma once
#include <exception>

// Macro because noexcept is NYI in VS 2013
#ifdef _WIN32
#define NOEXCEPT
#else
#define NOEXCEPT noexcept
#endif

class DNAExcept : public std::exception
{
	
};

class FileLoadExcept : public DNAExcept
{
public:
	const char* what() const NOEXCEPT override { return "FASTA file load exception"; }
};
