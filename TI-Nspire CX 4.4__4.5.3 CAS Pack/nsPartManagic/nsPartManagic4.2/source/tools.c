#include <os.h>

int chex2int(char c)
{	if(c>='0' && c<='9')
		return c-'0';
	return c-'A'+10;
}

int filelength(FILE* file)
{	int current = ftell(file);
	fseek(file,0,SEEK_END);
	int size = ftell(file);
	fseek(file,current,SEEK_SET);
	return size;
}

int waitConfirm()
{	int result=0;
	while(!any_key_pressed());
	if(isKeyPressed(KEY_NSPIRE_Y))
		result=1;
	while(any_key_pressed());
	return result;
}

char waitLetter()
{	char t=0;
	while(!any_key_pressed());	
	if(isKeyPressed(KEY_NSPIRE_ESC)) t=2;
	else if(isKeyPressed(KEY_NSPIRE_ENTER)) t=1;
	else if(isKeyPressed(KEY_NSPIRE_A)) t='A';
	else if(isKeyPressed(KEY_NSPIRE_B)) t='B';
	else if(isKeyPressed(KEY_NSPIRE_C)) t='C';
	else if(isKeyPressed(KEY_NSPIRE_D)) t='D';
	else if(isKeyPressed(KEY_NSPIRE_E)) t='E';
	else if(isKeyPressed(KEY_NSPIRE_F)) t='F';
	else if(isKeyPressed(KEY_NSPIRE_G)) t='G';
	else if(isKeyPressed(KEY_NSPIRE_H)) t='H';
	else if(isKeyPressed(KEY_NSPIRE_I)) t='I';
	else if(isKeyPressed(KEY_NSPIRE_J)) t='J';
	else if(isKeyPressed(KEY_NSPIRE_K)) t='K';
	else if(isKeyPressed(KEY_NSPIRE_L)) t='L';
	else if(isKeyPressed(KEY_NSPIRE_M)) t='M';
	else if(isKeyPressed(KEY_NSPIRE_N)) t='N';
	else if(isKeyPressed(KEY_NSPIRE_O)) t='O';
	else if(isKeyPressed(KEY_NSPIRE_P)) t='P';
	else if(isKeyPressed(KEY_NSPIRE_Q)) t='Q';
	else if(isKeyPressed(KEY_NSPIRE_R)) t='R';
	else if(isKeyPressed(KEY_NSPIRE_S)) t='S';
	else if(isKeyPressed(KEY_NSPIRE_T)) t='T';
	else if(isKeyPressed(KEY_NSPIRE_U)) t='U';
	else if(isKeyPressed(KEY_NSPIRE_V)) t='V';
	else if(isKeyPressed(KEY_NSPIRE_W)) t='W';
	else if(isKeyPressed(KEY_NSPIRE_X)) t='X';
	else if(isKeyPressed(KEY_NSPIRE_Y)) t='Y';
	else if(isKeyPressed(KEY_NSPIRE_Z)) t='Z';
	return t;
}