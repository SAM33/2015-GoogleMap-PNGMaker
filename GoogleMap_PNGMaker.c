/* 
 *    Author : Sam33
 *    Date   : 2015/08/11
 *    National Central University, Taiwan
 */


#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdbool.h>


//You can mark this to disable all output messages
//#define ENABLE_STDOUT
#define BUFFERSIZE 8192
#define PATHSIZE   100

#define NETWORK_ERROR -1
#define FILE_ERROR    -1
#define SUCCESS        0

int main(int argc , char **args)
{
    #ifdef ENABLE_STDOUT
    printf("Parameter=%d\n",argc);
    int i;
    for(i=0 ; i<argc ; i++){
        printf("%d:%s\n",(i+1),args[i]);
    }
    #endif
    double latitude,longitude;
    int sizex,sizey;
    int zoom;
    char maptype[PATHSIZE];
    char outputfile[PATHSIZE];
    memset(maptype,0x0,PATHSIZE);
    memset(outputfile,0x0,PATHSIZE);
    if(argc==8){
        latitude = atof(args[1]);
        longitude = atof(args[2]);
        zoom = atoi(args[3]);
        sizex = atoi(args[4]);
        sizey = atoi(args[5]);
        strcpy(maptype,args[6]);
        strcpy(outputfile,args[7]);
        #ifdef ENABLE_STDOUT
        printf("Info-->Locate(%lf,%lf),Zoom=%d,Size(%d,%d),Type=%s,Output=%s\n",latitude,longitude,zoom,sizex,sizey,maptype,outputfile);
        #endif
    }
    else if(argc==4){
        latitude = atof(args[1]);
        longitude = atof(args[2]);
        zoom=14;
        sizex=300;
        sizey=300;
        strcpy(maptype,"roadmap");
        strcpy(outputfile,args[3]);
        #ifdef ENABLE_STDOUT
        printf("Info-->Locate(%lf,%lf),Zoom=%d,Size(%d,%d),Type=%s,Output=%s\n",latitude,longitude,zoom,sizex,sizey,maptype,outputfile);
        #endif
    }else{
        printf("\n-----------------------------------------------\n");
        printf("Usage: GoogleMap_PNGMaker <latitude> <longitude> <output file name>\n");
        printf("For example:\n./GoogleMap_PNGMaker 24.9708264 121.1882077 test.png\n");
        printf("It will create a picture called test.png that show National Central University, Taiwan\n");
        printf("\n");
        printf("Usage: GoogleMap_PNGMaker <latitude> <longitude> <zoom> <ouput file width> <output file height> <maptype> <output file name>\n");
        printf("maptype: roadmap, satellite, hybrid, terrain\n");
        printf("For example:\n./GoogleMap_PNGMaker 24.9708264 121.1882077 15 500 500 satellite test.png\n");
        printf("It will create a 500x500 satellite map called test.png that show National Central University, Taiwan\n");
        printf("\n-----------------------------------------------\n");
        return 0;

    }
    
    char *hostname = "maps.google.com";
    char parameter[BUFFERSIZE];
    memset(parameter,0x0,BUFFERSIZE);
    snprintf(parameter,BUFFERSIZE,"/maps/api/staticmap?center=%lf,%lf&zoom=%d&size=%dx%d&maptype=%s&sensor=true",latitude,longitude,zoom,sizex,sizey,maptype);
    struct hostent *dsthost = gethostbyname(hostname);
    if(dsthost==NULL){
        #ifdef ENABLE_STDOUT
        printf("Error-->Host not found!\n");
        #endif
        return NETWORK_ERROR;
    }else{
        int i=0;
        while(dsthost->h_addr_list[i]!=NULL){
            char *str=inet_ntoa(*( struct in_addr*)(dsthost->h_addr_list[i]));
            #ifdef ENABLE_STDOUT
            printf("%d:%s\n",(i+1),str);
            #endif
            i++;
        }
        
        if(i>0){
            char *addr = inet_ntoa(*( struct in_addr*)(dsthost->h_addr_list[0]));
            int dstsocket = socket(AF_INET,SOCK_STREAM,0);
            if(dstsocket<0){
                #ifdef ENABLE_STDOUT
                printf("Error-->Can't open socket!\n");
                #endif
                return NETWORK_ERROR;
            }
            struct sockaddr_in dstaddr;
            memset((void*)&dstaddr,0x0,sizeof(dstaddr));
            dstaddr.sin_addr.s_addr = inet_addr(addr);
            dstaddr.sin_family = AF_INET;
            dstaddr.sin_port = htons(80);
            int c = connect(dstsocket,(struct sockaddr *)&dstaddr,sizeof(dstaddr));
            if(c<0){
                #ifdef ENABLE_STDOUT
                printf("Error-->Can't connect to %s\n",addr);
                #endif
                return NETWORK_ERROR;
            }
            char sendbuf[BUFFERSIZE];
            memset(sendbuf,0x0,BUFFERSIZE);
            snprintf(sendbuf,BUFFERSIZE,"GET %s HTTP/1.0\r\nUser-Agent: Wget/1.15 (linux-gnu)\r\nAccept: */*\r\nHost: maps.google.com\r\nConnection: Drop\r\n\r\n",parameter);
            #ifdef ENABLE_STDOUT
            printf("Info-->Send %s\n",sendbuf);
            #endif
            int s = write(dstsocket,sendbuf,strlen(sendbuf));
            #ifdef ENABLE_STDOUT
            printf("Info-->%d bytes sent\n",s);
            #endif
            char rcvbuf[BUFFERSIZE];
            int r;
            FILE *f = fopen(outputfile,"wb");
            if(f==NULL){
                return FILE_ERROR;
            }
            
            bool foundheader = false;
            do{
                memset(rcvbuf,0x0,BUFFERSIZE);
                r = read(dstsocket,rcvbuf,BUFFERSIZE);
                #ifdef ENABLE_STDOUT
                printf("Info-->%d bytes received\n",r);
                #endif
                if(!foundheader){
                    char * bufptr  = strstr(rcvbuf,"\r\n\r\n");
                    int shift = bufptr-rcvbuf+strlen("\r\n\r\n");
                    if(bufptr!=NULL){
                        #ifdef ENABLE_STDOUT
                        printf("Info-->%d bytes writed\n",r-shift);
                        #endif
                        fwrite(rcvbuf+shift,1,r-shift,f);
                        fflush(f);
                        foundheader = true;
                    }
                    
                }else{
                    #ifdef ENABLE_STDOUT
                    printf("Info-->%d bytes writed\n",r);
                    #endif
                    fwrite(rcvbuf,1,r,f);
                    fflush(f);
                }
            }while(r>0);
            fclose(f);
            
        }
        
    }
    return SUCCESS;
    
    
	

}
