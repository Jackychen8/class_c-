#include "rle-files.h"
#include "rle-algo.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include "FileSystem.h"


    // Objective: Take the input from "source", compress it
    // creating a new file, and store it in a new compressed file
void RLE_v1::CreateArchive(const std::string& source)
{
    std::ifstream::pos_type size;
    char* memblock;
    
    // Take the input from "source" and compress it
    // Open the file for input, in binary mode, and start ATE (at the end)
    std::ifstream file (source, std::ios::in|std::ios::binary|std::ios::ate);
    if (file.is_open())
    {
        size = file.tellg(); // Save the size of the file
        memblock = new char [static_cast<unsigned int>(size)];
        file.seekg (0, std::ios::beg); // Seek back to start of file
        file.read (memblock, size);
        file.close();
        // File data has now been loaded into memblock array
        
        // Compress the file
        m_Data.Compress( memblock, static_cast<unsigned int>(size));
        
        // Make sure to clean up!
        delete[] memblock;
    }
    
    ///////// The Resulting Compression ///////// Can cast to float for accuracy
    unsigned int originalSize = static_cast<unsigned int>(size);
    unsigned int newSize = m_Data.m_Size;
    
    int compression = originalSize - newSize;
    
    int result = (compression*100)/originalSize;
    
    std::cout << "The file was compressed by " << result << "%!" << std::endl;
    ///////////////////////////////////////////////////////////////////////
    
    ///////////////// Creating the New File /////////////////
    std::string newName = source + ".rl1";
    m_Header.sig[0] = 'R';
    m_Header.sig[1] = 'L';
    m_Header.sig[2] = 'E';
    m_Header.sig[3] = 0x01;
    m_Header.fileSize = originalSize;
    m_Header.fileNameLength = source.length();
    m_Header.fileName = source;
    
    /////// Take m_Data.m_Data and put it in the new file after the header ///////
    // Open the file for output, in binary mode, and overwrite an existing file
    std::ofstream arc(newName, std::ios::out|std::ios::binary|std::ios::trunc);
    if (arc.is_open())
    {
        arc.write(m_Header.sig, 4);
        arc.write(reinterpret_cast<char*>(&(m_Header.fileSize)), 4); // Works!
        arc.write(reinterpret_cast<char*>(&(m_Header.fileNameLength)), 1);
        arc.write(m_Header.fileName.c_str(), m_Header.fileNameLength);
        arc.write(m_Data.m_Data, m_Data.m_Size);
    }
    
}

// Objective: Take the source name (.rl1 file), read in the file and temp store in memblock,
// take data in memblock and move/decompress it into m_data
// Create a new file that is not a .rl1 file, move m_data into file
void RLE_v1::ExtractArchive(const std::string& source)
{
    std::ifstream::pos_type size;
    char* memblock;
    
    std::ifstream arc(source, std::ios::in|std::ios::binary|std::ios::ate);
    if (arc.is_open())
    {
        size = arc.tellg(); // save the size of the file;
        int intsize = static_cast<int>(size);
        
        // allocate space for memblock to temporary storage
        memblock = new char [static_cast<unsigned int>(size)];
        
        arc.seekg(0, std::ios::beg);
        arc.read(m_Header.sig, 4);
        
        if(strcmp(m_Header.sig, "RLE\x01")){
            
            arc.read ( memblock, intsize-4 );
            m_Header.fileSize = *(reinterpret_cast<int*>(&memblock[0]));
            
            m_Header.fileNameLength = *(reinterpret_cast<unsigned char*>(&memblock[4]));// Taking a '.' so that fiName then gets .bmp?
            std::string fiName;
            
            // take in the chars to a char array
            for( int i = 0; i < m_Header.fileNameLength; i++){
                fiName += *(reinterpret_cast<char*>(&memblock[i+5]));
            }
            
            m_Header.fileName = fiName;

            m_Data.Decompress( memblock + 5 + m_Header.fileNameLength , (intsize - 9) - m_Header.fileNameLength, m_Header.fileSize);
            
            arc.close();
        }
    }
    std::ofstream out(m_Header.fileName, std::ios::out|std::ios::binary|std::ios::trunc);
    if (out.is_open())
    {
        out.write(m_Data.m_Data, m_Header.fileSize);
    }
}

