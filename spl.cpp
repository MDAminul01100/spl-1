#include<iostream>
#include<sstream>
#include<bitset>
#include<iomanip>

using namespace std;

bitset<32> H[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};

bitset<32> constants[64] = {

        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
        };

int strlen(string str)
{
    int i;
    for(i = 0; str[i] != '\0'; i++){}
    return i;
}

string paddingMessage(string str)
{
    string msgNew;
    for (size_t i = 0; i < strlen(str); ++i)

    {
        bitset<8> b(str.c_str()[i]);
        msgNew += b.to_string();
    }
    int bitlen = strlen(msgNew);

    msgNew += '1';

    int padcount = (448 - ((bitlen % 512) + 1));

    for (int i = 0; i < padcount; i++)
    {
        msgNew += '0';
    }

    bitset <64> LengthInBits(bitlen);

    msgNew += LengthInBits.to_string();

    return msgNew;
}

bitset<32> rotateRight(bitset<32> bits,int n)
{
    return (bits >> n) | (bits << (32-n));
}

bitset<32> Ch(bitset<32> x,bitset<32> y,bitset<32> z)
{
    return (x & y) ^ (~x & z);
}

bitset<32> Maj(bitset<32> x, bitset<32> y, bitset<32> z)
{
    return (x & y) ^ (y & z) ^ (z & x);
}

bitset<32> add(bitset<32> a, bitset<32> b)
{
    unsigned long c = a.to_ulong();
    unsigned long d = b.to_ulong();
    return bitset<32>((c + d) % 4294967296);

}

string binToHex(string hashInBin)
{
    string digits[16] = {"0","1","2","3", "4", "5", "6", "7","8", "9", "A", "B", "C", "D", "E", "F"};
    //int x = 0;
    string str;
    for(int i = 0; i < hashInBin.size(); i+=4)
    {
        string subString = hashInBin.substr(i,4);
        int x = 0;
        if(subString.substr(0,1) == "1") x += 8;
        if(subString.substr(1,1) == "1") x += 4;
        if(subString.substr(2,1) == "1") x += 2;
        if(subString.substr(3,1) == "1") x += 1;

        str += digits[x];
    }
    return str;
}


string makingDigest(string messege)
{
    bitset<32> words[64];

    for(int i = 0,counter = 0;i < 16; i++)
    {
        for(int j = 15; j >= 0; j--)
        {
            words[i][j] = messege[counter];
            counter++;
        }
    }
    for (int k = 16; k < 64; k++)
    {
        bitset<32> s0 = rotateRight(words[k - 15], 7) ^ rotateRight(words[k - 15], 18) ^ (words[k - 15] >> 3);
        bitset<32> s1 = rotateRight(words[k - 2], 17) ^ rotateRight(words[k - 2], 19) ^ (words[k - 2] >> 10);
        words[k] = add(add(words[k - 16], s0), add(words[k - 7], s1));

    }
    bitset<32> a, b, c, d, e, f, g, h;

    a = H[0];
    b = H[1];
    c = H[2];
    d = H[3];
    e = H[4];
    f = H[5];
    g = H[6];
    h = H[7];


    for(int t = 0; t < 64; t++)
    {
        bitset<32> s1 = rotateRight(e,6) ^ rotateRight(e,11) ^ rotateRight(e,25);
        bitset<32> temp1 = add( h , add( s1 , add( Ch(e,f,g) , add( constants[t] , words[t] ) ) ) );
        bitset<32> s0 = rotateRight(e,2) ^ rotateRight(e,13) ^ rotateRight(e,22);
        bitset<32> temp2 = add(s0, Maj(a,b,c));
        h = g;
        g = f;
        f = e;
        e = add( d , temp1);
        d = c;
        c = b;
        b = a;
        a = add(temp1,temp2);
    }
    H[0] = add(a,H[0]);
    H[1] = add(b,H[1]);
    H[2] = add(c,H[2]);
    H[3] = add(d,H[3]);
    H[4] = add(e,H[4]);
    H[5] = add(f,H[5]);
    H[6] = add(g,H[6]);
    H[7] = add(h,H[7]);

    string hashInBin = H[0].to_string() + H[1].to_string() + H[2].to_string()+ H[3].to_string() +
                        H[4].to_string() + H[5].to_string() + H[6].to_string() + H[7].to_string();

    return binToHex(hashInBin);
}



int main()
{
    string str;
    cin >> str;

    string processedSms = paddingMessage(str);
    cout << makingDigest(processedSms) << endl;
    return 0;
}
