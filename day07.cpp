//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>

//-----------------------------------------------------------------------------

// static const char cInputFileName[] = "test.txt";
static const char cInputFileName[] = "input07.txt";

static std::ifstream infile(cInputFileName);

//-----------------------------------------------------------------------------

typedef std::tuple<uint32_t, std::string> fspec;

struct Directory
{
    std::string name;
    std::vector<Directory *> dirs;
    std::vector<fspec *> files;
    uint32_t filesTotal;
    Directory *parent;
};

static Directory fs;
static Directory *root = &fs;
static Directory *cwd = root;

//-----------------------------------------------------------------------------

void processDirTree(Directory *root, std::string indent)
{
    std::cout << indent << root->name << std::endl;
    for (int dir=0; dir < root->dirs.size(); ++dir)
    {
        processDirTree(root->dirs[dir], (indent + "...."));
        root->filesTotal += root->dirs[dir]->filesTotal;
    }
    for (auto file : root->files)
    {
        auto [ fsize, fname ] = *file;
        printf("%s....[%8d] %s\n", indent.c_str(), fsize, fname.c_str());
        root->filesTotal += fsize;
    }
    std::cout << indent << "Total: " << root->filesTotal << " bytes" << std::endl;
}

//-----------------------------------------------------------------------------

void processInputCmd(std::string cmd, std::string arg)
{
    if (cmd.compare("cd") == 0)
    {
        if (arg.compare("/") == 0)
        {
            cwd = root;
            // std::cout << "* New current dir = ROOT: " << cwd->name << std::endl; // @DEBUG
        }
        else if (arg.compare("..") == 0)
        {
            cwd = cwd->parent;
            // std::cout << "* New current dir = PARENT: " << cwd->name << std::endl; // @DEBUG
        }
        else
        {
            for (int dir = 0; dir < cwd->dirs.size(); ++dir)
            {
                if (arg.compare(cwd->dirs[dir]->name) == 0)
                {
                    cwd = cwd->dirs[dir];
                    // std::cout << "* New current dir: " << cwd->name << std::endl; // @DEBUG
                    break;
                }
            }
        }
    }
    else if (cmd.compare("ls") == 0)
    {
        ; // NOP
    }
    else
    {
        // std::cout << "* BAD COMMAND: '" << cmd << "' args: '" << arg << "'" << std::endl; // @DEBUG
    }
}

// First word:
// $ <command> <arg> => command ("cd <dir>", "ls")
// dir <dir> => subdirectory, name follows
// ###... => file size, name follows

void loadInputs(void)
{
    if (!infile)
    {
        std::cerr << "Error: unable to open input file: " << cInputFileName << std::endl;
        exit(1);
    }

    root->name = "/";
    root->filesTotal = 0;
    root->parent = root;

    std::cout << "Building file system..." << std::endl;
    std::string iline;
    while (!infile.eof())
    {
        getline(infile, iline);
        // Skip blank lines.
        if (iline.length() == 0)
        {
            break;
        }

        // std::cout << iline << std::endl; // @DEBUG

        std::size_t sp1 = iline.find_first_of(' '); // Position of first space
        if (iline.find_first_of('$') == 0)
        {
            // Command
            std::size_t sp2 = iline.find(' ', sp1+1); // Position of next space
            if (sp2 != std::string::npos)
            {
                processInputCmd(iline.substr(sp1+1, (sp2-sp1-1)), iline.substr(sp2+1));
            }
            else
            {
                processInputCmd(iline.substr(sp1+1), "");
            }
        }
        else if (iline.find_first_of("dir") == 0)
        {
            // Directory spec
            Directory *dir = new Directory();
            dir->name = iline.substr(sp1+1);
            dir->filesTotal = 0;
            dir->parent = cwd;
            cwd->dirs.push_back(dir);
            // std::cout << "* Dir: " << cwd->name << "/" << dir->name << std::endl; // @DEBUG
        }
        else
        {
            // Treat as file
            uint32_t fsize = std::stoi(iline.substr(0, sp1));
            std::string name = iline.substr(sp1+1);
            fspec *file = new fspec;
            *file = std::make_tuple(fsize, name);
            cwd->files.push_back(file);
            // std::cout << "* File: " << cwd->name << "/" << name << " (" << fsize << " bytes)" << std::endl; // @DEBUG
        }
    }

    std::cout << std::endl;
    std::cout << "Inputs info: " << std::endl;
    processDirTree(root, "");
    std::cout << std::endl;
}

//-----------------------------------------------------------------------------

static const uint32_t K100 = 100000;
static uint32_t total100s = 0;

void part_1(Directory *root)
{
    if (root->filesTotal <= K100)
    {
        std::cout << "Found: " << root->name << ", " << root->filesTotal << std::endl;
        total100s += root->filesTotal;
    }
    for (int dir=0; dir < root->dirs.size(); ++dir)
    {
        part_1(root->dirs[dir]);
    }
}

//-----------------------------------------------------------------------------

static const uint32_t totalDisk = 70000000;
static const uint32_t needSpace = 30000000;

static uint32_t smDir = totalDisk;

void findSmallest(Directory *root, uint32_t min)
{
    if ((root->filesTotal >= min) && (root->filesTotal <= smDir))
    {
        std::cout << "Found: " << root->name << ", " << root->filesTotal << std::endl;
        smDir = root->filesTotal;
    }
    for (int dir=0; dir < root->dirs.size(); ++dir)
    {
        findSmallest(root->dirs[dir], min);
    }
}

void part_2(void)
{
    uint32_t avail = totalDisk - root->filesTotal;
    std::cout << "Available space = " << avail << std::endl;
    if (avail >= needSpace)
    {
        std::cout << "Lucky! That's < " << needSpace << std::endl;
        return;
    }
    uint32_t needMore = needSpace - avail;
    smDir = root->filesTotal - 1;
    std::cout << "Need to delete enough to free up " << needMore << std::endl;

    // Find the smallest directory whose size > needMore
    findSmallest(root, needMore);
    std::cout << "Smallest directory > " << needMore << " is " << smDir << std::endl;
}

//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    std::cout << std::endl;
    //-------------------------------------------------------------------------

    loadInputs();

    total100s = 0;
    std::cout << "Processing Part 1..." << std::endl;
    part_1(root);
    std::cout << "Sum of all directories whose size <= 100,000 : " << total100s << std::endl;

    std::cout << std::endl;
    std::cout << "Processing Part 2..." << std::endl;
    part_2();

    //-------------------------------------------------------------------------

    std::cout << std::endl;
    return EXIT_SUCCESS;
}

//-----------------------------------------------------------------------------
