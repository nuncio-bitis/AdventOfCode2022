//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>

//-----------------------------------------------------------------------------

// static const char cInputFileName[] = "test.txt";
static const char cInputFileName[] = "input12.txt";

static std::ifstream infile(cInputFileName);

struct locn {
    uint height;
    uint dist; // distance from start
    bool visited;
};

typedef std::vector<locn> mapRow;
static std::vector<mapRow> map;

static int mapW = 0;
static int mapH = 0;

typedef std::pair<int, int> point;
static point start;
static point fin;

//-----------------------------------------------------------------------------

void printMap(void)
{
    std::cout << std::endl;
    std::cout << "Map:" << std::endl;
    std::cout << "Dimensions: " << mapH << " x " << mapW << std::endl;
    printf("  Start: (%d, %d)\n", start.first, start.second);
    printf(" Finish: (%d, %d)\n", fin.first, fin.second);
    for (auto row : map)
    {
        for (locn loc : row)
        {
            printf(" (%c,%2u, %d )", loc.height, loc.dist, loc.visited);
        }
        std::cout << std::endl;
    }
}

//-----------------------------------------------------------------------------

void loadInputs(void)
{
    if (!infile)
    {
        std::cerr << "Error: unable to open input file: " << cInputFileName << std::endl;
        exit(1);
    }

    std::cout << "Loading map..." << std::endl;
    std::string iline;
    uint rowNum = 0;
    while (!infile.eof())
    {
        getline(infile, iline);
        // Skip blank lines.
        if (iline.length() == 0)
        {
            break;
        }

        mapRow row;

        for (uint col = 0; col < iline.size(); ++col)
        {
            char c = iline[col];
            locn here;
            here.visited = false;
            here.dist = 0; //UINT_MAX;
            if (c == 'S')
            {
                here.height = 'a'; // - 'a';
                start = std::make_pair(rowNum, col);
            }
            else if (c == 'E')
            {
                here.height = 'z'; // - 'a';
                fin = std::make_pair(rowNum, col);
            }
            else
            {
                here.height = c; // - 'a';
            }
            row.push_back(here);
        }

        map.push_back(row);
        rowNum++;
    }
    mapW = map[0].size();
    mapH = map.size();
}

//-----------------------------------------------------------------------------

// Recursively check the path starting at the given point.
// Update the distance from the start.
// Returns true if endpoint found.
bool checkPath(point loc)
{
    uint tmp;
    point up = {999, 999};
    point left = {999, 999};
    point down = {999, 999};
    point right  = {999, 999};
    int upDiff = 999;
    int leftDiff = 999;
    int downDiff = 999;
    int rightDiff = 999;

    printf("loc: (%d, %d) %c\n", loc.first, loc.second, map[loc.first][loc.second].height); // @DEBUG

    // Check if end point
    if (loc == fin)
    {
        std::cout << "*** FIN ***" << std::endl;
        map[loc.first][loc.second].dist = 0;
        return true;
    }

    // Don't revisit a location.
    if (map[loc.first][loc.second].visited)
    {
        return false;
    }
    // This location is visited...
    map[loc.first][loc.second].visited = true;

    // Check the path up...
    if (loc.first > 0)
    {
        up.first  = loc.first - 1;
        up.second = loc.second;
        upDiff = map[up.first][up.second].height - map[loc.first][loc.second].height;
    }

    // Check the path left...
    if (loc.second > 0)
    {
        left.first  = loc.first;
        left.second = loc.second - 1;
        leftDiff = map[left.first][left.second].height - map[loc.first][loc.second].height;
    }

    // Check the path right...
    if (loc.second < mapW-1)
    {
        right.first  = loc.first;
        right.second = loc.second + 1;
        rightDiff = map[right.first][right.second].height - map[loc.first][loc.second].height;
    }

    // Check the path down...
    if (loc.first < mapH-1)
    {
        down.first  = loc.first + 1;
        down.second = loc.second;
        downDiff = map[down.first][down.second].height - map[loc.first][loc.second].height;
    }

    // printf("^(%d, %d) <(%d, %d) >(%d, %d) v(%d, %d) \n",
    //         up.first, up.second,
    //         left.first, left.second,
    //         right.first, right.second,
    //         down.first, down.second
    //       ); // @DEBUG

    // Prefer to go up a level
    // Somehow it's ok to go down 2 levels?
    // That's the only way to transition from q to r: ...p-q-o-p-q-r...
    for (int i = 1; i >= -1; --i)
    {
        if (upDiff == i)
        {
            if (checkPath(up))
            {
                tmp = 1 + map[up.first][up.second].dist;
                map[loc.first][loc.second].dist = tmp;
                return true;
            }
        }
        if (leftDiff == i)
        {
            if (checkPath(left))
            {
                tmp = 1 + map[left.first][left.second].dist;
                map[loc.first][loc.second].dist = tmp;
                return true;
            }
        }
        if (rightDiff == i)
        {
            if (checkPath(right))
            {
                tmp = 1 + map[right.first][right.second].dist;
                map[loc.first][loc.second].dist = tmp;
                return true;
            }
        }
        if (downDiff == i)
        {
            if (checkPath(down))
            {
                tmp = 1 + map[down.first][down.second].dist;
                map[loc.first][loc.second].dist = tmp;
                return true;
            }
        }
    } // end for (1, 0)

    return false;
}

void part_1(void)
{
    std::cout << "Navigating..." << std::endl;

    // Starting at the start...
    if (checkPath(start))
    {
        std::cout << "SUCCESS" << std::endl;
        printf("Distance from start = %d\n", map[start.first][start.second].dist);
    }
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

    loadInputs();
    // printMap();

    std::cout << std::endl;
    part_1();
    // printMap();

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
