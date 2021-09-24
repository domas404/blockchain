#include <iostream>
#include <vector>
#include <string>

using namespace std;

string toHex(int num){
    string el = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
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
    vector<string> hex;
    string hash_val="";
    int tarp;
    int a = int(input[0])/2;
    while(hash_val.length()<16){
        for(int i=0; i<n; ++i){
            tarp = (tarp + a + int(input[i]))%256;
            a += (int(input[i])+500)/3-77;
        }
        // hex.push_back(to_string(tarp));
        hash_val += toHex(tarp);
    }

    if(hash_val.length()>16){
        hash_val.pop_back();
    }
    cout << "#" << hash_val << " hash length: " << hash_val.length() << endl;
    return hash_val;
    
}
int main(){
    string input, input2;
    cout << "Input text: ";
    cin >> input;
    cout << "Inupt text 2: ";
    cin >> input2;
    string hash1 = hashfunc(input, input.length());
    string hash2 = hashfunc(input2, input2.length());
    int dif=0;
    for(int i=0; i<16; ++i){
        if(hash1[i]==hash2[i])
            dif++;
    }
    cout << "Skirtingumas: " << ((16-dif)*100/16) << " proc." << endl;
}