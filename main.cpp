#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <random>

using namespace std;
using hrClock = chrono::high_resolution_clock;

string to_nBase(int num, int n){
    string el = "0123456789ABCDEF";
    vector<int> liek;
    string fnum="";
    int z=0;
    int dal;
    do{
        dal = num/n;
        liek.push_back(num%n);
        num = dal;
        z++;
    } while(num>0);

    if(n==2 && liek.size()<8){
        // cout << "liekana: " << liek.size();
        for(int i=0; i<8-liek.size(); ++i){
            fnum += "0";
        }
    }
    for (int j=z-1; j>=0; j--)
        fnum += el[liek[j]];
    // cout << " " << fnum << endl;
    return fnum;
}

void hashfunc(string input, string &hex_val, vector<string> &bi_value){
    int tarp=0;
    hex_val="";
    int a=0;
    int b = 77;
    if(input==""){
        while(hex_val.length()<64){
            for(int i=0; i<1; ++i){
                tarp = (tarp + a)%256;
                a += 500/3-77;
            }
            hex_val += to_nBase(tarp, 16);
            bi_value.push_back(to_nBase(tarp, 2));
        }
    }
    else{
        a = int(input[0])/2;
        while(hex_val.length()<64){
            for(int i=0; i<input.length(); ++i){
                tarp = (tarp + a + int(input[i]))%256;
                a += (int(input[i])+500)/3-77;
            }
            hex_val += to_nBase(tarp, 16);
            bi_value.push_back(to_nBase(tarp, 2));
            // cout << tarp << " " << bi_value.back() << endl;
        }
    }
    if(hex_val.length()>64){
        hex_val.pop_back();
    }
    if(bi_value.size()>64){
        bi_value.pop_back();
    }
    // cout << "#" << hash_val << " hash length: " << hash_val.length() << endl;
    // return hash_val;
}
string hashfunc(string input){
    string hash_val="";
    int tarp=0;
    int a=0;
    int b = 77;
    if(input==""){
        while(hash_val.length()<64){
            for(int i=0; i<1; ++i){
                tarp = (tarp + a)%256;
                a += 500/3-77;
            }
            hash_val += to_nBase(tarp, 16);
        }
    }
    else{
        a = int(input[0])/2;
        while(hash_val.length()<64){
            for(int i=0; i<input.length(); ++i){
                tarp = (tarp + a + int(input[i]))%256;
                a += (int(input[i])+500)/3-77;
            }
            hash_val += to_nBase(tarp, 16);
        }
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
string fileInput(string fileName){
    ifstream d(fileName);
    stringstream buffer;
    buffer << d.rdbuf();
    return buffer.str();
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

void difference(string hash1, string hash2, double &sum){
    int dif=0;
    for(int i=0; i<64; ++i){
        if(hash1[i]==hash2[i])
            dif++;
    }
    sum += (64-dif)*100/64;
    // cout << "Skirtingumas: " << ((64-dif)*100/64) << " proc." << endl;
}

void difference(vector<string> hash1, vector<string> hash2, double &sum){
    int dif=0;
    for(int i=0; i<32; ++i){
        for(int j=0; j<8; ++j){
            if(hash1[i][j]==hash2[i][j])
            dif++;
        }
    }
    sum += ((32*8)-dif)*100/(32*8);
    // cout << (((32*8)-dif)*100/(32*8)) << " proc." << endl;
}

void test_case(string fileName1, string fileName2){
    string file1, file2;
    string hex_val1, hex_val2;
    vector<string> bi_val1, bi_val2;

    file1 = fileInput(fileName1);
    file2 = fileInput(fileName2);

    hashfunc(file1, hex_val1, bi_val1);
    hashfunc(file2, hex_val2, bi_val2);

    double hex_dif=0;
    difference(hex_val1, hex_val2, hex_dif);
    cout << "Skirtingumas hex lygmenyje: " << hex_dif;
    
    double bi_dif=0;
    difference(bi_val1, bi_val2, bi_dif);
    cout << "Skirtingumas binary lygmenyje: " << bi_dif << endl;
    
}

double myRandom(){
    static mt19937 mt(static_cast<long unsigned int>(hrClock::now().time_since_epoch().count()));
    static uniform_int_distribution<int> dist(0, 255);
    return dist(mt);
}

void symbolGenerator(){
    ofstream r1("randomFile1.txt");
    ofstream r2("randomFile2.txt");
    for(int i=0; i<1500; ++i){
        r1 << char(myRandom());
        r2 << char(myRandom());
    }
    r1.close();
    r2.close();
}

void tests(){
    cout << "Pasirinkite testa:\n[1] dvieju skirtingu vieno simbolio failu hashu palyginimas\n[2] dvieju skirtingu random failu is >1000 simboliu hashu palyginimas\n[3] dvieju failu is >1000 simboliu hashu palyginimas, kai failai skiriasi tik 1 simboliu" << endl;
    int test;
    cin >> test;
    switch(test){
        case 1:
            test_case("test1a.txt", "test1b.txt");
            break;
        case 2:
            symbolGenerator();
            test_case("randomFile1.txt", "randomFile2.txt");
            break;
        case 3:
            test_case("konstitucija.txt", "konstitucija2.txt");
            break;
    }
}

void konst(){
    vector<string> lines;
    vector<string> hashes;

    fileInput("konstitucija.txt", lines);

    auto pr = chrono::high_resolution_clock::now();
    for(vector<string>::iterator it=lines.begin(); it!=lines.end(); ++it){
        hashes.push_back(hashfunc((*it)));
    }

    auto pab = chrono::high_resolution_clock::now();
    double time_taken = chrono::duration_cast<chrono::nanoseconds>(pab - pr).count();
    time_taken *= 1e-9;
    cout << setw(50) << left << "konstitucijos hash'avimas uztruko: ";
    cout << time_taken << " s" << endl;
}
void generatePairs(vector<string> &pair1, vector<string> &pair2, int n, int len){
    string word="";
    for(int i=0; i<n; ++i){
        for(int j=0; j<len; ++j){
            word += char(myRandom());
        }
        pair1.push_back(word);
        word="";
        for(int j=0; j<len; ++j){
            word += char(myRandom());
        }
        pair2.push_back(word);
        word="";
    }
}

void collision(){
    vector<string> pair1;
    vector<string> pair2;
    pair1.reserve(100000);
    pair2.reserve(100000);
    
    string hex_val1;
    string hex_val2;
    vector<string> bi_val1;
    vector<string> bi_val2;

    cout << "Generuojamos 10 simboliu poros..." << endl;
    generatePairs(pair1, pair2, 25000, 10);
    cout << "Generuojamos 100 simboliu poros..." << endl;
    generatePairs(pair1, pair2, 25000, 100);
    cout << "Generuojamos 500 simboliu poros..." << endl;
    generatePairs(pair1, pair2, 25000, 500);
    cout << "Generuojamos 1000 simboliu poros..." << endl;
    generatePairs(pair1, pair2, 25000, 1000);

    double hex_dif_sum=0;
    // double bi_dif_sum=0;

    int dif=0;
    cout << "Generuojami ir lyginami hash\'ai..." << endl;
    for(int i=0; i<100000; ++i){
        hashfunc(pair1[i], hex_val1, bi_val1);
        hashfunc(pair2[i], hex_val2, bi_val2);

        if(hex_val1 == hex_val2){
            dif++;
            // cout << pair1[i] << " " << pair2[i] << " " << hex_val1 << " " << hex_val2 << endl;
        }
        hex_val1="";
        hex_val2="";
        bi_val1.clear();
        bi_val2.clear();
    }
    cout << "Koliziju kiekis: " << dif << endl;
}

int main(){
    int whatToDo;
    cout << "Pasirinkite, ka norite daryti:\n[1] Paprasti testai is failu\n[2] Konstitucijos testas\n[3] Koliziju paieska" << endl;
    cin >> whatToDo;
    string input;
    switch(whatToDo){
        case 1:
            tests();
            break;
        case 2:
            konst();
            break;
        case 3:
            collision();
            break;
        case 4:
            input = fileInput("empty.txt");
            cout << hashfunc(input) << endl;
            break;
    }
    // int inputType;
    // string fileName;
    // vector<string> lines;
    // vector<string> hashes;
    // cout << "Duomenu ivestis:\n[1] is failo\n[2] is konsoles" << endl;
    // cin >> inputType;
    // switch(inputType){
    //     case 1:
    //         cout << "Failo pavadinimas: ";
    //         cin >> fileName;
    //         fileInput(fileName+".txt", lines);
    //         break;
    //     case 2:
    //         manualInput();
    //         break;
    // }
    // cout << "nuskaityta" << endl;
    // auto pr = chrono::high_resolution_clock::now();
    // for(vector<string>::iterator it=lines.begin(); it!=lines.end(); ++it){
    //     hashes.push_back(hashfunc((*it)));
    // }

    // auto pab = chrono::high_resolution_clock::now();
    // double time_taken = chrono::duration_cast<chrono::nanoseconds>(pab - pr).count();
    // time_taken *= 1e-9;
    // cout << setw(50) << left << "konstitucijos hash'avimas uztruko: ";
    // cout << time_taken << " s" << endl;

    // writeHash(hashes);

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