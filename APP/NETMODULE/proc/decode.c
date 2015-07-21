#define HEXSYMBOLS 22

typedef int size_t;

typedef struct
{
   char symb;
   unsigned char value;
} hex_table_t;

hex_table_t symb2hex_table[HEXSYMBOLS] =
{
   {'0',0x0},
   {'1',0x1},
   {'2',0x2},
   {'3',0x3},
   {'4',0x4},
   {'5',0x5},
   {'6',0x6},
   {'7',0x7},
   {'8',0x8},
   {'9',0x9},
   {'A',0xA},
   {'B',0xB},
   {'C',0xC},
   {'D',0xD},
   {'E',0xE},
   {'F',0xF},
   {'a',0xA},
   {'b',0xB},
   {'c',0xC},
   {'d',0xD},
   {'e',0xE},
   {'f',0xF}
};

/* - - - - - - - - - - - - - - - - - - - - - -
* convert string of hex symbols to number
*/
unsigned long symb2hex(char c[], size_t len)
{
   unsigned long res=0;
   int i=0;
   int j=0;

   for(j=0;j<len;j++)
   {
	  for(i=0;i<HEXSYMBOLS;i++)
	  {
		 if(symb2hex_table[i].symb==c[j])
		 {
			c[j]=symb2hex_table[i].value;
			break;
		 }
	  }
   }
   if(len<2)
   {
	   return c[0];
   }
   for(res=0,j=0;j<len;j++)
   {
	   res+=c[j]<<(len-j-1)*4;
   }
   return res;
}

/* - - - - - - - - - - - - - - - - - - - - -
* convert number in hex[0] to 2 hex symbols
* result in hex[0] hex[1]
*/
int hex2symb(char hex[])
{
   char c[2];
   int i,j;

   c[0]=(hex[0]&0xf0)>>4;
   c[1]=hex[0]&0x0f;

   for(j=0;j<2;j++)
   {
	  for(i=0;i<HEXSYMBOLS;i++)
	  {
		 if(symb2hex_table[i].value==c[j])
		 {
			c[j]=symb2hex_table[i].symb;
			break;
		 }
	  }
   }
   hex[0]=c[0];
   hex[1]=c[1];

   return 0;
}



/* - - - - - - - - - - - - - - - - - - - - - - - - - - -
* convert hex symbolic string to byte array
* return size of new array in bytes
*/
int str2hex(char* inbuf, size_t insize, char* outbuf, size_t outsize)
{
   int j,k;

   for(k=0,j=0 ; (j<insize-1)&&(k<outsize) ; j+=2,k++ )
   {
	  outbuf[k]=symb2hex(&inbuf[j],2);
   }
   return k;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - -
* convert byte array to hex symbolic string
* return size of new array in bytes
*/
int hex2str(char* inbuf, size_t insize, char* outbuf, size_t outsize)
{
   int j,i;

   for(j=0,i=0 ; (j<outsize)&&(i<insize) ; i++,j+=2 )
   {
	  outbuf[j]=inbuf[i];
	  hex2symb(&outbuf[j]);
   }
   outbuf[j++]=0;
   return (j-1);
}

