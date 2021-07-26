//kart_relay_051199.cpp
#include <iostream>
#include <errno.h>
#include <wiringPiSPI.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <pthread.h>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#include <stdio.h> //printf
#include <string.h> //memset
#include <stdlib.h> //exit(0);
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <linux/input.h>
#include <termios.h>
#include <pthread.h>

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <linux/types.h>
#include <linux/watchdog.h>
#include <ctime>
#include <chrono>
#include <math.h>

int chek_pin_status_2=1,End_change_pin=0;
bool read_pin_bussy=true;



//#define total_sample 7500808
#define per_matrix 4096
#define total_matrix 12
#define total_sample (per_matrix*total_matrix)/8  //6,144
using namespace std;

///////////////////////////////////////////////
///////ethernet variable///////////////////////


#define ip_setting  1
#define port_setting 0
#define Is_on_or_off 2


#define ip_setting  1
#define port_setting 0
#define BUFLEN 97249100 //Max length of buffer
#define STATUS_CHANGE_TIMES     (11)
#define ACK 3
#define Is_on_or_off 2

 struct sockaddr_in si_me, si_other;
    int s,   recv_len;
    unsigned int slen = sizeof(si_other) ;
  
    struct sockaddr_in si_meR , si_otherR;
    int sR,   recv_lenR;
    
    char this_ip[50]="";
    //char RelationList[640]="";
    int Relation_list_[256][256];
    int Relation_list_0__[256][256];
    int usual_Relay[16][2][16];
    int chanely_Relay[128];

    int operator_list[256];
    char this_Port[50]="";
    int return_s[16]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
  
	int SPORT =8887 ; //The port on which to listen for outgoing data
    int PORT =8888 ;  //The port on which to listen for incoming data
    
    char Recive_buf[BUFLEN];
	char Send_buf[BUFLEN];
	int Status_Table[256];
	
///////////////calculator var//////////////
double signal_avrage_sensor[9]={2.17,2.17,2.17,2.17,2.17,2.17,2.17,2.17,2.17};
int start_time[9]={-1,-1,-1,-1,-1,-1,-1,-1,-1};
int start_time_sorted[9][2];
int start_time_sorting[9][2]={{-2,-2},{-2,-2},{-2,-2},{-2,-2},{-2,-2},{-2,-2},{-2,-2},{-2,-2},{-2,-2}};
int start_time_sorted_dif[9][2];
int max_sensor_index=-1;
int max__index=-1;
double x_shot=0,y_shot=0;
double y_shot_list[10];
bool out_detect=false;
int ritgh_sensor_sorted[8][3];
int ritgh_sensor_sorted_new[8][3];
int left_sensor_sorted_new[8][3];

int ritgh_max,left_max; 
int ritgh_max_new,left_max_new;
int left_sensor_sorted[8][3];
double matris_sorted_sot_func[3][10];
double final_sort[10][3];
double x_avr[10];
double show_avr_x[20][3];
///////////////////////////////////////////


unsigned char buffer1[total_matrix][per_matrix];
unsigned char buffer2[total_matrix][per_matrix];
unsigned char sen1[total_sample];
unsigned char sen2[total_sample];
unsigned char sen3[total_sample];
unsigned char sen4[total_sample];
unsigned char sen5[total_sample];
unsigned char sen6[total_sample];
unsigned char sen7[total_sample];
unsigned char sen8[total_sample];

double  FIFO_sen1[(total_sample/2)*3];
double  FIFO_sen2[(total_sample/2)*3];
double  FIFO_sen3[(total_sample/2)*3];
double  FIFO_sen4[(total_sample/2)*3];
double  FIFO_sen5[(total_sample/2)*3];
double  FIFO_sen6[(total_sample/2)*3];
double  FIFO_sen7[(total_sample/2)*3];
double  FIFO_sen8[(total_sample/2)*3];
unsigned char  SEND_bufer[per_matrix*total_matrix];

bool bufer_number_1=true;
bool R_spi_compelete=false;
int index_read_bufer=-1;
static const int CHANNEL = 0;
char time_now_v[80];


void time_now()
{
		time_t rawtime;
  struct tm * timeinfo;


  time (&rawtime);
  timeinfo = localtime(&rawtime);

  strftime(time_now_v,sizeof(time_now_v),"%d-%m-%Y_%H:%M:%S",timeinfo);
  //std::string str(buffer);
  //time_now_v=buffer;
 
}









void die(char *s)
{
    perror(s);
    exit(1);
}
 

