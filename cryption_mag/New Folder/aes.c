/*************************************************************************************************************
*    Titre:         Module d'encryption AES
*    Fichier:       aes.c
*    Date:          07/02/2007
*************************************************************************************************************/

#include "aes.h"

/**************************************************************************************************************
 ** @file 	aes.c
 ** @brief	Module d'encryption AES
 **************************************************************************************************************/														

/*************************************************************************************************************
 	Fonction :		rcon 
 	Description :	Calcule la constante de round, renvoie, pour i valeur d'entree, la valeur 2 ** (i - 1).
 	Param�tres: 	Round
 	Valeur retour:	Constante de round
**************************************************************************************************************/
/** @brief	Calcule la constante de round, renvoie, pour i valeur d'entree, la valeur 2 ** (i - 1).
**************************************************************************************************************/
unsigned char rcon(int i)
{
	int p;
	unsigned char j=1;

	for (p = 0; p < i - 1; p++)
		j = j*2;
	return j;
}

/*************************************************************************************************************
 	Fonction :		sub_s_box 
 	Description :	Effectue le produit matriciel de l'octet c passe en parametre avec la matrice de la s_box:
 			
 					[1 0 0 0 1 1 1 1]
 					[1 1 0 0 0 1 1 1]
 					[1 1 1 0 0 0 1 1]
 					[1 1 1 1 0 0 0 1]
 					[1 1 1 1 1 0 0 0]
 					[0 1 1 1 1 1 0 0]
 					[0 0 1 1 1 1 1 0]
 					[0 0 0 1 1 1 1 1]
 	Param�tres: 	
 	Valeur retour:	
**************************************************************************************************************/
/** @brief	Effectue le produit matriciel de l'octet c passe en parametre avec la matrice de la s_box.
**************************************************************************************************************/
unsigned char sub_s_box(unsigned char c, unsigned char cmd)
{
	int i,nb_1 = 0;
	unsigned char s_box[8];
	unsigned char temp[8];
	unsigned char r[8];

	if (cmd == 'c')
	{
		s_box[0] = 143;	//matrice de la s_box (une case du tableau correspond a une ligne de la matrice)
		s_box[1] = 199;
		s_box[2] = 227;
		s_box[3] = 241;
		s_box[4] = 248;
		s_box[5] = 124;
		s_box[6] = 62;
		s_box[7] = 31;
	}
	else
	{
		s_box[0] = 37;	//matrice inverse de la s_box (decryptage)
		s_box[1] = 146;
		s_box[2] = 73;
		s_box[3] = 164;
		s_box[4] = 82;
		s_box[5] = 41;
		s_box[6] = 148;
		s_box[7] = 74;
	}

	for (i = 0; i < 8; i++) 
		temp[i] = s_box[i] & c; //ET bit a bit

	for (i = 0; i < 8; i++)
	{
		nb_1=0;
		do
		{
			if (temp[i] % 2 != 0)
			{
				temp[i] -= 1;
				nb_1++; //calcul du nombre de '1' dans le code binaire de temp[i]
			}

			temp[i] /= 2;
		} while (temp[i]>0);

		r[i] = nb_1 % 2;
	}

	//generation du r�sultat de la multiplication matricielle
	return (r[0]*rcon(8) + r[1]*rcon(7) + r[2]*rcon(6) + r[3]*rcon(5) + r[4]*rcon(4) + r[5]*rcon(3) + r[6]*rcon(2) + r[7]*rcon(1));
}

/*************************************************************************************************************
 	Fonction :		multi 
 	Description :	Effectue la multiplication bijective des deux caract�res pass�s en param�tre.
 	Param�tres: 	
 	Valeur retour:	
**************************************************************************************************************/
/** @brief	Effectue la multiplication bijective des deux caract�res pass�s en param�tre.
**************************************************************************************************************/
unsigned char multi(char a, char b)
{
	int bb[4] = {0,0,0,0}, p = 0, intemp;
	unsigned char temp[4];
	temp[0] = b;
	
	/* Ici on r�cup�re les 4 bits de poids faible de b.
	En fait, l'octet le plus �lev� (si on le regarde comme une valeur d�cimale) 
	que l'on passe dans b est 14 (cf. fonction mix_column) */
	
	do
	{
		if (temp[0] % 2 != 0) 
		{
			temp[0] -= 1;
			bb[p] = 1;
		}

		p++;
		temp[0] /= 2;
	} while (temp[0] > 0);

	temp[0] = a;
	temp[1] = a;

	/* on calcule les 4 multiples succ�ssifs de a */
	for(p = 1; p < 4; p++)	
	{
		intemp = temp[p] * 2;
		if (intemp > 255) temp[p] = (intemp - 256) ^ 27;
		else temp[p] = intemp;
		if (p < 3) temp[p + 1] = temp[p];
	}

	/* on met � 0 les cases de temp correspondant aux 0 du code binaire de b */
	for(p = 0; p < 4; p++)
	{
		if (bb[p] == 0) temp[p] = 0;
	}
	
	/* on somme (OU EXCLUSIF) les cases de temp pour g�n�rer le r�sultat de la multiplication */
	return temp[0] ^ temp[1] ^ temp[2] ^ temp[3];
}

