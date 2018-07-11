

//=======================================
void reset_status()
{
      posofstart=0;
      posofend=0;
      numbyteinbuf=0;
      dir=0; // dir=0 SL, dir=1 SR
      numbitshift=0;
      con_dat=0;
      postocopy=0;
      startinbuf=0;
}
//=======================================
void earse_buffer()
{
   int16 i;
   for(i=0;i<250;i++)
   {
      if(i<10)buffer_check[i]=0;
      if(i<79)Track1[i]=0;
      if(i<40)Track2[i]=0;
      if(i<numbyteofbuffer1)
      {
         buffertrack1[i]=0;
         buffertrack2[i]=0;
         //buffertrackmirro[i]=0;
      }
   }
}
//==================================
/*void del_buf(int8 numbyte,int8 *buf)
{
   int8 i;
   for(i=0;i<numbyte;i++)
   {
      buf[i]=0;
   }
}*/
//====================================
int8 con_byte(int8 inputbyte)
{
   int8 i,temp=0,temp1=0,temp2=0;
   temp=inputbyte;
   for(i=0;i<7;i++)
   {
      temp2=(temp>>i)&0x01;
      temp1=temp1|temp2;
      temp1<<=1;
   }
   temp1=temp1|temp>>7;
   return(temp1);
}
//======================================
int8 crcofbyte(int8 bitnum,unsigned int8 byteinput)
{
   int8 i;
   int1 st=1;
   for (i=0;i<bitnum;i++)
   {
      if(bit_test(byteinput,i)==1)st=~st;
   }
   if(bit_test(byteinput,bitnum)==st) st=1;
      else st=0;
   return(st);
}
//====================================
int8 checkcrcoftrack(int8 track,int8 endofbyte,*buf)
{
  int8 i,st=0;
  switch(track)
  {
      case 1:
        for (i=0;i<endofbyte;i++)
        {
           if(crcofbyte(6,buf[i])==1)
           {
               st=1;
           }
            else 
            {
               st=0;
               i=endofbyte;
               return(0);
            }
        }
      break;
      case 2:
        for (i=0;i<endofbyte;i++)
        {
           if(crcofbyte(4,buf[i])==1)
           {
               st=1;
           }
            else 
            {
               st=0;
               i=endofbyte;
               return(0);
            }
        }         
      break;
  }
  return(st);
}
//=====================================
int8 detect_datainbuffer(int8 *datin)
{
   int8 i,j;
   for(i=0;i<numbyteofbuffer1;i++)
   {
      
      if((datin[i])!=0)
      {
         j=i;
         i=numbyteofbuffer1+1;
      }
   }
   return(j);
}
//======================================
void SLbuffertrack_7bit()
{
   int8 u;
   for(u=0;u<7;u++)shift_left(buffertrack1,numbyteofbuffer,0);
}
//======================================
void SRbuffertrack_7bit()
{
   int8 u;
   for(u=0;u<7;u++)shift_right(buffertrack1,numbyteofbuffer,0);
}
//======================================
//======================================
void SLbuffertrack_5bit()
{
   int8 u;
   for(u=0;u<5;u++)shift_left(buffertrack2,numbyteofbuffer,0);
}
//======================================
void SRbuffertrack_5bit()
{
   int8 u;
   for(u=0;u<5;u++)shift_right(buffertrack2,numbyteofbuffer,0);
}
//======================================
void copy_buf2track(int8 track,int8 direct,int8 bytestart,int8 numbyte,int8 pos_byte,int8 conbyte,int8 *trackin,int8 *trackout)
{
   int8 i,temp;
   for(i=0;i<numbyte;i++)
   {
      if(conbyte==0)temp=(trackin[pos_byte]); 
         else temp=con_byte(trackin[pos_byte]);         
      if(bytestart==0)trackout[i]=temp;
         else trackout[bytestart-i]=temp;
      if(track==0)
      {
         if(direct==1) SRbuffertrack_7bit();
            else SLbuffertrack_7bit();
      }
         else
            {
               if(direct==1) SRbuffertrack_5bit();
                  else SLbuffertrack_5bit();             
            }
   }
   if(bytestart==0)trackout[numbyte+1]=0x1f;
      else trackout[(bytestart)+1]=0x1f;
}
//======================================
int8 checkcrcoftrack1(int8 numbyte)
{
   int8 st;
   st=checkcrcoftrack(1,numbyte,track1+1);
   if(st==1) return(1);
      else return(0);
}
//======================================
int8 checkcrcoftrack2(int8 numbyte)
{
   int8 st;
   st=checkcrcoftrack(2,10,track2+1);
   if(st==1)return(1);
      else return(0);
}
//======================================
int8 copybuffer(int8 numbyte,int8 pos_byte,int8 *bufin,int8 *bufout)
{
   int8 i;
   for(i=0;i<numbyte;i++)bufout[numbyte-i]=bufin[pos_byte-i];
   return(0);
}
//=======================================
int8 copybufferinc(int8 numbyte,int8 pos_byte,int8 *bufin,int8 *bufout)
{
   int8 i;
   for(i=0;i<numbyte;i++)bufout[i]=bufin[pos_byte+i];
   return(0);
}
//===========================================================
void copy2bufdec(int8 numbyte,int8*bufin,int8*bufout)
{
   int8 i;
   for(i=0;i<=numbyte;i++)bufout[numbyte-i]=bufin[posofstart+i];
}
//===============================================
//#include "track1.c"
//#include "track2.c"
#include "track123.c"
//==============================================
//int8 dir=0; // dir=0 SL, dir=1 SR
//numbitshift
//
//===============================================
int8 get_parameter(int8 track)
{
   int8 i,count_byte=0,temp;
   int16 temp2=0;
   if(track==0)posofstart=detect_datainbuffer(buffertrack1);
      else posofstart=detect_datainbuffer(buffertrack2);
   for(i=0;i<numbyteofbuffer;i++)
   {
      count_byte++;
      if(track==0)if((buffertrack1[posofstart+i+1]==0)&&(buffertrack1[posofstart+i+2]==0))i=numbyteofbuffer;
         else if((buffertrack2[posofstart+i+1]==0)&&(buffertrack2[posofstart+i+2]==0))i=numbyteofbuffer;
   }
   posofend=posofstart+count_byte-1;
   temp2=(int16)count_byte*8;
   if(track==0) numbyteinbuf=(int8)(temp2/7);  
      else numbyteinbuf=(int8)(temp2/5); 
   if(track==0)
   {
         temp=detect_start_bits_track123_dir(0,0);
         if(temp!=0) 
         {
            numbitshift=temp;
            startinbuf=0;
            con_dat=0;
            dir=1;
            postocopy=posofstart;
            return(1);
         }
         temp=detect_start_bits_track123_dir(0,1);
         if(temp!=0) 
         {
            numbitshift=temp;
            startinbuf=0;
            con_dat=1;
            dir=0;
            postocopy=posofend;
            return(2);
         }
         temp=detect_end_bits_track123_dir(0,0);
         if(temp!=0) 
         {
            numbitshift=temp;
            startinbuf=numbyteinbuf;
            con_dat=1;
            dir=1;
            postocopy=posofstart;
            return(2);
         }  
         temp=detect_end_bits_track123_dir(0,1);
         if(temp!=0) //ok
         {
            numbitshift=temp;
            startinbuf=numbyteinbuf;
            con_dat=0;
            dir=0;
            postocopy=posofend;
            return(2);
         }  
   }
      else 
      {
         temp=detect_start_bits_track123_dir(1,0);
         if(temp!=0) 
         {
            numbitshift=temp;
            startinbuf=0;
            con_dat=0;
            dir=1;
            postocopy=posofstart;
            return(1);
         }
         temp=detect_start_bits_track123_dir(1,1);
         if(temp!=0) 
         {
            numbitshift=temp;
            startinbuf=0;
            con_dat=1;
            dir=0;
            postocopy=posofend;
            return(2);
         }
         temp=detect_end_bits_track123_dir(1,0);
         if(temp!=0) 
         {
            numbitshift=temp;
            startinbuf=numbyteinbuf;
            con_dat=1;
            dir=1;
            postocopy=posofstart;
            return(3);
         }  
         temp=detect_end_bits_track123_dir(1,1);
         if(temp!=0) //ok
         {
            numbitshift=temp;
            startinbuf=numbyteinbuf;
            con_dat=0;
            dir=0;
            postocopy=posofend;
            return(4);
         }           
      }
//!   printf("\r\n");
//!   printf("get_parameter=%x,%x,%u,%u,%u",buffertrack[posofstart],buffertrack[posofend],numbyteinbuf,posofstart,count_byte);
//!   printf("\r\n");
//!   printf("shiftbit=%u,Dir=%u,pos_start=%u,pos_end=%u,numbyte=%u",temp,dir,posofstart,posofend,numbyteinbuf);
//!   printf("\r\n");
   return(0);
}
//===============================================
