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

#if defined(WIN32)
#define NOMINMAX
#include <windows.h>
#include <ShlObj_core.h>
#include <Shlwapi.h>
#undef NOMINMAX
#endif


#include <locale>
#include <codecvt>
#include <sstream>
#include <stdexcept>

#include "nfd.hpp"


namespace
{
    #if defined(WIN32)
    COMDLG_FILTERSPEC FileTypeFilter[] =
    {
        { L"Micropolis City", L"*.cty"}
    };


    const std::string StringFromWString(const std::wstring& str)
    {
        const auto length = WideCharToMultiByte(CP_UTF8, 0, &str.at(0), (int)str.size(), nullptr, 0, nullptr, nullptr);

        if (length <= 0)
        {
            throw std::runtime_error("WideCharToMultiByte() failed.");
        }

        std::string out(length, 0);
        WideCharToMultiByte(CP_UTF8, 0, &str.at(0), (int)str.size(), &out.at(0), length, nullptr, nullptr);
        return out;
    }
    #endif

};


FileIo::FileIo(SDL_Window&)
{
    if(NFD::Init() != NFD_OKAY)
    {
        throw std::runtime_error("Unable to initialise file picker library.");
    }
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
    std::size_t location = mFileName.find_last_of("/\\");
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

    return true;
}
