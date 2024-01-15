// This file is part of Micropolis-SDL2PP
// Micropolis-SDL2PP is based on Micropolis
//
// Copyright © 2022 Leeor Dicker
//
// Portions Copyright © 1989-2007 Electronic Arts Inc.
//
// Micropolis-SDL2PP is free software; you can redistribute it and/or modify
// it under the terms of the GNU GPLv3, with additional terms. See the README
// file, included in this distribution, for details.
#include "FileIo.h"

#include <filesystem>
#include <stdexcept>

#include "nfd.hpp"


FileIo::FileIo(SDL_Window&)
{
    if(NFD::Init() != NFD_OKAY)
    {
        throw std::runtime_error("Unable to initialise file picker library.");
    }
    
    mSeparator = std::filesystem::path::preferred_separator;
}


FileIo::~FileIo()
{
    NFD::Quit();
}


bool FileIo::filePicked() const
{
    return !mFileName.empty();
}


/**
 * \return Returns true if a file name was selected, false otherwise.
 */
bool FileIo::pickSaveFile()
{
    const auto filePicked = showFileDialog(FileOperation::Save);
    
    if (filePicked)
    {
        extractFileName();
    }

    return filePicked;
}


/**
 * \return Returns true if a file name was selected, false otherwise.
 */
bool FileIo::pickOpenFile()
{
    const auto filePicked = showFileDialog(FileOperation::Open);

    if (filePicked)
    {
        extractFileName();
    }

    return filePicked;
}


void FileIo::extractFileName()
{
    std::size_t location = mFileName.find_last_of(mSeparator);
    mFileName = mFileName.substr(location + 1);
}


/**
 * \return Returns true if a file name has been picked. False otherwise.
 */
bool FileIo::showFileDialog(FileOperation operation)
{
    NFD::UniquePath outPath;
    nfdfilteritem_t filterItem[1] = {{"Micropolis City", "cty"}};
       
    if(operation == FileOperation::Open)
    {
        if(NFD::OpenDialog(outPath, filterItem, 1) != NFD_OKAY)
        {
            return false;
        }
    }
    else
    {
        if(NFD::SaveDialog(outPath, filterItem, 1) != NFD_OKAY)
        {
            return false;
        }
    }
    
    mFileName = outPath.get();
    
    std::size_t location = mFileName.find_last_of(mSeparator);
    mSavePath = mFileName.substr(0, location);
    mFileName = mFileName.substr(location + 1);

    return true;
}