/*************************************************************************************************************
 	Fonction :		expand_key
 	Description :	Effectue l'expansion de la cl�. 
 	Param�tres: 	key, la cl� du chiffreur et w, la cl� r�sultante - expanded key
 	Valeur retour:	
**************************************************************************************************************/
/** @brief	Effectue l'expansion de la cl�. 
**************************************************************************************************************/
int expand_key(unsigned char key[4*4], unsigned char w[4*4*11])
{
	int i,j;
	unsigned char temp=0;

	for(i = 0; i < 16; i++)
		w[i] = key[i];

	for(i = 16; i < 4*4*11; i++)
	{
		if (i % 16 < 4)
		{
			j = i % 4;
			if (j == 3) temp = w[i - 7];
			else temp = w[i - 3];
			temp = sub_s_box(temp, 'c');
			if (i % 16 == 0) temp = temp ^ rcon(i/16);
		}
		w[i] = w[i - 16] ^ temp;
	}

	return 0;
}

/*************************************************************************************************************
 	Fonction :		select_key
 	Description :	Applique simplement une partie de la expanded_key (du tableau du meme nom) a un round. 
 	Param�tres: 	
 	Valeur retour:	
**************************************************************************************************************/
/** @brief	Applique simplement une partie de la expanded_key (du tableau du meme nom) a un round.
**************************************************************************************************************/
int select_key(unsigned char expanded_key[4*4*11], unsigned char round_key[4][4], int round)
{
	int i,j;

	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			round_key[j][i] = expanded_key[(round * 16) + (4 * i) + j];
		}
	}

	return 0;
}
	
/*************************************************************************************************************
 	Fonction :		byte_sub
 	Description :	Effectue l'op�ration byte_sub.
 	Param�tres: 	
 	Valeur retour:	
**************************************************************************************************************/
/** @brief	Effectue l'op�ration byte_sub.
**************************************************************************************************************/
int byte_sub(unsigned char state[4][4], unsigned char cmd)
{
	int i,j;

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if(cmd == 'd') state[i][j] ^= 198; //ajout du vecteur constant (d�cryptage)
			state[i][j] = sub_s_box(state[i][j], cmd);
			if(cmd == 'c') state[i][j] ^= 198; //ajout du vecteur constant (cryptage)
		}
	}

	return 0;
}

/*************************************************************************************************************
 	Fonction :		shift_row
 	Description :	Effectue l'op�ration shift_row.
 	Param�tres: 	
 	Valeur retour:	
**************************************************************************************************************/
/** @brief	Effectue l'op�ration shift_row.
**************************************************************************************************************/
int shift_row(unsigned char state[4][4], unsigned char cmd)
{
	unsigned char temp;

	if (cmd == 'c') //cryptage
	{
		temp = state[1][0];
		state[1][0] = state[1][1];
		state[1][1] = state[1][2];
		state[1][2] = state[1][3];
		state[1][3] = temp;

		temp = state[2][0];
		state[2][0] = state[2][2];
		state[2][2] = temp;
		temp = state[2][1];
		state[2][1] = state[2][3];
		state[2][3] = temp;

		temp = state[3][3];
		state[3][3] = state[3][2];
		state[3][2] = state[3][1];
		state[3][1] = state[3][0];
		state[3][0] = temp;
	}
		
	else //d�cryptage
	{
		temp = state[1][3];
		state[1][3] = state[1][2];
		state[1][2] = state[1][1];
		state[1][1] = state[1][0];
		state[1][0] = temp;

		temp = state[2][0];
		state[2][0] = state[2][2];
		state[2][2] = temp;
		temp = state[2][1];
		state[2][1] = state[2][3];
		state[2][3] = temp;

		temp = state[3][0];
		state[3][0] = state[3][1];
		state[3][1] = state[3][2];
		state[3][2] = state[3][3];
		state[3][3] = temp;
	}

	return 0;
}

