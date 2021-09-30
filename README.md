# 1-oji užduotis. Hash generatoriaus kūrimas.

## Naudojimasis programa
Paleidus programą, reikia pasirinkti, kurią funkciją ketinate atlikti:
    [1] Simple tests with data from files;
    [2] Search of collisions;
    [3] Individual input hashing;
    [4] Comparison of hashes from different inputs;
    [5] Test hash differences of 100'000 similar string pairs;
## Hash funkcijos pseudokodas
    string hashfunc(string input, int n):
        string hash_val="";
        int tarp=0;              // kintanti tarpinė reikšmė
        int a = int(input[0])/2; // kita kintanti tarpinė reikšmė, kurios pradinė reikšmė - įvesties pirmo simbolio ascii kodo pusė
        while(hash_val.length()<64){
            for(int i=0; i<n; ++i){
                tarp = (tarp + a + int(input[i]))%256; // naudojant tarpines reikšmes ir įvesties simbolių ascii kodus,
                a += (int(input[i])+500)/3-77;         // atliekamos matematinės funkcijos, kuriomis bandoma pagal tam tikrą
            }                                          // dėsningumą sumaišyti hash reikšmes pagal simbolių seką.
            hash_val += toHex(tarp);                   // gautas skaičius konvertuojamas į hex sistemą ir pridedamas prie hash išraiškos
        }                                              // ciklas kartojamas, kol hash'as pasiekia 64 simbolių ilgį
        return hash_val;