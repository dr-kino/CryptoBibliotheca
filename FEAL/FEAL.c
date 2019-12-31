#include <stdio.h>
#include <string.h>
/* static unsigned char KS[80];*/   /* chaves extendidas , (N+8)*2 bytes */
/*static int N;*/
#define encrypt   AX
#define setkey    BX
#define sbox      s1
#define INSERIR   I2
#define SUBSTRING XD
static void encrypt( unsigned char blk1[8], unsigned char blk2[8], int ed,unsigned char KS[80]);
static void setkey ( unsigned char key0[8], int n,unsigned char KS[80]);
static unsigned char sbox( unsigned char data);
void DRAM0191 (int modo, unsigned char *chave, unsigned char *buf_entrada,
	       unsigned char *buf_saida, unsigned int tamanho,int *result);
static void INSERIR ( char *str_dst, char *str_org, int inic_dst, int inic_org, int fim);
static void SUBSTRING ( char *str,char *dst, int inic, int fim);

static void setkey(unsigned char key0[8],int n, unsigned char KS[80])
{ unsigned char t[4],b[4],d[4],key[8];
  int N,i,j;

  N = n;
  for ( i = 0; i < 8; i++) key[i] = key0[i];
  for ( i = 0; i < 4; i++) d[i] = 0;
   for ( i = 0; i < N/2+4; i++) {
	for ( j = 0; j < 4; j++) {
		t[j] = key[j+4];
		b[j] = key[j+4] ^ d[j];     /* ^ em c = XOR */
		d[j] = key[j];
		}
		key[5] = key[1] ^ key[0];
		key[6] = key[2] ^ key[3];
		key[5] = sbox( key[5] + ( key[6] ^ b[0] ) + 1);
		key[6] = sbox( key[6] + ( key[5] ^ b[1] ));
		key[4] = sbox( key[0] + ( key[5] ^ b[2] ));
		key[7] = sbox( key[3] + ( key[6] ^ b[3] ) + 1);
		for ( j = 0; j < 4; j++)  key[j] = t[j]; /* crossing */
		for ( j = 0; j < 4; j++)  KS[4*i+j] = key[j+4];
	}
}


static void encrypt( unsigned char blk1[8],unsigned char blk2[8],int ed,unsigned char KS[80])
{ unsigned char t[4];
  int i,j,k,ni,no,N;

  N=32;
  ni = N*2;
  no = ni + 8;
  if ( !ed )  k = ni; else k = no;
   for ( i = 0; i < 8; i++) blk2[i] = blk1[i] ^ KS[k++];
    for ( i = 0; i < 4; i++) blk2[i+4] = blk2[i+4] ^ blk2[i];
     if  ( !ed ) k = 0; else k = ni - 2;
	for ( i = 0; i < N; i++)
	{ for ( j = 0; j < 4; j++) t[j] = blk2[j+4];
	  blk2[5] = blk2[5] ^ blk2[4] ^ KS[k] ;
	  blk2[6] = blk2[6] ^ blk2[7] ^ KS[k+1];
	  blk2[5] = sbox( blk2[5] + blk2[6] + 1);
	  blk2[6] = sbox( blk2[6] + blk2[5] );
	  blk2[4] = sbox( blk2[4] + blk2[5] );
	  blk2[7] = sbox( blk2[7] + blk2[6] + 1 );
	  for ( j = 0; j < 4; j++ ) blk2[j+4] = blk2[j+4] ^ blk2[j];
	   for ( j = 0; j < 4; j++ ) blk2[j]   = t[j];
		if  ( !ed) k += 2; else k -= 2;
	}
	for ( i = 0; i < 4; i++) t[i] = blk2[i+4];
	      for ( i = 0; i < 4; i++) blk2[i+4] = blk2[i] ^ blk2[i+4];
	for ( i = 0; i < 4; i++) blk2[i]   = t[i];
	if  ( !ed ) k = no; else k = ni;
	for ( i = 0; i < 8; i++) blk2[i] = blk2[i] ^ KS[k++];
}

/****************************************************************************/

static unsigned char sbox( unsigned char data)
{  union {
	    unsigned int DATA;
	    struct {
		     unsigned char low;
		     unsigned char high;
		    } reg;
	} s;
	s.reg.high = 0;
	s.reg.low  = data;
	s.DATA   <<= 2;
	return( s.reg.high | s.reg.low) ;
}

static void INSERIR ( char *str_dst, char *str_org, int inic_dst, int inic_org, int fim)
{ int i;
  inic_dst--;
  for (i=inic_org-1; i < fim+inic_org-1; i++)
   *(str_dst+inic_dst++)= *(str_org+i);

}

