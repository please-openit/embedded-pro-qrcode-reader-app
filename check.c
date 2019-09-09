#include <stdio.h>
#include <curl/curl.h>
#include <fcntl.h>   /* File Control Definitions           */
#include <termios.h> /* POSIX Terminal Control Definitions */
#include <unistd.h>  /* UNIX Standard Definitions 	   */
#include <errno.h>   /* ERROR Number Definitions           */
#include <unistd.h>
#include <string.h>

int openSerial(){
       	int fd;/*File Descriptor*/

       	fd = open("/dev/ttyUSB0",O_RDWR | O_NOCTTY | O_NDELAY);	/* ttyUSB0 is the FT232 based USB2SERIAL Converter   */

       	if(fd == -1)						/* Error Checking */
		printf("\n  Error! in Opening ttyUSB0  ");
        else
		printf("\n  ttyUSB0 Opened Successfully ");

	struct termios SerialPortSettings;	/* Create the structure                          */
	tcgetattr(fd, &SerialPortSettings);	/* Get the current attributes of the Serial port */

	cfsetispeed(&SerialPortSettings,B9600); /* Set Read  Speed as 9600                       */
	cfsetospeed(&SerialPortSettings,B9600); /* Set Write Speed as 9600                       */

	SerialPortSettings.c_cflag &= ~PARENB;   /* Disables the Parity Enable bit(PARENB),So No Parity   */
	SerialPortSettings.c_cflag &= ~CSTOPB;   /* CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit */
	SerialPortSettings.c_cflag &= ~CSIZE;	 /* Clears the mask for setting the data size             */
	SerialPortSettings.c_cflag |=  CS8;      /* Set the data bits = 8                                 */

	SerialPortSettings.c_cflag &= ~CRTSCTS;       /* No Hardware flow Control                         */
	SerialPortSettings.c_cflag |= CREAD | CLOCAL; /* Enable receiver,Ignore Modem Control lines       */


	SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);          /* Disable XON/XOFF flow control both i/p and o/p */
	SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);  /* Non Cannonical mode                            */

	SerialPortSettings.c_oflag &= ~OPOST;/*No Output Processing*/
	if((tcsetattr(fd,TCSANOW,&SerialPortSettings)) != 0) /* Set the attributes to the termios structure*/
		printf("\n  ERROR ! in Setting attributes");
	else
		printf("\n  BaudRate = 9600 \n  StopBits = 1 \n  Parity   = none");

	char open_write_buffer[] = {0xA0, 0x01, 0x01, 0xA2};	/* Buffer containing characters to write into port	     */
	int  bytes_written  = 0;  	/* Value for storing the number of bytes written to the port */

	bytes_written = write(fd,open_write_buffer,sizeof(open_write_buffer));/* use write() to send data to port                                            */
	
	sleep(1);
        
	char close_write_buffer[] = {0xA0, 0x01, 0x00, 0xA1};    /* Buffer containing characters to write into port           */
        bytes_written  = 0;        /* Value for storing the number of bytes written to the port */

        bytes_written = write(fd,close_write_buffer,sizeof(close_write_buffer));/* use write() to send data to port                                            */
	close(fd);/* Close the Serial port */
	return 0;
}

int sendError(char* token, char* error)
{
	CURL *curl;
        CURLcode res;
        char doorId[37] = "54947df8-0e9e-4471-a2f9-9af509fb5889";
        char headerAuthorization[53] = "Authorization: ";
        char headerControllerId[45] = "ControllerId: ";
	char errorField[255] = "error=";
	strcat(errorField, error);
        strcat(headerAuthorization, token);
        strcat(headerControllerId, doorId);
        curl = curl_easy_init();
        struct curl_slist *chunk = NULL;
        curl_easy_setopt(curl, CURLOPT_URL, "https://access.please-open.it/log/");
        chunk = curl_slist_append(chunk, headerAuthorization);
        chunk = curl_slist_append(chunk, headerControllerId);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, errorField);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        curl_slist_free_all(chunk);
	return 0;
}

int main(void)
{
        CURL *curl;
        CURLcode res;
	char line[37];
	char doorId[37] = "54947df8-0e9e-4471-a2f9-9af509fb5889";
	char headerAuthorization[53] = "Authorization: ";
	char headerControllerId[45] = "ControllerId: ";
	while(1){
		strcpy(line,  "");
		strcpy(headerAuthorization, "Authorization: ");
		strcpy(headerControllerId, "DoorId: ");
	        fgets(line, sizeof(line), stdin);

		strcat(headerAuthorization, line);
		strcat(headerControllerId, doorId);
		curl = curl_easy_init();
		if(curl){
		        struct curl_slist *chunk = NULL;
			curl_easy_setopt(curl, CURLOPT_URL, "https://access.please-open.it/access/");
			chunk = curl_slist_append(chunk, headerAuthorization);
        	        chunk = curl_slist_append(chunk, headerControllerId);
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
			res = curl_easy_perform(curl);
			
			if(res  != CURLE_OK)
				fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
			else
				openSerial();
 
			curl_easy_cleanup(curl);
			curl_slist_free_all(chunk);
	  	}
	}
	return 0;
}

