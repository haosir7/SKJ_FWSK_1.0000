/*! \file    gZip.c
   \brief    gz格式的压缩与解压
   \author   zl
 */

#include <stdio.h>
#include "gZip.h"

#if (USE_GZIP_LIB != 0)


int decompress_one_file(const char *infilename, const char *outfilename)
{
   char buffer[128];
   int num_read = 0;
   
   gzFile infile = gzopen(infilename, "rb");
   FILE *outfile = fopen(outfilename, "wb");
   if (!infile || !outfile) return -1;

   while ((num_read = gzread(infile, buffer, sizeof(buffer))) > 0) {
      fwrite(buffer, 1, num_read, outfile);
      }

   gzclose(infile);
   fclose(outfile);

   return 0;
}

int compress_one_file(const char *infilename, char *outfilename)
{
	int nRet = 0;
	int i = 0;
   char inbuffer[128];
   int num_read = 0;
   unsigned long total_read = 0, total_wrote = 0;
   
   FILE *infile = fopen(infilename, "rb");
   gzFile outfile = gzopen(outfilename, "wb");
   if (!infile || !outfile) 
   {
	   printf("if (!infile || !outfile)  压缩文件失败!\n");
	   return -1;
   }

   while ((num_read = fread(inbuffer, 1, sizeof(inbuffer), infile)) > 0)
   {
      total_read += num_read;
      nRet = gzwrite(outfile, inbuffer, num_read);
	  if (0 == nRet)
	  {
		  fclose(infile);
		  gzclose(outfile);
		  printf("gzwrite = 0 压缩文件失败!");
		  return -1;
	  }
	   i++;
   }
   
   fclose(infile);
   nRet = gzclose(outfile);


  return 0;
}




int compress_zlib(unsigned char *dest, unsigned long *destLen, 
				  const unsigned char *source, unsigned int sourceLen)
{
	return compress(dest, destLen, source, sourceLen);
}

int uncompress_zlib(unsigned char *dest, unsigned long *destLen, 
					const unsigned char *source, unsigned int sourceLen)
{
	return uncompress(dest, destLen, source, sourceLen);
}

unsigned int compressBound_zlib(unsigned int sourceLen)
{
	return compressBound(sourceLen);
}

#endif	//USE_GZIP_LIB

