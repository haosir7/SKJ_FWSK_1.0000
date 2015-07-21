#include "idea.h"

//#include "LogUtil.h"


//---------------------------------------------------------------------------

int TIdea::bytesToInt(byte* inBytes, int startPos)

{
  return ((inBytes[startPos] << 8) & 0xff00) + (inBytes[startPos + 1] & 0xff);
}
//---------------------------------------------------------------------------

void TIdea::intToBytes(int inputInt, byte* outBytes, int startPos)
{
  outBytes[startPos] = (byte) (inputInt >> 8);
  outBytes[startPos + 1] = (byte) inputInt;
}
//---------------------------------------------------------------------------

int TIdea::x_multiply_y(int x, int y)
{
  if (x == 0)
  {
      x = 0x10001 - y;
  }
  else if (y == 0)
  {
      x = 0x10001 - x;
  }
  else
  {
      unsigned int tmp = x * y;
      y = tmp & 0xffff;
      x = tmp >> 16;
      x = (y - x) + ((y < x) ? 1 : 0);
  }
  return x & 0xffff;
}
//---------------------------------------------------------------------------

int TIdea::fun_a(int a)
{
  if (a < 2)
  {
    return a;
  }
  int b = 1;
  int c = 0x10001 / a;
  for (int i = 0x10001 % a; i != 1;)
  {
    int d = a / i;
    a %= i;
    b = (b + (c * d)) & 0xffff;
    if (a == 1)
    {
      return b;
    }
    d = i / a;
    i %= a;
    c = (c + (b * d)) & 0xffff;
  }
  return (1 - c) & 0xffff;
}
//---------------------------------------------------------------------------

int TIdea::fun_b(int b)
{
  return (0 - b) & 0xffff;
}
//---------------------------------------------------------------------------

void TIdea::encrypt(byte* inbytes, byte* outbytes,int* key)
{
  int k = 0;
  int a = bytesToInt(inbytes, 0);
  int b = bytesToInt(inbytes, 2);
  int c = bytesToInt(inbytes, 4);
  int d = bytesToInt(inbytes, 6);
  for (int i = 0; i < 8; i++)
  {
    a = x_multiply_y(a, key[k++]);
    b += key[k++];
    b &= 0xffff;
    c += key[k++];
    c &= 0xffff;
    d = x_multiply_y(d, key[k++]);
    int tmp1 = b;
    int tmp2 = c;
    c ^= a;
    b ^= d;
    c = x_multiply_y(c, key[k++]);
    b += c;
    b &= 0xffff;
    b = x_multiply_y(b, key[k++]);
    c += b;
    c &= 0xffff;
    a ^= b;
    d ^= c;
    b ^= tmp2;
    c ^= tmp1;
  }
  intToBytes(x_multiply_y(a, key[k++]), outbytes, 0);
  intToBytes(c + key[k++], outbytes, 2);
  intToBytes(b + key[k++], outbytes, 4);
  intToBytes(x_multiply_y(d, key[k]), outbytes, 6);
}
//---------------------------------------------------------------------------

void TIdea::encrypt_subkey(byte byteKey[8])
{
  for (int i = 0; i < 8; i++)
  {
    key[i] = bytesToInt(byteKey, i * 2);
  }
  for (int j = 8; j < 52; j++)
  {
    if ((j & 0x7) < 6)
    {
      key[j] = (((key[j - 7] & 0x7f) << 9) | (key[j - 6] >> 7)) &
          0xffff;
    }
    else if ((j & 0x7) == 6)
    {
      key[j] = (((key[j - 7] & 0x7f) << 9) | (key[j - 14] >> 7)) &
          0xffff;
    }
    else
    {
      key[j] = (((key[j - 15] & 0x7f) << 9) | (key[j - 14] >> 7)) &
          0xffff;
    }
  }
}
//---------------------------------------------------------------------------

void TIdea::uncrypt_subkey()
{
  int dec = 52;
  int asc = 0;
  int aa = fun_a(key[asc++]);
  int bb = fun_b(key[asc++]);
  int cc = fun_b(key[asc++]);
  int dd = fun_a(key[asc++]);
  unkey[--dec] = dd;
  unkey[--dec] = cc;
  unkey[--dec] = bb;
  unkey[--dec] = aa;
  for (int k1 = 1; k1 < 8; k1++)
  {
    aa = key[asc++];
    bb = key[asc++];
    unkey[--dec] = bb;
    unkey[--dec] = aa;
    aa = fun_a(key[asc++]);
    bb = fun_b(key[asc++]);
    cc = fun_b(key[asc++]);
    dd = fun_a(key[asc++]);
    unkey[--dec] = dd;
    unkey[--dec] = bb;
    unkey[--dec] = cc;
    unkey[--dec] = aa;
  }
  aa = key[asc++];
  bb = key[asc++];
  unkey[--dec] = bb;
  unkey[--dec] = aa;
  aa = fun_a(key[asc++]);
  bb = fun_b(key[asc++]);
  cc = fun_b(key[asc++]);
  dd = fun_a(key[asc]);
  unkey[--dec] = dd;
  unkey[--dec] = cc;
  unkey[--dec] = bb;
  unkey[--dec] = aa;
}
//---------------------------------------------------------------------------

