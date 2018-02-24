#include<TimeLib.h>

int end_device_status=0,a=0, count=0, incoming_data=0, sum=0, temp=0;    //from packet tracing code    b is replaced by sum

int start_time=0, end_time=0;
int length_of_frame=1;
int lenght_of_the_frame=0;
int frame[100];
int device=0;
int xbee_address[2][10];

void setup() {
  // put your setup code here, to run once:
  
  xbee_address[0][0]=0x0;
  xbee_address[0][1]=0x13;
  xbee_address[0][2]=0xA2;
  xbee_address[0][3]=0x0;
  xbee_address[0][4]=0x40;
  xbee_address[0][5]=0xE8;
  xbee_address[0][6]=0x29;
  xbee_address[0][7]=0x63;
  xbee_address[0][8]=0xFF;
  xbee_address[0][9]=0xFE;
  
  xbee_address[1][0]=0x0;
  xbee_address[1][1]=0x13;
  xbee_address[1][2]=0xA2;
  xbee_address[1][3]=0x0;
  xbee_address[1][4]=0x40;
  xbee_address[1][5]=0xD6;
  xbee_address[1][6]=0xA1;
  xbee_address[1][7]=0xDF;
  xbee_address[1][8]=0xFF;
  xbee_address[1][9]=0xFE; 
  
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  if(a==0)
  {
    //aayi=0;
    a++;
    start_time=now();
    Initial_frame(device, xbee_address);
  }
  data:
  if(Serial.available())
  {
    
  if(count==0)
  {
    Serial.print("\n");
  } 
    incoming_data=Serial.read();
    frame[count]=incoming_data;                                   //storing data into "frame array"
    count++;
    Serial.print(incoming_data, HEX);
    Serial.print(" ");
    
    /*if(incoming_data!=0x7E && count==1)
    {
      goto data;
      lenght_of_the_frame=0;
      Serial.print("Frame Started\n");
    }*/
      if(count==2 || count==3)
      {
        length_of_frame = length_of_frame + incoming_data;
      }
    if(count>3)
    {
         sum=( 0xFF - ( temp & 0xFF) );
         if(incoming_data==sum && count==length_of_frame)
         {
            Serial.print("Frame Ends\n");
            Serial.print("No. of Bytes in a frame: ");
            Serial.print(count);
            Serial.print("\n");
            temp=0;
            lenght_of_the_frame=count;
            count=0;
            start_time=now();
         }
         if(incoming_data!=sum)
         {
            temp = temp + incoming_data;
         }
    }
    
    if(count>0)
    {
      goto data;
    }
    if(lenght_of_the_frame==0)
    {
      Serial.print("\n\n");
      goto data;
    }
    
      if(frame[3]==0x97)// && aayi==0)                       //Frame type=Remote command response
      {
        //aayi=1;
        end_device_status=Frame1(lenght_of_the_frame, frame);
      
        if(end_device_status==1)
        {
          device1_sp(device, xbee_address);             //
          device1_st(device, xbee_address);             //
          device1_ir(device, xbee_address);             //
          device1_sm(device, xbee_address);             //
          
          device2_sp(~device, xbee_address);            //
          device2_st(~device, xbee_address);            //
          device2_sm(~device, xbee_address);            //
        }
        
      }
      
      if(frame[3]==12)
      {
        Serial.print("\n\n...........DATA............\n\n");
      }
  }
  end_time=now();
  if(end_time>=3)
  {
    device=~device;
    a=0;
  }
}

int Frame1(int lenght, int frame[])
{
    //Serial.print(frame[lenght-3], HEX);
   if(frame[lenght-2]==0x0)
   {
     
      Serial.print("\n\nStatus is OK\n\n");
      return 1;
   }
   else
   {
    return 0;
   }
}


void Initial_frame(int device, int xbee_address[2][10])
{
  Serial.write(0x7E);             //
  Serial.write(0x0);              
  Serial.write(0x10);
  Serial.write(0x17);
  Serial.write(0x01);

  for(int i=0; i<10; i++)
  {
    Serial.write(xbee_address[0][i]);
  }
  Serial.write(0x02);

  Serial.write(0x44);
  Serial.write(0x31);

  Serial.write(0x04);
  Serial.write(0x06);
}

