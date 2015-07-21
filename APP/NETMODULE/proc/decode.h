#ifndef DECODE_H_
#define DECODE_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* - - - - - - - - - - - - - - - - - - - - - - - - - - -
* convert hex symbolic string to byte array
* return size of new array in bytes
*/
int str2hex(char* inbuf, size_t insize, char* outbuf, size_t outsize);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - -
* convert byte array to hex symbolic string
* return size of new array in bytes
*/
int hex2str(char* inbuf, size_t insize, char* outbuf, size_t outsize);


#ifdef __cplusplus
}
#endif

#endif /* DECODE_H_ */

