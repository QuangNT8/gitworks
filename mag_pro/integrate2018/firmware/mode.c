
#define LOGOFF                0
#define LOGON                 1
#define get_data              2
#define changer_pass          3
//==============================

int8 mode=0;
int8 ee_dat=0;
int16 count_card=1,count_card_tg=0;
int8 mode_sl=0;
int32 addr_key=0;
char console[16];
//===============================
void rstfact()
{
   write_ext_eeprom(strobe_delaytime,3); 
   //write_ext_eeprom(strobe_delaykb,100);
   //write_ext_eeprom(KB_time,2);
   write_ext_eeprom(strobe_Master_SLV,1);   
   mode_sl=read_ext_eeprom(strobe_Master_SLV); 
}
//================================
void adminmode()
{
   int8 index,i;
   int16 countchar;
   int8 temp[50];
      
      //printf("Admin>");
      printf("%s>",console);
      USART_getstring(EN_ECHO,20,buffer_uart);
      printf("\n\r");
      strcpy(buffer2,"d");
      index=0;
      ee_dat=0;
      count_card=1;
      if(!stringcomp(buffer_uart,buffer2))   // download data
      {
         unsigned long adr=0;
         //unsigned int count=0;
         rec[0]=0;
         count_card_tg=get_countcard();
         adr=count_card*numdata;
         addr_key=EEPROM_KEY_ST;
         while(count_card<=count_card_tg)
         {
            adr=count_card*numdata+ptr_start;
            I2CEEPROM_read((unsigned int16)(adr-numdata),numdata,rec);
            fprintf(COM2,"%ld)%02i/%02i %02i:%02i:%02i",count_card,rec[0],rec[1],rec[2],rec[3],rec[4]);
            ee_dat=0;
            index=0;
            fprintf(COM2,"\r\nTrack1 Data:\r\n");
            while((index<numbyteoftrack1)&&(ee_dat!='?'))
            {
               ee_dat = rec[index+5];
               if((ee_dat>31)&&(ee_dat<127))fputc(ee_dat,COM2);
               index++;
               if(ee_dat==0)break;
            }
            fprintf(COM2,"\r\nTrack2 Data:\r\n");
            index=0;
            ee_dat=0;
            while((index<numbyteoftrack2) && (ee_dat!='?'))
            {
               ee_dat = rec[index+5+numbyteoftrack1];
               if((ee_dat>31)&&(ee_dat<127)) fputc(ee_dat,COM2);
               index++;
               if(ee_dat==0)break;
            }
            index=0;
            //ee_dat=0;
            fprintf(COM2,"\r\nPIN Number:\r\n");
            //fprintf(COM2,"addr_key =%lu \n\r",addr_key); 
            //fprintf(COM2," ptr_card_key=%lu \n\r",ptr_card_key);
            
            if((ptr_card_key>addr_key)) //&&
            {
               i=0;
               //fprintf(COM2,"%02i/%02i %02i:%02i:%02i -> ",read_ext_eeprom(addr_key+i++),read_ext_eeprom(addr_key+i++),read_ext_eeprom(addr_key+i++),read_ext_eeprom(addr_key+i++),read_ext_eeprom(addr_key+i++));
               do
               {
                  ee_dat=read_ext_eeprom(i+addr_key);
                  if(cryption_enable==0)
                  {
                      if(((ee_dat>47)&&(ee_dat<58))||((ee_dat=='#')||(ee_dat=='*'))||(ee_dat>64)&&(ee_dat<91))
                      {
                         fprintf(COM2,"%c",ee_dat);
                      }
                  }
                  else
                  {
                      fprintf(COM2," %x",ee_dat);
                  }
                  i++;
               }
               while((i<wideofkeystore)&&(ee_dat!=0));
               //fprintf(COM2," addr_key=%lu\n\r",addr_key);
               addr_key=addr_key+wideofkeystore;
               fprintf(COM2,"\n\r");
            }
            count_card++;
         }
      } 
      strcpy(buffer2,"f");
      if(!stringcomp(buffer_uart,buffer2))   // format mem
      {
         //mode=LOGOFF;
         fprintf(COM2,"FORMATTING...");
         format_eepromext();
         fprintf(COM2," exit\n\r"); 
      }
      //#if defined(PIC26)
      strcpy(buffer2,"f mem");
      if(!stringcomp(buffer_uart,buffer2))   // format mem
      {
         //mode=LOGOFF;
         fprintf(COM2,"FORMATTING...");
         ease_eeprom();
         fprintf(COM2," exit\n\r"); 
      }    
     strcpy(buffer2,"RSPW");
      if(!stringcomp(buffer_uart,buffer2))   // format mem
      {
         //mode=LOGOFF;
         fprintf(COM2,"reseting password...");
         reset_password();
         init_password();
         fprintf(COM2," exit\n\r"); 
      }   
      strcpy(buffer2,"!kbd!");
      if(!stringcomp(buffer_uart,buffer2))   // format mem
      {
         keydebug_en=1;
         //en_getpin;
         fprintf(COM2,"Key board testing\n\r");
      }  
      strcpy(buffer2,"rsfact");
      if(!stringcomp(buffer_uart,buffer2))   // format mem
      {
         //keydebug_en=1;
         //en_getpin;
         fprintf(COM2,"reset factory\n\r");
         rstfact();
      }  
      //#endif
      strcpy(buffer2,"e");
      if(!stringcomp(buffer_uart,buffer2))   // download data
      {
         mode=LOGOFF;
         keydebug_en=0;
         set_tris_a(0xff);
         //dis_getpin;
         fprintf(COM2," exit\n\r"); 
      }
      
      strcpy(buffer2,"n");
      if(!stringcomp(buffer_uart,buffer2))   // format mem
      {
         fprintf(COM2,"New console name>");
         USART_getstring(EN_ECHO,wide_strobe_nameconsole, console);
         EEPROM_write(strobe_nameconsole,wide_strobe_nameconsole,console);
         fprintf(COM2," OK\n\r");
      }
      
      strcpy(buffer2,"c");
      if(!stringcomp(buffer_uart,buffer2))   //show clock
      {
         mode_sl=read_ext_eeprom(strobe_Master_SLV);
         if(mode_sl==0) fprintf(COM2,"[M]");
         else fprintf(COM2,"[S]");  
         delaycharaction=read_ext_eeprom(strobe_delaytime); 
         fprintf(COM2,"[%u]",delaycharaction);
         
         fprintf(COM2,"%02i/%02i %02i:%02i:%02i",date,mon,h,min,sec);
         
         fprintf(COM2,"\n\r");
      }
      strcpy(buffer2,"sc");
      if(!stringcomp(buffer_uart,buffer2))   //setting clock
      {
         fprintf(COM2,"Year>");
         USART_getstring(EN_ECHO, 3, temp);
         year=(unsigned int8)strtoi(temp);
         fprintf(COM2,"Month>");
         USART_getstring(EN_ECHO, 3, temp);
         mon=(unsigned int8)strtoi(temp);
         fprintf(COM2,"Date>");
         USART_getstring(EN_ECHO, 3, temp);
         date = (unsigned int8)strtoi(temp);
         fprintf(COM2,"Day>");
         USART_getstring(EN_ECHO, 3, temp);
         day = (unsigned int8)strtoi(temp);
         fprintf(COM2,"Hour>");
         USART_getstring(EN_ECHO, 3, temp);
         h = (unsigned int8)strtoi(temp);
         fprintf(COM2,"Minutes>");
         USART_getstring(EN_ECHO, 3, temp);
         min = (unsigned int8)strtoi(temp);                  
         fprintf(COM2,"Seconds>");
         USART_getstring(EN_ECHO, 3, temp);
         sec = (unsigned int8)strtoi(temp);                  
         fprintf(COM2,"\n\r");
         /* rtc_set_datetime(date,mon,year,day,h,min); */
      }
      strcpy(buffer2,"MS");
      if(!stringcomp(buffer_uart,buffer2))
      {
          fprintf(COM2,"Press '1' to select Slaver mode");
          fprintf(COM2,"\n\r");
          fprintf(COM2,"Press '0' to select Master mode");
          fprintf(COM2,"\n\r");
          USART_getstring(EN_ECHO, 3, temp);
          mode_sl = (unsigned int8)strtoi(temp);
          write_ext_eeprom(strobe_Master_SLV,mode_sl);
          set_tris_a(0xff);
          fprintf(COM2,"\n\r");
      }
     
      strcpy(buffer2,"tran time");
      if(!stringcomp(buffer_uart,buffer2))
      {
          fprintf(COM2,"Input Number");
          fprintf(COM2,"\n\r");
          USART_getstring(EN_ECHO, 3, temp);
          delaycharaction = (unsigned int8)strtoi(temp);
          write_ext_eeprom(strobe_delaytime,delaycharaction);
          fprintf(COM2,"\n\r");
      }   
      /*strcpy(buffer2,"delaykb");
      if(!stringcomp(buffer_uart,buffer2))
      {
          fprintf(COM2,"Input Number");
          fprintf(COM2,"\n\r");
          USART_getstring(EN_ECHO, 3, temp);
          delaykey = (unsigned int8)strtoi(temp);
          write_ext_eeprom(strobe_delaykb,delaykey);
          //fprintf(COM2,"delaykey=%u\n\r",delaykey);
          fprintf(COM2,"\n\r");
      } */
      strcpy(buffer2,"kofkb");
      if(!stringcomp(buffer_uart,buffer2))
      {
          fprintf(COM2,"Press '1' to select the new keyboard");
          fprintf(COM2,"\n\r");
          fprintf(COM2,"Press '0' to select the old keyboard");
          fprintf(COM2,"\n\r");
          USART_getstring(EN_ECHO, 3, temp);
          type_KB=(unsigned int8)strtoi(temp);
          write_ext_eeprom(kindofKB,type_KB);
          //fprintf(COM2,"delaykey=%u\n\r",delaykey);
          fprintf(COM2,"\n\r");
      }//*/  
      /*strcpy(buffer2,"KBlogic");
      if(!stringcomp(buffer_uart,buffer2))
      {
          fprintf(COM2,"Input Number");
          fprintf(COM2,"\n\r");
          USART_getstring(EN_ECHO, 3, temp);
          write_ext_eeprom(KB_logic,(unsigned int8)strtoi(temp));
          //fprintf(COM2,"delaykey=%u\n\r",delaykey);
          fprintf(COM2,"\n\r");
      }  */
      strcpy(buffer2,"h");
      if(!stringcomp(buffer_uart,buffer2))   //setting clock
      {
         printf("h-> Help\n\r");
         printf("sc-> Set Clock\n\r");
         printf("c-> show Clock\n\r");
         printf("password-> changing PASSWORD\n\r");
         printf("n -> change console Name\n\r");
         printf("f-> Format\n\r");
         printf("e-> Exit\n\r");  
         printf("tran time-> time of each transaction\n\r");
         printf("debug-> on/off debug mode\n\r");     
         //printf("delaykb %d -> Keyboard's Timing\n\r",read_ext_eeprom(strobe_delaykb)); 
         printf("kofkb %d -> select kind of keyboard\n\r",read_ext_eeprom(kindofKB));
         printf("rsfact-> reset factory\n\r");     
         //printf("KBlogic %d -> Keyboard's logic\n\r",read_ext_eeprom(KB_logic)); 
      }
      strcpy(buffer2,"password");
      if(!stringcomp(buffer_uart,buffer2)) 
      {
         fprintf(COM2,"old password:");
         EEPROM_read(strobe_pass_addr,20,password);
         USART_getstring(STAR_ECHO, 20, entpassword);
         if(!stringcomp(entpassword,password))
         {
            fprintf(COM2,"\n\rnew:");
            USART_getstring(STAR_ECHO, 20, entpassword);
            fprintf(COM2,"\n\ragain:");
            USART_getstring(STAR_ECHO, 20, password);
            if(!stringcomp(entpassword,password))
            {
               EEPROM_write(strobe_pass_addr,20,password);
               fprintf(COM2,"\n\rOK\n\r");   
            }
               else
               {
                  fprintf(COM2,"\n\rERR\n\r");
               }
         }
            else
            {
               fprintf(COM2," X\n\r");
            }   
      }
        strcpy(buffer2,"crypto");
        if(!stringcomp(buffer_uart,buffer2)) 
        {
            memset(crypto_key,0,sizeof(crypto_key));
            fprintf(COM2,"New crypto key>");
            countchar = (int16)USART_getstring(EN_ECHO, CRYPTO_KEY_SIZE, crypto_key);
            EEPROM_write(strobe_crypto_key,CRYPTO_KEY_SIZE,crypto_key);
            
            if(countchar>2)
            {
                cryption_enable = 1;
                write_ext_eeprom(crypto_en,cryption_enable);                   
                fprintf(COM2,"\n\rNew crypto key: ");
                for(i=0;i<CRYPTO_KEY_SIZE;i++)
                {
                    fprintf(COM2,"%c",read_ext_eeprom(strobe_crypto_key+i));
                }
                fprintf(COM2,"\n\r");
            }
            else
            {
                cryption_enable = 0;
                write_ext_eeprom(crypto_en,cryption_enable);
                fprintf(COM2,"\n\rcrypto is disable\n\r");
            }
        }
      #if defined(debug)
      strcpy(buffer2,"debug");
      if(!stringcomp(buffer_uart,buffer2))
      {
          fprintf(COM2,"Press '1' to enable debug mode");
          fprintf(COM2,"\n\r");
          fprintf(COM2,"Press '0' to disable debug mode");
          fprintf(COM2,"\n\r");
          USART_getstring(EN_ECHO, 3, temp);
          debugmode = (unsigned int8)strtoi(temp);
          write_ext_eeprom(strobe_debugmode,debugmode);
          fprintf(COM2,"\n\r");
      }   
      #endif
}
//===============================================