/*************************************************************************************************************
 	Fonction :		mix_column
 	Description :	Effectue l'op�ration mix_column.
 	Param�tres: 	
 	Valeur retour:	
**************************************************************************************************************/
/** @brief	Effectue l'op�ration mix_column.
**************************************************************************************************************/
int mix_column(unsigned char state[4][4], unsigned char cmd)
{
	int i,j;
	unsigned char temp[4][4];

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++) temp[i][j] = state[i][j];
	}

	if (cmd == 'c') //cryptage
	{
		state[0][0] = multi(temp[0][0],2) ^ multi(temp[1][0],3) ^ multi(temp[2][0],1) ^ multi(temp[3][0],1);
		state[1][0] = multi(temp[0][0],1) ^ multi(temp[1][0],2) ^ multi(temp[2][0],3) ^ multi(temp[3][0],1);
		state[2][0] = multi(temp[0][0],1) ^ multi(temp[1][0],1) ^ multi(temp[2][0],2) ^ multi(temp[3][0],3);
		state[3][0] = multi(temp[0][0],3) ^ multi(temp[1][0],1) ^ multi(temp[2][0],1) ^ multi(temp[3][0],2);

		state[0][1] = multi(temp[0][1],2) ^ multi(temp[1][1],3) ^ multi(temp[2][1],1) ^ multi(temp[3][1],1);
		state[1][1] = multi(temp[0][1],1) ^ multi(temp[1][1],2) ^ multi(temp[2][1],3) ^ multi(temp[3][1],1);
		state[2][1] = multi(temp[0][1],1) ^ multi(temp[1][1],1) ^ multi(temp[2][1],2) ^ multi(temp[3][1],3);
		state[3][1] = multi(temp[0][1],3) ^ multi(temp[1][1],1) ^ multi(temp[2][1],1) ^ multi(temp[3][1],2);

		state[0][2] = multi(temp[0][2],2) ^ multi(temp[1][2],3) ^ multi(temp[2][2],1) ^ multi(temp[3][2],1);
		state[1][2] = multi(temp[0][2],1) ^ multi(temp[1][2],2) ^ multi(temp[2][2],3) ^ multi(temp[3][2],1);
		state[2][2] = multi(temp[0][2],1) ^ multi(temp[1][2],1) ^ multi(temp[2][2],2) ^ multi(temp[3][2],3);
		state[3][2] = multi(temp[0][2],3) ^ multi(temp[1][2],1) ^ multi(temp[2][2],1) ^ multi(temp[3][2],2);

		state[0][3] = multi(temp[0][3],2) ^ multi(temp[1][3],3) ^ multi(temp[2][3],1) ^ multi(temp[3][3],1);
		state[1][3] = multi(temp[0][3],1) ^ multi(temp[1][3],2) ^ multi(temp[2][3],3) ^ multi(temp[3][3],1);
		state[2][3] = multi(temp[0][3],1) ^ multi(temp[1][3],1) ^ multi(temp[2][3],2) ^ multi(temp[3][3],3);
		state[3][3] = multi(temp[0][3],3) ^ multi(temp[1][3],1) ^ multi(temp[2][3],1) ^ multi(temp[3][3],2);
	}
		
	else //d�cryptage
	{
		state[0][0] = multi(temp[0][0],14) ^ multi(temp[1][0],11) ^ multi(temp[2][0],13) ^ multi(temp[3][0],9);
		state[1][0] = multi(temp[0][0],9) ^ multi(temp[1][0],14) ^ multi(temp[2][0],11) ^ multi(temp[3][0],13);
		state[2][0] = multi(temp[0][0],13) ^ multi(temp[1][0],9) ^ multi(temp[2][0],14) ^ multi(temp[3][0],11);
		state[3][0] = multi(temp[0][0],11) ^ multi(temp[1][0],13) ^ multi(temp[2][0],9) ^ multi(temp[3][0],14);

		state[0][1] = multi(temp[0][1],14) ^ multi(temp[1][1],11) ^ multi(temp[2][1],13) ^ multi(temp[3][1],9);
		state[1][1] = multi(temp[0][1],9) ^ multi(temp[1][1],14) ^ multi(temp[2][1],11) ^ multi(temp[3][1],13);
		state[2][1] = multi(temp[0][1],13) ^ multi(temp[1][1],9) ^ multi(temp[2][1],14) ^ multi(temp[3][1],11);
		state[3][1] = multi(temp[0][1],11) ^ multi(temp[1][1],13) ^ multi(temp[2][1],9) ^ multi(temp[3][1],14);

		state[0][2] = multi(temp[0][2],14) ^ multi(temp[1][2],11) ^ multi(temp[2][2],13) ^ multi(temp[3][2],9);
		state[1][2] = multi(temp[0][2],9) ^ multi(temp[1][2],14) ^ multi(temp[2][2],11) ^ multi(temp[3][2],13);
		state[2][2] = multi(temp[0][2],13) ^ multi(temp[1][2],9) ^ multi(temp[2][2],14) ^ multi(temp[3][2],11);
		state[3][2] = multi(temp[0][2],11) ^ multi(temp[1][2],13) ^ multi(temp[2][2],9) ^ multi(temp[3][2],14);

		state[0][3] = multi(temp[0][3],14) ^ multi(temp[1][3],11) ^ multi(temp[2][3],13) ^ multi(temp[3][3],9);
		state[1][3] = multi(temp[0][3],9) ^ multi(temp[1][3],14) ^ multi(temp[2][3],11) ^ multi(temp[3][3],13);
		state[2][3] = multi(temp[0][3],13) ^ multi(temp[1][3],9) ^ multi(temp[2][3],14) ^ multi(temp[3][3],11);
		state[3][3] = multi(temp[0][3],11) ^ multi(temp[1][3],13) ^ multi(temp[2][3],9) ^ multi(temp[3][3],14);
	}
	return 0;
}

