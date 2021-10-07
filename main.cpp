#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <random>
#include "sha256.h"
#include "md5.h"

using namespace std;
using hrClock = chrono::high_resolution_clock;

// konvertavimas i n-taine sistema
string to_nBase(int num, int n){
    string el = "0123456789abcdef";
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
        for(int i=0; i<8-liek.size(); ++i){
            fnum += "0";
        }
    }
    for (int j=z-1; j>=0; j--)
        fnum += el[liek[j]];
    return fnum;
}

// hash funkcijos
void hashfunc(string input, string &hex_val, vector<string> &bi_value){
    int tarp=0;
    hex_val="";
    int a=0;
    int n=input.length();
    if(input==""){
        while(hex_val.length()<64){
            for(int i=0; i<1; ++i){
                tarp = (tarp + a)%256;
                a += 50;
            }
            hex_val += to_nBase(tarp, 16);
            bi_value.push_back(to_nBase(tarp, 2));
        }
    }
    else{
        a = ((int(input[0])+256)/2)%256;
        while(hex_val.length()<64){
            for(int i=0; i<n; ++i){
                tarp = (a + i*(n-i) + i)%256;
                a += (int(input[i])+256)%256;
            }
            tarp = abs(tarp);
            hex_val += to_nBase(tarp, 16);
            bi_value.push_back(to_nBase(tarp, 2));
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
    int n=input.length();
    if(input==""){
        while(hash_val.length()<64){
            for(int i=0; i<1; ++i){
                tarp = (tarp + a)%256;
                a += 50;
            }
            hash_val += to_nBase(tarp, 16);
        }
    }
    else{
        a = ((int(input[0])+256)/2)%256;
        while(hash_val.length()<64){
            for(int i=0; i<n; ++i){
                tarp = (a + i*(n-i) + i)%256;
                a += (int(input[i])+256)%256;
            }
            tarp = abs(tarp);
            hash_val += to_nBase(tarp, 16);
        }
    }
    if(hash_val.length()>64){
        hash_val.pop_back();
    }
    // cout << "#" << hash_val << " hash length: " << hash_val.length() << endl;
    return hash_val;
}

// duomenu ivesties budai
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

// doumenu isvestis
void writeHash(vector<string> hashes){
    ofstream r("hashedInput.txt");
    for(vector<string>::iterator it=hashes.begin(); it!=hashes.end(); ++it){
        r << "#" << *it << endl;
    }
    cout << "Hashes saved to file \'hashedInput.txt\'" << endl;
    r.close();
}

// dvieju hash lyginimas hex ir binary lygmenyse
void difference(string hash1, string hash2, double &sum){
    int dif=0;
    for(int i=0; i<64; ++i){
        if(hash1[i]==hash2[i])
            dif++;
    }
    sum = (double)(64-dif)*100/64;
}
void difference(vector<string> hash1, vector<string> hash2, double &sum){
    int dif=0;
    for(int i=0; i<32; ++i){
        for(int j=0; j<8; ++j){
            if(hash1[i][j]==hash2[i][j])
            dif++;
        }
    }
    sum = (double)((32*8)-dif)*100/(32*8);
}

// atliekami testai su failais arba ivestinimi is konsoles
void test_case(string fileName1, string fileName2){
    string file1, file2;
    string hex_val1, hex_val2;
    vector<string> bi_val1, bi_val2;

    file1 = fileInput(fileName1);
    file2 = fileInput(fileName2);

    hashfunc(file1, hex_val1, bi_val1);
    hashfunc(file2, hex_val2, bi_val2);

    cout << "\n#" << hex_val1 << endl;
    cout << "#" << hex_val2 << endl;

    double hex_dif=0;
    difference(hex_val1, hex_val2, hex_dif);
    cout << "\nHash hex difference: " << fixed << setprecision(2) << hex_dif << "%" << endl;
    
    double bi_dif=0;
    difference(bi_val1, bi_val2, bi_dif);
    cout << "Hash binary difference: " << fixed << setprecision(2) << bi_dif << "%" << endl;
}
void test_case_console(string input1, string input2){
    string hex_val1, hex_val2;
    vector<string> bi_val1, bi_val2;

    hashfunc(input1, hex_val1, bi_val1);
    hashfunc(input2, hex_val2, bi_val2);

    cout << "#" << hex_val1 << endl;
    cout << "#" << hex_val2 << endl;

    double hex_dif=0;
    difference(hex_val1, hex_val2, hex_dif);
    cout << "\nHash hex difference: " << fixed << setprecision(2) << hex_dif << "%" << endl;
    
    double bi_dif=0;
    difference(bi_val1, bi_val2, bi_dif);
    cout << "Hash binary difference: " << fixed << setprecision(2) << bi_dif << "%" << endl; 
}

// atsitiktiniu skaiciu generavimo funkcija
double myRandom(){
    static mt19937 mt(static_cast<long unsigned int>(hrClock::now().time_since_epoch().count()));
    static uniform_int_distribution<int> dist(0, 255);
    return dist(mt);
}

// atsitiktiniu simboliu eiluciu generavimas
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

// paprasti testai
void tests(){
    cout << "Choose a test:\n[1] Comparison of two hashes from single-symbol containing files;\n[2] Comparison of hashes from two randomly generated files, containing >1000 symbols;\n[3] Comparison of hashes from two files, containing >1000 symbols, when files differ by only one symbol;\n[4] Empty file;" << endl;
    int test;
    string file;
    cin >> test;
    switch(test){
        case 1:
            test_case("test_files/test1a.txt", "test_files/test1b.txt");
            break;
        case 2:
            symbolGenerator();
            test_case("test_files/randomFile1.txt", "test_files/randomFile2.txt");
            break;
        case 3:
            test_case("test_files/konstitucija.txt", "test_files/konstitucija2.txt");
            break;
        case 4:
            file = fileInput("empty.txt");
            cout << "Empty file hash: #" << hashfunc(file) << endl;
    }
}

// generuojamos simboliu eiluciu poros
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
void generateSimilarPairs(vector<string> &pair1, vector<string> &pair2, int n, int len){
    string word="";
    char sym, old;
    for(int i=0; i<n; ++i){
        for(int j=0; j<len; ++j){
            word += char(myRandom());
        }
        pair1.push_back(word);
        old = word[1];
        do {
            sym = char(myRandom());
            word[1] = sym;
        } while(old == sym);
        
        pair2.push_back(word);
        word="";
    }
}

// ieskoma koliziju
void collision(){
    vector<string> pair1;
    vector<string> pair2;
    pair1.reserve(100000);
    pair2.reserve(100000);
    
    string hex_val1;
    string hex_val2;
    vector<string> bi_val1;
    vector<string> bi_val2;

    cout << "Generating 10-symbol pairs..." << endl;
    generatePairs(pair1, pair2, 25000, 10);
    cout << "Generating 100-symbol pairs..." << endl;
    generatePairs(pair1, pair2, 25000, 100);
    cout << "Generating 500-symbol pairs..." << endl;
    generatePairs(pair1, pair2, 25000, 500);
    cout << "Generating 1000-symbol pairs..." << endl;
    generatePairs(pair1, pair2, 25000, 1000);

    int dif=0;
    cout << "Generating and comparing hashes..." << endl;
    for(int i=0; i<100000; ++i){
        hashfunc(pair1[i], hex_val1, bi_val1);
        hashfunc(pair2[i], hex_val2, bi_val2);

        if(hex_val1 == hex_val2){
            dif++;
            // cout << pair1[i] << "\n\n" << pair2[i] << "\n\n";
        }
        hex_val1="";
        hex_val2="";
        bi_val1.clear();
        bi_val2.clear();
    }
    cout << "\nAmount of collisions: " << dif << endl;
}

// tikrinamas input'u skirtingumas
void similarity(){
    vector<string> pair1;
    vector<string> pair2;
    pair1.reserve(100000);
    pair2.reserve(100000);
    
    string hex_val1;
    string hex_val2;
    vector<string> bi_val1;
    vector<string> bi_val2;

    cout << "Generating 10-symbol pairs..." << endl;
    generateSimilarPairs(pair1, pair2, 25000, 10);
    cout << "Generating 100-symbol pairs..." << endl;
    generateSimilarPairs(pair1, pair2, 25000, 100);
    cout << "Generating 500-symbol pairs..." << endl;
    generateSimilarPairs(pair1, pair2, 25000, 500);
    cout << "Generating 1000-symbol pairs..." << endl;
    generateSimilarPairs(pair1, pair2, 25000, 1000);

    double hex_dif, bi_dif;
    double hex_dif_sum=0, bi_dif_sum=0;
    double min_val=100, max_val=0;
    double min_bi_val=100, max_bi_val=0;

    cout << "Generating and comparing hashes...\n\n";
    for(int i=0; i<100000; ++i){
        hashfunc(pair1[i], hex_val1, bi_val1);
        hashfunc(pair2[i], hex_val2, bi_val2);

        difference(hex_val1, hex_val2, hex_dif);
        difference(bi_val1, bi_val2, bi_dif);
        hex_dif_sum += hex_dif;
        bi_dif_sum += bi_dif;

        if(hex_dif>max_val) max_val=hex_dif;
        if(hex_dif<min_val) min_val=hex_dif;

        if(bi_dif>max_bi_val) max_bi_val=bi_dif;
        if(bi_dif<min_bi_val) min_bi_val=bi_dif;

        hex_val1="";
        hex_val2="";
        bi_val1.clear();
        bi_val2.clear();
    }
    cout << setw(25) << left << "Mean hex difference: " <<  fixed << setprecision(2) << hex_dif_sum/100000 << "%" << endl;
    cout << setw(25) << left << "Min hex difference: " <<  fixed << setprecision(2) << min_val << "%" << endl;
    cout << setw(25) << left << "Max hex difference: " <<  fixed << setprecision(2) << max_val << "%\n" << endl;
    cout << setw(25) << left << "Mean binary difference: " <<  fixed << setprecision(2) << bi_dif_sum/100000 << "%" << endl;
    cout << setw(25) << left << "Min binary difference: " <<  fixed << setprecision(2) << min_bi_val << "%" << endl;
    cout << setw(25) << left << "Max binary difference: " <<  fixed << setprecision(2) << max_bi_val << "%" << endl;
}

// laiko skaiciavimo funkcija
void time_taken(string input, string (*func)(string)){
    auto pr = chrono::high_resolution_clock::now();
    cout << func(input) << endl;
    auto pab = chrono::high_resolution_clock::now();
    double time_taken = chrono::duration_cast<chrono::nanoseconds>(pab - pr).count();
    time_taken *= 1e-9;
    cout << "Input hashed in: ";
    cout << time_taken << " s" << endl;
}

// hash generavimo funkciju palyginimas
void comp_test(vector<string> pair1, vector<string> pair2, string (*func)(string)){
    string hex_val1, hex_val2;

    double hex_dif;
    double hex_dif_sum=0;
    double min_val=100, max_val=0;

    // cout << "Generating and comparing hashes..." << endl;
    for(int i=0; i<100000; ++i){
        hex_val1 = func(pair1[i]);
        hex_val2 = func(pair2[i]);
        difference(hex_val1, hex_val2, hex_dif);
        hex_dif_sum += hex_dif;
        if(hex_dif>max_val) max_val=hex_dif;
        if(hex_dif<min_val) min_val=hex_dif;
        // if(hex_dif == 25) cout << pair1[i] << "\n" << hex_val1 << "\n" << pair2[i] << "\n" << hex_val2 << "\n";
        hex_val1="";
        hex_val2="";
    }
    cout << "\nHash hex difference: " <<  fixed << setprecision(2) << hex_dif_sum/100000 << "%" << endl;
    cout << "Min hex difference: " <<  fixed << setprecision(2) << min_val << "%" << endl;
    cout << "Max difference: " <<  fixed << setprecision(2) << max_val << "%" << endl;
}

// vieno input'o hash'avimas
void onlyHash(){
    int inputType;
    string fileName, input;
    cout << "Choose data input:\n[1] file\n[2] console" << endl;
    cin >> inputType;
    switch(inputType){
        case 1:
            cout << "File title: ";
            cin >> fileName;
            input = fileInput("test_files/"+fileName+".txt");
            cout << "\nHash value: #";
            time_taken(input, hashfunc);
            break;
        case 2:
            input = manualInput();
            cout << "\nHash value: #";
            time_taken(input, hashfunc);
            break;
    }
}

// dvieju input'u hash'u palyginimas
void hashCompare(){
    int inputType;
    string fileName1, fileName2;
    string input1, input2;
    double sum=0;
    cout << "Choose file input:\n[1] file\n[2] console" << endl;
    cin >> inputType;
    switch(inputType){
        case 1:
            cout << "Title of first file: ";
            cin >> fileName1;
            cout << "Title of second file: ";
            cin >> fileName2;
            test_case("test_files/"+fileName1+".txt", "test_files/"+fileName2+".txt");
            break;
        case 2:
            input1 = manualInput();
            input2 = manualInput();
            test_case_console(input1, input2);
            break;
    }
}

// hash generavimo funkciju palyginimo testai
void comparison(){
    int wha, what;
    cout << "What to compare:\n[1] hashing speed with single input\n[2] hash difference\n";
    cin >> wha;
    string input, fileName;
    if (wha==1){
        cout << "Input from:\n[1] file\n[2] console" << endl;
        cin >> what;
        switch(what){
            case 1:
                cout << "File title: ";
                cin >> fileName;
                input = fileInput("test_files/"+fileName+".txt");
                break;
            case 2:
                input = manualInput();
                break;
        }
        cout << "\nsha256:";
        time_taken(input, sha256);
        cout << "\nmd5:   ";
        time_taken(input, md5);
        cout << "\nmyHash:";
        time_taken(input, hashfunc);
    }
    else{
        vector<string> pair1;
        vector<string> pair2;
        pair1.reserve(100000);
        pair2.reserve(100000);

        string myHash1, myHash2;
        string sha1, sha2;
        string md51, md52;

        cout << "Generating 10-symbol pairs..." << endl;
        generateSimilarPairs(pair1, pair2, 25000, 10);
        cout << "Generating 100-symbol pairs..." << endl;
        generateSimilarPairs(pair1, pair2, 25000, 100);
        cout << "Generating 500-symbol pairs..." << endl;
        generateSimilarPairs(pair1, pair2, 25000, 500);
        cout << "Generating 1000-symbol pairs...\n" << endl;
        generateSimilarPairs(pair1, pair2, 25000, 1000);

        cout << "sha256:\n";
        comp_test(pair1, pair2, sha256);
        cout << "\nmd5:\n";
        comp_test(pair1, pair2, md5);
        cout << "\nmyHash:\n";
        comp_test(pair1, pair2, hashfunc);
    }
}

int main(){
    int whatToDo;
    cout << "\nChoose what to do:\n[1] Simple tests with data from files;\n[2] Search of collisions;\n[3] Individual input hashing;\n[4] Comparison of hashes from different inputs;\n[5] Test hash differences of 100\'000 similar string pairs;\n[6] Comparison with sha256 and md5;" << endl;
    cin >> whatToDo;
    string input;
    switch(whatToDo){
        case 1:
            tests();
            break;
        case 2:
            collision();
            break;
        case 3:
            onlyHash();
            break;
        case 4:
            hashCompare();
            break;
        case 5:
            similarity();
            break;
        case 6:
            comparison();
            break;
    }
    return 0; 
}