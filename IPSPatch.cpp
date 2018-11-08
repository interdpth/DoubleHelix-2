#include "MainHeader.h"
long GetIPSAddress(FILE* fp){
	unsigned char tmp[4]={0};
	MemFile::currentFile->fread(tmp,3,1,fp);
	return  (tmp[0]<<16)|(tmp[1]<<8)|(tmp[2]);
};

long GetIPSShort(FILE* fp){
    unsigned char tmp[2]={0};
	MemFile::currentFile->fread(tmp,2,1,fp);
	return tmp[0]<<8|tmp[1];

}
int ApplyIPS(char* ipsfile, char* dstfile){
FILE* fips=fopen(ipsfile,"r+");
FILE* fdst=fopen(dstfile,"r+");

char  cpatch[6]={0};
char  ceof[3]={0};
long  eofoffset=0;
unsigned short length=0;
unsigned char bytebuffer=0;
int i =0;
long check=0;
if(fips==0 || fdst==0) return 0;
MemFile::currentFile->seek(0);
MemFile::currentFile->fread(cpatch,5,1,fips);
if(memcmp(cpatch,"PATCH",5)==0){

	MemFile::currentFile->seek(5);
    //Now we actually do patching

	while(!feof(fips)){
		MemFile::currentFile->seek(GetIPSAddress(fips));
		length = GetIPSShort(fips);
		if(length ==0){//RLE
			 //get the RLE length
			length = GetIPSShort(fips);
			bytebuffer=fgetc(fips);
			for(i=0;i<length;i++) fputc(bytebuffer,fdst);
		}else{
           //direct copy
			for(i=0;i<length;i++){
				bytebuffer=fgetc(fips);
				fputc(bytebuffer,fdst);
			}
		}
 

	}

fclose(fips);
fclose(fdst);
MessageBox(0,"Patch Completed","NULL",MB_OK);
}
return 0;
}

