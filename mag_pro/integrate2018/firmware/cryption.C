#include "STRING.H"
#include "aes.h"
//===========================================================
unsigned int crcccitt(unsigned int crc_init, char donnee[], unsigned long int size)
{
   char index;
    unsigned long int i;

	for(i = 0; i < size; i++)
	{
		crc_init = (unsigned int)(crc_init ^ (donnee[i] << 8));
		for(index = 0; index <= 7; index++)
		{
		   if(crc_init & 0x8000)
			   crc_init = (unsigned int)((crc_init << 1) ^ 0x1021);
		   else
			   crc_init = (unsigned int)(crc_init << 1);
		}
	}
    return crc_init;
}
//===========================================================

//===========================================================
void write_kb_i2ceeprom(void)
{
      unsigned long adr;
      unsigned int i;
      unsigned int crc;
      int8 rec[34];
      int8 retval;

      rec[0]=33;        	/* Prochaine chaine dans 33 cars. */
      rec[1]=32;           	/* Nombre de bytes de la chaine */
      rec[2]=0;
      rec[3]=0;          	/* Mois en byte */
      rec[4]=0;;         	/* Jour en byte */
      rec[5]=0;          	/* Heure en byte */
      rec[6]=0;          	/* Minute en byte */
      rec[7]=0;          	/* Seconde en byte */
      rec[8]=0x01;                                 /* Type Clavier */

      crc=crcccitt(0, &rec[1], 30);   /* Calcul du CRC 16 (CCITT) de la chaine */

      rec[31]=(unsigned char)(crc & 0x00FF);
      rec[32]=(unsigned char)((crc & 0xFF00)>>8);

      rec[33]=0xFF;            /* Terminaison de la liste par 0xFF */

      /** Encryption de la chaine */
      rijndael('c', (unsigned char *)&rec[1], (unsigned char *)&crypto_key[0]);
      rijndael('c', (unsigned char *)&rec[17], (unsigned char *)&crypto_key[16]);

      //read_eeptr(&adr);         /* Lecture de l'adresse de fin de liste */

      EEPROM_write(adr,34,rec);   /* Écriture de la chaine */
}
