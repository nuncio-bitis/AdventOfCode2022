//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

//-----------------------------------------------------------------------------

// static const char cInputFileName[] = "test.txt";
static const char cInputFileName[] = "input08.txt";

static std::ifstream infile(cInputFileName);

struct treeSpec {
    int h;
    bool vis;
    int score;
};

static std::vector< std::vector<treeSpec> > forest;

static int totalVis = 0;

static int maxScore = 0;
static int maxScoreRow = 0;
static int maxScoreCol = 0;

//-----------------------------------------------------------------------------

void printForest(void)
{
    std::cout << std::endl;
    std::cout << "Forest: " << std::endl;
    for (auto row : forest)
    {
        for (auto tree : row)
        {
            std::cout << "[" << tree.h << "," << tree.vis << "," << tree.score << "] ";
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

    std::cout << "Loading forest..." << std::endl;
    std::string inputLine;
    while (!infile.eof())
    {
        getline(infile, inputLine);
        // Skip blank lines.
        if (inputLine.length() == 0)
        {
            break;
        }

        std::cout << inputLine << std::endl; // @DEBUG

        std::vector<treeSpec> row;
        for (size_t col = 0; col < inputLine.size(); ++col)
        {
            int h = (int)inputLine[col] - '0';
            treeSpec tree = { h, false, 1 };
            row.push_back(tree);
        }
        forest.push_back(row);
    }

    // The outside trees are definitively visible
    //            2*Rows       +   2*(Columns-2)
    totalVis = 2*forest.size() + 2*(forest[0].size() - 2);
    std::cout << "Given: The outside " << totalVis << " trees are visible." << std::endl;
    for (size_t r = 0; r < forest.size(); ++r)
    {
        if ((r == 0) || r == (forest.size() - 1))
        {
            for (size_t c = 0; c < forest[r].size(); ++c)
            {
                forest[r][c].vis = true;
            }
        }
        else
        {
            forest[r][0].vis = true;
            forest[r][forest[r].size()-1].vis = true;
        }
    }
}

//-----------------------------------------------------------------------------

void part_1(void)
{
    std::cout << "PART 1: Processing forest..." << std::endl;

    // Process inner rows (skip 0 and size-1)
    for (size_t r = 1; r < (forest.size() - 1); ++r)
    {
        for (size_t c = 1; c < (forest[r].size() - 1); ++c)
        {
            // Trees to left
            int maxL = 0;
            for (size_t i = 0; i < c; ++i)
            {
                if (forest[r][i].h > maxL) maxL = forest[r][i].h;
            }

            // Trees to right
            int maxR = 0;
            for (size_t i = c+1; i < forest[r].size(); ++i)
            {
                if (forest[r][i].h > maxR) maxR = forest[r][i].h;
            }

            // Trees above
            int maxT = 0;
            for (size_t i = 0; i < r; ++i)
            {
                if (forest[i][c].h > maxT) maxT = forest[i][c].h;
            }

            // Trees under
            int maxB = 0;
            for (size_t i = r+1; i < forest.size(); ++i)
            {
                if (forest[i][c].h > maxB) maxB = forest[i][c].h;
            }

            if ((forest[r][c].h > maxL) ||
                (forest[r][c].h > maxR) ||
                (forest[r][c].h > maxT) ||
                (forest[r][c].h > maxB) )
            {
                forest[r][c].vis = true;
                totalVis++;
            }
        }
    }
    std::cout << "Total trees visible: " << totalVis << std::endl;
}

//-----------------------------------------------------------------------------

void part_2(void)
{
    std::cout << "PART 2: Processing forest..." << std::endl;

    // Process inner rows (skip 0 and size-1)
    for (size_t r = 1; r < (forest.size() - 1); ++r)
    {
        for (size_t c = 1; c < (forest[r].size() - 1); ++c)
        {
            // std::cout << "[" << r << "," << c << "]:" << std::endl; // @DEBUG

            // Trees to left
            int maxL = 0;
            for (int i = c-1; i >= 0; --i)
            {
                // std::cout << "    [" << r << "," << i << "]" << std::endl; // @DEBUG
                maxL++;
                if (forest[r][i].h >= forest[r][c].h)
                {
                    break;
                }
            }

            // Trees to right
            int maxR = 0;
            for (int i = c+1; i < forest[r].size(); ++i)
            {
                // std::cout << "    [" << r << "," << i << "]" << std::endl; // @DEBUG
                maxR++;
                if (forest[r][i].h >= forest[r][c].h)
                {
                    break;
                }
            }

            // Trees above
            int maxT = 0;
            for (int i = r-1; i >= 0; --i)
            {
                // std::cout << "    [" << i << "," << c << "]" << std::endl; // @DEBUG
                maxT++;
                if (forest[i][c].h >= forest[r][c].h)
                {
                    break;
                }
            }

            // Trees under
            int maxB = 0;
            for (int i = r+1; i < forest.size(); ++i)
            {
                // std::cout << "    [" << i << "," << c << "]" << std::endl; // @DEBUG
                maxB++;
                if (forest[i][c].h >= forest[r][c].h)
                {
                    break;
                }
            }
            // @DEBUG
            // std::cout << "  "
            //           << "maxL = " << maxL << ", "
            //           << "maxR = " << maxR << ", "
            //           << "maxT = " << maxT << ", "
            //           << "maxB = " << maxB
            //           << std::endl;
            // @DEBUG

            // Calculate scenic score for this tree
            forest[r][c].score = maxL * maxR * maxT * maxB;
            // std::cout << "  Scenic score: " << forest[r][c].score << " @ [" << r << "," << c << "]" << std::endl; // @DEBUG

            if (forest[r][c].score > maxScore)
            {
                maxScore = forest[r][c].score;
                maxScoreRow = r;
                maxScoreCol = c;
            }
        }
    }
    std::cout << "Maximum scenic score: " << maxScore
        << " @ [" << maxScoreRow << "," << maxScoreCol << "]" << std::endl;
}

//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    std::cout << std::endl;
    //-------------------------------------------------------------------------

    loadInputs();
    // printForest();

    std::cout << std::endl;
    part_1();
    // printForest();

    std::cout << std::endl;
    part_2();
    // printForest();

    //-------------------------------------------------------------------------

    std::cout << std::endl;
    return EXIT_SUCCESS;
}

//-----------------------------------------------------------------------------