static void SUBSTRING ( char *str,char *dst, int inic, int fim)
{ int i,k=0;

  for (i=inic-1; i < fim+inic-1; i++)
    *(dst+k++)= *(str+i);
	dst[k]='\0';
}

static  void Expandir( char *origem,
					   char *destino,
					   int tamanho_da_origem )
{ int i,j;
  char c;

for ( i = 0, j = 0; i < tamanho_da_origem ; i++ )
 {
    c = origem[i];
    c = (c & 0xF0) >> 4;
    if ( c>=0 && c<=9 )
	 c |= '0';
    else
	 c += ( 0x37);
    destino[j++] = c;
   
	c = (origem[i] & 0x0F);
    if ( c>=0 && c<=9 )
	 c |= '0';
    else
	 c += (0x37);
    destino[j++] = c;
 }
 destino[j]=0;
}

static void Compactar( char *origem,
					   char *destino,
					   int tamanho_da_origem )
{
 unsigned char caracter, c, d;
 int i;


 for ( i = 0, tamanho_da_origem /= 2; i < tamanho_da_origem; i++ )
 {	 c =  origem[i*2];
	 d =  origem[i*2+1];
	 if ((c >= 48) && (c <= 57 ))
		 caracter = ( c & 0x0f ) << 4;
	 else
	 {   c = c & 0xDF; // tornando as letras Maiusculas;
		 caracter = ( c - 'A' + 0x0A ) << 4;
	 }
	 if (( d >= 48) && ( d <= 57))
        destino[i] = ( d & 0x0f ) | caracter;
     else
	 { d = d & 0xDF; // tornando as letras Maiusculas;
	   destino[i] = ( d - 'A' + 0x0A ) | caracter;
     }
 }
 destino[i]=0;
}

int main(int argc, char *argv[])
{ char bloco_entrada[9];
  char bloco_saida[9];
  char chave[9];
  char bloco_saida_expandido[17];
  char bloco_entrada_expandido[17];
  char chave_expandida[17];
  int modo;
  unsigned char KS[80];

  strcpy(bloco_entrada,"testando");

  strcpy(bloco_saida,"        ");
  strcpy(chave,"banespa1");
 
  setkey( chave, 32,KS); 
  
 // criptografar 
  modo = 0; // criptografar
  encrypt( bloco_entrada, bloco_saida, modo,KS);
  bloco_saida[8]=0;
 
  Expandir(bloco_saida,bloco_saida_expandido,8);
  strcpy(bloco_saida,"        ");
  Compactar(bloco_saida_expandido,bloco_saida,16);
 
  Expandir(bloco_entrada,bloco_entrada_expandido,8);
  Expandir(chave,chave_expandida,8);
  
  printf("CRIPTOGRAFAR\n\n");
  printf("Bloco de Entrada ( 64 bits ou  8 bytes)...............: %s\n",bloco_entrada);
  printf("Bloco de Entrada Expandido ( 128 bits ou  16 bytes)...: %s\n",bloco_entrada_expandido);
  printf("Chave criptografica ( 64 bits ou  8 bytes)............: %s\n",chave);
  printf("Chave criptografica Expandida ( 128 bits ou  16 bytes): %s\n",chave_expandida);  
  printf("Bloco de Saida ( 64 bits ou  8 bytes).................: %s\n",bloco_saida);
  printf("Bloco de Saida Expandido (128 bits ou 16 bytes).......: %s\n",bloco_saida_expandido);

  //decriptografar
  modo = -1; // decriptografar
  encrypt( bloco_saida, bloco_entrada, modo,KS);
  bloco_entrada[8]=0;
  
  printf("\n\nDECRIPTOGRAFAR\n\n");
  printf("Bloco de Entrada ( 64 bits ou  8 bytes)...............: %s\n",bloco_saida);
  printf("Bloco de Entrada Expandido ( 128 bits ou  16 bytes)...: %s\n",bloco_saida_expandido);
  printf("Chave criptografica ( 64 bits ou  8 bytes)............: %s\n",chave);
  printf("Chave criptografica Expandida ( 128 bits ou  16 bytes): %s\n",chave_expandida);  
  printf("Bloco de Saida ( 64 bits ou  8 bytes).................: %s\n",bloco_entrada);
  printf("Bloco de Saida Expandido (128 bits ou 16 bytes).......: %s\n",bloco_entrada_expandido);
  printf("\n\n\n\n");
  return 0;
}


