#include <assert.h>
#include <fstream>
#include <iostream>
#include <string>
#include <iomanip>

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


    ifstream inFile(wayInFile.c_str(), ifstream::binary);
    ofstream outFile(wayOutFile.c_str());
    while (inFile)
    {
        unsigned char synchroBits1;
        unsigned char synchroBits2;
        unsigned char synchroBits3;
        // reading the header

        inFile.read((char*)&synchroBits1, 1);
        if (synchroBits1 == 0xFA) {
            inFile.read((char*)&synchroBits2, 1);
            if (synchroBits2 == 0x3C){
                inFile.read((char*)&synchroBits3, 1);
                if(synchroBits3 == 0x5F){

                    std::cout << std::hex << (int)synchroBits1 << " " << std::hex << (int)synchroBits2 << " " << std::hex << (int)synchroBits3 << "\n";

                    unsigned short typeOfMessage = 0;
                    unsigned short sizeOfMessage = 0;

                    inFile.read((char*)&typeOfMessage, 2);
                    inFile.read((char*)&sizeOfMessage, 2);
                    inFile.seekg(11, inFile.cur);

                    if (typeOfMessage == 6){
                        std::cout << typeOfMessage <<"\n";
                        float  r, p,y;
                        double lon,lat;
                        float height;

                        inFile.seekg(5, inFile.cur);  // зсув резерв
                        inFile.read(as_bytes(&r), sizeof(r));
                        inFile.read(as_bytes(&p), sizeof(p));
                        inFile.read(as_bytes(&y), sizeof(y));
                        inFile.seekg(14, inFile.cur);  // зсув
                        inFile.read(as_bytes(&lat), sizeof(lat));
                        inFile.read(as_bytes(&lon), sizeof(lon));
                        inFile.read(as_bytes(&height), sizeof(height));

                        cout << p << endl;
                        cout << r << endl;
                        cout << y << endl;

                        outFile << r << " ";
                        outFile << p << " ";
                        outFile << y << " ";

                        outFile << std::fixed <<lat << " ";
                        outFile << std::fixed <<lon << " ";
                        outFile << std::fixed <<height << " ";
                        outFile <<"\n";
                        inFile.seekg(36 + 4, inFile.cur);// перерахувати
                    } else {

                    // correct comment must be here
                    inFile.seekg(sizeOfMessage + 4, inFile.cur);}
                }
            }
        }

    }
    outFile.close();
    inFile.close();
    return 0;
}
