#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>

#include "GraphTheory.h"    https://github.com/JamesBremner/PathFinder

raven::graph::sGraphData gd;

std::vector<std::string> theStations;
std::vector < std::pair<int, int>> theLines;

void displayLines()
{
    for( auto& p : theLines )
        std::cout << p.first <<" "<< p.second << ", ";
    std::cout << "\n";
}

void parseName( const std::string& line )
{
    int vi = atoi( line.c_str());
    std::string name = line.substr(5);
    theStations.push_back( name );
}

void parselines(const std::vector<std::string> &vtoken)
{
    int s = atoi(vtoken[0].c_str());
    int t = atoi(vtoken[1].c_str());
    if (s > t)
        return;
    theLines.push_back(std::make_pair(s, t));
}

void parselink(const std::vector<std::string> &vtoken)
{
    int ei = gd.g.add( vtoken[0],vtoken[1] );
    if( ei >= gd.edgeWeight.size() )
        gd.edgeWeight.resize(1 + 2*ei );
    int w = atoi( vtoken[2].c_str());
    if( w == -1 )
        w = 90;
    gd.edgeWeight[ei] = w;
}

void displayEdges()
{
    for( auto& e : gd.g.edgeList() )
        std::cout << "l "<< e.first <<" "<< e.second 
        << " " << gd.edgeWeight[gd.g.find( e.first,e.second)] << "\n";
}

void readfile()
{
    enum class eInput
    {
        head,
        names,
        links,
        lines
    };
    eInput state = eInput::head;

    std::ifstream ifs("../dat/metro.txt");
    if (!ifs.is_open())
        throw std::runtime_error("Cannot open input file");
    std::string line;

    while (getline(ifs, line))
    {
        std::vector<std::string> vtoken;
        std::stringstream sst(line);
        std::string a;
        while (getline(sst, a, ' '))
            if (a.length())
                vtoken.push_back(a);


        switch (state)
        {

        case eInput::head:
            state = eInput::names;
            break;

        case eInput::names:
            if (line[0] == '$') {
                state = eInput::links;
                break;
            }
            parseName(line);
            break;

        case eInput::links:
            // if (vtoken[2] == "-1")
            // {
            //     state = eInput::lines;
            //     parselines(vtoken);
            //     break;
            // }
            parselink( vtoken);
            break;

        case eInput::lines:
            parselines(vtoken);
            break;
        }
    }
}
/// @brief Find shotest path between stations
/// @param start index of start station
/// @param end index of end station
/// Use Dijsktra algorithm implemented by the PathFinder library https://github.com/JamesBremner/PathFinder

void ShortestPath( int start, int end )
{
    gd.startName = std::to_string( start );
    gd.endName = std::to_string( end );
    auto p = path( gd );
    for( int vi : p.first )
        std::cout << theStations[vi] << " -> ";
}

main()
{
    readfile();
    // displayLines();
    // displayEdges();
    ShortestPath(1,12);
    return 0;
}
