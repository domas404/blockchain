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
        for(int i=0; i<8-liek.size(); ++i){
            fnum += "0";
        }
    }
    for (int j=z-1; j>=0; j--)
        fnum += el[liek[j]];
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
    cout << "Hashes saved to file \'hashedInput.txt\'" << endl;
    r.close();
}

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

    cout << "\n" << input1 << left <<  ": #" << hex_val1 << endl;
    cout << input2 << left <<  ": #" << hex_val2 << endl;

    double hex_dif=0;
    difference(hex_val1, hex_val2, hex_dif);
    cout << "\nHash hex difference: " << fixed << setprecision(2) << hex_dif << "%" << endl;
    
    double bi_dif=0;
    difference(bi_val1, bi_val2, bi_dif);
    cout << "Hash binary difference: " << fixed << setprecision(2) << bi_dif << "%" << endl; 
}

double myRandom(){
    static mt19937 mt(static_cast<long unsigned int>(hrClock::now().time_since_epoch().count()));
    static uniform_int_distribution<int> dist(0, 255);
    return dist(mt);
}
double myRandom2(int n){
    static mt19937 mt(static_cast<long unsigned int>(hrClock::now().time_since_epoch().count()));
    static uniform_int_distribution<int> dist(0, n);
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
    cout << "Choose a test:\n[1] Comparison of two hashes from single-symbol containing files;\n[2] Comparison of hashes from two randomly generated files, containing >1000 symbols;\n[3] Comparison of hashes from two files, containing >1000 symbols, when files differ by only one symbol;\n[4] Empty file;" << endl;
    int test;
    string file;
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
        case 4:
            file = fileInput("empty.txt");
            cout << "Empty file hash: #" << hashfunc(file) << endl;
    }
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
void generateSimilarPairs(vector<string> &pair1, vector<string> &pair2, int n, int len){
    string word="";
    int a;
    for(int i=0; i<n; ++i){
        for(int j=0; j<len; ++j){
            word += char(myRandom());
        }
        pair1.push_back(word);
        a = myRandom2(len-1);
        word[a] = char(myRandom());
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
        }
        hex_val1="";
        hex_val2="";
        bi_val1.clear();
        bi_val2.clear();
    }
    cout << "\nAmount of collisions: " << dif << endl;
}
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

    cout << "Generating and comparing hashes..." << endl;
    for(int i=0; i<100000; ++i){
        hashfunc(pair1[i], hex_val1, bi_val1);
        hashfunc(pair2[i], hex_val2, bi_val2);

        difference(hex_val1, hex_val2, hex_dif);
        difference(bi_val1, bi_val2, bi_dif);
        hex_dif_sum += hex_dif;
        bi_dif_sum += bi_dif;

        if(hex_dif>max_val) max_val=hex_dif;
        if(hex_dif<min_val) min_val=hex_dif;

        hex_val1="";
        hex_val2="";
        bi_val1.clear();
        bi_val2.clear();
    }
    cout << "\nHash hex difference: " <<  fixed << setprecision(2) << hex_dif_sum/100000 << "%" << endl;
    cout << "Min hex difference: " <<  fixed << setprecision(2) << min_val << "%" << endl;
    cout << "Max difference: " <<  fixed << setprecision(2) << max_val << "%" << endl;
    cout << "Hash binary difference: " <<  fixed << setprecision(2) << bi_dif_sum/100000 << "%" << endl;
}
void time_spent(vector<string> &lines){
    auto pr = chrono::high_resolution_clock::now();
    for(vector<string>::iterator it=lines.begin(); it!=lines.end(); ++it){
        hashfunc(*it);
    }
    auto pab = chrono::high_resolution_clock::now();
    double time_taken = chrono::duration_cast<chrono::nanoseconds>(pab - pr).count();
    time_taken *= 1e-9;
    cout << "File hashed in: ";
    cout << time_taken << " s" << endl;
}
void onlyHash(){
    int inputType;
    string fileName, input;
    vector<string> lines;
    vector<string> hashes;
    cout << "Choose data input:\n[1] file (one line at a time)\n[2] file (whole)\n[3] console" << endl;
    cin >> inputType;
    switch(inputType){
        case 1:
            cout << "File title: ";
            cin >> fileName;
            fileInput(fileName+".txt", lines);
            time_spent(lines);
            break;
        case 2:
            cout << "File title: ";
            cin >> fileName;
            input = fileInput(fileName+".txt");
            cout << "Hash value: #" << hashfunc(input) << endl;
            break;
        case 3:
            input = manualInput();
            cout << "Hash value: #" << hashfunc(input) << endl;
            break;
    }
    for(vector<string>::iterator it=lines.begin(); it!=lines.end(); ++it){
        hashes.push_back(hashfunc((*it)));
    }
}
void hashCompare(){
    int inputType;
    string fileName1, fileName2;
    string input1, input2;
    double sum=0;
    cout << "Duomenu ivestis:\n[1] visas failas\n[2] is konsoles" << endl;
    cin >> inputType;
    switch(inputType){
        case 1:
            cout << "Pirmo failo pavadinimas: ";
            cin >> fileName1;
            cout << "Antro failo pavadinimas: ";
            cin >> fileName2;
            input1 = fileInput(fileName1+".txt");
            input2 = fileInput(fileName2+".txt");
            test_case(input1, input2);
            break;
        case 2:
            input1 = manualInput();
            input2 = manualInput();
            test_case_console(input1, input2);
            break;
    }
}
int main(){
    int whatToDo;
    cout << "\nChoose what to do:\n[1] Simple tests with data from files;\n[2] Search of collisions;\n[3] Individual input hashing;\n[4] Comparison of hashes from different inputs;\n[5] Test hash differences of 100\'000 similar string pairs;" << endl;
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
    }
    return 0; 
}