void Ethe_init(int send_socket)
{  
   if(send_socket==0)
   {
		//create a UDP socket
		if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		{
			perror("socket");
		}
		
		// zero out the structure
		memset((char *) &si_me, 0, sizeof(si_me));
		 
		si_me.sin_family = AF_INET;
		si_me.sin_port = htons(PORT);
		//si_me.sin_addr.s_addr =  inet_addr("192.168.1.5");
		 si_other.sin_addr.s_addr = htonl(INADDR_BROADCAST);
		 
		//bind socket to port
		if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
		{
			perror("bind");
		}
   }
   else if(send_socket==1)
   {
	    //create a UDP socket
			if ((sR=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
			{
				perror("socket");
			}
			 int on=1;
			// sR.setsocketopt(socket.SOL_SOCKET,socket.SO_BROADCAST,1);
			setsockopt(sR,SOL_SOCKET,SO_BROADCAST,&on,sizeof(on));
			// zero out the structure
			memset((char *) &si_meR, 0, sizeof(si_meR));
			 
			si_meR.sin_family = AF_INET;
			si_meR.sin_port = htons(SPORT);
			//si_meR.sin_addr.s_addr = inet_addr("192.168.1.5");
			si_meR.sin_addr.s_addr = htonl(INADDR_BROADCAST);

			if( bind(sR ,(struct sockaddr*)&si_meR, sizeof(si_meR) ) == -1)
			{
				perror("bind");
			}
	
   }
     
}

 void set_this_port()
 { 
	  int recivePort,sendport=0;
	  int i_sti=0;
	  int flage1=-1,flage2=-1;
	  char Recive_Port[27]="";
	  char Send_Port[27]="";
	
	  for(i_sti=0;i_sti<50;i_sti++)
	  {
			if(this_Port[i_sti]=='_')
			{
				  if(flage1==-1){flage1=i_sti;}
				  else if(flage2==-1){flage2=i_sti;}
			}
	  }
	  
	   for(i_sti=0;i_sti<flage1;i_sti++)
	  {
		 Recive_Port[i_sti]=this_Port[i_sti];		 
	  }
	  
	  for(i_sti=flage1+1;i_sti<flage2;i_sti++)
	  {
		 Send_Port[i_sti-flage1-1]=this_Port[i_sti];	 
	  }

	
	if(flage1==1)     {recivePort=((Recive_Port[0]-48));}
	else if(flage1==2){recivePort=((Recive_Port[0]-48)*10)+((Recive_Port[1]-48));}
	else if(flage1==3){recivePort=((Recive_Port[0]-48)*100)+((Recive_Port[1]-48)*10)+((Recive_Port[2]-48));}
	else if(flage1==4){recivePort=((Recive_Port[0]-48)*1000)+((Recive_Port[1]-48)*100)+((Recive_Port[2]-48)*10)+((Recive_Port[3]-48));}
	
	 
	if(flage2-flage1-1==1)    {sendport=((Send_Port[0]-48));}
	else if(flage2-flage1-1==2){sendport=((Send_Port[0]-48)*10)+((Send_Port[1]-48));}  
	else if(flage2-flage1-1==3){sendport=((Send_Port[0]-48)*100)+((Send_Port[1]-48)*10)+((Send_Port[2]-48));}
	else if(flage2-flage1-1==4){sendport=((Send_Port[0]-48)*1000)+((Send_Port[1]-48)*100)+((Send_Port[2]-48)*10)+((Send_Port[3]-48));}
	
    printf("RecivePort:");
	printf("%d",recivePort);
	printf("\n");
	printf("SendPort:");
	printf("%d",sendport);
	printf("\n------------------------------\n");
	

	SPORT=sendport;
	PORT=recivePort;
	if(SPORT!=sendport){Ethe_init(1);}
	if(PORT!=recivePort){Ethe_init(0);}	  
 }
 
 
 

int  Relay_nr[8]={5,12,13,6,14,10,11,30};
int  read_pin_nr[8]={7,15,16,0,1,2,3,4};
int  control_nr[4]={31,21,22,26};
int control_A_W_first=8;
int control_B_R_first=9;
int control_C_R_last=24;
int control_d_W_last=23;
int control_oe_R=29;
void kart_on(int addres)
{
  int set_1=(addres & 1);
  int set_2=(addres & 2)>>1;
  int set_3=(addres & 4)>>2;
  int set_4=(addres & 8)>>3;
  /*printf("\n set_1=%d",set_1);
  printf("\n set_2=%d",set_2);
  printf("\n set_3=%d",set_3);
  printf("\n set_4=%d",set_4);*/
  
   digitalWrite (control_nr[0], set_1) ; 
   digitalWrite (control_nr[1], set_2) ; 
   digitalWrite (control_nr[2], set_3) ; 
   digitalWrite (control_nr[3], set_4) ; 
}

void pin_change_old(int kart_number,int control_latch,int first_last)
{
     //printf("\n kart_number=%d",kart_number);
  
    int del=30;
    ////////////////////Relay Pin OutPut////////////////////////////////
    int i=0;                                                    ////////
    for(i=0;i<=7;i++)                                           ////////
    {                                                           ////////
      pinMode (Relay_nr[i], OUTPUT) ;                           ////////
    }                                                           ////////
   delay(del);                                                 ////////
    ////////////////////////////////////////////////////////////////////
    
    
    /////////////////Relay Pin half kart change///////////////////////////////////////////////////////////////
     for(i=0;i<=7;i++)                                                                                ////////
    {                                                                                                 ////////
       digitalWrite (Relay_nr[i], usual_Relay[kart_number][first_last][i]) ;                          ////////
    }                                                                                                 ////////
    delay(del);										              ////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
   
    
    digitalWrite (control_latch, 1) ;
    delay(del);
    kart_on(kart_number+1);
    delay(del);
    
    digitalWrite (control_latch, 0) ;
    kart_on(0);
    delay(del);
    
    /////////////////Relay Pin half kart change///////////////////////////////////////////////////////////////
     for(i=0;i<=7;i++)                                                                                ////////
    {                                                                                                 ////////
       digitalWrite (Relay_nr[i], 0) ;                          ////////
    }                                                                                                 ////////
    delay(del);										              ////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
}
 
 


void pin_change(int kart_number,int control_latch,int first_last)
{
     //printf("\n kart_number=%d",kart_number);
  
    int del=100;
    ////////////////////Relay Pin OutPut////////////////////////////////
    int i=0;                                                    ////////
    for(i=0;i<=7;i++)                                           ////////
    {                                                           ////////
      pinMode (Relay_nr[i], OUTPUT) ;                           ////////
    }                                                           ////////
   delay(del);                                                 ////////
    ////////////////////////////////////////////////////////////////////
    
    
    /////////////////Relay Pin half kart change///////////////////////////////////////////////////////////////
    printf("\n");
     for(i=0;i<=7;i++)                                                                                ////////
    {     
      printf(" %d",usual_Relay[kart_number][first_last][i]);                                          
     //  printf("\nusual_Relay[%d][%d][%d]=%d",kart_number,first_last,i,usual_Relay[kart_number][first_last][i]);                                                                                    ////////
       digitalWrite (Relay_nr[i], usual_Relay[kart_number][first_last][i]) ;                          ////////
    }                                                                                                 ////////
    delay(del);										              ////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
   
    
    digitalWrite (control_latch, 1) ;
    delay(del);
    kart_on(kart_number+1);
    delay(del);
    
    digitalWrite (control_latch, 0) ;
    kart_on(0);
    delay(del);

    /////////////////Relay Pin half kart change///////////////////////////////////////////////////////////////
     for(i=0;i<=7;i++)                                                                                ////////
    {                                                                                                 ////////
       digitalWrite (Relay_nr[i], 0) ;                          ////////
    }                                                                                                 ////////
    delay(del);										              ////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
}
 
 

void  Read_Old_Status_Usual()
{
  fstream fp("/home/pi/Desktop/aps/Old_Status_Usual",ios::in);   
  if(!fp)
	{
		printf("con not read file");
		exit(1);	
	}
	
	string input_text;
	unsigned int data[65537];
	int i=0;
	
	while(!fp.eof())
	{
	 
		fp>>input_text;
		//printf("\n %d=%d ",i, std::stoi(input_text));
		data[i]=atoi(input_text.c_str());
	        i++;
	}
	
	
	
int first_last=0,Relay_number=0;
	int j=0;
	 for (j=0;j<=15;j++)
	  {
	      for (i=0;i<=15;i++)
		{
		  if(i<=7){first_last=0;}else{first_last=1;}
		  if(i<=7){Relay_number=i;}else{Relay_number=i-8;}
		   usual_Relay[j][first_last][Relay_number]=data[(j*16)+i];  
		  // printf("\n data[(j*255)+i]=%d",data[(j*256)+i]);
		   // printf("\n i=%d,j=%d,(j*255)+i=%d",i,j,(j*256)+i);
		}
	    
	  }
  				    
}

void  Read_Old_Status_Chanel()
{
  fstream fp("/home/pi/Desktop/aps/Old_Status_chanel",ios::in);   
  if(!fp)
	{
		printf("con not read file");
		exit(1);	
	}
	
	string input_text;
	unsigned int data[65537];
	int i=0;
	
	while(!fp.eof())
	{
	 
		fp>>input_text;
		//printf("\n %d=%d ",i, std::stoi(input_text));
		data[i]=atoi(input_text.c_str());
	        i++;
	}
	
	
	

	int j=0;
	 for (j=0;j<128;j++)
	  {
	     
		   chanely_Relay[j]=data[j];  
		  // printf("\n data[(j*255)+i]=%d",data[(j*256)+i]);
		   // printf("\n i=%d,j=%d,(j*255)+i=%d",i,j,(j*256)+i);
		
	  }
	  
	  
  

				    
}

void  Write_Old_Status_Usual()
{
      int i,j,k=0;
      
 /*  for(i=0;i<=15;i++)
  {
     printf("\n Kart=%d  ",i+1);
      for(j=0;j<=1;j++)
      {
	
	  for(k=0;k<=7;k++)
	  {
	    printf("%d ",usual_Relay[i][j][k]);
	  }
      }
  }*/
    


  ofstream myfile ("/home/pi/Desktop/aps/Old_Status_Usual");
  if (myfile.is_open())
  {
    for(i=0;i<16;i++)
    {
      for(j=0;j<=1;j++)
      {
	 for(k=0;k<=7;k++)
	  {
	      myfile <<  usual_Relay[i][j][k]<< " " ;
	  }
      }
       myfile <<"\n" ;
    }
    myfile.close();
  }
  else cout << "Unable to open file";
   
   
  
}

void  Write_Old_Status_Chanel()
{
   
    
    int i=0;

  ofstream myfile ("/home/pi/Desktop/aps/Old_Status_chanel");
  if (myfile.is_open())
  {
    for(i=0;i<128;i++)
    {
     
        myfile << chanely_Relay[i]<< " " ;
       myfile <<"\n" ;
    }
    myfile.close();
  }
  else cout << "Unable to open file";
   
   
  
}

void On_OF_set_chanelly_to_usual(int number,int on_off)
{
    int first_last=0;
  int relay_number=number%16;
  int kart_number=number/16;
 // printf("\n\n kart_number=%d",kart_number+1);
   //printf("\n relay_number=%d",relay_number);
 
   if(relay_number>7)
   {
     first_last=1;
     relay_number=relay_number-8;
   
   }else
   {
     first_last=0;
     relay_number=relay_number;
   }


 
    usual_Relay[kart_number][first_last][relay_number]=on_off; 
}

void on_off(int number,int on_off)
{
  int first_last=0;
   int first_last_2=0;
    while(1)
	{
	   if(read_pin_bussy==true)
		{
		      break;
		}
	}
  
  read_pin_bussy=false;
  delay(100);
  int relay_number=number%16;
  int kart_number=number/16;
 // printf("\n\n kart_number=%d",kart_number+1);
   //printf("\n relay_number=%d",relay_number);
   int control_latch=0;
   int control_latch_2=0;
   if(relay_number>7)
   {
     first_last=1;
     first_last_2=0;
     relay_number=relay_number-8;
     control_latch=control_d_W_last;
     control_latch_2=control_A_W_first;
   
   }else
   {
     first_last=0;
     first_last_2=1;
     relay_number=relay_number;
     control_latch=control_A_W_first;
     control_latch_2=control_d_W_last;
   }


 
      usual_Relay[kart_number][first_last][relay_number]=on_off; 
      Write_Old_Status_Usual();
     //       printf("\n ______________________________________");
     // printf("\n   kart_number][first_last][i]=%d,on_off=%d",usual_Relay[kart_number][first_last][relay_number],on_off);
        digitalWrite (control_latch_2, 0) ;
      pin_change(kart_number,control_latch,first_last);
     pin_change(kart_number,control_latch_2,first_last_2);
  
 read_pin_bussy=true;
   
    
}



void Old_Status_On_Off_chanel_set_on()
{
  int Chanel_ID=1;
 for(Chanel_ID=1;Chanel_ID<=128;Chanel_ID++)
 {
			if(chanely_Relay[Chanel_ID-1]>=0) 
			{
				  if(chanely_Relay[Chanel_ID-1]==1)
				    {
				      On_OF_set_chanelly_to_usual(((Chanel_ID-1)*2)+1,0);
				      On_OF_set_chanelly_to_usual(((Chanel_ID-1)*2)  ,1);
				    }
				    
				  if(chanely_Relay[Chanel_ID-1]==0)
				    {
				      On_OF_set_chanelly_to_usual(((Chanel_ID-1)*2),  0);
				      On_OF_set_chanelly_to_usual(((Chanel_ID-1)*2)+1,1); 
				    }
			}
}
}

void Old_Status_On_Off_chanel_set_off()
{
  int Chanel_ID=1;
			if(chanely_Relay[Chanel_ID-1]>=0) 
			{
				    delay(500);
			}
				
for(Chanel_ID=1;Chanel_ID<=128;Chanel_ID++)
 {
			if(chanely_Relay[Chanel_ID-1]>=0) 
			{	    
				      if(chanely_Relay[Chanel_ID-1]==1)
				    {
				      On_OF_set_chanelly_to_usual(((Chanel_ID-1)*2),0);
				    }
				    
				  if(chanely_Relay[Chanel_ID-1]==0)
				    {  
				      On_OF_set_chanelly_to_usual(((Chanel_ID-1)*2)+1,0);
				      
				    }
			    }
				  //  printf("\n Chanel_ID=%d",Chanel_ID);
}
  

   
    
}

 void Old_Status_On_Off()
{
    while(1)
	{
	   if(read_pin_bussy==true)
		{
		      break;
		}
	}
  
  read_pin_bussy=false;
   delay(100);     
  
 
  

  int kart_number=1;
   
 for(kart_number=0;kart_number<=15;kart_number++)
 {

  pin_change_old(kart_number,control_d_W_last,1);
  pin_change_old(kart_number,control_A_W_first,0);
  //delay(100);
}
  
 read_pin_bussy=true;
   
    
}
 



int Read_pin_2(int number)
{
  delay(20);
  
  int relay_number=number%16;
  int kart_number=number/16;
  //printf("\n\n kart_number=%d",kart_number+1);
  // printf("\n relay_number=%d",relay_number);
   int control_latch=0;
   if(relay_number>7)
   {
     relay_number=relay_number-8;
     control_latch=control_C_R_last;
   
   }else
   {
     relay_number=relay_number;
       control_latch=control_B_R_first;
   }
   //printf("\n control_latch=%d",control_latch);
   //printf("\n read_pin_nr[relay_number]=%d",read_pin_nr[relay_number]);
   
   
   
  pinMode (read_pin_nr[relay_number], INPUT) ; 
   delay(5);
      // digitalWrite (control_A, 0) ;
       digitalWrite (control_latch, 1) ;
       kart_on(kart_number+1);
      delay(5);
      
       int pin1=digitalRead(read_pin_nr[relay_number]) ;
        printf("\n pin1=%d",pin1);
	delay(1);
         pin1=pin1+digitalRead(read_pin_nr[relay_number]) ;
	 printf("   pin1=%d",pin1);
	  delay(1);
	  pin1=pin1+digitalRead(read_pin_nr[relay_number]) ;
	   printf("   pin1=%d",pin1);
	   delay(1);
	   pin1=pin1+digitalRead(read_pin_nr[relay_number]) ;
	   printf("  pin1=%d",pin1);
	    delay(1);
	   pin1=pin1+digitalRead(read_pin_nr[relay_number]) ;
	    printf("  pin1=%d",pin1);
       delay(5);
       
      //digitalWrite (control_latch, 0) ;
       delay(5);
      //kart_on(0);
      //printf("\n  pin1=%d",pin1);
      double pin_sts=pin1/5;
       //printf("\n  ppin_sts=%g",pin_sts);
       if(pin_sts>=0.4)
       {
	 printf("\n  pin%d=%d",number,1);
	 return 1;
       }
       else
       {
	// printf("\n  pin%d=%d",number,0);
	 return 0;
       }
      
       
}



int Read_pin(int number)
{
   while(1)
			   {
			     if(read_pin_bussy==true)
			     {
			       break;
			     }
			   }
  
  read_pin_bussy=false;
  delay(100);
  int relay_number=number%16;
  int kart_number=number/16;
  //printf("\n\n kart_number=%d",kart_number+1);
  //printf("\n relay_number=%d",relay_number);
   int control_latch=0;
   if(relay_number>7)
   {
     relay_number=relay_number-8;
     control_latch=control_C_R_last;
   
   }else
   {
     relay_number=relay_number;
       control_latch=control_B_R_first;
   }
   //printf("\n control_latch=%d",control_latch);
   //printf("\n read_pin_nr[relay_number]=%d",read_pin_nr[relay_number]);
   
   
   
  pinMode (read_pin_nr[relay_number], INPUT) ; 
 //printf("read_pin_nr[relay_number]=%d");
   //delay(10);
      // digitalWrite (control_A, 0) ;
       digitalWrite (control_latch, 1) ;
       kart_on(kart_number+1);
      delay(100);
      
       int pin1=digitalRead(read_pin_nr[relay_number]) ;
        //printf("\n pin1=%d",pin1);
	delay(1);
         pin1=pin1+digitalRead(read_pin_nr[relay_number]) ;
	 //printf("   pin1=%d",pin1);
	  delay(1);
	  pin1=pin1+digitalRead(read_pin_nr[relay_number]) ;
	 // printf("   pin1=%d",pin1);
	   delay(1);
	   pin1=pin1+digitalRead(read_pin_nr[relay_number]) ;
	  // printf("  pin1=%d",pin1);
	    delay(1);
	   pin1=pin1+digitalRead(read_pin_nr[relay_number]) ;
	   // printf("  pin1=%d",pin1);
       delay(100);
       
  //  digitalWrite (control_latch, 0) ;
      // delay(1);
    //kart_on(0);
      //printf("\n  pin1=%d",pin1);
      double pin_sts=pin1/5;
       //printf("\n  ppin_sts=%g",pin_sts);
     // printf("\n  pin%d=%g",number,pin_sts);
       if(pin_sts>=0.4)
       {
	// printf("\n  pin%d=%d",number,1);
	  read_pin_bussy=true;
	 return 1;
       }
       else
       {
	// printf("\n  pin%d=%d",number,0);
	 read_pin_bussy=true;
	 return 0;
       }
      
       
}


 
 void Read_pin_kart(int kart_number)
{
  
  
  
   while(1)
			   {
			     if(read_pin_bussy==true)
			     {
			       break;
			     }
			   }
  
  read_pin_bussy=false;
  delay(100);
   int control_latch=0;
  
   
   
   

 //printf("read_pin_nr[relay_number]=%d");
   //delay(10);
      // digitalWrite (control_A, 0) ;

      int j=0;
      for(j=0;j<=1;j++)
      {
	  if(j==0){control_latch=control_B_R_first;}
	  if(j==1){control_latch=control_C_R_last;}
	  digitalWrite (control_latch, 1) ;
	  kart_on(kart_number+1);
	  
	  delay(10);
	  int relay_number;
	  for(relay_number=0;relay_number<=7;relay_number++)
	  {
	      pinMode (read_pin_nr[relay_number], INPUT) ; 
	  
	      int pin1=digitalRead(read_pin_nr[relay_number]) ;
	      //printf("\n pin1=%d",pin1);
	      delay(1);
	      pin1=pin1+digitalRead(read_pin_nr[relay_number]) ;
	      //printf("   pin1=%d",pin1);
	      delay(1);
	      pin1=pin1+digitalRead(read_pin_nr[relay_number]) ;
	      // printf("   pin1=%d",pin1);
	      delay(1);
	      pin1=pin1+digitalRead(read_pin_nr[relay_number]) ;
	      // printf("  pin1=%d",pin1);
	      delay(1);
	      pin1=pin1+digitalRead(read_pin_nr[relay_number]) ;
	       // printf("  pin1=%d",pin1);
	       
	       
		//printf("\n  pin1=%d",pin1);
		double pin_sts=pin1/5;
		//printf("\n  ppin_sts=%g",pin_sts);
		// printf("\n  pin%d=%g",number,pin_sts);
		 //printf("\n  %d+(8*%d)=%d",relay_number,j,(int)(relay_number+(8*j)));
		 if(pin_sts>=0.4)
		 {
		   return_s[(int)(relay_number+(8*j))] =1;
		 }
		 else
		 {
		   return_s[(int)(relay_number+(8*j))] =0;
		 }
	       
	     } 
	     delay(10);
	     
	     
	     digitalWrite (control_latch, 0) ;
	     kart_on(0);
       }  
	   
      
      
       
       read_pin_bussy=true;
       //return return_s;
       
}

 
 
 
 
void Get_data_From_Ethe()
{
      printf("\n ______________________________________");
	    printf("\n Waiting for data...");
        fflush(stdout);
       
        //watch_dog_flag=1;
        //try to receive some data, this is a blocking call
        if (( recvfrom(s, Recive_buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
        {
            //die("recvfrom()");
        }
       // while(chek_pin_status_2==1){}
       // End_change_pin=1;

        //print details of the client/peer and the data received
        printf("\nReceived packet from %s:%d  ", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
       // printf("Data: %s\n" , Recive_buf);  
      
      
}


void Sena_Data_to_Ethe()
{
	//now reply the client with the same data
       
       //SEND_bufer[0]=0;
      // SEND_bufer[1]=234;
   
 
        if (sendto(sR,Send_buf,5, 0,(struct sockaddr*) &si_meR, slen) == -1)
        {
            perror("sendto()");
        }
}


void set_this_ip()
{
	
      char Command1[100]="";
      char Command2[100]="";
	  char IP_manual[27]="";
	  char Mask_ip[27]="";
	  char GW_ip[27]="";
	  int i_sti=0;
	  int flage1=-1,flage2=-1,flage3=-1;
	  
	  for(i_sti=0;i_sti<50;i_sti++)
	  {
			if(this_ip[i_sti]=='_')
			{
				  if(flage1==-1){flage1=i_sti;}
				  else if(flage2==-1){flage2=i_sti;}
				  else {flage3=i_sti;}
			}
	  }
	  
	 	  
	  for(i_sti=0;i_sti<flage1;i_sti++)
	  {
		 IP_manual[i_sti]=this_ip[i_sti];		 
	  }
	  
	  for(i_sti=flage1+1;i_sti<flage2;i_sti++)
	  {
		 Mask_ip[i_sti-flage1-1]=this_ip[i_sti];	 
	  }
	  
	  
	   for(i_sti=flage2+1;i_sti<flage3;i_sti++)
	  {
		 GW_ip[i_sti-flage2-1]=this_ip[i_sti];	 
	  }
	    
	    printf("\n\n");	
	    strcat(Command2,"ifconfig eth0 ");
	    strcat(Command2,IP_manual);//Thisip
	    strcat(Command2," netmask ");
	    strcat(Command2,Mask_ip);//ThisMask
	    printf(Command2);
	   system(Command2);
	    printf("\n\n");	
	    
		strcat(Command1,"route add default gw ");
	    strcat(Command1,GW_ip);
	    system(Command1);
	    printf(Command1);
	    printf("\n");

 
}
/*
void set_this_port()
 { 
	  int recivePort,sendport=0;
	  int i_sti=0;
	  int flage1=-1,flage2=-1;
	  char Recive_Port[27]="";
	  char Send_Port[27]="";
	
	  for(i_sti=0;i_sti<50;i_sti++)
	  {
			if(this_Port[i_sti]=='_')
			{
				  if(flage1==-1){flage1=i_sti;}
				  else if(flage2==-1){flage2=i_sti;}
			}
	  }
	  
	   for(i_sti=0;i_sti<flage1;i_sti++)
	  {
		 Recive_Port[i_sti]=this_Port[i_sti];		 
	  }
	  
	  for(i_sti=flage1+1;i_sti<flage2;i_sti++)
	  {
		 Send_Port[i_sti-flage1-1]=this_Port[i_sti];	 
	  }

	
	if(flage1==1)     {recivePort=((Recive_Port[0]-48));}
	else if(flage1==2){recivePort=((Recive_Port[0]-48)*10)+((Recive_Port[1]-48));}
	else if(flage1==3){recivePort=((Recive_Port[0]-48)*100)+((Recive_Port[1]-48)*10)+((Recive_Port[2]-48));}
	else if(flage1==4){recivePort=((Recive_Port[0]-48)*1000)+((Recive_Port[1]-48)*100)+((Recive_Port[2]-48)*10)+((Recive_Port[3]-48));}
	
	 
	if(flage2-flage1-1==1)    {sendport=((Send_Port[0]-48));}
	else if(flage2-flage1-1==2){sendport=((Send_Port[0]-48)*10)+((Send_Port[1]-48));}  
	else if(flage2-flage1-1==3){sendport=((Send_Port[0]-48)*100)+((Send_Port[1]-48)*10)+((Send_Port[2]-48));}
	else if(flage2-flage1-1==4){sendport=((Send_Port[0]-48)*1000)+((Send_Port[1]-48)*100)+((Send_Port[2]-48)*10)+((Send_Port[3]-48));}
	
    printf("RecivePort:");
	printf("%d",recivePort);
	printf("\n");
	printf("SendPort:");
	printf("%d",sendport);
	printf("\n------------------------------\n");
	

	SPORT=sendport;
	PORT=recivePort;
	if(SPORT!=sendport){Ethe_init(1);}
	if(PORT!=recivePort){Ethe_init(0);}	  
 }
*/ 
 void File_WR_two( int r_w, int ip_port)
{
	char buffer[50];
    FILE* sourceFile;
    int numBytes=1;
	   if(ip_port==ip_setting)
	   {	
				if(r_w==1)
				{
						sourceFile = fopen("/home/pi/Desktop/aps/IP", "wb");
						if(sourceFile==NULL){printf("Could not open source file\n");}
						else 
						{	
							numBytes=fwrite(buffer, 1, 50, sourceFile);	
							if(numBytes==-1){printf("write file err");}	
							fclose(sourceFile);
						}
				 }
				 else if(r_w==0)
				 {   
						sourceFile = fopen("/home/pi/Desktop/aps/IP", "rb");   
						if(sourceFile==NULL){printf("Could not open destination file\n");}
						else
						{
						numBytes=fread(this_ip, 1, 50, sourceFile);
						if(numBytes==-1){printf("write file err");}	
						fclose(sourceFile);
						}
				 }
	 }
	 else if(ip_port==port_setting)
	 {
				 if(r_w==1)
						{
						sourceFile = fopen("/home/pi/Desktop/aps/PORT", "wb");
						if(sourceFile==NULL){printf("Could not open source file\n");}
						else 
						{	
							numBytes=fwrite(buffer, 1, 50, sourceFile);	
							if(numBytes==-1){printf("write file err");}	
							fclose(sourceFile);
						}
				 }
				 else if(r_w==0)
				 {   
						sourceFile = fopen("/home/pi/Desktop/aps/PORT", "rb");   
						if(sourceFile==NULL){printf("Could not open destination file\n");}
						else
						{
						numBytes=fread(this_Port, 1, 50, sourceFile);
						if(numBytes==-1){printf("write file err");}	
						fclose(sourceFile);
						}
				 }
	 }
	  
}
 
 
 void network_setting(int ip_port)
{
	
	if(ip_port==ip_setting)
	{
		File_WR_two( 0,ip_port);
		set_this_ip();
	}
	else if(ip_port==port_setting)
	{
		File_WR_two( 0,ip_port);
		set_this_port();
	}
	
}



void sort_matrix(int show)
{
	int index_min=0;
	int i=0;
	int a=0;
	int min=-100;
	
	for(i=0;i<9;i++)
	{
		start_time_sorting[i][0]=-2;
		start_time_sorting[i][1]=-2;
	}
	
	for(i=7;i>=1;i--)
	{  
	    min=-100;
		for(a=1;a<=7;a++)
		{   
			if(start_time_sorting[a][0]==-2)
             {		 
						if(min<start_time[a])
						{
							//printf("\n++++++++++++++++++++++++++");
							//	printf("\n\n\n\n****7777a=%d",a);
								//printf("\n\n\n\n****start_time[a]=%d",start_time[a]);
								min=start_time[a];
							    index_min=a;  							  
						}
			}
			 
		}				
		start_time_sorting[index_min][0]=min;
		start_time_sorting[index_min][1]=i;
		start_time_sorted[i][0]=min;
		start_time_sorted[i][1]=index_min;
						
	}
	start_time_sorting[8][0]=start_time[8];
    start_time_sorting[8][1]=8;
    start_time_sorted[8][0]=start_time[8];
    start_time_sorted[8][1]=8;
				/*	printf("\nstart_time_sorted[1,0]=%d",start_time_sorting[1][0]);
					printf("\nstart_time_sorted[2,0]=%d",start_time_sorting[2][0]);
					printf("\nstart_time_sorted[3,0]=%d",start_time_sorting[3][0]);
					printf("\nstart_time_sorted[4,0]=%d",start_time_sorting[4][0]);
					printf("\nstart_time_sorted[5,0]=%d",start_time_sorting[5][0]);
					printf("\nstart_time_sorted[6,0]=%d",start_time_sorting[6][0]);
					printf("\nstart_time_sorted[7,0]=%d",start_time_sorting[7][0]);
					printf("\nstart_time_sorted[8,0]=%d",start_time_sorting[8][0]);
					
					printf("\nstart_time_sorted[1,1]=%d",start_time_sorting[1][1]);
					printf("\nstart_time_sorted[2,1]=%d",start_time_sorting[2][1]);
					printf("\nstart_time_sorted[3,1]=%d",start_time_sorting[3][1]);
					printf("\nstart_time_sorted[4,1]=%d",start_time_sorting[4][1]);
					printf("\nstart_time_sorted[5,1]=%d",start_time_sorting[5][1]);
					printf("\nstart_time_sorted[6,1]=%d",start_time_sorting[6][1]);
					printf("\nstart_time_sorted[7,1]=%d",start_time_sorting[7][1]);
					printf("\nstart_time_sorted[8,1]=%d",start_time_sorting[8][1]);
					
					
					printf("\nstart_time[1,1]=%d",start_time[1]);
					printf("\nstart_time[2,1]=%d",start_time[2]);
					printf("\nstart_time[3,1]=%d",start_time[3]);
					printf("\nstart_time[4,1]=%d",start_time[4]);
					printf("\nstart_time[5,1]=%d",start_time[5]);
					printf("\nstart_time[6,1]=%d",start_time[6]);
					printf("\nstart_time[7,1]=%d",start_time[7]);
					printf("\nstart_time[8,1]=%d",start_time[8]);*/
					if(show==1)
					{
					printf("\nstart_time_sorted[%d]=%d",start_time_sorted[1][1],start_time_sorted[1][0]);
					printf("\nstart_time_sorted[%d]=%d",start_time_sorted[2][1],start_time_sorted[2][0]);
					printf("\nstart_time_sorted[%d]=%d",start_time_sorted[3][1],start_time_sorted[3][0]);
					printf("\nstart_time_sorted[%d]=%d",start_time_sorted[4][1],start_time_sorted[4][0]);
					printf("\nstart_time_sorted[%d]=%d",start_time_sorted[5][1],start_time_sorted[5][0]);
					printf("\nstart_time_sorted[%d]=%d",start_time_sorted[6][1],start_time_sorted[6][0]);
					printf("\nstart_time_sorted[%d]=%d",start_time_sorted[7][1],start_time_sorted[7][0]);
					printf("\nstart_time_sorted[%d]=%d",start_time_sorted[8][1],start_time_sorted[8][0]);
				    }
				
}

void MaxValue_index_find()
{
	int i=0;
	  for(i=1;i<=7;i++)
		{ 
			if(start_time_sorted[i][0]!=-1)
			{
				max_sensor_index=start_time_sorted[i][1];
				//printf("\nmax_sensor_index=%d",max_sensor_index);
				max__index=i;
				break;
			}
		}
}

bool Out_find()
{
	
	if(max_sensor_index==7)
	{
		printf("\nOUT_shot___7");
		return true;
	} 
	else if(max_sensor_index==1)
	{
		printf("\nOUT_shot___1");
		return true;
	}
	else if(max_sensor_index==7)
	{
			printf("\nOUT_shot___1");
		    return true;
	}
	return false;
}



bool error_find_mines()
{
	int i=0;
	int mines_error=0;
	  for(i=1;i<=8;i++)
		{ 
			//printf("\n\nstart_time_sorted[%d][0]=%d",i,start_time_sorted[i][0]);
			if(start_time_sorted[i][0]==-1)
			{
				mines_error++;
			}
		}
		//printf("\n\nmines_error=%d",mines_error);
		
		if(mines_error>3)
		{
		
			printf("\n\nError_mines_max");
				return true;
		}
		else{
			return out_detect;
		}
}

bool error_find_yeksan()
{
	int i=0,j=0;
	int yeksan=0;
	  for(i=1;i<=8;i++)
		{ 
			for(j=1;j<=8;j++)
			{ 
				if(start_time_sorted[i][0]==start_time_sorted[j][0])
				{
					yeksan++;
				}
			}
		}
		if(yeksan>=3)
		{
			printf("\nError_yeksan_max");
			return true;
			
		}
		else{
			return false;
		}
		
}
 
 void set_value_to_start_time_sorted_dif()
 {
	 printf("\n");
	 int i=0;
	 for(i=1;i<=8;i++)
	 {
	 start_time_sorted_dif[i][1]=start_time_sorted[i][1];
	 start_time_sorted_dif[i][0]=start_time_sorted[i][0]-start_time_sorted[1][0];
	 printf("\nstart_time_sorted_dif[%d][1]=%d start_time_sorted_dif[%d][0]=%d",i,start_time_sorted_dif[i][1],i,start_time_sorted_dif[i][0]);
	}
}

double x_calculator(int t1,int t2,int x1,int x2)
{
		
	int t1t2=t1;
	int t3t2=t2;
		//int t1t2=abs(t1-start_time_sorted[max__index][0]);  //t1-t2
        //int t3t2=abs(t2-start_time_sorted[max__index][0]);  //t3-t2
     
       /* printf("\nt1t2=%d",t1t2);	
		printf("\nt3t2=%d",t3t2);
		printf("\nx1=%d",x1);
		printf("\nx2=%d",x2);*/
	
					double total=	t1t2+t3t2;
					double m=abs(x1-x2)*0.15;
                    double L,X,x;
					if(x1<x2)
					{
					    L=(x1-1)*0.15;
						x=(t1t2*m)/total;
						X=(L)+x;
					}
					else
					{
						  L=(x2-1)*0.15;
						 x=(t3t2*m)/total;
						X=(L)+x;
					}
					X=X*100;
			
			
				//printf("\nX=%g",X);
				return(X);		
}
 
 
 double y_calculator_type1(int t1,int t2,double m)
 {
	 // printf("\n_________Y_calculate_____________________________________");
	//  	printf("\nmax__index=%d",max__index);
	 int t1t2=0,t3t2=0,C_sound_speed=350;
	 float L2=0,i=0,j=0,t12_d,t32_d,h2=0,h1=0;
	 
	 t1t2=abs(t1-start_time_sorted[max__index][0]);  //t1-t2
     t3t2=abs(t2-start_time_sorted[max__index][0]);  //t3-t2
     //t1t2=t1;  //t1-t2
     //t3t2=t2;  //t3-t2
     
     //t1t2=11;
     //t3t2=12;
    // start_time_sorted[max__index+1][1]=4;
     //printf("\nt1t2=%d",t1t2);
     //printf("\nt3t2=%d",t3t2);
     //printf("\nm=%g",m);
     
     t12_d=(t1t2*0.0000083333);
     t32_d=(t3t2*0.0000083333);
     //printf("\nt12_d=%g",t12_d);
     //printf("\nt32_d=%g",t32_d);
     
     i=((t12_d)*C_sound_speed);
     j=((t32_d)*C_sound_speed);
     
     //printf("\ni=%g",i);
     //printf("\nj=%g",j);
     
    double pi=3.1415;
    double _m2=pow(m,2),_2m=2*m;
    double _j2=(pow(j,2)), _i2=(pow(i,2));
    double _2j=2*j,_2i=2*i;
    double gama_p=0,gama=0;
     
     
     
    double q=_m2-_j2,p=-1*_2m ,t=_2i,s=_2j,r=_m2-_i2,w=_2m,qt=q*t,sr=s*r,wr=w*r,qp=q*p;
    double gama_p_cos=(qt-sr)/(wr-qp);
      //printf("\ngama_p_cos=%g",(gama_p_cos));
     gama_p=acos(gama_p_cos);
     
      //printf("\ngama_p=%g",(gama_p));
     //printf("\ngama_p_degree=%g",(gama_p*180)/pi);
     //printf("\ncos(gama_p)=%g",cos(gama_p));
     L2=(_m2-_j2)/(_2j+(_2m*cos(gama_p)));
     //printf("\nL2=%g",L2);
     
     L2=(_m2-_i2)/(_2i-(_2m*cos(gama_p)));
     //printf("\nL2=%g",L2);
     ///////////////////////////////////////////////////
     L2=((2*(pow(m,2)))-(pow(j,2))-(pow(i,2)))/((2*i)+(2*j));
     ///////////////////////////////////////////////////
     //printf("\nL2=%g",L2);
   
     
     
     //L1=i+L2;
     //L3=j+L2;
     //printf("\nL1=%g",L1);
     
     
    // double alfa,c1,beta,c2,h1,h2;
    // alfa=asin((L2*sin(gama))/L1);
    // c1=asin((m*sin(gama))/L1);
    // beta=asin((L2*sin(gama_p))/L3);
    // c2=asin((m*sin(gama_p))/L3);
     gama=pi-gama_p;
     
     if(gama>=gama_p)
     {
		 h1=L2*sin(gama);
		 y_shot=h1*100;
	 }
	 else
	 {
		 h2=L2*sin(gama_p);
		 y_shot=h2*100;
	 }
     
     return(y_shot);
    // printf("\nY=%d",y_shot);
    // printf("\nY=%d",y_shot);
   
     
 }
 
 double y_calculator_type2(int t1,int t2,int x1,int x2,int proses_type)
 {
	 int t1t2=0,t3t2=0,C_sound_speed=450;
	 float L2=0,i=0,j=0,t12_d,t32_d,h2=0,h1=0;
	 
	if(proses_type==1)
	{
        C_sound_speed=350;
	}
    else if(proses_type==3)
    {
        C_sound_speed=350;
	}
    else if(proses_type==2)
    {
        C_sound_speed=300;
	}
    else if(proses_type==4)
    {
        C_sound_speed=655;
    }
	 // printf("\n_________Y_calculate_____________________________________");
	//  	printf("\nmax__index=%d",max__index);
	 
	 
	 t1t2=abs(t1-start_time_sorted[max__index][0]);  //t1-t2
     t3t2=abs(t2-start_time_sorted[max__index][0]);  //t3-t2
     
     //t1t2=t1;  //t1-t2
     //t3t2=t2;  //t3-t2
     float m=abs(start_time_sorted[max__index][1]-x1)*0.15;
     float n=abs(start_time_sorted[max__index][1]-x2)*0.15;
     //t1t2=11;
     //t3t2=12;
    // start_time_sorted[max__index+1][1]=4;
    // printf("\nt1t2=%d",t1t2);
    // printf("\nt3t2=%d",t3t2);
     
     t12_d=(t1t2*0.0000083333);
     t32_d=(t3t2*0.0000083333);
     //printf("\nt12_d=%g",t12_d);
     //printf("\nt32_d=%g",t32_d);
     
     i=((t12_d)*C_sound_speed);
     j=((t32_d)*C_sound_speed);
     
     //printf("\ni=%g",i);
     //printf("\nj=%g",j);
     
    double pi=3.1415;

    double _n2=pow(n,2),_2n=2*n;
    double _m2=pow(m,2),_2m=2*m;
    double _j2=(pow(j,2)), _i2=(pow(i,2));
    double _2j=2*j,_2i=2*i;
    double gama_p=0,gama=0;
     
     
     
    double q=_n2-_j2,p=-1*_2m ,t=_2i,s=_2j,r=_m2-_i2,w=_2n,qt=q*t,sr=s*r,wr=w*r,qp=q*p;
    double gama_p_cos=(qt-sr)/(wr-qp);
      //printf("\ngama_p_cos=%g",(gama_p_cos));
     gama_p=acos(gama_p_cos);
     
     // printf("\ngama_p=%g",(gama_p));
     //printf("\ngama_p_degree=%g",(gama_p*180)/pi);
    // printf("\ncos(gama_p)=%g",cos(gama_p));
     L2=(_n2-_j2)/(_2j+(_2n*cos(gama_p)));
     //printf("\nL2=%g",L2);
     
     L2=(_m2-_i2)/(_2i-(_2m*cos(gama_p)));
    // printf("\nL2=%g",L2);
     ///////////////////////////////////////////////////
    // L2=((2*(pow(m,2)))-(pow(j,2))-(pow(i,2)))/((2*i)+(2*j));
     ///////////////////////////////////////////////////
    // printf("\nL2=%g",L2);
   
     
     
     //L1=i+L2;
     //L3=j+L2;
     //printf("\nL2=%g",L1);
     
     
    // double alfa,c1,beta,c2,h1,h2;
    // alfa=asin((L2*sin(gama))/L1);
    // c1=asin((m*sin(gama))/L1);
    // beta=asin((L2*sin(gama_p))/L3);
    // c2=asin((m*sin(gama_p))/L3);
     gama=pi-gama_p;
     if(gama>=gama_p)
     {
		 h1=L2*sin(gama_p);
		 y_shot=h1*100;
	 }
	 else
	 {
		 h2=L2*sin(gama);
		 y_shot=h2*100;
	 }
     return(y_shot);
     
    // printf("\nY=%d",y_shot);
    // printf("\nY=%d",y_shot);
    
     
 }
  
 int prosec_type()
 {
	 int proses_type=1;
	 int sensor_2_1_time=abs(start_time_sorted[max__index+1][0]-start_time_sorted[max__index][0]);
	 int sensor_3_2_time=abs(start_time_sorted[max__index+2][0]-start_time_sorted[max__index][0]);
	 
	 int peresent_of_min=(sensor_2_1_time*100)/sensor_3_2_time;

 
 
 
 if(peresent_of_min>=35)
 {
      proses_type=1;
  }
 else
 {
     proses_type=2;
 }
 
  if(peresent_of_min>45)
  {
         if(peresent_of_min<55)
         {             
              proses_type=3;
         }
  }
 
	 if(start_time_sorted[max__index+1][1]==1)
	 {
		 proses_type=1;
	 }
     
     if(start_time_sorted[max__index+1][1]==7)
     {
         proses_type=1;
     }
     
     if(start_time_sorted[max__index+2][1]==1)
     {
         proses_type=1;
     }
     
     if(start_time_sorted[max__index+2][1]==7)
     {
         proses_type=1;
     }
     
     if(start_time_sorted[max__index][1]==1)
     {
         proses_type=1;
     }
     
     if(start_time_sorted[max__index][1]==7)
     {
         proses_type=1;
     }
    
     // printf("\nsensor_2_1_time=%d \nsensor_3_2_time=%d \nperesent_of_min=%d",sensor_2_1_time,sensor_3_2_time,peresent_of_min);
 
     return proses_type;
	}
	double abs_me(double input)
	{
		if((input)<0)
		{
			return(-1*(input));
		}
		else
		{
			return((input));
		}
	}
	
void calculate_left_ritgh_sensor_sorted()
{
	 int n_shoot_s=start_time_sorted_dif[max__index][1];
     int i_ns=1;
     int i,j=0;
     for(i=n_shoot_s+1;i<=7;i++)
     {
         left_sensor_sorted[i_ns][2]=i;
         i_ns=i_ns+1;
     }
     
     i_ns=1;
     
     for(i=n_shoot_s-1;i>0;i--)
     {
         ritgh_sensor_sorted[i_ns][2]=i;
         i_ns=i_ns+1;
     }
     
     
     left_max=0;
     ritgh_max=0;
    
    for(i=1;i<=7;i++)
    {
        for(j=1;j<=7;j++)
        {
            if(left_sensor_sorted[i][2]==start_time_sorted_dif[j][1])
            {
                left_sensor_sorted[i][1]=start_time_sorted_dif[j][0];
                left_max=left_max+1;
            }
            
            if(ritgh_sensor_sorted[i][2]==start_time_sorted_dif[j][1])
            {
                ritgh_sensor_sorted[i][1]=start_time_sorted_dif[j][0];
                ritgh_max=ritgh_max+1;
            }
        }
       
    }
    
   printf("\n___________________________________________________________________________");
    for(i=1;i<=6;i++)
    {
    printf("\nritgh_sensor_sorted[%d][1]=%d  ritgh_sensor_sorted[%d][0]=%d",i,ritgh_sensor_sorted[i][1],i,ritgh_sensor_sorted[i][2]);
	}
	printf("\n");
	 for(i=1;i<=6;i++)
    {
    printf("\nleft_sensor_sorted[%d][1]=%d  left_sensor_sorted[%d][0]=%d",i,left_sensor_sorted[i][1],i,left_sensor_sorted[i][2]);
	}
	printf("\n_________________________________________________________________________");
	
	
}	

 void sort_(double M1,double M2,double M3,double M4,double M5,double M6,double M7,double M8)
{
	int  min=100;
	int index_min=-1;
	double max_domain_list_sorted[9][2];
	double max_domain_list[9][3];
	
	  max_domain_list[1][1]=M1;
	  max_domain_list[2][1]=M2;
	  max_domain_list[3][1]=M3;
	  max_domain_list[4][1]=M4;
	  max_domain_list[5][1]=M5;
	  max_domain_list[6][1]=M6;
	  max_domain_list[7][1]=M7;
	  max_domain_list[8][1]=M8;
	 
	  max_domain_list[1][2]=0;
	  max_domain_list[2][2]=0;
	  max_domain_list[3][2]=0;
	  max_domain_list[4][2]=0;
	  max_domain_list[5][2]=0;
	  max_domain_list[6][2]=0;
	  max_domain_list[7][2]=0;
	  max_domain_list[8][2]=8;


	

int b,a,i;
  for(b=7;b>0;b--)
  {    
       min=9999999;
      for(a=1;a<=8;a++)
      {
        
        if(max_domain_list[a][2]==0)
        {
				if(min>max_domain_list[a][1])
				{
					min=max_domain_list[a][1];
				   index_min=a;    
				}

		}
      }
      max_domain_list[index_min][2]=b;
       max_domain_list_sorted[b][1]=min;
       max_domain_list_sorted[b][2]=index_min;
  }
  
    max_domain_list_sorted[8][1]=max_domain_list[8][1];
    max_domain_list_sorted[8][2]=max_domain_list[8][2];
       
   



   for( i=1;i<=8;i++)
   { 
           matris_sorted_sot_func[1][i]=max_domain_list_sorted[i][1];
           matris_sorted_sot_func[2][i]=max_domain_list_sorted[i][2];  
    }

    
    
    	   
	
 /*       
	
 printf("\n\n");
for(i=1;i<=8;i++)
    {
   
		printf("\nmatris_sorted[1][%i]=%g   matris_sorted[2][%i]=%g",i,matris_sorted_sot_func[1][i],i,matris_sorted_sot_func[2][i]);   
	}
	*/
  
}


void sort_reapet_y_shot_calculated(double b[10],int nr_of_reapet )
{
	
	int i=0,j=0;
	double	reapet_value[10][3];
 for( i=1;i<10;i++)
 {
    reapet_value[i][2]=0;
    reapet_value[i][1]=0;
}
   
    
   for(i=1;i<=nr_of_reapet;i++)
   {
        for(j=1;j<=nr_of_reapet;j++)
        {
            if(b[i]==b[j])
            {
                reapet_value[i][2]=reapet_value[i][2]+1;
                reapet_value[i][1]=b[i];
            }
       
	}
   }
   
   
 
 
  for(i=1;i<=nr_of_reapet;i++)
  {
        for(j=1;j<=nr_of_reapet;j++)
        {
           if(reapet_value[i][1]==reapet_value[j][1])
           {
               if(i!=j)
               {
                   reapet_value[j][1]=0;
                   reapet_value[j][2]=0;
               }
           }
        }
  }

 /*  printf("\n\n");
 for(i=1;i<=nr_of_reapet;i++)
   {
	    printf("\nreapet_value[%d][1]=%g   reapet_value[%d][2]=%g",i,  reapet_value[i][1] ,i,reapet_value[i][2]);
      // printf("\nb[%d]=%g ",i,  b[i]);
   }*/
  
   
   sort_(reapet_value[1][2],reapet_value[2][2],reapet_value[3][2],reapet_value[4][2],reapet_value[5][2],reapet_value[6][2],reapet_value[7][2],reapet_value[8][2]);
    

    
     int index=0;
     for(i=1;i<=nr_of_reapet;i++)
     {
		 index=matris_sorted_sot_func[2][i];
         final_sort[i][1]=reapet_value[index][1];
         final_sort[i][2]=matris_sorted_sot_func[1][i];
     }
    
   /* for(i=1;i<=nr_of_reapet;i++)
    {
		printf("\nfinal_sort[%i][1]=%g   final_sort[%i][2]=%g",i,final_sort[i][1],i,final_sort[i][2]);   
	}*/
  
     
}	
	
	
double check_y_shot_test(int ns,int ts,int n_shooted_s,int proses_type)
{
	//printf("\nn_shooted_s=%d",n_shooted_s);
	//printf("\nns=%d",ns);
	//printf("\nts=%d",ts);
	double y=x_shot;
	double Defalt_des_sen=(ns-1)*15;
	double des_sen=abs_me((Defalt_des_sen-y));
	//printf("\ny=%g",y);
	//printf("\nproses_type=%d",proses_type);
	//printf("\nDefalt_des_sen=%g",Defalt_des_sen);
	//printf("\ndes_sen=%g",des_sen);

	double defalt_des_shoted_sensor=(n_shooted_s-1)*15;
	double des_shoted_sensor=abs_me(defalt_des_shoted_sensor-y);

	//printf("\ndefalt_des_shoted_sensor=%g",defalt_des_shoted_sensor);
	//printf("\ndes_shoted_sensor=%g",des_shoted_sensor);

  
  double result[13];
  int i=0;
  for(i=1;i<=12;i++)
  {
   result[i]=100;
  }
  
  int start_for,stop_for,ri;
  if(proses_type==2)
  {
      start_for=4;
      stop_for=6;
      ri=4;
  }else{
     start_for=2;
     stop_for=7;
     ri=0; 
  }
  
 
    double  min=100;
    int min_index=0;
       double h1,L1_test,L2_test,test,first,second,min_main;
       //printf("\nstart_for=%d",start_for);
       //printf("\nstop_for=%d",stop_for);
    for( i=start_for;i<=stop_for;i++)
	{
        h1=y_shot_list[i];
        L1_test=sqrt(pow(h1,2)+(pow(des_sen,2)));
        L2_test=sqrt(pow(h1,2)+pow(des_shoted_sensor,2));
        test=L1_test-L2_test;
       //printf("\nh1=%g",h1);
       //printf("\nL1_test=%g",L1_test);
       //printf("\nL2_test=%g",L2_test);
       //printf("\ntest=%g",test);
        
         if(i<4)
         {
            first=(ts*0.0000083333*350)*100;
              second=100;
          } 
          else
          {
               first=100;     
          }
           
           
           if(i>3)
           {
               if(i<6){
                  second=(ts*0.0000083333*450)*100;
                  first=100;   
               }
           }
           else
           {
               second=100;
           }
           
           if(i>5)
           {
                 first=(ts*0.0000083333*350)*100;
                 second=(ts*0.0000083333*350)*100;
           }
           
             ri=ri+1;
             result[ri]=test-first;
             
              ri=ri+1;
             result[ri]=test-second;
             
             if(proses_type==1)
             {
                 if(i>3)
                 {
                     if(i<6)
                     {
                          result[ri]=100;
                           result[ri-1]=100;
                     }
                 }
             }
              if(proses_type==3)
              {
                 if(i>3)
                 {
                     if(i<6)
                     {
                          result[ri]=100;
                           result[ri-1]=100;
                     }
                 }
             }
          //printf("\nresult[ri]=%g",result[ri]);
          //printf("\nresult[ri-1]=%g",result[ri-1]); 
    }
    
    
    
     for(i=1;i<=12;i++)
     {
		  //printf("\nresult[i]=%g",result[i]); 
		   //printf("\nabs_me(result[i])=%g",abs_me(result[i])); 
		   //printf("\nmin=%g",min);
               if(abs_me(result[i])<min)
               {
                   min=abs_me(result[i]);
                   min_index=i;
               }
              
     }
       
       
       min_index=min_index-1;
     
        min_main=(min_index-((min_index%2)))/2;
        
        min_main=min_main+2;
      
          //printf("\nmin_main=%g",min_main);
          
          
          
          
          
        return(min_main);
        //min_h_list(1,2)=min;
        

  
}
	
void pre_check_y_shot_test(int proses_type)
{

	//printf("\nleft_max=%d",left_max);
	//printf("\nritgh_max=%d",ritgh_max);
	int i,j;
	double check_res[10];
	for(i=1;i<=left_max;i++)
	{		
        check_res[i]=check_y_shot_test(left_sensor_sorted[i][2],left_sensor_sorted[i][1],start_time_sorted[max__index][1],proses_type);      
        //printf("\ncheck_res[%d]=%g",i,check_res[i]);
    }
    
    for(j=1;j<=ritgh_max;j++)
	{
        check_res[left_max+j]=check_y_shot_test(ritgh_sensor_sorted[j][2],ritgh_sensor_sorted[j][1],start_time_sorted[max__index][1],proses_type);  
        //printf("\n__check_res[%d]=%g",left_max+j,check_res[left_max+j]);
    }
    /*printf("\n\n\n______________________");
     for(j=1;j<=ritgh_max+left_max;j++)
	{
          printf("\n__check_res[%d]=%g",j,check_res[j]);
    }*/
    sort_reapet_y_shot_calculated(check_res,ritgh_max+left_max);
    
}
	
	/*int proses_type_4( )
	{
		int proses_type=0;
		//////////////////////////////////////////////////////////////////////////////////	 
					/////////////////////////////proses_type 4 calculator/////////////////////////////////////	 
						int index_kaji=0;
					  if(left_max_new>=ritgh_max_new)
					  {
						  index_kaji=ritgh_max_new;
					  }
					  else if(left_max_new<ritgh_max_new)
					  {
						  index_kaji=left_max_new ;    
					  }
					  // index_kaji;
					   
					   
					   
					
					   
					   
					int kaji_min=0;
					int kaji_max=0;
						
						
						
					int	left_kandid=left_sensor_sorted_new[index_kaji][1];
					int	ritgh_kandid=ritgh_sensor_sorted_new[index_kaji][1];
					   
					if(left_kandid<=ritgh_kandid)
					{
						kaji_min=left_kandid;
						kaji_max=ritgh_kandid;
					}
					else if(left_kandid>ritgh_kandid)
					{
						 kaji_min=ritgh_kandid;
						 kaji_max=left_kandid;
					}
					
						kaji_min=kaji_min+((kaji_min*30)/100);
						
						if(kaji_min<kaji_max)
						{
							proses_type=4;
							return proses_type;
						}
						return proses_type;
	}
*/	
	
void calculate_left_ritgh_sensor_sorted_new()
{
	int j;
		////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////create new left o ritgh by deleted zero and final sensor of left o ritgh//////////////////////////
				   for(j=1;j<=ritgh_max;j++)
				   {
							ritgh_sensor_sorted_new[j][1]=ritgh_sensor_sorted[j][1];
							ritgh_sensor_sorted_new[j][2]=ritgh_sensor_sorted[j][2];
				   }
                   
                   for(j=1;j<=left_max;j++)
                   {
                          left_sensor_sorted_new[j][1]=left_sensor_sorted[j][1];
                          left_sensor_sorted_new[j][2]=left_sensor_sorted[j][2];
                   }
                     ritgh_max_new=ritgh_max;
                     left_max_new=left_max;
           						  
							 if( start_time_sorted_dif[2][1]> start_time_sorted_dif[1][1])
							 {
								 
								   for(j=2;j<=left_max;j++)
								   {
										  left_sensor_sorted_new[j-1][1]=left_sensor_sorted[j][1];
										  left_sensor_sorted_new[j-1][2]=left_sensor_sorted[j][2];
								   }
								   left_max_new=left_max-1;
							 }
							 else
							 {
								  
								   for(j=2;j<=ritgh_max;j++)
								   {
										ritgh_sensor_sorted_new[j-1][1]=ritgh_sensor_sorted[j][1];
										ritgh_sensor_sorted_new[j-1][2]=ritgh_sensor_sorted[j][2];
								   }
								 ritgh_max_new=ritgh_max-1;
							 }
						 
						printf("\n");
									for(j=1;j<=left_max_new;j++)
								   { 
										  printf("\nleft_sensor_sorted_new[%d][1]=%d  left_sensor_sorted_new[%d][2]=%d",j,left_sensor_sorted_new[j][1],j,left_sensor_sorted_new[j][2]);
								   }
								  printf("\n");
								  
								   for(j=1;j<=ritgh_max_new;j++)
								   {
										printf("\nritgh_sensor_sorted_new[%d][1]=%d  ritgh_sensor_sorted_new[%d][2]=%d",j,ritgh_sensor_sorted_new[j][1],j,ritgh_sensor_sorted_new[j][2]);
								   }
						 printf("\n\n ritgh_max_new=%d",  ritgh_max_new);
						  printf("\n left_max_new =%d",  left_max_new);
						 
}
	

void x_calculator_avr_kol()
{
	double x_avr[10];
	double avr_1_2_kol,avr_1_3_kol,avr_1_2_3_kol,avr_1_2_3_4_kol,x_step2_2_kol;
	double x_step1_1_kol,x_step1_2_kol,x_step2_1_kol,x_avr_calc_kol,x_avr_calc=0;
	 int x_avr_index=1,j,i,a,b,c,d;
	 
        for(j=1;j<=left_max;j++)
        {
            for(i=1;i<=ritgh_max;i++)
            {
				
                 a=left_sensor_sorted[j][1];
                 b=ritgh_sensor_sorted[i][1];
                 c=left_sensor_sorted[j][2];
                 d=ritgh_sensor_sorted[i][2];
                   x_avr[x_avr_index] = x_calculator(a,b,c,d);
                   x_avr_calc=x_avr_calc+x_avr[x_avr_index];
                  
                   if(j==1)
                   {
                       if(i==1)
                       {
                           x_step1_1_kol= x_avr[x_avr_index] ;
                       }
                   }
                   if(j==2)
                   {
                       if(i==1)
                       {
                           x_step2_1_kol= x_avr[x_avr_index];
                       }
                   }
                   
                   if(j==1)
                   {
                       if(i==2)
                       {
                           x_step1_2_kol= x_avr[x_avr_index];
                       }
                   }
                   if(j==2)
                   {
                       if(i==2)
                       {
                           x_step2_2_kol= x_avr[x_avr_index];
                       }
                   }
                    x_avr_index=x_avr_index+1;
            }
        }
         
x_avr_calc_kol=x_avr_calc/(x_avr_index-1);


avr_1_2_kol=(x_step1_1_kol+x_step1_2_kol)/2;
avr_1_3_kol=(x_step1_1_kol+x_step2_1_kol)/2;
avr_1_2_3_kol=(x_step1_1_kol+x_step1_2_kol+x_step2_1_kol)/3;
avr_1_2_3_4_kol=(x_step1_1_kol+x_step1_2_kol+x_step2_1_kol+x_step2_2_kol)/4;

show_avr_x[1][1]=x_step1_1_kol;
show_avr_x[1][2]=avr_1_2_kol;
show_avr_x[1][3]=avr_1_3_kol;
show_avr_x[1][4]=avr_1_2_3_kol;
show_avr_x[1][5]=avr_1_2_3_4_kol;
show_avr_x[1][6]=x_avr_calc_kol;
}

void x_calculator_avr_kol_wl()
{
	double x_avr_kol_w_l[10],x_avr_calc_kol_W_l,x_avr_calc=0;
	int L_max,R_max;
	 if( ritgh_sensor_sorted[ritgh_max_new][1]>left_sensor_sorted[left_max_new][1])
	 {
           L_max=left_max;
           R_max=ritgh_max-1;
      } 
      else
      {
           L_max=left_max-1;
           R_max=ritgh_max;
       }

        
   
   
        int x_avr_index=1,a,b,c,d,i,j;
        for(j=1;j<=L_max;j++)
        {
            for(i=1;i<=R_max;i++)
            {
                 a=left_sensor_sorted[j][1];          
                 b=ritgh_sensor_sorted[i][1];    
                 c=left_sensor_sorted[j][2];
                 d=ritgh_sensor_sorted[i][2];
                   x_avr_kol_w_l[x_avr_index] = x_calculator(a,b,c,d);  
                   x_avr_calc=x_avr_calc+x_avr_kol_w_l[x_avr_index];
                   x_avr_index=x_avr_index+1;
            }
        }
   
       
    
        x_avr_calc_kol_W_l=x_avr_calc/(x_avr_index-1);
        show_avr_x[1][7]=x_avr_calc_kol_W_l;



}

void x_calculator_avr_kol_wf()
{
		
				double x_avr_w_f_l[10],x_avr_calc_kol_wfl,x_avr_calc=0;
				int x_avr_index=1,a,b,c,d,j,i;
				
				for(j=1;j<=left_max_new;j++)
				{
					for(i=1;i<=ritgh_max_new;i++)
					{
						 a=left_sensor_sorted_new[j][1];
						 b=ritgh_sensor_sorted_new[i][1];
						 c=left_sensor_sorted_new[j][2];
						 d=ritgh_sensor_sorted_new[i][2];
						 x_avr_w_f_l[x_avr_index] = x_calculator(a,b,c,d);
						 x_avr_calc=x_avr_calc+x_avr_w_f_l[x_avr_index];
						   x_avr_index=x_avr_index+1;
					}
				}
			 x_avr_calc_kol_wfl=x_avr_calc/(x_avr_index-1);
				
		

		
		show_avr_x[1][14]=x_avr_calc_kol_wfl;
}

void x_calculator_avr_kol_wfl()
{
	int L_max,R_max;
	 if( ritgh_sensor_sorted_new[ritgh_max_new][1]>left_sensor_sorted_new[left_max_new][1])
	 {
           L_max=left_max_new;
           R_max=ritgh_max_new-1;
	   }
       else
       {
           L_max=left_max_new-1;
           R_max=ritgh_max_new;
       }

        double x_avr_w_f_ll[10],x_avr_calc_kol_wfl,x_avr_calc=0;
        int x_avr_index=1,a,b,c,d,i,j;
        for(j=1;j<=L_max;j++)
        {
            for(i=1;i<=R_max;i++)
            {
                 a=left_sensor_sorted_new[j][1];
                 b=ritgh_sensor_sorted_new[i][1];
                 c=left_sensor_sorted_new[j][2];
                 d=ritgh_sensor_sorted_new[i][2];
                   x_avr_w_f_ll[x_avr_index] = x_calculator(a,b,c,d);
                   x_avr_calc=x_avr_calc+x_avr_w_f_ll[x_avr_index];
                   x_avr_index=x_avr_index+1;
            }
        }
     x_avr_calc_kol_wfl=x_avr_calc/(x_avr_index-1);
     
     show_avr_x[1][15]=x_avr_calc_kol_wfl;
     show_avr_x[1][16]=(show_avr_x[1][15]+show_avr_x[1][14]+show_avr_x[1][7]+show_avr_x[1][6])/4;
}

int x_calculator_final_check_and_find()
{
	int l[2][3];
	  l[1][1]=abs(left_sensor_sorted[1][1]-ritgh_sensor_sorted[1][1]);
      l[1][2]=abs(left_sensor_sorted[1][1]-ritgh_sensor_sorted[2][1]);
    int x_calc_step_two=0;
     if(left_sensor_sorted[1][1]<=1)
     {
         x_calc_step_two=1;
     }
     if(ritgh_sensor_sorted[1][1]<=1)
     {
         x_calc_step_two=1;
     }
     
     if(l[1][1]>=10)  
     {     
            if(l[1][2]>=40)
            {
               x_calc_step_two=1;
			}
     }
    
     
     if(x_calc_step_two==0)
     {
         //x_calc_new[1][1]=show_avr_x[1][2];
         //x_calc_new[1][2]=show_avr_x[2][2];
         return(2);
	 }
     else
     {
         //x_calc_new[1][1]=show_avr_x[1][16];
         //x_calc_new[1][2]=show_avr_x[2][16];
         return(16);
     }
}
	
void x_pre_calculate(int proses_type)
	{
		
		x_calculator_avr_kol();
		x_calculator_avr_kol_wl();
		x_calculator_avr_kol_wf();
		x_calculator_avr_kol_wfl();
		int x_calced_index=x_calculator_final_check_and_find();
		
		int i=0;
		for(i=1;i<=20;i++)
		{
			 printf("\n  show_avr_x=%g",show_avr_x[1][i]);
		}


         x_shot=show_avr_x[1][x_calced_index];
		 printf("\n\n.   x_shot=%g",x_shot);
		
		 
	}
	
	
	
void y_pre_calculate(int proses_type)
{
	double y_shot1=0,y_shot2=0,y_shot3=0,y_shot4=0,y_shot5=0,y_shot6=0;
	y_shot1=y_calculator_type1(start_time_sorted[max__index+1][0],start_time_sorted[max__index+2][0],0.15);             
	y_shot2=y_calculator_type1( start_time_sorted[max__index+3][0],start_time_sorted[max__index+4][0],0.3);
   
   
   
   
   y_shot3=y_calculator_type2(start_time_sorted[max__index+3][0],start_time_sorted[max__index+2][0],start_time_sorted[max__index+3][1],start_time_sorted[max__index+2][1],proses_type);             
   y_shot4=y_calculator_type2( start_time_sorted[max__index+5][0],start_time_sorted[max__index+4][0],start_time_sorted[max__index+5][1],start_time_sorted[max__index+4][1],proses_type);
   
       
   y_shot5=(y_shot1+y_shot2+y_shot3+y_shot4)/4;
   y_shot6=(y_shot1+y_shot2+y_shot3)/3;
   
   
   

      
		y_shot_list[2]=y_shot1;
		y_shot_list[3]=y_shot2;
		y_shot_list[4]=y_shot3;
		y_shot_list[5]=y_shot4;
		y_shot_list[6]=y_shot5;
		y_shot_list[7]=y_shot6;
		
      
      
      pre_check_y_shot_test(proses_type);
      int index1,index2;
      index1=final_sort[1][1];
      index2=final_sort[2][1];
       if(final_sort[1][2]==final_sort[2][2])
     {
		y_shot_list[8]= (y_shot_list[index1]+y_shot_list[index2])/2;	 
     }
     else
     {
		y_shot_list[8]= y_shot_list[index1];	 
     } 
      
      
}

void calculator_signal(int max_index)
{
	
	
	
	//////sorting start time//////////////
	sort_matrix(1);
	//////////////////////////////////////
	
	//////max find//////////////
	MaxValue_index_find();
	//////////////////////////////////////
	
	//////out find//////////////
    out_detect=false;
	out_detect=Out_find();
    //////////////////////////////////////
    
    //////error_find_mines//////////////
	out_detect=error_find_mines();
	//////////////////////////////////////
	
	 //////error_find_mines//////////////
	//out_detect=error_find_yeksan();
	//////////////////////////////////////
    
    
	if(out_detect==false)
	{
		printf("\n**********inside_shot****************");   
		
		start_time[1]=start_time[1]+0;
		start_time[2]=start_time[2]+1;
		start_time[3]=start_time[3]+2;
		start_time[4]=start_time[4]+3;
		start_time[5]=start_time[5]+4;
		start_time[6]=start_time[6]+5;
		start_time[7]=start_time[7]+6;
		start_time[8]=start_time[8]+7;
	
		//////sorting start time//////////////
		sort_matrix(1);
		//////////////////////////////////////
		set_value_to_start_time_sorted_dif();
	    calculate_left_ritgh_sensor_sorted();
	    calculate_left_ritgh_sensor_sorted_new();
		          
	 
	     //////calculate_proses_type//////////////
		 int proses_type=1;
		 proses_type=prosec_type();
		 printf("\nproses_type=%d",proses_type); 
		 
		 
		//////x_calculator//////////////
		printf("\n-----------------x_calculator-------------------");	
		x_pre_calculate(proses_type);

		//////y_calculator//////////////
		printf("\n-----------------y_calculator-------------------");
	    y_pre_calculate(proses_type);
	    y_shot=y_shot_list[8];
		  printf("\ny_shot1=%g",y_shot_list[1]);
		  printf("\ny_shot2=%g",y_shot_list[2]);
		  printf("\ny_shot3=%g",y_shot_list[3]);
		  printf("\ny_shot4=%g",y_shot_list[4]);
		  printf("\ny_shot5=%g",y_shot_list[5]);
		  printf("\ny_shot6=%g",y_shot_list[6]);
		  printf("\ny_shot7=%g",y_shot_list[7]);
		  printf("\ny_shot8=%g",y_shot);
		  printf("\ny_shot8=%g",y_shot);
		
////////////////////////////////////
		
	}
	else
	{
		printf("\nOUT_shot");
		printf("\nOUT_shot");
	}
	
	

}

 
void clump_and_send(int max_index)		
{
	
	
	calculator_signal(max_index);
	
	
	
	//if(out_detect==false)
	{
			int start_index=max_index-1000;
			int stop_indx=max_index+3000;
			
			
		

			
			int i=0;
			int cunter_send_buffer=0;
			int lenght=4010;
	
			for(i=start_index;i<stop_indx;i++)
			{
			  SEND_bufer[cunter_send_buffer]=((unsigned char)(FIFO_sen1[i]*100)%0x100);		
			  SEND_bufer[(lenght)+(cunter_send_buffer)]  =(unsigned char)(FIFO_sen2[i]*100)%0x100;	  
			  SEND_bufer[(lenght*2)+(cunter_send_buffer)]=(unsigned char)(FIFO_sen3[i]*100)%0x100;	  
			  SEND_bufer[(lenght*3)+(cunter_send_buffer)]=(unsigned char)(FIFO_sen4[i]*100)%0x100;	  
			  SEND_bufer[(lenght*4)+(cunter_send_buffer)]=(unsigned char)(FIFO_sen5[i]*100)%0x100;		  
			  SEND_bufer[(lenght*5)+(cunter_send_buffer)]=(unsigned char)(FIFO_sen6[i]*100)%0x100;		  
			  SEND_bufer[(lenght*6)+(cunter_send_buffer)]=(unsigned char)(FIFO_sen7[i]*100)%0x100;		  
			  SEND_bufer[(lenght*7)+(cunter_send_buffer)]=(unsigned char)(FIFO_sen8[i]*100)%0x100;	
			  
			   
			   	  cunter_send_buffer++;
			   	 
			}
			
			SEND_bufer[lenght-1]=0;
			SEND_bufer[lenght-2]=0;
			SEND_bufer[lenght-3]=0;
			SEND_bufer[lenght-4]=0;
			SEND_bufer[lenght-5]=0;
			SEND_bufer[lenght-6]=0;
			SEND_bufer[lenght-7]=0;
			SEND_bufer[lenght-8]=0;
			SEND_bufer[lenght-9]=0;
			SEND_bufer[lenght-10]=0;

		
			Sena_Data_to_Ethe();
	}		
	
}

void make_fifo()		
{
	//printf("\n---------------------make_fifo-------------------------------");
	int i=0;
	
	for(i=0;i<total_sample/2;i++)
	{
		FIFO_sen1[i]=FIFO_sen1[i+(total_sample/2)];
		FIFO_sen1[i+(total_sample/2)]=FIFO_sen1[i+((total_sample/2)*2)];
		FIFO_sen1[i+((total_sample/2)*2)]=(((sen1[i*2]*0x100)+sen1[(i*2)+1])*0.0008058608058);
		
		FIFO_sen2[i]=FIFO_sen2[i+(total_sample/2)];
		FIFO_sen2[i+(total_sample/2)]=FIFO_sen2[i+((total_sample/2)*2)];
		FIFO_sen2[i+((total_sample/2)*2)]=(((sen2[(i*2)]*0x100)+sen2[(i*2)+1])*0.0008058608058);
		
		FIFO_sen3[i]=FIFO_sen3[i+(total_sample/2)];
		FIFO_sen3[i+(total_sample/2)]=FIFO_sen3[i+((total_sample/2)*2)];
		FIFO_sen3[i+((total_sample/2)*2)]=(((sen3[(i*2)]*0x100)+sen3[(i*2)+1])*0.0008058608058);
		
		FIFO_sen4[i]=FIFO_sen4[i+(total_sample/2)];
		FIFO_sen4[i+(total_sample/2)]=FIFO_sen4[i+((total_sample/2)*2)];
		FIFO_sen4[i+((total_sample/2)*2)]=(((sen4[(i*2)]*0x100)+sen4[(i*2)+1])*0.0008058608058);
		
		FIFO_sen5[i]=FIFO_sen5[i+(total_sample/2)];
		FIFO_sen5[i+(total_sample/2)]=FIFO_sen5[i+((total_sample/2)*2)];
		FIFO_sen5[i+((total_sample/2)*2)]=(((sen5[(i*2)]*0x100)+sen5[(i*2)+1])*0.0008058608058);
		
		FIFO_sen6[i]=FIFO_sen6[i+(total_sample/2)];
		FIFO_sen6[i+(total_sample/2)]=FIFO_sen6[i+((total_sample/2)*2)];
		FIFO_sen6[i+((total_sample/2)*2)]=(((sen6[(i*2)]*0x100)+sen6[(i*2)+1])*0.0008058608058);
	
		FIFO_sen7[i]=FIFO_sen7[i+(total_sample/2)];
		FIFO_sen7[i+(total_sample/2)]=FIFO_sen7[i+((total_sample/2)*2)];
		FIFO_sen7[i+((total_sample/2)*2)]=(((sen7[(i*2)]*0x100)+sen7[(i*2)+1])*0.0008058608058);
		
		FIFO_sen8[i]=FIFO_sen8[i+(total_sample/2)];
		FIFO_sen8[i+(total_sample/2)]=FIFO_sen8[i+((total_sample/2)*2)];
		FIFO_sen8[i+((total_sample/2)*2)]=(((sen8[i*2]*0x100)+sen8[(i*2)+1])*0.0008058608058);
		
		//printf("\nFIFO_sen2[i]=%g",FIFO_sen2[i]);
	}
	
	//getchar();
	
}

 void check_for_max_index()		
{
	//printf("\n---------------------check_for_max_index---------------------");	
	//////////////////////signal  avrage  calculate /////////////////////////////////////////	
	int i=0;
	double signal_midel_avr[9]={0,0,0,0,0,0,0,0,0};
		for(i=(total_sample/2);i<((total_sample/2)*2);i++)
		{
			signal_midel_avr[1]=FIFO_sen1[i]+signal_midel_avr[1];
			signal_midel_avr[2]=FIFO_sen2[i]+signal_midel_avr[2];
			signal_midel_avr[3]=FIFO_sen3[i]+signal_midel_avr[3];
			signal_midel_avr[4]=FIFO_sen4[i]+signal_midel_avr[4];
			signal_midel_avr[5]=FIFO_sen5[i]+signal_midel_avr[5];
			signal_midel_avr[6]=FIFO_sen6[i]+signal_midel_avr[6];
			signal_midel_avr[7]=FIFO_sen7[i]+signal_midel_avr[7];
			signal_midel_avr[8]=FIFO_sen8[i]+signal_midel_avr[8];		
		}
		
		int RQ_peresent=35; //  %10   //
		double	signal_pick_max[9]={5,5,5,5,5,5,5,5,5};
		double	signal_pick_min[9]={0,0,0,0,0,0,0,0,0};
		for(i=1;i<9;i++)
		{
			signal_midel_avr[i]=signal_midel_avr[i]/(((total_sample/2)*2)-(total_sample/2));
			if(signal_midel_avr[i]!=0)
			{
			signal_avrage_sensor[i]=(signal_avrage_sensor[i]+signal_midel_avr[i])/2;
			}
			
			
			signal_pick_max[i]=signal_avrage_sensor[i]+((signal_avrage_sensor[i]*RQ_peresent)/100);
			signal_pick_min[i]=signal_avrage_sensor[i]-((signal_avrage_sensor[i]*RQ_peresent)/100);
			//printf("\nsignal_pick_max[%d]=%g",i,signal_pick_max[i]);
			//printf("\nsignal_pick_min[%d]=%g",i,signal_pick_min[i]);
		}
		
	
			
			int index_pick_detection=-1;
			bool pick_detection=false;
			start_time[0]=-1;
			start_time[1]=-1;
			start_time[2]=-1;
			start_time[3]=-1;
			start_time[4]=-1;
			start_time[5]=-1;
			start_time[6]=-1;
			start_time[7]=-1;
			start_time[8]=-1;
			for(i=(total_sample/2)+1;i<((total_sample/2)*2);i++)
			{
				                                                    
				if((FIFO_sen1[i]>signal_pick_max[1])||(FIFO_sen1[i]<signal_pick_min[1]))
				{
					if(start_time[1]==-1)
					{
						start_time[1]=i;
						if(index_pick_detection==-1){index_pick_detection=i;}
						pick_detection=true;
						//printf("\ni_1=%d",i);	
							
					/*	printf("\nsignal_pick_max[1]=%g",signal_pick_max[1]);	
						
						int yyy=10;	
						printf("\nFIFO_sen1[%d]=%g",yyy,FIFO_sen1[yyy]);	yyy--;
						printf("\nFIFO_sen1[%d]=%g",yyy,FIFO_sen1[yyy]);	yyy--;
						printf("\nFIFO_sen1[%d]=%g",yyy,FIFO_sen1[yyy]);	yyy--;
						printf("\nFIFO_sen1[%d]=%g",yyy,FIFO_sen1[yyy]);	yyy--;
						printf("\nFIFO_sen1[%d]=%g",yyy,FIFO_sen1[yyy]);	yyy--;
						printf("\nFIFO_sen1[%d]=%g",yyy,FIFO_sen1[yyy]);	yyy--;
						printf("\nFIFO_sen1[%d]=%g",yyy,FIFO_sen1[yyy]);	yyy--;
						printf("\nFIFO_sen1[%d]=%g",yyy,FIFO_sen1[yyy]);	yyy--;
						printf("\nFIFO_sen1[%d]=%g",yyy,FIFO_sen1[yyy]);	yyy--;
						printf("\nFIFO_sen1[%d]=%g",yyy,FIFO_sen1[yyy]);	yyy--;
				
						
						 yyy=i;	
						printf("\nFIFO_sen1[%d]=%g",yyy,FIFO_sen1[yyy]);	yyy--;
						printf("\nFIFO_sen1[%d]=%g",yyy,FIFO_sen1[yyy]);	yyy--;
						printf("\nFIFO_sen1[%d]=%g",yyy,FIFO_sen1[yyy]);	yyy--;
						printf("\nFIFO_sen1[%d]=%g",yyy,FIFO_sen1[yyy]);	yyy--;
						printf("\nFIFO_sen1[%d]=%g",yyy,FIFO_sen1[yyy]);	yyy--;
						printf("\nFIFO_sen1[%d]=%g",yyy,FIFO_sen1[yyy]);	yyy--;
						printf("\nFIFO_sen1[%d]=%g",yyy,FIFO_sen1[yyy]);	yyy--;*/
						
					}					
				}
				if((FIFO_sen2[i]>signal_pick_max[2])||(FIFO_sen2[i]<signal_pick_min[2]))
				{
					if(start_time[2]==-1)
					{
						start_time[2]=i;
						if(index_pick_detection==-1){index_pick_detection=i;}
						pick_detection=true;
						//printf("\ni_2=%d",i);
						//printf("\nFIFO_sen2[%d]=%g",i,FIFO_sen2[i]);
						//printf("\nsignal_pick_max[2]=%g",signal_pick_max[2]);	
						
						/*int yyy=i;	
						printf("\nFIFO_sen2[%d]=%g",yyy,FIFO_sen2[yyy]);	yyy--;
						printf("\nFIFO_sen2[%d]=%g",yyy,FIFO_sen2[yyy]);	yyy--;
						printf("\nFIFO_sen2[%d]=%g",yyy,FIFO_sen2[yyy]);	yyy--;
						printf("\nFIFO_sen2[%d]=%g",yyy,FIFO_sen2[yyy]);	yyy--;
						printf("\nFIFO_sen2[%d]=%g",yyy,FIFO_sen2[yyy]);	yyy--;
						printf("\nFIFO_sen2[%d]=%g",yyy,FIFO_sen2[yyy]);	yyy--;
						printf("\nFIFO_sen2[%d]=%g",yyy,FIFO_sen2[yyy]);	yyy--;*/
					}	
				}
				if((FIFO_sen3[i]>signal_pick_max[3])||(FIFO_sen3[i]<signal_pick_min[3]))
				{
					if(start_time[3]==-1)
					{
						start_time[3]=i;
						if(index_pick_detection==-1){index_pick_detection=i;}
						pick_detection=true;
						//printf("\ni_3=%d",i);
						/*printf("\nFIFO_sen3[%d]=%g",i,FIFO_sen3[i]);
						printf("\nsignal_pick_max[3]=%g",signal_pick_max[3]);
						
						int yyy=i;		
						printf("\nFIFO_sen3[%d]=%g",yyy,FIFO_sen3[yyy]);	yyy--;
						printf("\nFIFO_sen3[%d]=%g",yyy,FIFO_sen3[yyy]);	yyy--;
						printf("\nFIFO_sen3[%d]=%g",yyy,FIFO_sen3[yyy]);	yyy--;
						printf("\nFIFO_sen3[%d]=%g",yyy,FIFO_sen3[yyy]);	yyy--;
						printf("\nFIFO_sen3[%d]=%g",yyy,FIFO_sen3[yyy]);	yyy--;
						printf("\nFIFO_sen3[%d]=%g",yyy,FIFO_sen3[yyy]);	yyy--;*/
						
					}
				}
				if((FIFO_sen4[i]>signal_pick_max[4])||(FIFO_sen4[i]<signal_pick_min[4]))
				{
					if(start_time[4]==-1)
					{
						start_time[4]=i;
						if(index_pick_detection==-1){index_pick_detection=i;}
						pick_detection=true;
						//printf("\ni_4=%d",i);
						//printf("\nFIFO_sen4[%d]=%g",i,FIFO_sen4[i]);
						//printf("\nsignal_pick_max[4]=%g",signal_pick_max[4]);
					}	
				}
				if((FIFO_sen5[i]>signal_pick_max[5])||(FIFO_sen5[i]<signal_pick_min[5]))
				{
					if(start_time[5]==-1)
					{
						start_time[5]=i;
						if(index_pick_detection==-1){index_pick_detection=i;}
						pick_detection=true;
						//printf("\ni_5=%d",i);
						//printf("\nFIFO_sen5[%d]=%g",i,FIFO_sen5[i]);
						//printf("\nsignal_pick_max[5]=%g",signal_pick_max[5]);	
					}
				}
				if((FIFO_sen6[i]>signal_pick_max[6])||(FIFO_sen6[i]<signal_pick_min[6]))
				{
					if(start_time[6]==-1)
					{
						start_time[6]=i;
						if(index_pick_detection==-1){index_pick_detection=i;}
						pick_detection=true;
						//printf("\ni_6=%d",i);
						//printf("\nFIFO_sen6[%d]=%g",i,FIFO_sen6[i]);
						//printf("\nsignal_pick_max[6]=%g",signal_pick_max[6]);
					}	
				}
				if((FIFO_sen7[i]>signal_pick_max[7])||(FIFO_sen7[i]<signal_pick_min[7]))
				{
					if(start_time[7]==-1)
					{
						start_time[7]=i;
						if(index_pick_detection==-1){index_pick_detection=i;}
						pick_detection=true;
						//printf("\ni_7=%d",i);
						//printf("\nFIFO_sen7[%d]=%g",i,FIFO_sen7[i]);
						//printf("\nsignal_pick_max[7]=%g",signal_pick_max[7]);
					}	
				}
				if((FIFO_sen8[i]>signal_pick_max[8])||(FIFO_sen8[i]<signal_pick_min[8]))
				{
					if(start_time[8]==-1)
					{
						start_time[8]=i;
						if(index_pick_detection==-1){index_pick_detection=i;}
						pick_detection=true;
						//printf("\ni_8=%d",i);
						//printf("\nFIFO_sen8[%d]=%g",i,FIFO_sen8[i]);
						//printf("\nsignal_pick_max[8]=%g",signal_pick_max[8]);	
					}
				}
			}
			if(pick_detection==true)
			{
				//printf("\n\n\n******************");
				/*	printf("\nFIFO_sen1[%d]=%g",start_time[1],FIFO_sen1[start_time[1]]);
					printf("\nFIFO_sen2[%d]=%g",start_time[2],FIFO_sen2[start_time[2]]);
					printf("\nFIFO_sen3[%d]=%g",start_time[3],FIFO_sen3[start_time[3]]);
					printf("\nFIFO_sen4[%d]=%g",start_time[4],FIFO_sen4[start_time[4]]);
					printf("\nFIFO_sen5[%d]=%g",start_time[5],FIFO_sen5[start_time[5]]);
					printf("\nFIFO_sen6[%d]=%g",start_time[6],FIFO_sen6[start_time[6]]);
				    printf("\nFIFO_sen7[%d]=%g",start_time[7],FIFO_sen7[start_time[7]]);
					printf("\nFIFO_sen8[%d]=%g",start_time[8],FIFO_sen8[start_time[8]]);
					printf("\ni_1=%d",index_pick_detection);	*/
					clump_and_send(index_pick_detection);	
							
			}
    ////////////////////////////////////////////////////////////////////////////////////////	
}

void make_data_sensor()
{
	//printf("\n---------------------make_data_sensor---------------------");
int i=0,j=0,k=0;
	if(bufer_number_1==true)
			{
				
				for(i=0;i<total_matrix;i++)
				{
					for(j=0;j<per_matrix;j=j+2)
					{
						sen1[k]=buffer1[i][j];
						sen1[k+1]=buffer1[i][j+1];
						buffer1[i][j]=0x8;
						j=j+2;
						
						sen2[k]=buffer1[i][j];
						sen2[k+1]=buffer1[i][j+1];
						buffer1[i][j]=0x10;
						j=j+2;
						
						sen3[k]=buffer1[i][j];
						sen3[k+1]=buffer1[i][j+1];
						buffer1[i][j]=0x18;
						j=j+2;
						
						sen4[k]=buffer1[i][j];
						sen4[k+1]=buffer1[i][j+1];
						buffer1[i][j]=0x20;
						j=j+2;
						
						sen5[k]=buffer1[i][j];
						sen5[k+1]=buffer1[i][j+1];
						buffer1[i][j]=0x28;
						j=j+2;
						
						sen6[k]=buffer1[i][j];
						sen6[k+1]=buffer1[i][j+1];
						buffer1[i][j]=0x30;
						j=j+2;
						
						sen7[k]=buffer1[i][j];
						sen7[k+1]=buffer1[i][j+1];
						buffer1[i][j]=0x38;
						j=j+2;
						
						sen8[k]=buffer1[i][j];
						sen8[k+1]=buffer1[i][j+1];
						buffer1[i][j]=0x0;
						k=k+2;
						//printf("\nsen2_buf1[%d]=%d",k,sen2[k]);
						//printf("\nsen2_buf1[%d]=%d",k+1,sen2[k+1]);
					}
				}
			//	printf("\nk=%d",k);
		
			}
			else
			{
				k=0;
				for(i=0;i<total_matrix;i++)
				{
					for(j=0;j<per_matrix;j=j+2)
					{
						sen1[k]=buffer2[i][j];
						sen1[k+1]=buffer2[i][j+1];
						buffer2[i][j]=0x8;
						j=j+2;
						
						sen2[k]=buffer2[i][j];
						sen2[k+1]=buffer2[i][j+1];
						buffer2[i][j]=0x10;
						j=j+2;
						
						sen3[k]=buffer2[i][j];
						sen3[k+1]=buffer2[i][j+1];
						buffer2[i][j]=0x18;
						j=j+2;
						
						
						sen4[k]=buffer2[i][j];
						sen4[k+1]=buffer2[i][j+1];
						buffer2[i][j]=0x20;
						j=j+2;
						
						sen5[k]=buffer2[i][j];
						sen5[k+1]=buffer2[i][j+1];
						buffer2[i][j]=0x28;
						j=j+2;
						
						sen6[k]=buffer2[i][j];
						sen6[k+1]=buffer2[i][j+1];
						buffer2[i][j]=0x30;
						j=j+2;
						
						sen7[k]=buffer2[i][j];
						sen7[k+1]=buffer2[i][j+1];
						buffer2[i][j]=0x38;
						j=j+2;
						
						sen8[k]=buffer2[i][j];
						sen8[k+1]=buffer2[i][j+1];
						buffer2[i][j]=0x0;
						k=k+2;
						
						//printf("\nsen2_buf2[%d]=%d",k,sen2[k]);
						//printf("\nsen2_buf2[%d]=%d",k+1,sen2[k+1]);
					}
				}
				
			}
	//getchar();
	
/*	
sen1[0]=0;sen1[1]=0;        sen1[2]=0x0;sen1[3]=0x0;
sen1[4]=0;sen1[5]=0;        sen1[6]=0x0;sen1[7]=0x0;
sen1[8]=0;sen1[9]=0;        sen1[10]=0x0;sen1[11]=0x0;
sen1[total_sample-4]=0x04;sen1[total_sample-3]=0xd9;
sen1[total_sample-2]=0x04;sen1[total_sample-1]=0xd9;

sen2[0]=0;sen2[1]=0;        sen2[2]=0x0;sen2[3]=0x0;
sen2[4]=0;sen2[5]=0;        sen2[6]=0x0;sen2[7]=0x0;
sen2[8]=0;sen2[9]=0;        sen2[10]=0x0;sen2[11]=0x0;
sen2[total_sample-4]=0x04;sen2[total_sample-3]=0xd9;
sen2[total_sample-2]=0x04;sen2[total_sample-1]=0xd9;


sen3[0]=0;sen3[1]=0;        sen3[2]=0x0;sen3[3]=0x0;
sen3[4]=0;sen3[5]=0;        sen3[6]=0x0;sen3[7]=0x0;
sen3[8]=0;sen3[9]=0;        sen3[10]=0x0;sen3[11]=0x0;
sen3[total_sample-4]=0x04;sen3[total_sample-3]=0xd9;
sen3[total_sample-2]=0x04;sen3[total_sample-1]=0xd9;

sen4[0]=0;sen4[1]=0;        sen4[2]=0x0;sen4[3]=0x0;
sen4[4]=0;sen4[5]=0;        sen4[6]=0x0;sen4[7]=0x0;
sen4[8]=0;sen4[9]=0;        sen4[10]=0x0;sen4[11]=0x0;
sen4[total_sample-4]=0x04;sen4[total_sample-3]=0xd9;
sen4[total_sample-2]=0x04;sen4[total_sample-1]=0xd9;

sen5[0]=0;sen5[1]=0;        sen5[2]=0x0;sen5[3]=0x0;
sen5[4]=0;sen5[5]=0;        sen5[6]=0x0;sen5[7]=0x0;
sen5[8]=0;sen5[9]=0;        sen5[10]=0x0;sen5[11]=0x0;
sen5[total_sample-4]=0x04;sen5[total_sample-3]=0xd9;
sen5[total_sample-2]=0x04;sen5[total_sample-1]=0xd9;

sen6[0]=0;sen6[1]=0;        sen6[2]=0x0;sen6[3]=0x0;
sen6[4]=0;sen6[5]=0;        sen6[6]=0x0;sen6[7]=0x0;
sen6[8]=0;sen6[9]=0;        sen6[10]=0x0;sen6[11]=0x0;
sen6[total_sample-4]=0x04;sen6[total_sample-3]=0xd9;
sen6[total_sample-2]=0x04;sen6[total_sample-1]=0xd9;

sen7[0]=0;sen7[1]=0;        sen7[2]=0x0;sen7[3]=0x0;
sen7[4]=0;sen7[5]=0;        sen7[6]=0x0;sen7[7]=0x0;
sen7[8]=0;sen7[9]=0;        sen7[10]=0x0;sen7[11]=0x0;
sen7[total_sample-4]=0x04;sen7[total_sample-3]=0xd9;
sen7[total_sample-2]=0x04;sen7[total_sample-1]=0xd9;

sen8[0]=0;sen8[1]=0;        sen8[2]=0x0;sen8[3]=0x0;
sen8[4]=0;sen8[5]=0;        sen8[6]=0x0;sen8[7]=0x0;
sen8[8]=0;sen8[9]=0;        sen8[10]=0x0;sen8[11]=0x0;
sen8[total_sample-4]=0x04;sen8[total_sample-3]=0xd9;
sen8[total_sample-2]=0x04;sen8[total_sample-1]=0xd9;
*/
}



void Packet_send(int packet_type,int lastCmd, int id_PS, int cart_add_pt)
{
	
	if(packet_type==ACK)
	{
		Send_buf[0]=0x3;		
		Send_buf[1]=lastCmd;
		Send_buf[2]=id_PS;
		Send_buf[3]=cart_add_pt;
		

		Sena_Data_to_Ethe();
		
	} 
	else if(packet_type==Is_on_or_off)
	{
		Send_buf[0]=0x2;
		Send_buf[1]=id_PS;
		Send_buf[2]=cart_add_pt;

		Sena_Data_to_Ethe();
		
		
	}    	
}


void operate_relation_list(int curent_sts,int number_change)
{
   printf("\n ----------");
   printf("\n curent_sts=%d",curent_sts);
   printf("\n number_change=%d",number_change);
  //printf("\n Relation_list_[14][0]=%d",Relation_list_[0][0]); 
  int i=0;
  if(curent_sts==1)
  {
    for(i=0;i<256;i++)
    {
      if(Relation_list_[number_change][i]==1)
      {
	operator_list[i]=1;
	  printf("\n operator_list[%d]=%d",i,operator_list[i]);
      } 
      else if(Relation_list_[number_change][i]==0)
      {
	 operator_list[i]=0;
	 printf("\n Relation_list_[%d][%d]=%d",number_change,i,Relation_list_[number_change][i]);
	 // printf("\n 6666^^^^^^^^^\n operator_list[%d]=%d",i,operator_list[i]);
      }  
      
      
    }
  }
  if(curent_sts==0)
  {
    for(i=0;i<256;i++)
    {
      if(Relation_list_0__[number_change][i]==1)
      {
	operator_list[i]=1;
	 // printf("\n operator_list[%d]=%d",i,operator_list[i]);
      } 
      else if(Relation_list_0__[number_change][i]==0)
      {
	operator_list[i]=0;
	 // printf("\n noperator_list[%d]=%d",i,operator_list[i]);
      }   
      
      
    }
  }
  
}

void  write_relation_list_0_()
{
   
    
    int i,j=0;

  ofstream myfile ("/home/pi/Desktop/aps/RL0");
  if (myfile.is_open())
  {
    for(i=0;i<256;i++)
    {
      for(j=0;j<256;j++)
      {
        myfile << Relation_list_0__[i][j]<< " " ;
      }
       myfile <<"\n" ;
    }
    myfile.close();
  }
  else cout << "Unable to open file";
   
   
  
}


void  write_relation_list_1_()
{
  printf("\n w_f_RL_1");
    
    int i,j=0;

  ofstream myfile ("/home/pi/Desktop/aps/RL");
  if (myfile.is_open())
  {
    for(i=0;i<256;i++)
    {
      for(j=0;j<256;j++)
      {
        myfile << Relation_list_[i][j]<< " " ;
      }
       myfile <<"\n" ;
    }
    myfile.close();
  }
  else cout << "Unable to open file";
  
}

void usual_relay_Write_and_save(int number,int on_off)
{
  int first_last=0;
  int relay_number=number%16;
  int kart_number=number/16;
 // printf("\n\n kart_number=%d",kart_number+1);
   //printf("\n relay_number=%d",relay_number);
   if(relay_number>7)
   {
     first_last=1;
     relay_number=relay_number-8;
   
   }else
   {
     first_last=0;
     relay_number=relay_number;
   }
   usual_Relay[kart_number][first_last][relay_number]=on_off;
   Write_Old_Status_Usual();
}

void set_usual_relay(int number,int on_off)
{
  int first_last=0;
  int relay_number=number%16;
  int kart_number=number/16;
 // printf("\n\n kart_number=%d",kart_number+1);
   //printf("\n relay_number=%d",relay_number);
   if(relay_number>7)
   {
     first_last=1;
     relay_number=relay_number-8;
   
   }else
   {
     first_last=0;
     relay_number=relay_number;
   }
   usual_Relay[kart_number][first_last][relay_number]=on_off;
}

void check_data()
{
	//int Relay_control_pin=-1,Add_Cart_pin=-1;
	int Chanel_ID=-1,On_Or_Off=-1,Contactor_mode=-1;
	printf(" %d %d %d %d",Recive_buf[0],Recive_buf[1],Recive_buf[2],Recive_buf[3]);
	if(Recive_buf[0]==1)//click Req   |  ID  |-------| 0=off 1=on |
	{
				Chanel_ID=Recive_buf[1];
				On_Or_Off=Recive_buf[2];
				Contactor_mode=Recive_buf[3];
				
		if(Contactor_mode==0)
		{
		 
			    //usual_relay_Write_and_save(Chanel_ID-1,On_Or_Off);
			    //printf("\n chanel=%d",(Chanel_ID-1)/2);
			    chanely_Relay[(Chanel_ID-1)/2]=-1;
			    Write_Old_Status_Chanel();
			    
			    on_off(Chanel_ID-1,On_Or_Off);
			    delay(20);
			    
			    int status_now=Read_pin(Chanel_ID-1);
			    Packet_send(Is_on_or_off,-1,(Chanel_ID),status_now);
			    Packet_send(ACK,1,(Chanel_ID),0);			    
			    if(status_now!=Status_Table[Chanel_ID-1])
			    {
				Status_Table[Chanel_ID-1]=status_now; 
			    }
					
		}
		else if(Contactor_mode==1)
		{
			   
			     if((On_Or_Off==1)||(On_Or_Off==0))
			    {
			        chanely_Relay[Chanel_ID-1]=On_Or_Off;
			        Write_Old_Status_Chanel();
				//int i=0;
			      //  printf("\n"); for(i=0;i<=127;i++){printf("%d:%d ",i,chanely_Relay[i]);}printf("\n");
				
				 
				    if(On_Or_Off==1)
				    {
					  on_off(((Chanel_ID-1)*2)+1,0);
					  on_off(((Chanel_ID-1)*2)  ,1);
					  //printf("\n ((Chanel_ID-1)*2)=%d",((Chanel_ID-1)*2)); 
					  delay(500);
					  on_off(((Chanel_ID-1)*2),0);
				    }
				    
				    if(On_Or_Off==0)
				    {
				       on_off(((Chanel_ID-1)*2)  ,0);
				       on_off(((Chanel_ID-1)*2)+1,1);
				        /* delay(10);
					 on_off(((Chanel_ID-1)*2)+1,1);
				         delay(10);
					 on_off(((Chanel_ID-1)*2)+1,1);
				         delay(10);*/
					 
					delay(500);
					
				       on_off(((Chanel_ID-1)*2)+1,0);
				     /*  delay(10);
				        on_off(((Chanel_ID-1)*2)+1,0);
				       delay(10);
				        on_off(((Chanel_ID-1)*2)+1,0);
				       delay(10);*/
				      
				    }
				      int status_now=Read_pin((Chanel_ID-1)*2);
			    Packet_send(Is_on_or_off,-1,(Chanel_ID),status_now);
			    Packet_send(ACK,1,(Chanel_ID),0);
			    
				  if(status_now!=Status_Table[(Chanel_ID-1)*2])
				   {
				     Status_Table[(Chanel_ID-1)*2]=status_now; 
				   }
				    
			    }
			    //Packet_send(ACK,1,Chanel_ID,Add_Cart_pin);
						/*Add_Cart_pin=(((((Chanel_ID)*2)-1)-1)/16);     
						Relay_control_pin=(((((Chanel_ID)*2)-1)-1)-(Add_Cart_pin*16));		
					
						
						if((On_Or_Off==1)||(On_Or_Off==0))
						{
							if(On_Or_Off==1)
							{
							  Normal_Table[Add_Cart_pin][Relay_control_pin]=1;
							  Normal_Table[Add_Cart_pin][Relay_control_pin+1]=0;
							}
							else 
							{
							  Normal_Table[Add_Cart_pin][Relay_control_pin]=0;
							  Normal_Table[Add_Cart_pin][Relay_control_pin+1]=1;
							}
						
					
						
						if((Chaneg_Chanel_state_chanelly(Add_Cart_pin,Relay_control_pin))!=-1)
						   {	
							   sleep(2);
							   Normal_Table[Add_Cart_pin][Relay_control_pin]=0;
							   Normal_Table[Add_Cart_pin][Relay_control_pin+1]=0;
							   if((Chaneg_Chanel_state_chanelly(Add_Cart_pin,Relay_control_pin))!=-1)
							   {
								Packet_send(ACK,1,Chanel_ID,Add_Cart_pin);
								//Packet_send(Is_on_or_off,-1,Chanel_ID,Add_Cart_pin);
							   }
							   gpio_off(Enable_GPIO[Add_Cart_pin]);
						   }
						} */
			 }
}
	/*else if(Recive_buf[0]==100)//Ip_change   This ip:4  this_mask:4   GW:4
	{
		int This_Ip[4]={100,100,100,101};
		int This_Mask_IP[4]={255,255,255,0};
		int This_DefaltGW_Ip[4]={100,100,100,100};  
		
			This_Ip[0]=Recive_buf[1];
			This_Ip[1]=Recive_buf[2];
			This_Ip[2]=Recive_buf[3];
			This_Ip[3]=Recive_buf[4];
			This_Mask_IP[0]=Recive_buf[5];
			This_Mask_IP[1]=Recive_buf[6];
			This_Mask_IP[2]=Recive_buf[7];
			This_Mask_IP[3]=Recive_buf[8];
			This_DefaltGW_Ip[0]=Recive_buf[9];
			This_DefaltGW_Ip[1]=Recive_buf[10];
			This_DefaltGW_Ip[2]=Recive_buf[11];
			This_DefaltGW_Ip[3]=Recive_buf[12];
		   
			int i_ns=0;
			int ip_ns [4];
			char Command[50]="";
			for(i_ns=0;i_ns<=2;i_ns++)
			{	
				
				if(i_ns==0)
					{
						ip_ns[0]=This_Ip[0]; ip_ns[1]=This_Ip[1]; ip_ns[2]=This_Ip[2]; ip_ns[3]=This_Ip[3];
					}
				if(i_ns==1)
					{
						ip_ns[0]=This_Mask_IP[0]; ip_ns[1]=This_Mask_IP[1]; ip_ns[2]=This_Mask_IP[2]; ip_ns[3]=This_Mask_IP[3];
					}
				if(i_ns==2)
					{
						ip_ns[0]=This_DefaltGW_Ip[0]; ip_ns[1]=This_DefaltGW_Ip[1]; ip_ns[2]=This_DefaltGW_Ip[2]; ip_ns[3]=This_DefaltGW_Ip[3];
					}
				
				char str1[4]="";
				char str2[4]="";
				char str3[4]="";
				char str4[4]="";
				char strF[60]="";
					
				sprintf(str1,"%d",ip_ns[0]);
				sprintf(str2,"%d",ip_ns[1]);
				sprintf(str3,"%d",ip_ns[2]);
				sprintf(str4,"%d",ip_ns[3]);
				
				strcat(strF,str1);
				strcat(strF,".");
				strcat(strF,str2);
				strcat(strF,".");
				strcat(strF,str3);
				strcat(strF,".");
				strcat(strF,str4);
				strcat(strF,"_");
				
					strcat(Command,strF);   //ip.ip.ip.ip_ip.ip.ip.ip_ip.ip.ip.ip_	   
			}
		   File_WR_two(Command,1,ip_setting);
		   network_setting(ip_setting);
		   Packet_send(ACK,100,0,0);

	}
	
	else if(Recive_buf[0]==101)//Port_change   Receive_port:2  send_port:2   
	{
			int recive_Port=0,Send_Port=0;
			char spstr[10]="";
			char rpstr[10]="";
			char command[10]="";
			
			recive_Port=(Recive_buf[1]*0x100)+ Recive_buf[2];
			Send_Port  =(Recive_buf[3]*0x100) + Recive_buf[4];
			
			sprintf(rpstr,"%d",recive_Port);
			sprintf(spstr,"%d",Send_Port);
			
			strcat(command,rpstr);
			strcat(command,"_");
			strcat(command,spstr);
			strcat(command,"_");
		

	        File_WR_two(command,1,port_setting);
            network_setting(port_setting);
            Packet_send(ACK,101,0,0);
	}	
*/
else if(Recive_buf[0]==4)//request status  
{
	  Chanel_ID=Recive_buf[1];
	  Contactor_mode=Recive_buf[3];
	  
	   if(Contactor_mode==0)
			 {
			   int status_now=0;
			   
			    status_now=Read_pin(Chanel_ID-1);
			  
			    Packet_send(Is_on_or_off,-1,(Chanel_ID),status_now);
			    //printf("\n status_now=%d",status_now);
			    //printf("\n Status_Table[%d]=%d",Chanel_ID-1,Status_Table[Chanel_ID-1]);
				   if(status_now!=Status_Table[Chanel_ID-1])
				   {
				     Status_Table[Chanel_ID-1]=status_now; 
				     operate_relation_list(status_now,Chanel_ID-1);
				   }
				   
				   
			   
			    
				   
	   
				   
					
			 }
			 else if(Contactor_mode==1)
			 {
			   int status_now=0;
			   
			     
			     status_now=Read_pin((Chanel_ID-1)*2);
			     printf("\n (Chanel_ID-1)*2=%d    Chanel_ID=%d", (Chanel_ID-1)*2,Chanel_ID);
			     Packet_send(Is_on_or_off,-1,((Chanel_ID-1)*2)+1,status_now);
			    
				  if(status_now!=Status_Table[(Chanel_ID-1)*2])
				   {
				     Status_Table[(Chanel_ID-1)*2]=status_now; 
				     operate_relation_list(status_now,(Chanel_ID-1)*2);
				   }
			   
			   
				
				   
			    
			 }


      
      /*
      
      gpio_on(15);

			
		printf("/n1");
		int i_cps=0;int Add_Cart_status=0;int pin_nr_status=0;
		int old_Add_Cart_status=8;
		int curent_status=-1,old_status=-1;
		if((Recive_buf[1]<=64) || (Recive_buf[1]>=0))
		{			
			i_cps=Recive_buf[1];	
			i_cps=i_cps-1;
			Add_Cart_status=((i_cps)/8); 
			pin_nr_status=((i_cps)-(Add_Cart_status*8));
			Add_Cart_status=Add_Cart_status+8;
			gpio_on(Add_GPIO[Add_Cart_status]);
										
			curent_status=get_valu_gpio(Add_GPIO[pin_nr_status]);
			old_status=curent_status;
			
				delay_ms(1);
				curent_status=get_valu_gpio(Add_GPIO[pin_nr_status]);
				if((old_status==curent_status))
				{
						delay_ms(1);
						curent_status=get_valu_gpio(Add_GPIO[pin_nr_status]);
						if((curent_status==old_status))
						{
							Status_Table[pin_nr_status][Add_Cart_status-8]=curent_status;
							Packet_send(Is_on_or_off,-1,i_cps+1,curent_status);
							//getchar();
						//printf("\n%d     i_cps",i_cps);	
						//printf("     %dAdd_Cart_status",Add_Cart_status);	
						//printf("     %dpin_nr_status",pin_nr_status);
						//printf("     %dcurent_status",curent_status);
						}
				}
			
		gpio_off(Add_GPIO[Add_Cart_status]);
		}
		gpio_off(15);
       	*/
	    
		
}

else if(Recive_buf[0]==5)//auto operator 
{
	    int input_nr=(Recive_buf[1]-1);  
	    int Raise_event=Recive_buf[2];    
	    int outPut_nr=(Recive_buf[3]-1); 
	    int operate=Recive_buf[4];
	    printf("\n outPut_nr=%d",outPut_nr);
	    if(Raise_event==0)
	    {
	      Relation_list_0__[input_nr][outPut_nr]=operate;
	      write_relation_list_0_();
	    }
	    else if(Raise_event==1)
	    {
	      Relation_list_[input_nr][outPut_nr]=operate;
	      write_relation_list_1_();
	    }	    
}
else if(Recive_buf[0]==6)//reset Relation_list
{
  int i=0,j=0;
  for(i=0;i<256;i++)
  {
    for(j=0;j<256;j++)
    {
      Relation_list_[i][j]=255;
      Relation_list_0__[i][j]=255;
    }
  }
   write_relation_list_0_();
   write_relation_list_1_();
  Packet_send(ACK,1,(6),0);
  
}


}

void *auto_operateor(void *x)
{
  int i=0;
  while(1)
	{
	   
	  for(i=0;i<256;i++)
	  {	
	    //printf("\n operator_list[%d]=%d",i,operator_list[i]);
	    if(operator_list[i]==1)
	    {
	         chanely_Relay[i-1]=1;
		 Write_Old_Status_Chanel();
		
		on_off(((i-1)*2)+1,0);
		on_off(((i-1)*2)  ,1);
		delay(500);
		on_off(((i-1)*2),0);
				

	     operator_list[i]=-1;	
	    }
	     else if(operator_list[i]==0)
	    {
	      printf("\n *********operator_list[%d]=%d",i,operator_list[i]);
	        chanely_Relay[i-1]=0;
		 Write_Old_Status_Chanel();
	      
	        on_off(((i-1)*2)  ,0);
		on_off(((i-1)*2)+1,1);
                delay(500);
		on_off(((i-1)*2)+1,0);
		
		
	      
	      operator_list[i]=-1;
	      	
	    }
	   
	 }
	 
	}
  
   return NULL;
}


void *myFun(void *x)
{
	while(1)
	{	
	
    	           Get_data_From_Ethe();
		   check_data();
		   End_change_pin=0;

	}
	
  //int tid;
  //tid = *((int *) x);
  //printf("Hi from thread %d!\n", tid);
  return NULL;
}



void ether_init()
{
		///////////ethernet init /////////
	si_other.sin_family = AF_INET;
    si_other.sin_port = htons(SPORT);
   // si_other.sin_addr.s_addr = inet_addr("192.168.43.196");
     si_other.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    memset(&(si_other.sin_zero),'\0',8);
 
	 
	//network_setting(ip_setting);
	network_setting(port_setting);
	 
	 Ethe_init(1);
   Ethe_init(0);	
   
   
   
   
   //////////////gpio init//////////////////////
   if (wiringPiSetup () == -1)
   printf("wirinig pi error");
    pinMode (0, OUTPUT) ;    
    digitalWrite (0, 1) ;       // On
    digitalWrite (0, 0) ;       // On 
   /////////////////////////////////////////////
}
void  read_relation_list_0_()
{
  fstream fp("/home/pi/Desktop/aps/RL0",ios::in);   
  if(!fp)
	{
		printf("con not read file");
		exit(1);	
	}
	
	string input_text;
	int data[65537];
	int i=0;
	
	while(!fp.eof())
	{
		fp>>input_text;
		data[i]=atoi(input_text.c_str());
		i++;
	}
	
	
	
	
	int j=0;
	for (j=0;j<256;j++)
	  {
	      for (i=0;i<256;i++)
		{
		   Relation_list_0__[j][i]=data[(j*256)+i];  
		}
	  }
}



void  read_relation_list()
{
  fstream fp("/home/pi/Desktop/aps/RL",ios::in);   
  if(!fp)
	{
		printf("con not read file");
		exit(1);	
	}
	
	string input_text;
	unsigned int data[65537];
	int i=0;
	
	while(!fp.eof())
	{
	 
		fp>>input_text;
		//printf("\n %d=%d ",i, std::stoi(input_text));
		data[i]=atoi(input_text.c_str());
	        i++;
	}
	
	
	

	int j=0;
	 for (j=0;j<256;j++)
	  {
	      for (i=0;i<256;i++)
		{
		   Relation_list_[j][i]=data[(j*256)+i];  
		  // printf("\n data[(j*255)+i]=%d",data[(j*256)+i]);
		   // printf("\n i=%d,j=%d,(j*255)+i=%d",i,j,(j*256)+i);
		}
	  }
  

 /* for(int j=0;j<128;j++)
  {
    printf("\n");
    for(int i=0;i<128;i++)
      {
	printf("%d",Relation_list_[j][i]);
    }
  }*/
				    
}


void Read_old_status_Main()
{
  
int i=0,j=0,k=0; 

 for(i=0;i<=16;i++)
{
  for(j=0;j<=1;j++)
  {
    for(k=0;k<=7;k++)
    {
      usual_Relay[i][j][k]=0;
    }
  }
}

  Read_Old_Status_Usual();
  
 /*for(i=0;i<=15;i++)
  {
     printf("\n Kart=%d  ",i);
      for(j=0;j<=1;j++)
      {
	
	  for(k=0;k<=7;k++)
	  {
	    printf("%d ",usual_Relay[i][j][k]);
	  }
      }
  }*/

  
  
  for(i=0;i<=127;i++){chanely_Relay[i]=-1;}
 // Write_Old_Status_Chanel();
  Read_Old_Status_Chanel();
 // printf("\n"); for(i=0;i<=127;i++){printf("%d:%d ",i,chanely_Relay[i]);}printf("\n");

 
  
}


int main()
{
   int i=0; 


  read_relation_list();
  
  read_relation_list_0_();

  Read_old_status_Main();

 ether_init();
 
 for(i=0;i<=255;i++)
 {
   Status_Table[i]=0;
 }

 for(i=0;i<256;i++)
	  {	
	    operator_list[i]=-1;
	  }
	 
 //Get_data_From_Ethe();
 //Sena_Data_to_Ethe();
 

 
  pthread_t threads[5];
		  int thread_args[5];
		  thread_args[1] = 444;
		  pthread_create(&threads[1], NULL, myFun, (void *) &thread_args[1]);
		  
		   pthread_t threads2[5];
		  int thread_args2[5];
		  thread_args2[1] = 444;
		  
		  pthread_create(&threads2[1], NULL, auto_operateor, (void *) &thread_args2[1]);
			
   ////////////  INIT ///////////////////////////////////////
  ///////////////////////////////////////////////////////////
   if (wiringPiSetup () == -1)
   printf("wirinig pi error");
   
   
  
    for(i=0;i<16;i++)
    {
       pinMode (Relay_nr[i], OUTPUT) ; 
       digitalWrite (Relay_nr[i], 0) ; 
    }
    
     i=0;
    for(i=0;i<4;i++)
    {
       pinMode (control_nr[i], OUTPUT) ; 
       digitalWrite (control_nr[i], 0) ; 
    }
    
     pinMode (control_A_W_first, OUTPUT) ;  
     pinMode (control_B_R_first, OUTPUT) ; 
     pinMode (control_C_R_last, OUTPUT) ;  
     pinMode (control_d_W_last, OUTPUT) ; 
	
     digitalWrite (control_A_W_first, 0) ;
     digitalWrite (control_B_R_first, 0) ;  
     digitalWrite (control_C_R_last,0) ;
     digitalWrite (control_d_W_last, 0) ;
     
   /////////////////////////////////////////////////////////////////
   ////////////       END INIT ///////////////////////////////////////
  
  
  
  ///////////////////////////////restor  old status ////////////////////
  ////////////////////////////usualy old status////////////////////////

   
   
  ////////////////////////////chanellyy old status////////////////////

 printf("\nStart OLd status chanell");
  Old_Status_On_Off_chanel_set_on();
 printf("\nEND OLd status chanell");
 
    printf("\nStart OLd status UUU");
  Old_Status_On_Off();
   printf("\nend OLd status UUU");
   
   
    printf("\nStart OLd status chanell");
  Old_Status_On_Off_chanel_set_off();
 printf("\nEND OLd status chanell");
 
    printf("\nStart OLd status UUU");
  Old_Status_On_Off();
   printf("\nend OLd status UUU");
//////////////////////////////////////////////////////////////////////
 
 
 
    


   while(1)
     {
 
	
	
	
	
int curent_status=0;
       for(i=0;i<=15;i++)
	{
	  
	    //printf("\n"); 
	  Read_pin_kart(i); 
	  int j=0;
	  int pin_number=0;
	    
	  for(j=0;j<=15;j++)
	  {
	    
	    if(return_s[j]==1)
	    {
	       printf("\n Kart_number=%d    ",i);
	        printf(" %d=%d",j,return_s[j]);
	      }
	       pin_number=j+(i*16);
	       //printf("\n pin_number=%d",pin_number);
	       curent_status=return_s[j];
	       if(curent_status!=Status_Table[pin_number])
	       {
		 Status_Table[pin_number]=curent_status;
		 Packet_send(Is_on_or_off,-1,pin_number+1,curent_status);
		 operate_relation_list(curent_status,pin_number);
		 printf("\n curent_status=%d i=%d \n",curent_status,pin_number);
		
	       }
	  }
  
	   
	  
	   delay(5);
	  
	  
	 }
        

  delay(5);
	  
	  
     }
    
    
      
}