void RLE_v2::CreateArchive(const std::string& source)
{
	/////////////////////////////////////////////////////////////////////////////////
    // Steps
    // 1. At the directory given as source, generate list of all files in directory
    //      A. Function given as FileSystem::getFilesInDirectory, also make a list to put file names in
    /////////////////////////////////////////////////////////////////////////////////
    
    /////////////////////////////////////////////////////////////////////////////////
    // 2. Open files one by one and grab all the info, compress the data, store the data
    //      A. (ifstream) pass in the path including the directory name "letters/a.bmp" instead of "a.bmp"
    //      B. Make a file_info struct on the heap with new
    //          1. Compress the data
    //          2. Stores compressed data (should be automatic) and archive relevant info
    //          3. Remember to delete struct instance in destructor of RLE_v2 /////////////////
    //          4. Add file_info struct to m_Files list
    
    std::list<std::string> fileList;
    
    FileSystem::getFilesInDirectory(source, fileList);
        
        std::string pathName;
        std::ifstream::pos_type size;
        char* memblock;
        
        for (std::list<std::string>::iterator it = fileList.begin(); it != fileList.end(); ++it){
            
            pathName = source + '/' + *it;
            file_info* temp = new file_info;
            
            std::ifstream file( pathName, std::ios::in|std::ios::binary|std::ios::ate);
            if (file.is_open())
            {
                size = file.tellg(); // Save the size of the file
                memblock = new char [static_cast<unsigned int>(size)];
                file.seekg (0, std::ios::beg); // Seek back to start of file
                file.read (memblock, size);
                file.close();
                // File data has now been loaded into memblock array
                
                // Compress the file
                temp->fileSize = static_cast<int>(size);//uncompressed size
                temp->data.Compress( memblock, static_cast<unsigned int>(size));
                
                // Make sure to clean up!
                delete[] memblock;
            }
            
            // All relevant Info
            temp->fileName = *it;
            temp->fileNameLength = temp->fileName.length();
            temp->compressedSize = temp->data.m_Size;
            
            m_Files.push_back(temp);
            std::cout << "File: " << temp->fileName << " read!" << std::endl;
        }
    
    /////////////////////////////////////////////////////////////////////////////////
    ///////    Now, everything should be loaded into the structs memories   /////////
    ///////    and we are ready to create the .rl2 file                     /////////
    /////////////////////////////////////////////////////////////////////////////////
    
    // 3. Create New File (Archive)
    //      A. Write the file header
    //          1. File Sig, Directory Name Length, Directory Name...
    //      B. Write out the rest of the compressed data and other file number & names
    
    m_Header.sig[0] = 'R';
    m_Header.sig[1] = 'L';
    m_Header.sig[2] = 'E';
    m_Header.sig[3] = 0x02;
    m_Header.dirNameLength = source.length();
    m_Header.dirName = source + ".rl2";
    std::cout << m_Header.dirNameLength;
    
    std::ofstream arc(m_Header.dirName, std::ios::out|std::ios::binary|std::ios::trunc);
    if (arc.is_open()){
        
        arc.write(m_Header.sig, 4);
        arc.write(reinterpret_cast<char*>(&(m_Header.dirNameLength)), 1);
        arc.write(m_Header.dirName.c_str(), m_Header.dirNameLength);
        
        for (std::list<file_info*>::iterator it = m_Files.begin(); it != m_Files.end(); ++it){
            
            arc.write(reinterpret_cast<char*>(&((*it)->fileSize)), 4);
            arc.write(reinterpret_cast<char*>(&((*it)->compressedSize)), 4);
            arc.write(reinterpret_cast<char*>(&((*it)->fileNameLength)), 1);
            arc.write((*it)->fileName.c_str(), (*it)->fileNameLength);
            arc.write((*it)->data.m_Data, (*it)->data.m_Size);
            
        }
        
    }
    
    while(m_Files.size() > 0){
        
        file_info* temp = m_Files.back();
        delete temp;
        m_Files.pop_back();
        
    }
    
}

