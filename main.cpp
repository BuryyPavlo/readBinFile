#include <assert.h>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

typedef char byte;

template<typename T>
byte* as_bytes(T* ptr) {
    return reinterpret_cast<byte*>(ptr);
}

int main(){
    string
            wayInFile= "//home/buryi/NDU24.BIN",
            wayOutFile = "//home//buryi//third.txt";

    float  r, p,y;
    double lon,lat;
    float height;

    std::ifstream inFile(wayInFile.c_str(), ifstream::binary);
    inFile.seekg(18, inFile.beg); //зсув на повідомлення
    inFile.seekg(5, inFile.cur);  // зсув резерв
    inFile.read(as_bytes(&r), sizeof(r));
    inFile.read(as_bytes(&p), sizeof(p));
    inFile.read(as_bytes(&y), sizeof(y));
    inFile.seekg(18+31, inFile.beg);  // зсув резерв
    inFile.read(as_bytes(&lat), sizeof(lat));
    inFile.read(as_bytes(&lon), sizeof(lon));
    inFile.read(as_bytes(&height), sizeof(height));
    std::cout << r << std::endl;
    std::cout << p << std::endl;
    std::cout << y << std::endl;


    ofstream outFile(wayOutFile.c_str());


    outFile << r << " ";
    outFile << p << " ";
    outFile << y << " ";

    outFile /*<< std::fixed*/ <<lat << " ";
    outFile /*<< std::fixed*/ <<lon << " ";
    outFile /*<< std::fixed*/ <<height << " ";

    outFile.close();
    inFile.close();
}
