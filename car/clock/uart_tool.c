
#define NO_ECHO         0
#define EN_ECHO         1
#define STAR_ECHO       2
#define FAUX            0
#define VRAI            1
//=========================
unsigned char buffer_uart[100];
int8 UART_index=0;
int8 sms_unread_flag=0;
//=========================
//=========================
unsigned int USART_getstring(unsigned int8 echo, unsigned int8 nbcarmax,char* chaine)
{
   int8 c,i;
   unsigned int16 nbcar=0;
   unsigned int8 fini=FAUX;
      
   while(nbcar<nbcarmax&&!fini)
   {
      c = fgetc(COM2);//getc();
      switch(c)
      {
         case 0x7f:   
            if(nbcar>0)  
            {
               fputc(0x7f,COM2);//fputc(cdata, stream)
               nbcar--;
               fgetc(COM2);//getc();
            }
            else fputc(0x07,COM2);   
         break; 
         case 0x0D:  
               //keydebug_en=0;
               set_tris_a(0xff);
               for(i=nbcar;i<nbcarmax;i++)
               {
                 chaine[i]=0;
               }
               if(nbcar==0)return(FAUX);
               fini=VRAI;
         break;
         default:
            if(c>=32 && c<=126 && nbcar<(nbcarmax-1))  
            {
               chaine[nbcar]=c;
               nbcar++;
               if(echo==EN_ECHO) fputc(c,COM2);
                  else if(echo==STAR_ECHO) fputc('*',COM2);
            }
        // else
            //putc(0x07); 
      }
      
   }
   return(nbcar);
}
//===================================
int8 stringcomp(int8 *a, int8 *b)
{
   int8 *aa=a;
   int8 *bb=b;

   while(*aa !=0x00 && *bb !=0x00)
   {
      if(*aa!=*bb)
         return(1);
      aa++;
      bb++;
   }
   if(*aa!=*bb)return(1);
   return(0);
}
//====================================
unsigned int16 strtoi(char *str)
{
   unsigned int8 val = 0, digit;
   char c;

   while ((c = *str++) != 0x00) 
   {
        if (c >= '0' && c <= '9')
            digit = (unsigned int) (c - '0');
        else
            break;

        val = (val * 10) + digit;
    }

    return val;
}