void device1_sm(int device, int xbee_address[2][10])
{
  Serial.write(0x7E);             //7E 00 10 17 00 00 13 A2 00 40 E8 29 63 FF FE 02 53 4D 05 DB
  Serial.write(0x0);              
  Serial.write(0x10);
  Serial.write(0x17);
  Serial.write(0x0);
  
 for(int i=0; i<10; i++)
 {
    Serial.write(xbee_address[0][i]);
 }

  Serial.write(0x02);
  
  Serial.write(0x53);
  Serial.write(0x4D);

  Serial.write(0x05);
  Serial.write(0xDB);
}

void device1_sp(int device, int xbee_address[2][10])
{
  Serial.write(0x7E);             //7E 00 10 17 00 00 13 A2 00 40 E8 29 63 FF FE 02 53 50 64 79    1sec
  Serial.write(0x0);              
  Serial.write(0x10);
  Serial.write(0x17);
  Serial.write(0x0);
  
 for(int i=0; i<10; i++)
 {
    Serial.write(xbee_address[0][i]);
 }
 
  Serial.write(0x02);

  Serial.write(0x53);
  Serial.write(0x50);

  Serial.write(0x64);
  Serial.write(0x79);
}

void device1_st(int device, int xbee_address[2][10])
{
  Serial.write(0x7E);             //7E 00 11 17 00 00 13 A2 00 40 E8 29 63 FF FE 02 53 54 03 F2 E4    1.01sec
  Serial.write(0x0);              
  Serial.write(0x11);
  Serial.write(0x17);
  Serial.write(0x0);
  
  for(int i=0; i<10; i++)
  {
    Serial.write(xbee_address[0][i]);
  }
 
  Serial.write(0x02);

  Serial.write(0x53);
  Serial.write(0x54);

  Serial.write(0x03);
  Serial.write(0xF2);
  Serial.write(0xE4);
}


void device1_ir(int device, int xbee_address[2][10])
{
  Serial.write(0x7E);             //7E 00 11 17 00 00 13 A2 00 40 E8 29 63 FF FE 02 49 52 03 E8 FA    1sec
  Serial.write(0x0);              
  Serial.write(0x11);
  Serial.write(0x17);
  Serial.write(0x0);
  
  for(int i=0; i<10; i++)
  {
    Serial.write(xbee_address[0][i]);
  }
 
  Serial.write(0x02);

  Serial.write(0x49);
  Serial.write(0x52);

  Serial.write(0x03);
  Serial.write(0xE8);
  Serial.write(0xFA);
}

void device2_sm(int device, int xbee_address[2][10])
{
  Serial.write(0x7E);             //7E 00 10 17 00 00 13 A2 00 40 E8 29 63 FF FE 02 53 4D 05 DB
  Serial.write(0x0);              
  Serial.write(0x10);
  Serial.write(0x17);
  Serial.write(0x0);
  
 for(int i=0; i<10; i++)
 {
    Serial.write(xbee_address[1][i]);
 }
 
  Serial.write(0x02);

  Serial.write(0x53);
  Serial.write(0x4D);

  Serial.write(0x05);
  Serial.write(0xDB);
}

void device2_sp(int device, int xbee_address[2][10])
{
  Serial.write(0x7E);             //7E 00 11 17 00 00 13 A2 00 40 E8 29 63 FF FE 02 53 50 01 F4 E8    5sec
  Serial.write(0x0);              
  Serial.write(0x11);
  Serial.write(0x17);
  Serial.write(0x0);
  
 
 for(int i=0; i<10; i++)
 {
    Serial.write(xbee_address[1][i]);
 }
 
  Serial.write(0x02);

  Serial.write(0x53);
  Serial.write(0x50);

  Serial.write(0x01);
  Serial.write(0xF4);
  Serial.write(0xE8);
}

void device2_st(int device, int xbee_address[2][10])
{
  Serial.write(0x7E);             //7E 00 11 17 00 00 13 A2 00 40 E8 29 63 FF FE 02 53 54 03 F2 E4    1.01sec
  Serial.write(0x0);              
  Serial.write(0x11);
  Serial.write(0x17);
  Serial.write(0x0);
  
 for(int i=0; i<10; i++)
 {
    Serial.write(xbee_address[1][i]);
 }
 
  Serial.write(0x02);

  Serial.write(0x53);
  Serial.write(0x54);

  Serial.write(0x03);
  Serial.write(0xF2);
  Serial.write(0xE4);
}
