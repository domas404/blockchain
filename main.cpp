#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>

using namespace std;
using hrClock = chrono::high_resolution_clock;

string toHex(int num){
    string el = "0123456789ABCDEF";
    vector<int> liek;
    string fnum;
    int z=0;
    int dal;
    do{
        dal = num/16;
        liek.push_back(num%16);
        num = dal;
        z++;
    } while(num>0);

    for (int j=z-1; j>=0; j--)
        fnum += el[liek[j]];

    return fnum;
}

string hashfunc(string input, int n){
    string hash_val="";
    int tarp=0;
    // if(input==""){
    //     tarp=-1;
    // }
    int a = int(input[0])/2;
    while(hash_val.length()<64){
        for(int i=0; i<n; ++i){
            tarp = (tarp + a + int(input[i]))%256;
            a += (int(input[i])+500)/3-77;
        }
        hash_val += toHex(tarp);
    }
    if(hash_val.length()>64){
        hash_val.pop_back();
    }
    // cout << "#" << hash_val << " hash length: " << hash_val.length() << endl;
    return hash_val;
}

void fileInput(string fileName, vector<string> &lines){
    ifstream d(fileName);
    string line;
    while(!d.eof()){
        getline(d, line);
        lines.push_back(line);
    }
    d.close();
}
string manualInput(){
    string input;
    cout << "Input text: ";
    cin >> input;
    return input;
}

void writeHash(vector<string> hashes){
    ofstream r("hashedInput.txt");
    for(vector<string>::iterator it=hashes.begin(); it!=hashes.end(); ++it){
        r << "#" << *it << endl;
    }
    cout << "Hash'ai issaugoti faile \'hashedInput.txt\'" << endl;
    r.close();
}

int main(){
    int inputType;
    string fileName;
    vector<string> lines;
    vector<string> hashes;
    cout << "Duomenu ivestis:\n[1] is failo\n[2] is konsoles" << endl;
    cin >> inputType;
    switch(inputType){
        case 1:
            cout << "Failo pavadinimas: ";
            cin >> fileName;
            fileInput(fileName+".txt", lines);
            break;
        case 2:
            manualInput();
            break;
    }
    cout << "nuskaityta" << endl;
    auto pr = chrono::high_resolution_clock::now();
    for(vector<string>::iterator it=lines.begin(); it!=lines.end(); ++it){
        hashes.push_back(hashfunc((*it), (*it).length()));
    }

    auto pab = chrono::high_resolution_clock::now();
    double time_taken = chrono::duration_cast<chrono::nanoseconds>(pab - pr).count();
    time_taken *= 1e-9;
    cout << setw(50) << left << "konstitucijos hash'avimas uztruko: ";
    cout << time_taken << " s" << endl;

    writeHash(hashes);

    // int inputType;
    // string input, input2;
    // string file_input, file2_input;
    // cout << "Duomenis ivesti: \n[1] is failo\n[2] per command line\n";
    // cin >> inputType;
    // if (inputType == 1){
        // file_input = fileInput("input.txt");
        // file2_input = fileInput("konstitucija2.txt");
    // }
    // else if (inputType == 2){
        // input = manualInput();
        // input2 = manualInput();
    // }
    // string hash1 = hashfunc(input, input.length());
    // string hash2 = hashfunc(input2, input2.length());
    // string hash3 = hashfunc(file_input, file_input.length());
    // string hash4 = hashfunc(file2_input, file2_input.length());
    // int dif=0;
    // for(int i=0; i<64; ++i){
    //     if(hash1[i]==hash2[i])
    //         dif++;
    // }
    // cout << "Skirtingumas: " << ((64-dif)*100/64) << " proc." << endl;
    return 0; 
}