/*************************************************************************************************************
 	Fonction :		add_round_key
 	Description :	Effectue l'op�ration add_round_key.
 	Param�tres: 	
 	Valeur retour:	
**************************************************************************************************************/
/** @brief	Effectue l'op�ration add_round_key.
**************************************************************************************************************/
int add_round_key(unsigned char state[4][4], unsigned char round_key[4][4])
{
	int i,j;

	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			state[i][j] ^= round_key[i][j];
		}
	}

	return 0;
}

/*************************************************************************************************************
 	Fonction :		aes_round
 	Description :	Effectue un round comprenant les 4 op�rations. 
 					(Sauf le dernier qui ne contient pas mix_column).
 	Param�tres: 	
 	Valeur retour:	
**************************************************************************************************************/
/** @brief	Effectue un round comprenant les 4 op�rations.
**************************************************************************************************************/
int aes_round(unsigned char state[4][4], unsigned char round_key[4][4], int round, unsigned char cmd)
{
	if (cmd == 'c')
	{
		byte_sub(state, cmd);
		shift_row(state, cmd);
		if (round != 9) mix_column(state, cmd);
		add_round_key(state, round_key);
	}
	else
	{
		add_round_key(state, round_key);
		if (round != 9) mix_column(state, cmd);
		shift_row(state, cmd);
		byte_sub(state, cmd);
	}

	return 0;
}

/*************************************************************************************************************
 	Fonction :		rijndael
 	Description :	Effectue le cryptage ou le d�cryptage.
 	Param�tres: 	cmd (commande), data (bloc de donn�es de 16 bytes), cl� (cipher_key)
 	Valeur retour:	
**************************************************************************************************************/
/** @brief	Effectue le cryptage ou le d�cryptage.
**************************************************************************************************************/
int rijndael(unsigned char cmd, unsigned char data[16], unsigned char cipher_key[16])
{
	int i, j;
	unsigned char state[4][4];
	unsigned char expanded_key[11*16];
	unsigned char round_key[4][4];

	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			state[i][j] = data[i * 4 + j];
		}
	}

	expand_key(cipher_key, expanded_key);
	
	for(i = 0; i < 10; i++)
	{
		if (cmd == 'd') j = 9 - i;
		else j = i;

		select_key(expanded_key, round_key, j);
		aes_round(state, round_key, j, cmd);
	}

	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			data[i * 4 + j] = state[i][j];
		}
	}
	
	return 0;
}