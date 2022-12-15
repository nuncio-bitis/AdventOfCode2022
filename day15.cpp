//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>

//-----------------------------------------------------------------------------

// static const char cInputFileName[] = "test.txt";
static const char cInputFileName[] = "input15.txt";

static std::ifstream infile(cInputFileName);

// Grid: [row][col] = [y][x]
static int minX = INT_MAX;
static int maxX = INT_MIN;
static int minY = INT_MAX;
static int maxY = INT_MIN;
static int gridWidth = 0;
static int gridHeight = 0;

struct sensor {
    int x;
    int y;
    int d2b;
};
std::vector<sensor> sensors;
std::vector<sensor> beacons;

//-----------------------------------------------------------------------------

static bool disblk = false;
static int testRow = 0;

bool ignoreLine(std::string line)
{
    // Skip blank lines.
    if (line.length() == 0)
    {
        return true;
    }
    // Check if a block of input has been disabled
    if ((line[0] == '#') && (line.find("DISABLE") != std::string::npos))
    {
        // std::cout << "--- INPUT DISABLED ---" << std::endl; // @DEBUG
        disblk = true;
        return true;
    }
    // Check if block disable is ended
    if ((line[0] == '#') && (line.find("ENABLE") != std::string::npos))
    {
        // std::cout << "--- INPUT ENABLED ---" << std::endl; // @DEBUG
        disblk = false;
        return true;
    }
    // Skip disabled block
    if (disblk)
    {
        return true;
    }
    // @NOTE Day 13
    // Look for row input
    size_t pos = line.find("ROW");
    if ((line[0] == '#') && (pos != std::string::npos))
    {
        testRow = std::stoi(line.substr(pos+4));
        std::cout << "Input row number: " << testRow << std::endl; // @DEBUG
        return true;
    }
    // @NOTE Day 13
    // Skip comment lines.
    if (line[0] == '#')
    {
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------

void loadInputs(void)
{
    if (!infile)
    {
        std::cerr << "Error: unable to open input file: " << cInputFileName << std::endl;
        exit(1);
    }

    std::cout << "Loading inputs..." << std::endl;
    std::string iline;
    while (!infile.eof())
    {
        getline(infile, iline);
        // Check if line should be ignored.
        if (ignoreLine(iline))
        {
            continue;
        }
        std::cout << " > " << iline << std::endl; // @DEBUG

        int sx, sy, bx, by;
        sscanf(iline.c_str(), "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d",
                &sx, &sy, &bx, &by);

        if (sx < minX) minX = sx;
        if (bx < minX) minX = bx;
        if (sy < minY) minY = sy;
        if (by < minY) minY = by;

        if (sx > maxX) maxX = sx;
        if (bx > maxX) maxX = bx;
        if (sy > maxY) maxY = sy;
        if (by > maxY) maxY = by;

        // Add sensor to list, including its distance to the nearest beacon.
        sensor s = {sx, sy, (abs(sx - bx) + abs(sy - by))};
        sensors.push_back(s);
        // Add beacon to list.
        sensor b = {bx, by, 0};
        beacons.push_back(b);
    }
    infile.close();

    std::cout << std::endl;
    std::cout << "Y: [" << minY << ".." << maxY << "]" << std::endl;
    std::cout << "X: [" << minX << ".." << maxX << "]" << std::endl;
    gridWidth = (abs(maxX - minX)+ 1);
    gridHeight = (abs(maxY - minY) + 1);
    std::cout << "Grid size: " << gridWidth << " x " << gridHeight << std::endl;
}

//-----------------------------------------------------------------------------

void part_1(int rownum)
{
    std::cout << "Processing row " << rownum << " ..." << std::endl;
    int totalEmpty = 0;

    // Create row of unknown values.
    std::vector<char> row;
    for (int x = 0; x < gridWidth; ++x)
    {
        row.push_back('.');
    }
    // Mark all beacons on this row
    for (auto b : beacons)
    {
        if (rownum == b.y)
        {
            row[b.x] = 'B';
        }
    }

    // Given a row (y), test each point against sensors and beacons.
    // Count the grid point if:
    // Its dist to sensor < (sensor to beacon)
    // AND
    // Its not a beacon

    // Iterate over each point in the row
    for (int x = minX; x <= maxX; ++x)
    {
        int lx = x - minX;
        // Check if this point is a beacon
        if (row[lx] == 'B')
        {
            std::cout << "Found beacon at [" << lx << "," << rownum << "]" << std::endl; // @DEBUG
            continue;
        }

        // Not a beacon. Check if it's within distance of a sensor.
        for (auto s : sensors)
        {
            int distToS = abs(x - s.x) + abs(rownum - s.y);
            // std::cout << "[" << x << "," << row << "]: " << distToS << ", " << s.d2b << std::endl; // @DEBUG
            if (distToS <= s.d2b)
            {
                totalEmpty++;
                // std::cout << "  Found possibility at [" << x << "," << row << "]" << std::endl; // @DEBUG
               break;
            }
        }

        // std::cout << "---" << std::endl; // @DEBUG
    } // end row iteration

    std::cout << "Found " << totalEmpty << " possibilities" << std::endl;
}

//-----------------------------------------------------------------------------

void part_2(void)
{
    std::cout << "Processing..." << std::endl;

    // @TODO
}

//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    std::cout << std::endl;
    //-------------------------------------------------------------------------

    // if (argc < 2)
    // {
    //     std::cerr << "ERROR: Need row #" << std::endl;
    //     std::cout << std::endl;
    //     return EXIT_FAILURE;
    // }
    // int testRow = 10;
    // int testRow = 2000000;
    // int testRow = std::atoi(argv[1]);

    loadInputs();

    std::cout << std::endl;
    part_1(testRow);

    // std::cout << std::endl;
    // part_2();

    // std::cout << "Results: " << std::endl;
    // // @TODO
    // std::cout << std::endl;

    //-------------------------------------------------------------------------

    std::cout << std::endl;
    return EXIT_SUCCESS;
}

//-----------------------------------------------------------------------------
