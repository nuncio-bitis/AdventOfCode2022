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
static int64_t gridMinX = INT_MAX;
static int64_t gridMaxX = INT_MIN;
static int64_t gridMinY = INT_MAX;
static int64_t gridMaxY = INT_MIN;

static int64_t gridWidth = 0;
static int64_t gridHeight = 0;

struct sensor {
    int64_t x;
    int64_t y;
    int64_t d2b;
};
std::vector<sensor> sensors;
std::vector<sensor> beacons;

//-----------------------------------------------------------------------------

static bool disblk = false;
static int64_t testRow = 0;

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
    uint idx=1;
    while (!infile.eof())
    {
        getline(infile, iline);
        // Check if line should be ignored.
        if (ignoreLine(iline))
        {
            continue;
        }
        // std::cout << " > " << iline << std::endl; // @DEBUG

        int64_t sx, sy, bx, by;
        sscanf(iline.c_str(), "Sensor at x=%ld, y=%ld: closest beacon is at x=%ld, y=%ld",
                &sx, &sy, &bx, &by);

        if (sx < gridMinX) gridMinX = sx;
        if (bx < gridMinX) gridMinX = bx;
        if (sy < gridMinY) gridMinY = sy;
        if (by < gridMinY) gridMinY = by;

        if (sx > gridMaxX) gridMaxX = sx;
        if (bx > gridMaxX) gridMaxX = bx;
        if (sy > gridMaxY) gridMaxY = sy;
        if (by > gridMaxY) gridMaxY = by;

        // Add sensor to list, including its distance to the nearest beacon.
        sensor s = {sx, sy, (labs(sx - bx) + labs(sy - by))};
        printf(" > %2d [%8lld,%8lld] : %8lld\n", idx++, s.x, s.y, s.d2b); // @DEBUG

        // Dynamically expand the grid
        if ((s.x - s.d2b/2) < gridMinX)
        {
            gridMinX = (s.x - s.d2b/2);
        }
        if ((s.y - s.d2b/2) < gridMinY)
        {
            gridMinY = (s.y - s.d2b/2);
        }
        if ((s.x + s.d2b/2) > gridMaxX)
        {
            gridMaxX = (s.x + s.d2b/2);
        }
        if ((s.y + s.d2b/2) > gridMaxY)
        {
            gridMaxY = (s.y + s.d2b/2);
        }
        sensors.push_back(s);
        // Add beacon to list.
        sensor b = {bx, by, 0};
        beacons.push_back(b);
    }
    infile.close();

    std::cout << std::endl;
    std::cout << "Y: [" << gridMinY << ".." << gridMaxY << "]" << std::endl;
    std::cout << "X: [" << gridMinX << ".." << gridMaxX << "]" << std::endl;
    gridWidth = (labs(gridMaxX - gridMinX)+ 1);
    gridHeight = (labs(gridMaxY - gridMinY) + 1);
    std::cout << "Grid size: " << gridWidth << " x " << gridHeight << std::endl;
}

//-----------------------------------------------------------------------------

void part_1(int64_t rownum)
{
    std::cout << "PART 1: Processing row " << rownum << " ..." << std::endl;
    int64_t totalEmpty = 0;

    // Create row of unknown values.
    std::vector<char> row;
    for (int64_t x = 0; x < gridWidth; ++x)
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
    for (int64_t x = gridMinX; x <= gridMaxX; ++x)
    {
        int64_t lx = x - gridMinX;
        // Check if this point is a beacon
        if (row[lx] == 'B')
        {
            std::cout << "Found beacon at [" << lx << "," << rownum << "]" << std::endl; // @DEBUG
            continue;
        }

        // Not a beacon. Check if it's within distance of a sensor.
        for (auto s : sensors)
        {
            int64_t distToS = labs(x - s.x) + labs(rownum - s.y);
            // std::cout << "[" << x << "," << row << "]: " << distToS << ", " << s.d2b << std::endl; // @DEBUG
            if (distToS <= s.d2b)
            {
                totalEmpty++;
                row[x] = '#';
                // std::cout << "  Found possibility at [" << x << "," << row << "]" << std::endl; // @DEBUG
                break;
            }
        }
    } // end row iteration

    std::cout << "Found " << totalEmpty << " possibilities" << std::endl;
}

//-----------------------------------------------------------------------------

int64_t maxSize  = 4000000;
int64_t freqMult = 4000000;

void part_2(void)
{
    std::cout << "PART 2: Processing ..." << std::endl;

    int64_t minX = gridMinX;
    int64_t maxX = gridMaxX;
    int64_t minY = gridMinY;
    int64_t maxY = gridMaxY;

    int64_t dbx = 0;
    int64_t freq = 0;

    // Constrain grid
    if (minX < 0) minX = 0;
    if (minY < 0) minY = 0;
    if (maxX > maxSize) maxX = maxSize;
    if (maxY > maxSize) maxY = maxSize;

    bool found = false;
    for (int64_t y = minY; y < maxY; ++y)
    {
        // @DEBUG
        if ((y % 10000) == 0)
        {
            std::cout << "  Processing row " << y << std::endl;
        }
        // @DEBUG

        // Create row of unknown values.
        std::vector<char> row;
        for (int64_t x = minX; x < maxX; ++x)
        {
            row.push_back('.');
        }
        // Mark all beacons on this row
        for (auto b : beacons)
        {
            if (y == b.y)
            {
                if ((b.x >= minX) && (b.x < maxX))
                {
                    row[b.x] = 'B';
                }
            }
        }

        // Given a row (y), test each point against sensors and beacons.
        // Count the grid point if:
        // Its dist to sensor < (sensor to beacon)
        // AND
        // Its not a beacon

        // Iterate over each point in the row
        for (int64_t x = minX; x <= maxX; ++x)
        {
            // Check if this point is a beacon
            if (row[x] == 'B')
            {
                // std::cout << "Found beacon at [" << x << "," << rownum << "]" << std::endl; // @DEBUG
                continue;
            }

            // Not a beacon. Check if it's within distance of a sensor.
            for (auto s : sensors)
            {
                if ((s.x >= minX) && (s.x < maxX))
                {
                    int64_t distToS = labs(x - s.x) + labs(y - s.y);
                    // std::cout << "[" << x << "," << rownum << "]: " << distToS << ", " << s.d2b << std::endl; // @DEBUG
                    if (distToS <= s.d2b)
                    {
                        row[x] = '#';
                        break;
                    }
                }
            }
        } // end row iteration

        for (size_t x = 0; x < row.size(); ++x)
        {
            // std::cout << " " << row[x];
            if (row[x] == '.')
            {
                dbx = x;
                freq = (freqMult * dbx + y);
                // std::cout << " " << row[x];
                std::cout << "Found distress beacon @ [" << x << "," << y << "]" << std::endl;
                found = true;
                break;
            }
        }

        row.clear();

        if (found) break;

    } // end y

    std::cout << "Tuning freq = " << freq << std::endl;
}

//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    std::cout << std::endl;
    //-------------------------------------------------------------------------

    loadInputs();

    std::cout << std::endl;
    part_1(testRow);

    std::cout << std::endl;
    part_2();

    //-------------------------------------------------------------------------

    std::cout << std::endl;
    return EXIT_SUCCESS;
}

//-----------------------------------------------------------------------------
