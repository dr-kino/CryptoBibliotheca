#include <stdio.h>
#include <string.h>

#include "des.h"

#define ENCRIPTA 0
#define DECRIPTA 1

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
  char chave1  [9];
  char chave2  [9];
  char chave3  [9];
  char chave  [25];
  char bloco_saida_expandido[17];
  char bloco_entrada_expandido[17];
  char chave1_expandida[17];
  char chave2_expandida[17];
  char chave3_expandida[17];
  DES3_KS k3;

  strcpy(bloco_entrada,"testando");
  strcpy(chave1,"laranja1");
  strcpy(chave2,"abacaxi2");
  strcpy(chave3,"melancia");
  strcpy(chave, "laranja1abacaxi2melancia");
  strcpy(bloco_saida,"        ");
 // strcpy(chave,"banespa1");
 
  // criptografar 
  des3key(k3, chave, ENCRIPTA);
  des3(k3,bloco_entrada);
  
  memcpy(bloco_saida,bloco_entrada,8);
  bloco_saida[8]=0;

  Expandir(bloco_saida,bloco_saida_expandido,8);
  strcpy(bloco_saida,"        ");
  Compactar(bloco_saida_expandido,bloco_saida,16);
  
  strcpy(bloco_entrada,"testando");
  Expandir(bloco_entrada,bloco_entrada_expandido,8);
  Expandir(chave1,chave1_expandida,8);
  Expandir(chave2,chave2_expandida,8);
  Expandir(chave3,chave3_expandida,8);
 
  printf("CRIPTOGRAFAR\n\n");
  printf("Bloco de Entrada ( 64 bits ou  8 bytes)...............: %s\n",bloco_entrada);
  printf("Bloco de Entrada Expandido ( 128 bits ou  16 bytes)...: %s\n",bloco_entrada_expandido);
  printf("Chave criptografica 1 ( 64 bits ou  8 bytes)..........: %s\n",chave1);
  printf("Chave criptografica 1 Expandida (128 bits ou 16 bytes): %s\n",chave1_expandida);  
  printf("Chave criptografica 2 (64 bits ou  8 bytes)...........: %s\n",chave2);
  printf("Chave criptografica 2 Expandida (128 bits ou 16 bytes): %s\n",chave2_expandida);  
  printf("Chave criptografica 3 ( 64 bits ou  8 bytes)..........: %s\n",chave3);
  printf("Chave criptografica 3 Expandida (128 bits ou 16 bytes): %s\n",chave3_expandida);  
  printf("Bloco de Saida ( 64 bits ou  8 bytes).................: %s\n",bloco_saida);
  printf("Bloco de Saida Expandido (128 bits ou 16 bytes).......: %s\n",bloco_saida_expandido);

  //decriptografar
  memcpy(bloco_entrada,bloco_saida,8);
  bloco_entrada[8]=0;
  des3key(k3, chave, DECRIPTA);
  des3(k3,bloco_entrada);
  bloco_entrada[8]=0;
  
  printf("\n\nDECRIPTOGRAFAR\n\n");
  printf("Bloco de Entrada ( 64 bits ou  8 bytes)...............: %s\n",bloco_saida);
  printf("Bloco de Entrada Expandido ( 128 bits ou  16 bytes)...: %s\n",bloco_saida_expandido);
  printf("Chave criptografica 1 ( 64 bits ou  8 bytes)..........: %s\n",chave1);
  printf("Chave criptografica 1 Expandida (128 bits ou 16 bytes): %s\n",chave1_expandida);  
  printf("Chave criptografica 2 (64 bits ou  8 bytes)...........: %s\n",chave2);
  printf("Chave criptografica 2 Expandida (128 bits ou 16 bytes): %s\n",chave2_expandida);  
  printf("Chave criptografica 3 ( 64 bits ou  8 bytes)..........: %s\n",chave3);
  printf("Chave criptografica 3 Expandida (128 bits ou 16 bytes): %s\n",chave3_expandida);  
  printf("Bloco de Saida ( 64 bits ou  8 bytes).................: %s\n",bloco_entrada);
  printf("Bloco de Saida Expandido (128 bits ou 16 bytes).......: %s\n",bloco_entrada_expandido);

  printf("\n\n\n\n");
  return 0;
}