string TIdea::IdeaEncode(string data, string keystr)

{
  string result = "";

  char* in = new char[8];
  memset(in,0,8);
  char* out = new char[8];
  memset(out,0,8);

  char* BufferIn = new char[data.length()+1];
  memset(BufferIn,0,data.length()+1);
  memcpy(BufferIn,data.c_str(),data.length()+1);

  int num = 0;
  if(data.length()%8 == 0)
  {
    num = data.length();
  }
  else
  {
    num = (int(data.length()/8)+1)*8;
  }
  char* BufferOut = new char[num];
  memset(BufferOut,0,num);

  encrypt_subkey((byte *)keystr.c_str());
  uncrypt_subkey();

//  try
  {
    int i,j;
    for (i=0; i<data.length()/8; i++)
    {
      for (j = 0; j < 8; j ++)
      {
        in[j] = BufferIn[8*i+j];
      }
      encrypt((byte *)in,(byte *)out,key);
      for (j = 0; j < 8; j ++)
      {
        BufferOut[8*i+j] = out[j];
      }
      memset(in,0,8);
      memset(out,0,8);
    }
    if(i*8 < data.length())
    {
      for (j = 0; j < data.length() - i*8; j ++)
      {
        in[j] = BufferIn[8*i+j];
      }
//      int templen = 8 - data.length()%8;
//      for (j = data.length()%8 ; j < 8 ; j ++)
//      {
//        in[j] = (byte) templen;
//      }
      encrypt((byte *)in,(byte *)out,key);
      for (j = 0; j < 8; j ++)
      {
        BufferOut[8*i+j] = out[j];
      }
    }
//{
//for (int ii=0;ii<num;ii++)
//	printf("%02x ",BufferOut[ii]);	
//printf("\n");
//}    
 //   result = AnsiString(BufferOut, num);
    result = string(BufferOut, num);
    delete [] BufferIn;
    delete [] BufferOut;
    delete [] in;
    delete [] out;
  }
/*  catch(Exception &e)
  {
    delete [] BufferIn;
    delete [] BufferOut;
    delete [] in;
    delete [] out;
    printf("加密出现异常：%s", e.Message.c_str());
    return "";
  }*/
  
  return result;
}
//---------------------------------------------------------------------------

string TIdea::IdeaDecode(string data, string keystr)

{
  string result = "";
  char* in = new char[8];
  memset(in,0,8);
  char* out = new char[8];
  memset(out,0,8);

  encrypt_subkey((byte *)keystr.c_str());
  uncrypt_subkey();

  char *BufferIn = new char[data.length()+1];
  memset(BufferIn,0,data.length()+1);
  memcpy(BufferIn,data.c_str(),data.length()+1);
  int num = 0;
  if(data.length()%8 == 0)
  {
    num = data.length();
  }
  else
  {
    num = (int(data.length()/8)+1)*8;
  }
  char* BufferOut = new char[num];
  memset(BufferOut,0,num);

//  try
  {
    int i,j;
    for (i = 0; i < data.length()/8; i++)
    {
      for (j = 0; j < 8; j ++)
      {
        in[j] = BufferIn[8*i+j];
      }
      encrypt((byte *)in,(byte *)out,unkey);
      for (int j = 0; j < 8; j ++)
      {
        BufferOut[8*i+j] = out[j];
      }
      memset(in,0,8);
      memset(out,0,8);
    }
    if(i*8 < data.length())
    {
      for (j = 0; j < data.length() - i*8; j ++)
      {
        in[j] = BufferIn[8*i+j];
      }
      encrypt((byte *)in,(byte *)out,unkey);
      for (j = 0; j < 8; j ++)
      {
        BufferOut[8*i+j] = out[j];
      }
    }
 //   result = AnsiString(BufferOut, num);
    result = string(BufferOut, num);
    delete [] BufferIn;
    delete [] BufferOut;
    delete [] in;
    delete [] out;
  }
/*  catch(Exception &e)
  {
    delete [] BufferIn;
    delete [] BufferOut;
    delete [] in;
    delete [] out;
    printf("解密出现异常：%s", e.Message.c_str());
    return "";
  }
*/
  return result;
}
//---------------------------------------------------------------------------


