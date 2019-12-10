#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <linux/reboot.h>
#include <sys/reboot.h>

int get_ip()
{
    struct ifaddrs *ifaddr, *ifa;
    int family, s;
    char host[NI_MAXHOST];

    if (getifaddrs(&ifaddr) == -1) 
    {
        perror("getifaddrs");
        return -1;
        exit(EXIT_FAILURE);
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) 
    {
        family = ifa->ifa_addr->sa_family;

        if (family == AF_INET) 
        {
            s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
            if (s != 0) 
            {
                printf("getnameinfo() failed: %s\n", gai_strerror(s));
                exit(EXIT_FAILURE);
            }
            printf("<Interface>: %s \t <Address>: %s\n", ifa->ifa_name, host);
        }
    }
    return 0;
}

int set_static_ip(char* ip, char* netmask, char* gateway)
{
    FILE *fp;
    
    if ((fp = fopen("/etc/network/interfaces","w+")) == NULL)
    {
        printf("File not open\n");
        return -1;
    }
    else
    {
        fprintf(fp,"# The loopback network interface\n");
        fprintf(fp,"auto lo\n");
        fprintf(fp,"iface lo inet loopback\n");
        fprintf(fp,"\n");
        fprintf(fp,"# The primary network interface\n");
        fprintf(fp,"auto eth0\n");
        fprintf(fp,"iface eth0 inet static\n");
        if (ip == NULL)
            fprintf(fp,"gateway 10.1.1.101\n");
        else    
            fprintf(fp,"address %s\n", ip);
        if (netmask == NULL)
            fprintf(fp,"netmask 255.255.255.0");
        else
            fprintf(fp,"netmask %s\n", netmask);
        if (gateway == NULL)
            fprintf(fp,"gateway 10.1.1.1\n");
        else
            fprintf(fp,"gateway %s\n", gateway);
    }

    fclose(fp);
    return 0;
    
    sync();
	reboot(LINUX_REBOOT_CMD_RESTART);
}

int set_dynamic_ip()
{
    FILE *fp;
    
    if ((fp = fopen("/etc/network/interfaces","w+")) == NULL)
    {
        printf("File not open\n");
        return -1;
    }
    else
    {
        fprintf(fp,"# The loopback network interface\n");
        fprintf(fp,"auto lo\n");
        fprintf(fp,"iface lo inet loopback\n");
        fprintf(fp,"\n");
        fprintf(fp,"# The primary network interface\n");
        fprintf(fp,"auto eth0\n");
        fprintf(fp,"iface eth0 inet dhcp\n");
    }

    fclose(fp);
    return 0;
    
    sync();
	reboot(LINUX_REBOOT_CMD_RESTART);
}

int configure_ip()
{
    int return_value = 0, config = 0;
    char ip[1024];
	char netmask[1024];
	char gateway[1024];
    
    printf("Enter your configuration: \n");
    printf("For Static, press 1\nFor Dynamic, press 2\n");
    scanf("%d", &config);

    if (config == 1)
    {
        printf("Enter IP: ");
    	scanf("%s", ip);
    	printf("Enter Netmask: ");
    	scanf("%s", netmask);
    	printf("Enter Gateway: ");
    	scanf("%s", gateway);

    	return_value = set_static_ip(ip,netmask,gateway);
        if (return_value != 0 )
            return -1;
    }
    else if (config == 2)
    {
        return_value = set_dynamic_ip(ip,netmask,gateway);
        if (return_value != 0 )
            return -1;
    }
    else
    {
        printf("Please give a valid input\n");
        return -1;
    }

    return 0;
}