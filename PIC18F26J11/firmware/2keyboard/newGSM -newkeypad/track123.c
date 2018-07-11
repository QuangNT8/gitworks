//================================================================
int8 detect_start_bits_track123_dir(int8 track,int8 direct)
{
   int8 buf[16],buf1[30],i,j,u,temp,temp0,countbyte=0,countshift=0,st,st0=0;
   int8 bitoftrack,byteand;
   int16 temp1;
   for(i=0;i<30;i++)
   {
      if(i<16)buf[i]=0;
      buf1[i]=0;
   }
   if(direct==0)
   {
      if(track==0)copybufferinc(14,posofstart,buffertrack1,buf);
         else copybufferinc(14,posofstart,buffertrack2,buf);
      for(i=0;i<14;i++)
      {
        countbyte++;
        if(buf[i]==0)i=15;
      }
   }
   else 
   {
      if(track==0) copybuffer(14,posofend,buffertrack1,buf);
         else copybuffer(14,posofend,buffertrack2,buf);
      for(i=0;i<14;i++)
      {
        countbyte++;
        if(buf[14-i]==0)i=15;
      }
   }
   countbyte--; 
   temp1=(int16)countbyte*8;
   if(track==0)
   {
      countbyte=(int8)temp1/7;    
      bitoftrack=7;
       if(direct==0) byteand=0b01111111;
         else byteand=0b11111110;
   }
      else 
      {
         //printf("\r\n");   
         //printf("countbyte=%u,direct=%u",countbyte,direct);  
         countbyte=(int8)temp1/5;
         bitoftrack=5;
         if(direct==0)byteand=0b00011111;
            else byteand=0b11111000;
         /*printf("\r\n");
         printf("buf1before=");
         for(j=0;j<=14;j++)
         {
            printf("%x,",buf[j]);
         }//*/
      }
   for(i=0;i<21;i++)
   {
          if(track==0) 
          {
            if(direct==0)
            {
               temp=(buf[0])&0b01111111;
               temp0=0b01000101;
            }
               else
               {
                   temp=(buf[14])&0b11111110;
                   temp0=0b10100010;
               }
          }
            else 
            {
               if(direct==0)
               {
                  temp=(buf[0])&0b00011111;
                  temp0=0b00001011;     //11010
               }
                  else
                  {
                     temp=(buf[14])&0b11111000;
                     temp0=0b11010000;
                  }
            }
          //else temp=buf[5]&0xf8;
          if(temp==temp0)
          {
            //printf("\r\n");
            //printf("byteand=%x",byteand);
            for(j=0;j<countbyte;j++)
            {   
               if(direct==0) buf1[j]=((buf[0])&byteand);
                  else buf1[j]=con_byte((buf[14])&byteand);
               for(u=0;u<bitoftrack;u++)
               {
                  if(direct==0) shift_right(buf,16,0);
                     else shift_left(buf,16,0);
               }
            }
            if(track==0)
            {
               st=checkcrcoftrack(1,countbyte-2,buf1);
               if((buf1[0]==0b01000101)&&(buf1[1]!=0b01111100)&&(buf1[1]!=0b00111110)&&(buf1[1]!=0b00011111))st0=1;
                  else st0=0;
            }
               else 
               {
                  if((buf1[0]==0b00001011)&&(buf1[1]<0x1a))st0=1;
                     else st0=0;
                  st=checkcrcoftrack(2,countbyte-2,buf1);
               }  
            if((st0==1)&&(st==1))
            {   
              //printf("\r\n");
              //printf("countshift=%u,buf1[0]=%x",countshift,buf1[0]);
              
              //dir=1;
              if(direct==0)con_dat=0;
               else con_dat=1;
              for(i=0;i<countshift;i++) 
              {
                  if(direct==0)
                  {
                     if(track==0)shift_right(buffertrack1,numbyteofbuffer,0);
                        else shift_right(buffertrack2,numbyteofbuffer,0);
                  }
                     else
                     {
                        if(track==0)shift_left(buffertrack1,numbyteofbuffer,0);
                           else shift_left(buffertrack2,numbyteofbuffer,0);   
                     }
              }
              i=21;
              //printf("buffertrack=%x,%x",buffertrack[posofstart],buffertrack[posofstart+1]);
              return(countshift);
            }
               else
                 {
                     if(direct==0)
                     {
                        if(track==0)copybufferinc(14,posofstart,buffertrack1,buf);
                           else copybufferinc(14,posofstart,buffertrack2,buf);
                        for(j=0;j<countshift;j++)shift_right(buf,16,0);
                     }
                        else 
                           {
                             if(track==0) copybuffer(14,posofend,buffertrack1,buf);
                              else copybuffer(14,posofend,buffertrack1,buf);
                              for(j=0;j<countshift;j++)shift_left(buf,16,0);
                           }
                 } 
            
          }    
            if(direct==0)shift_right(buf,16,0); 
               else shift_left(buf,16,0); 
            countshift++;
   }
   return(0);
}
//=================================================
int8 detect_end_bits_track123_dir(int8 track,int8 direct)
{
   int8 buf[16],buf1[30],i,j,u,temp,countbyte=0,countshift=0,st,temp0,st0=0;
   int8 bitoftrack,byteand;
   int16 temp1;
   for(i=0;i<30;i++)
   {
      if(i<16)buf[i]=0;
      buf1[i]=0;
   } 
   if(direct==0)
   {
      if(track==0)copybufferinc(14,posofstart,buffertrack1,buf);
         else copybufferinc(14,posofstart,buffertrack2,buf);
      for(i=0;i<14;i++)
      {
        countbyte++;
        if(buf[i]==0)i=15;
      }
   }
      else 
      {
         if(track==0)copybuffer(14,posofend,buffertrack1,buf);
            else copybuffer(14,posofend,buffertrack2,buf);
         for(i=0;i<14;i++)
         {
           countbyte++;
           if(buf[14-i]==0)i=15;
         }
      }
   countbyte--;
   temp1=(int16)countbyte*8;  
   if(track==0)
   {
      countbyte=(int8)temp1/7;
      bitoftrack=7;
      if(direct==0) byteand=0b11111110;//
      else byteand=0b01111111;
   }
      else
         {
            countbyte=(int8)temp1/5;
            bitoftrack=5;
            if(direct==0)byteand=0b11111000;
               else byteand=0b00011111;
         }
   for(i=0;i<30;i++)
   {
         if(track==0)
         {
            if(direct==0) 
            {
               temp=(buf[0])&0b11111110;
               temp0=0xf8;
            }
            else 
               {
                  temp=(buf[14])&0b01111111;
                  temp0=0x1f;
               }
         }
            else
               {
                  if(direct==0) 
                  {
                     temp=(buf[0])&0b11111000;
                     temp0=0xf8;
                  }
                  else 
                     {
                        temp=(buf[14])&0b00011111;
                        temp0=0x1f;
                     }                  
               }
        if(temp==temp0)
        {
          for(j=0;j<countbyte;j++)
          {
            if(direct==0) buf1[j]=con_byte((buf[0])&byteand);
               else buf1[j]=((buf[14])&byteand);
            for(u=0;u<bitoftrack;u++)
            {
              if(direct==0) shift_right(buf,16,0); 
                  else shift_left(buf,16,0); 
            }
          }
          if(track==0)
          {
            st=checkcrcoftrack(1,countbyte-2,buf1);
            if((buf1[0]==0b00011111)&&(buf1[1]!=0b01111100)&&(buf1[1]!=0b00111110))st0=1;
               else st0=0;
          }
            else 
               {
                   st=checkcrcoftrack(2,countbyte-2,buf1);
                   if((buf1[0]==0b00011111)&&(buf1[1]<0x1a))st0=1;
                     else st0=0;
               }
         if((st0==1)&&(st==1))
         {
            i=30;  
            for(i=0;i<countshift;i++) 
            {
               if(direct==0)
               {
                  if(track==0)shift_right(buffertrack1,numbyteofbuffer,0);
                     else shift_right(buffertrack2,numbyteofbuffer,0);
               }
                  else
                  {
                     if(track==0)shift_left(buffertrack1,numbyteofbuffer,0);
                        else shift_left(buffertrack2,numbyteofbuffer,0);
                  }
            }
            if(direct==0)con_dat=1;
               else con_dat=0;
            return(countshift);
         }
          else
            {
               if(direct==0)
               {
                  if(track==0)copybufferinc(14,posofstart,buffertrack1,buf);
                     else copybufferinc(14,posofstart,buffertrack2,buf);
                  for(j=0;j<countshift;j++)shift_right(buf,16,0);
               }
                  else
                  {
                     if(track==0)copybuffer(14,posofend,buffertrack1,buf);
                        else copybuffer(14,posofend,buffertrack2,buf);
                     for(j=0;j<countshift;j++)shift_left(buf,16,0);
                  }
            }
             
        }
        if(direct==0) shift_right(buf,16,0);  
         else shift_left(buf,16,0);  
        countshift++;
   }
   return(0);
}

