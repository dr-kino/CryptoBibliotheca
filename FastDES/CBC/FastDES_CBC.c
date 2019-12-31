#include <stdio.h>
#include <string.h>

#include "des.h"

#define ENCRIPTA 0
#define DECRIPTA 1

void INSERIR ( char *str_dst,
	    	   char *str_org,
			   int inic_dst, 
			   int inic_org,
			   int fim)
{ int i;
  inic_dst--;
  for (i=inic_org-1; i < fim+inic_org-1; i++)
	*(str_dst+inic_dst++)= *(str_org+i);

}
static  void Xoriza ( char *orig1, char *orig2_dst, int tam_origem )
{ int i;
  char c1;
  char c2;
  for ( i = 0 ; i < tam_origem ; i++ )
  { c1 = orig1[i];
    c2 = orig2_dst[i];
    orig2_dst[i] = c1 ^ c2;
  }
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

void FastDES_CBC (int modo,
				  char * bloco_entrada,
		          char * chave,
                  char * vetor_inicial,
		          char * bloco_saida)
{ int i;
  char entrada[9];
  char vetor[9];
  int tam_entrada;
  DES_KS k;

  tam_entrada = strlen(bloco_entrada);
  strcpy(vetor,vetor_inicial);

  if (modo == 0) // criptografar
  { deskey(k, chave, ENCRIPTA);
   for (i=0; i < tam_entrada; i+=8)
   { INSERIR(entrada,bloco_entrada,1,i+1,8);
     Xoriza(entrada,vetor,8); 
     des(k, vetor);
     INSERIR(bloco_saida,vetor,i+1,1,8);
   }
  }
  else
  { deskey(k, chave, DECRIPTA);
   for (i=0; i < tam_entrada; i+=8)
   { INSERIR(entrada,bloco_entrada,1,i+1,8);
     des(k, entrada);
	 Xoriza(entrada,vetor,8);
     INSERIR(bloco_saida,vetor,i+1,1,8);
	 INSERIR(vetor,bloco_entrada,1,i+1,8);
   }
  }
}

int main(int argc, char *argv[])
{ char bloco_entrada[17];
  char bloco_saida[17];
  char chave[9];
  char vetor_inicial[9];
  char bloco_saida_expandido[33];
  char bloco_entrada_expandido[33];
  char chave_expandida[17];
  char vetor_inicial_expandido[17];

  strcpy(bloco_entrada,"testandotestando");
  strcpy(vetor_inicial,"inicio12");

  strcpy(bloco_saida,"                ");
  strcpy(chave,"banespa1");
 
  // criptografar 
  FastDES_CBC (ENCRIPTA,bloco_entrada,chave,vetor_inicial,bloco_saida);
  bloco_saida[16]=0;

  Expandir(bloco_saida,bloco_saida_expandido,16);
  strcpy(bloco_saida,"                ");
  Compactar(bloco_saida_expandido,bloco_saida,32);
 
  Expandir(bloco_entrada,bloco_entrada_expandido,16);
  Expandir(chave,chave_expandida,8);
  Expandir(vetor_inicial,vetor_inicial_expandido,8);
  
  printf("CRIPTOGRAFAR\n\n");
  printf("Bloco de Entrada (128 bits ou  16 bytes).....: %s\n",bloco_entrada);
  printf("Entrada Expandida (256 bits ou 32 bytes).....: %s\n",bloco_entrada_expandido);
  printf("Chave criptografica ( 64 bits ou  8 bytes)...: %s\n",chave);
  printf("Chave cripto Expandida (128 bits ou 16 bytes): %s\n",chave_expandida);   
  printf("Vetor Inicial ( 64 bits ou  8 bytes).........: %s\n",vetor_inicial);
  printf("Vetor Expandido ( 128 bits ou  16 bytes).....: %s\n",vetor_inicial_expandido);   
  printf("Bloco de Saida (128 bits ou 16 bytes)........: %s\n",bloco_saida);
  printf("Saida Expandida (256 bits ou 32 bytes).......: %s\n",bloco_saida_expandido);

  //decriptografar
  FastDES_CBC (DECRIPTA,bloco_saida,chave,vetor_inicial,bloco_entrada);
  bloco_entrada[16]=0;
  
  printf("\n\nDECRIPTOGRAFAR\n\n");
  printf("Bloco de Entrada (128 bits ou  16 bytes).....: %s\n",bloco_saida);
  printf("Entrada Expandida (256 bits ou 32 bytes).....: %s\n",bloco_saida_expandido);
  printf("Chave criptografica ( 64 bits ou  8 bytes)...: %s\n",chave);
  printf("Chave cripto Expandida (128 bits ou 16 bytes): %s\n",chave_expandida);   
  printf("Vetor Inicial ( 64 bits ou  8 bytes).........: %s\n",vetor_inicial);
  printf("Vetor Expandido ( 128 bits ou  16 bytes).....: %s\n",vetor_inicial_expandido);   
  printf("Bloco de Saida (128 bits ou 16 bytes)........: %s\n",bloco_entrada);
  printf("Saida Expandida (256 bits ou 32 bytes).......: %s\n",bloco_entrada_expandido);
  printf("\n\n\n\n");
  return 0;
}

