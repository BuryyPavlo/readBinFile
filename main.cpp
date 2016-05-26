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

bool checkCRC(unsigned long CRC, unsigned char *message, int messageSize){
    unsigned long calculated_CRC = 0;
    for (int position = 0; position < messageSize; position++){
        unsigned long temp_long;
        unsigned char i;
        temp_long = (calculated_CRC>>8)&0x00FFFFFFL;
        calculated_CRC=(calculated_CRC^message[position])&0xFF;
        for(i=8; i>0; i-- ){
            if(calculated_CRC&1){
                calculated_CRC=(calculated_CRC>>1)^0xEDB88320L;
            } else {
                calculated_CRC>>=1;
            }
        }
        calculated_CRC = temp_long^calculated_CRC;
    }
    if (CRC == calculated_CRC){
        return true;
    } else {
        return false;
    }
}

int main(){
    string
            wayInFile= "//home/buryi/NDU24.BIN",
            wayOutFile = "//home//buryi//third3.txt";

    ifstream inFile(wayInFile.c_str(), ifstream::binary);
    ofstream outFile(wayOutFile.c_str());
    while (inFile)
    {
        unsigned char synchroBits[3];
        unsigned char synchroBitsKey[3];
        //synchroBits
        synchroBitsKey[0] = 0xFA;
        synchroBitsKey[1] = 0x3C;
        synchroBitsKey[2] = 0x5F;
        // reading the header

        inFile.read((char*)&synchroBits[0], 1);
        if (synchroBits[0] != synchroBitsKey[0]){
            continue;
        } else {
            inFile.read((char*)&synchroBits[1], 1);
            if (synchroBits[1] == synchroBitsKey[1]){
                inFile.read((char*)&synchroBits[2], 1);
                if(synchroBits[2] == synchroBitsKey[2]){

                    unsigned short typeOfMessage = 0;
                    unsigned short sizeOfMessage = 0;

                    inFile.read((char*)&typeOfMessage, 2);
                    inFile.read((char*)&sizeOfMessage, 2);
                    int  needMessageType = 6;
                    if (typeOfMessage == needMessageType){
                        inFile.seekg(-7,inFile.cur);// зсув на початок
                        unsigned char message[87+22];
                        for(int i = 0; i<87+18+4; i++){
                            inFile.read(as_bytes(&message[i]), 1);
                        }
                        inFile.seekg(-4, inFile.cur);
                        unsigned long CRC;
                        inFile.read(as_bytes(&CRC), 4);
                        bool statusCRC;
                        statusCRC = checkCRC(CRC, message, 87+18+4);
                       // if (statusCRC == true)
                        {
                            inFile.seekg(-(87+4), inFile.cur);

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

                            int countSignAngle = 5;
                            int countSignCoordinate = 10;
                            outFile << fixed << setprecision(countSignAngle);
                            outFile << r << " ";
                            outFile << p << " ";
                            outFile <<y << " ";

                            outFile << fixed << setprecision(countSignCoordinate);
                            outFile << lat << " ";
                            outFile << lon << " ";
                            outFile << height << " ";
                            outFile <<"\n";
                            inFile.seekg(36 + 4, inFile.cur);//зсув на кінець повідомлення
                        }
                    } else {

                        // correct comment must be here
                        inFile.seekg(sizeOfMessage + 4, inFile.cur);}
                }
            }
        }
    }
    outFile.close();
    inFile.close();
    cout << "Finish work! I did it!!!"<< "\n";
    return 0;
}