void RLE_v2::ExtractArchive(const std::string& source)
{
    
    // Steps
    // 1. Read in files from archive
    //      A. Read in Header
    //      B. Read in a file_info* for each file in the archive, adding it to m_Files array
    
    std::ifstream::pos_type size;
    char* memblock;
    
    std::ifstream arc(source, std::ios::in|std::ios::binary|std::ios::ate);
    if (arc.is_open())
    {
        size = arc.tellg(); // save the size of the file;
        int intsize = static_cast<int>(size);
        
        memblock = new char [static_cast<unsigned int>(size)];
        
        arc.seekg(0, std::ios::beg);
        arc.read(m_Header.sig, 4);
        
        if(strcmp(m_Header.sig, "RLE0x02")){
        
            arc.read ( memblock, intsize-4 );
            
            //arc.read(m_Header.dirNameLength, 1);
            //m_Header.dirNameLength = *(reinterpret_cast<unsigned char*>(&memblock[0]));
            m_Header.dirNameLength = *(reinterpret_cast<unsigned char*>(&memblock[0]));
            
            std::string diName;
            
            // grab the directory name
            for( int i = 0; i < m_Header.dirNameLength; i++){
                diName += *(reinterpret_cast<char*>(&memblock[i+1]));
            }
            
            m_Header.dirName = diName;
            
            //int remainFileSize;
            //remainFileSize = (intsize - 5) - m_Header.dirNameLength;
            int index = 1 + m_Header.dirNameLength;
            
            while( index + 4 < intsize ){
                
                file_info* temp = new file_info;
                
                temp->fileSize = *(reinterpret_cast<int*>(&memblock[ index ]));
                index = index + 4;
                temp->compressedSize = *(reinterpret_cast<unsigned int*>(&memblock[ index ]));
                index = index + 4;
                temp->fileNameLength = *(reinterpret_cast<unsigned int*>(&memblock[ index ]));
                ++index;
                
                std::string fiName;
                
                for( int j = 0; j < temp->fileNameLength; j++){
                    fiName += *(reinterpret_cast<char*>(&memblock[j + index]));
                }
                
                temp->fileName = fiName;
                
                index = index + temp->fileNameLength;
                
                temp->data.Decompress( reinterpret_cast<char*>(&memblock[ index ]), temp->compressedSize, temp->fileSize);
                
                index = index + temp->compressedSize;
                
                m_Files.push_back(temp);
            }
            
            //delete temp;
        }
        arc.close();
    }
    
    // 2. Create Directory
    std::string directoryName = source.substr(0, source.length()-4);// get rid of .rl2
    
    FileSystem::createDirectory( directoryName );
    
    // 3. Iterate through m_Files list and output each file to the directory
    for (std::list<file_info*>::iterator it = m_Files.begin(); it != m_Files.end(); ++it){
        
        std::string pathName = directoryName + "/" + (*it)->fileName;
        
        std::ofstream out(pathName, std::ios::out|std::ios::binary|std::ios::trunc);
        if (out.is_open())
        {
            out.write((*it)->data.m_Data, (*it)->data.m_Size);
            out.close();
        }
    }
    
    while(m_Files.size() > 0){
        
        file_info* temp = m_Files.back();
        delete temp;
        m_Files.pop_back();
        
    }
    
}

/////////////////////////////////////////////////////////////////////////////////
// Store:
// File Sig [4],            Dir Name length Len [1],        Directory Name [Dir Name Length],
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
// File 1: File Size [4],   File 1: Compressed Size [4],    F1 Name Length [1],
// File 1: File Name [ F1 Name Length ],                    RLE Data [Compressed File Length],
// File 2: File Size [4],   File 2: Compressed Size [4],    F2 Name Length [1],
// File 2: File Name [ F2 Name length ],                    RLE Data [Compressed File Length]
// EOF
// One compressed directory with 2 files
/////////////////////////////////////////////////////////////////////////////////


RLE_v2::~RLE_v2()
{
	for (auto i = m_Files.begin(); i != m_Files.end(); ++i)
	{
		delete *i;
	}
    
	m_Files.clear();
}
