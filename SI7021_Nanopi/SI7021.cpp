#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <iostream>
#include <stdio.h>
#include <fstream>

using namespace std;

// Global variables
double temp_code;
double Y_out1,Y_out2;

// Create I2C bus
int i2cCreate();

// Read 2 bytes from the SI7021
// First byte will be from 'address'
// Second byte will be from 'address'+1
int readRH(int file);
int readTemp(int file);


int main()
{
	int file;
	file = i2cCreate();
	ofstream log;
	//log.open ("log.txt");
	
	while(1)
	{	
		log.open ("/home/blink/log2.txt", fstream::app);
		cout<< "Temp: "<<readTemp(file)<<" C	";
		log<< "Temp: "<< readTemp(file)<< "\n";
		
		cout<< "Hum: "<<readRH(file)<<" %"<<endl;
		log<< "Hum: "<< readRH(file)<< "\n";
		
		log.close();
		usleep(1000000);
		
	}
	
}



int i2cCreate()
{
	// Create I2C bus
	int file;
	char *bus = "/dev/i2c-0";
	if ((file = open(bus, O_RDWR)) < 0) 
	{
		printf("Failed to open the bus. \n");
		exit(1);
	}
	return file;
}


int readTemp(int file)
{
	float temperature;
	double temp_code;
	ioctl(file,I2C_SLAVE,0x40);
    
	char config[1]={0xE3};
	write(file,config,1);
	
	char data[2];
	read(file,data,2);
  	
    temp_code = (data[0]<<8) + data[1];
   	temperature = ((175.72 * temp_code)/65536)-46.85;
	 
	return (temperature);

}


int readRH(int file)
{
	float RH;
	double Y_out1, Y_out2;
	ioctl(file,I2C_SLAVE,0x40);
    
	char config[1]={0xE5};
	write(file,config,1);
	
	char data[2];
	read(file,data,2);
	
	Y_out1 = (125*(data[0]/100)*25600)>>16;
    Y_out2 = (125*((data[0]%100)*256+data[1]))>>16;
	
   	RH = Y_out1 + Y_out2 - 6;
	 
	return (RH);
}


//#######  #######
