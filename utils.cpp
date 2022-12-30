/*
 * utils.cpp
 *
 * Created on: 30 Dec 2022
 * Author: Jim Parziale
 */
// ****************************************************************************

#include <stdarg.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#include <ctime>
#include <iomanip>

#include "utils.h"

// ****************************************************************************

Utils *Utils::s_pInstance = nullptr;
bool Utils::s_test = false;     // Using test input
bool Utils::s_debug = false;    // Debug output enabled
bool Utils::s_disblk = false;   // Input data disabled
uint Utils::s_inpLineNo = 0;
bool Utils::s_blank = false;    // Last input line was blank

// ****************************************************************************

Utils::Utils()
{
    s_pInstance = this;
}

Utils *Utils::getInstance()
{
    return s_pInstance;
}

// ****************************************************************************

bool Utils::ignoreLine(std::string line)
{
    s_inpLineNo++;
    s_blank = false;

    // Skip blank lines.
    if (!s_disblk && line.length() == 0)
    {
        // std::cout << "## BLANK @ " << s_inpLineNo << std::endl; // @DEBUG
        s_blank = true;
        return true;
    }
    // Check if a block of input has been disabled
    if ((line[0] == ';') && (line.find("DISABLE") != std::string::npos))
    {
        std::cout << "## INPUT DISABLED @ " << s_inpLineNo << std::endl; // @DEBUG
        s_disblk = true;
        return true;
    }
    // Check if block disable is ended
    if ((line[0] == ';') && (line.find("ENABLE") != std::string::npos))
    {
        std::cout << "## INPUT ENABLED @ " << s_inpLineNo << std::endl; // @DEBUG
        s_disblk = false;
        return true;
    }
    // Check if test input
    if (!s_disblk && (line[0] == ';') && (line.find("TEST") != std::string::npos))
    {
        std::cout << "## PROCESSING TEST INPUT @ " << s_inpLineNo << std::endl; // @DEBUG
        s_test = true;
        return true;
    }
    if (!s_disblk && (line[0] == ';') && (line.find("Test") != std::string::npos))
    {
        std::cout << "## PROCESSING TEST INPUT @ " << s_inpLineNo << std::endl; // @DEBUG
        s_test = true;
        return true;
    }
    // Check if debug output should be enabled
    if (!s_disblk && (line[0] == ';') && (line.find("DEBUG") != std::string::npos))
    {
        std::cout << "## DEBUG enabled @ " << s_inpLineNo << std::endl; // @DEBUG
        s_debug = true;
        return true;
    }
    // Skip disabled block
    if (s_disblk)
    {
        return true;
    }
    // Skip comment lines.
    if (line[0] == ';')
    {
        return true;
    }
    return false;
}

// ****************************************************************************

void Utils::DPRINTF(const char *logline, ...)
{
    if (s_debug)
    {
        va_list argList;
        char cbuffer[1024];
        std::stringstream m_outString;
        va_start(argList, logline);
        vsnprintf(cbuffer, 1024, logline, argList);
        va_end(argList);
        m_outString.fill(' ');
        m_outString << TimeStamp() << " " << cbuffer << std::endl;
        std::cout << m_outString.str();
    }
}

//******************************************************************************

void Utils::DPRINTF(const std::string &input)
{
    if (s_debug)
    {
        std::stringstream m_outString;
        m_outString.fill(' ');
        m_outString << TimeStamp() << " "
                    << input << std::endl;
        std::cout << m_outString.str();
    }
}

//******************************************************************************

void Utils::DPRINTF(std::stringstream &input)
{
    if (s_debug)
    {
        std::stringstream m_outString;
        m_outString.fill(' ');
        m_outString << TimeStamp() << " "
                    << input.str() << std::endl;
        std::cout << m_outString.str();

        // Empty the stringstream after use
        input.str(std::string());
        input.clear();
    }
}

//*****************************************************************************

// Return date string in the form: yyyy-mm-dd or yyyymmdd
std::string Utils::GetDateStr(bool useSep)
{
    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);
    char tsBuf[16];

    if (useSep)
    {
        // Format: YYYY-MM-DD
        (void)std::strftime(tsBuf, sizeof(tsBuf), "%F", local_time);
    }
    else
    {
        // Format: YYYYMMDD
        (void)std::strftime(tsBuf, sizeof(tsBuf), "%Y%m%d", local_time);
    }

    return std::string(tsBuf);
}

//*****************************************************************************

// Return time string in the form: HH:MM:SS.nnn, HHMMSS.nnn, HH:MM:SS, or HHMMSS
std::string Utils::GetTimeStr(bool useSep, bool useMsec)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    int msec = (int)((ts.tv_nsec / 1000000) % 1000);

    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);
    char tsBuf[32];
    int idx = 0;

    if (useSep)
    {
        // Format: HH:MM:SS
        idx = std::strftime(tsBuf, sizeof(tsBuf), "%T", local_time);
    }
    else
    {
        // Format: HHMMSS
        idx = std::strftime(tsBuf, sizeof(tsBuf), "%H%M%S", local_time);
    }

    // Append milliseconds
    if (useMsec)
    {
        snprintf(&tsBuf[idx], sizeof(tsBuf), ".%03d", msec);
    }

    return std::string(tsBuf);
}

//*****************************************************************************

// Return timestamp in the form: "yyyy-mm-dd HH:MM:SS.nnn"
std::string Utils::TimeStamp(bool useSep)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    int msec = (int)((ts.tv_nsec / 1000000) % 1000);

    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);
    char tsBuf[32];
    int idx = 0;

    if (useSep)
    {
        // Format: YYYY-MM-DD HH:MM:SS
        idx = std::strftime(tsBuf, sizeof(tsBuf), "%F %T", local_time);
    }
    else
    {
        // Format: YYYYMMDD HHMMSS
        idx = std::strftime(tsBuf, sizeof(tsBuf), "%Y%m%d %H%M%S", local_time);
    }

    // Append milliseconds
    snprintf(&tsBuf[idx], sizeof(tsBuf), ".%03d", msec);

    return std::string(tsBuf);
}

// ****************************************************************************
