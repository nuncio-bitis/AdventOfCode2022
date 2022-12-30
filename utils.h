/*
 * utils.h
 *
 * Created on: 30 Dec 2022
 * Author: Jim Parziale
 */

#ifndef utils_H_
#define utils_H_
// ****************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <string>
#include <vector>
#include <utility>
#include <iterator>

class Utils : public std::enable_shared_from_this<Utils>
{
    // ------------------------------------------------------------------------
public:
    Utils();
    virtual ~Utils() = default;
    static Utils *getInstance();

    // True if processing test input (specified in input file)
    static bool isTest(void) { return s_test; };

    // True if debug output enabled (specified in input file)
    static bool debugEnabled(void) { return s_debug; };

    // True is input line is to be ignored (comment, blank line, or directive)
    static bool ignoreLine(std::string line);
    static bool isBlank(void) { return s_blank; };

    static int nInputLines(void) { return s_inpLineNo; };

    static void DPRINTF(const char *logline, ...);
    static void DPRINTF(const std::string &input);
    static void DPRINTF(std::stringstream &input);

    static std::string GetDateStr(bool useSep = true);
    static std::string GetTimeStr(bool useSep = true, bool useMsec = true);
    static std::string TimeStamp(bool useSep = true);

    // ------------------------------------------------------------------------
private:
    static Utils *s_pInstance;
    static bool s_test;
    static bool s_debug;
    static bool s_disblk;
    static uint s_inpLineNo;
    static bool s_blank;
};

template<typename T>
void printVec(std::vector<T> v)
{
    for (auto x : v)
    {
        std::cout << " " << x;
    }
    std::cout << std::endl;
}

// ****************************************************************************
#endif /* utils_H_ */
