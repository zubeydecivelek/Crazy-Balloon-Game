#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

void showGrid(vector<vector<int>> &grid){
    for (const vector<int>& i : grid){
        for(int j : i){
            cout<<j<<" ";
        }
        cout<<"\n";
    }
}

bool contains(const vector<int>& value,vector<vector<int>> &sameValues){
    //return find(sameValues.begin(), sameValues.end(), value) != sameValues.end();
}

int sides[8][2] = {{-1,0},{1,0},{0,-1},{0,1},{-1,-1},{-1,1},{1,-1},{1,1}}, score = 0;

void sideControl(vector<vector<int>> &grid, int gridSize, int i, int j, int value,vector<vector<int>> &sameValues){
    for (int k = 0 ; k < 4 ; k++){
        int newI = i + sides[k][0], newJ = j + sides[k][1];
        if (0<=newI && newI<gridSize && 0<=newJ && newJ<gridSize && grid[newI][newJ]==value && !contains({newI,newJ},sameValues)){
            sameValues.push_back({newI,newJ});
            sideControl(grid,gridSize,newI,newJ,value,sameValues);
        }
    }
}

void putValue(vector<vector<int>> &grid, int value, int i, int j, int gridSize){
    grid[i][j] = value;
    vector<vector<int>> sameValues;
    sameValues.push_back({i,j});
    sideControl(grid,gridSize,i,j,value, sameValues);
    if (sameValues.size()>2){
        for (vector<int> index:sameValues){
            grid[index[0]][index[1]] = 0;
        }
        putValue(grid,value+1,i,j,gridSize);
    }
}

void bombSideControl(vector<vector<int>> & bombGrid, int bombGridSize, int i, int j){
    int bombNumber = 1, bombValue = bombGrid[i][j];
    bombGrid[i][j] = 0;
    for (auto & bombSide : sides){
        int newI= i + bombSide[0], newJ = j + bombSide[1];
        while ((0<=newI && newI<bombGridSize) && (0<=newJ && newJ<bombGridSize)){
            if (bombGrid[newI][newJ]==bombValue){
                bombNumber++;
                bombGrid[newI][newJ] = 0;
            }
            newI += bombSide[0], newJ += bombSide[1];
        }
    }
    score += bombNumber*bombValue;
}

int main(int argc, char *argv[]) {
    freopen(argv[3],"w",stdout);
    fstream files;
    cout<<"PART 1:"<<endl;
    files.open(argv[1],ios::in);
    string line;
    getline(files,line); // ilk satırı oku
    int gridSize = stoi(line); //tablo boyutu için integer a çevir
    vector<vector<int>> grid(gridSize, vector<int> (gridSize, 0));
    while (getline(files,line)){
        stringstream stream(line);
        int value, i, j;
        stream >> value >> i >> j;
        putValue(grid,value,i,j,gridSize);
    }
    files.close();
    showGrid(grid);

    cout<<"\nPART 2:"<<endl;
    files.open(argv[2],ios::in);
    getline(files,line); // ilk satırı oku
    const int bombGridSize = stoi(line); //tablo boyutu için integer a çevir
    int count1 = 0, count2 = 0;
    vector<vector<int>> bombGrid(bombGridSize,vector<int>(bombGridSize,0));
    while(getline(files,line)) {      //sadece tabloyu input olarak alıyor ve oluşturuyor
        stringstream stream(line);
        if (count1<bombGridSize) {
            string x;
            while (getline(stream, x, ' ')) {
                bombGrid[count1][count2] = stoi(x);
                count2++;
                if (count2 == bombGridSize) {
                    count2 = 0;
                    break;
                }
            }
        }
        else{
            int i, j;
            stream>> i >> j;
            bombSideControl(bombGrid,bombGridSize,i,j);
        }
        count1++;
    }
    files.close();
    showGrid(bombGrid);
    cout<<"Final Point: "<<score<<"p";
    return 0;
}
