#ifndef __IDEA_H

#define __IDEA_H



#include <string>
using namespace std;

#define	byte	unsigned char



class TIdea
{
private :
  int key[52];
  int unkey[52];

  int fun_a(int a);
  int fun_b(int b);
  int x_multiply_y(int x, int y);
  int bytesToInt(byte* inBytes, int startPos);
  void intToBytes(int inputInt, byte* outBytes, int startPos);

  void encrypt(byte* inbytes, byte* outbytes,int* key);
  void encrypt_subkey(byte byteKey[8]);
  void uncrypt_subkey();
  
public :
  string IdeaEncode(string data, string keystr);
  string IdeaDecode(string data, string keystr);
};



#endif 

