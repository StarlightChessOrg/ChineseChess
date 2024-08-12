/*
eleeye.cpp - Source Code for ElephantEye, Part IX

ElephantEye - a Chinese Chess Program (UCCI Engine)
Designed by Morning Yellow, Version: 3.3, Last Modified: Mar. 2012
Copyright (C) 2004-2012 www.xqbase.com

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <stdio.h>
#include "base2.h"
#include "parse.h"
#include "ucci.h"
#include "pregen.h"
#include "position.h"
#include "hash.h"
#include "search.h"

#include <iostream>
#include <random>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <io.h>
using namespace std;

void getFiles(string path, vector<string>& files)
{
    intptr_t hFile = 0;
    struct _finddata_t fileinfo{};
    string p;
    if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
    {
        do
        {
            if ((fileinfo.attrib &  _A_SUBDIR))
            {
                if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
                    getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
            }
            else
            {
                files.push_back(p.assign(path).append("\\").append(fileinfo.name));
            }
        } while (_findnext(hFile, &fileinfo) == 0);
        _findclose(hFile);
    }
}


int main(void) {
	PreGenInit();

    string rootPath = "E:\\Projects_chess\\dump_2";
    vector<string> filePaths;
    getFiles(rootPath,filePaths);
    cout<<filePaths.size()<<endl;
    int cnt = 0;
    for(const string& path : filePaths){
        cnt += 1;
        Search.pos.FromFen(cszStartFen);
        Search.pos.nDistance = 0;

        ifstream in(path);
        string moveStr;
        //vector<string> moveStrVec;
        vector<string> dumps;
        while(getline(in,moveStr)) {
            Search.pos.PreEvaluate();
            int vlMaterial = Search.pos.Material();
            int vlAdvisorShape = Search.pos.AdvisorShape();
            int vlStringHold = Search.pos.StringHold();
            int vlKnightTrap = Search.pos.KnightTrap();
            int vlRookMobility = Search.pos.RookMobility();

            string vlStringMaterial = to_string(vlMaterial);
            string vlStringAdvisorShape = to_string(vlAdvisorShape);
            string vlStringStringHold = to_string(vlStringHold);
            string vlStringKnightTrap = to_string(vlKnightTrap);
            string vlStringRookMobility = to_string(vlRookMobility);
            const int combinatePos = atoi(moveStr.c_str());
            string mv = to_string(combinatePos);
            string scores = vlStringMaterial + " ";
            scores += vlStringAdvisorShape + " ";
            scores += vlStringStringHold + " ";
            scores += vlStringKnightTrap + " ";
            scores += vlStringRookMobility;
            string output = mv + " " + scores;
            dumps.push_back(output);

            if(!Search.pos.MakeMove(combinatePos)){
                break;
            }
        }
        string filename = to_string(cnt) + ".txt";
        string filepath = "E:\\Projects_chess\\dump_3\\" + filename;
        ofstream outfile;
        outfile.open(filepath);
        for(string& output : dumps){
            outfile<<output<<endl;
        }
        outfile.close();
    }
}
