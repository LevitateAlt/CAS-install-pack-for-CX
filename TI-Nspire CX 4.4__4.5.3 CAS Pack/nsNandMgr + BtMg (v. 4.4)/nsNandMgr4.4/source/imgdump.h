int decompressFiles(char* buf, int size, char** outbuf);
int getFieldData(char* buf, int cfield, int num, int dsize, char** pptr, char** fptr);
void copyFieldData(char* buf, int cfield, int num, int dsize, char* outbuf, int bufsize);
int getTotalFieldsSize(char* buf, int dsize, int multi);