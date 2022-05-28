#pragma once
#include <iostream>

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <sys/types.h>
#include <errno.h>
#include <net/route.h>
#include <net/if_arp.h>
#if 0
#include <stdio.h>
#include <error.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <net/if.h>
#include <net/route.h>
//#include <sys/wait.h>
#include <sys/ioctl.h>
//#include <sys/socket.h>
#include <linux/if_tun.h>
#include <assert.h>
#include <sys/types.h>
//#include <sys/stat.h>
//#include <fcntl.h>
//#include <arpa/inet.h>
#endif
inline char* set_tun_name(char* dest, char* name) { strcpy(dest, name); return dest;}


inline int tun_create(char *dev, int flags)
{
    struct ifreq ifr;
    int fd, err;

    assert(dev != NULL);

    if ((fd = open("/dev/net/tun", O_RDWR)) < 0)
        return fd;

    memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_flags |= flags;

    if (*dev != '\0')
        strncpy(ifr.ifr_name, dev, IFNAMSIZ);

    if ((err = ioctl(fd, TUNSETIFF, (void *)&ifr)) < 0) {
        close(fd);
        return err;
    }

    strcpy(dev, ifr.ifr_name);
    std::cout << "tun_create for name:" << dev << " success!!"  << std::endl;
    return fd;
}

inline int set_if_flags(char *ifname, short flags)
{
    int sockfd;
    struct ifreq ifr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("create socket error");
        return -1;
    }

    /* set ip address */
    memset(&ifr, 0, sizeof(ifr));
    strcpy(ifr.ifr_name, ifname);

    if (ioctl(sockfd, SIOCGIFFLAGS, &ifr) < 0 ) {
        perror("SIOCSIFADDR");
        close(sockfd);
        return -1;
    }

    ifr.ifr_flags |= flags;

    if (ioctl(sockfd, SIOCSIFFLAGS, &ifr) < 0 ) {
        perror("SIOCSIFADDR");
        close(sockfd);
        return -1;
    }

    close(sockfd);
    return 0;
}

inline int set_if_addr(char *ifname, char *ipaddr)
{
    int sockfd;
    struct ifreq ifr;
    struct sockaddr_in *sin;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("create socket error");
        return -1;
    }

    /* set ip address */
    memset(&ifr, 0, sizeof(ifr));
    strcpy(ifr.ifr_name, ifname);

    sin = (struct sockaddr_in *)&ifr.ifr_addr;
    sin->sin_family = AF_INET;
    inet_pton(AF_INET, ipaddr, &sin->sin_addr);

    if (ioctl(sockfd, SIOCSIFADDR, &ifr) < 0 ) {
        perror("SIOCSIFADDR");
        close(sockfd);
        return -1;
    }

    close(sockfd);
    return 0;
}

inline int init_tap_interface()
{
    int tun;
    char tun_name[IFNAMSIZ];

    tun_name[0] = '\0';
    tun = tun_create(tun_name, IFF_TAP | IFF_NO_PI);

    if (tun < 0) 
    {
        perror("tun_create");
        return -1;
    }
    printf("TUN name is %s\n", tun_name);

    set_if_flags(tun_name, IFF_UP);
#if 0
    set_if_addr(tun_name, "192.168.100.1");
#endif

    return tun;
}


/**
 *  增加到10.0.0.1的路由
 *  同命令:route add 10.0.0.1 dev tun0
 */
inline int route_add(char * interface_name)
{
    int skfd;
    struct rtentry rt;

    struct sockaddr_in dst;
    //struct sockaddr_in gw;
    struct sockaddr_in genmask;

    std::cout << "[route_add] for tun:" << interface_name << std::endl; 

    memset(&rt, 0, sizeof(rt));

    genmask.sin_addr.s_addr = inet_addr("255.255.255.255");

    bzero(&dst,sizeof(struct sockaddr_in));
    dst.sin_family = PF_INET;
    dst.sin_addr.s_addr = inet_addr("10.0.0.1");

    rt.rt_metric = 0;
    rt.rt_dst = *(struct sockaddr*) &dst;
    rt.rt_genmask = *(struct sockaddr*) &genmask;

    rt.rt_dev = interface_name;
    rt.rt_flags = RTF_UP | RTF_HOST ;

    skfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(ioctl(skfd, SIOCADDRT, &rt) < 0)
    {
        std::cout << "Error route add :" <<  errno << std::endl;
        close(skfd);
        return -1;
    }
    close(skfd);
    return 0;
}

/**
 *  增加到10.0.0.i/24的路由
 *  同命令:route add -net 10.0.0.0 netmast 255.255.255.0 dev tun0
 */
inline int route_add_net(char * interface_name,const char* target,const char* dst_mask)
{
    int skfd;
    struct rtentry rt;

    struct sockaddr_in dst;
    //struct sockaddr_in gw;
    struct sockaddr_in genmask;

    std::cout << "[route_ add_net] for tun:" << interface_name << " net:" << target << " mask:" << dst_mask << std::endl;

    memset(&rt, 0, sizeof(rt));
    genmask.sin_family = AF_INET;
    genmask.sin_addr.s_addr = inet_addr(dst_mask);
    //genmask.sin_addr.s_addr = inet_addr("255.255.0.0");

    bzero(&dst,sizeof(struct sockaddr_in));
    dst.sin_family = AF_INET;
    dst.sin_addr.s_addr = inet_addr(target);
    //dst.sin_addr.s_addr = inet_addr("192.168.0.0");

    rt.rt_metric = 0;
    rt.rt_dst = *(struct sockaddr*) &dst;
    rt.rt_genmask = *(struct sockaddr*) &genmask;

    rt.rt_dev = interface_name;
    rt.rt_flags = RTF_UP;

    skfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(ioctl(skfd, SIOCADDRT, &rt) < 0)
    {
        std::cout << "Error route add :" <<  errno << std::endl;
        close(skfd);
        return -1;
    }
    close(skfd);
    return 0;
}

/**
 *  ⚌~^⚌~J| ⚌~H⚌10.0.0.i/24⚌~Z~D路⚌~T⚌
 *  ⚌~P~L⚌~Q⚌令:route add -net 10.0.0.0 netmast 255.255.255.0 dev tun0
 */
inline int route_del_net(char * interface_name,const char* target,const char* dst_mask)
{
    int skfd;
    struct rtentry rt;

    struct sockaddr_in dst;
    //struct sockaddr_in gw;
    struct sockaddr_in genmask;

    memset(&rt, 0, sizeof(rt));
    genmask.sin_family = AF_INET;
    genmask.sin_addr.s_addr = inet_addr(dst_mask);
    //genmask.sin_addr.s_addr = inet_addr("255.255.0.0");

    bzero(&dst,sizeof(struct sockaddr_in));
    dst.sin_family = AF_INET;
    dst.sin_addr.s_addr = inet_addr(target);
    //dst.sin_addr.s_addr = inet_addr("192.168.0.0");

    rt.rt_metric = 0;
    rt.rt_dst = *(struct sockaddr*) &dst;
    rt.rt_genmask = *(struct sockaddr*) &genmask;

    rt.rt_dev = interface_name;
    rt.rt_flags = RTF_UP;

    skfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(ioctl(skfd, SIOCDELRT, &rt) < 0)
    {
        std::cout << "Error route del :" <<  errno << std::endl;
        close(skfd);
        return -1;
    }

    close(skfd);
    return 0;
}


inline int get_mac(char * interface_name, char* mac, int len_limit)
{
    struct ifreq ifreq;
    int sock;

    if ((sock = socket (AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror ("socket");
        close(sock);
        return -1;
    }
    strcpy (ifreq.ifr_name, interface_name);    //Currently, only get eth0

    if (ioctl (sock, SIOCGIFHWADDR, &ifreq) < 0)
    {
        perror ("ioctl");
        close(sock);
        return -1;
    }

    memcpy(mac, ifreq.ifr_hwaddr.sa_data, len_limit);
#if 0
    printf("mac = %x:%x:%x:%x:%x:%x\n",  (unsigned char) ifreq.ifr_hwaddr.sa_data[0], (unsigned char) ifreq.ifr_hwaddr.sa_data[1],
              (unsigned char) ifreq.ifr_hwaddr.sa_data[2], (unsigned char) ifreq.ifr_hwaddr.sa_data[3],
               (unsigned char) ifreq.ifr_hwaddr.sa_data[4], (unsigned char) ifreq.ifr_hwaddr.sa_data[5]); 

   printf("mac2 = %x:%x:%x:%x:%x:%x\n",  (unsigned char) mac[0], (unsigned char) mac[1],
              (unsigned char) mac[2], (unsigned char) mac[3],
               (unsigned char) mac[4], (unsigned char) mac[5]);
#endif

    close(sock);
    return 0;
#if 0
    return snprintf (mac, len_limit, "%X%X%X%X%X%X", (unsigned char) ifreq.ifr_hwaddr.sa_data[0], (unsigned char) ifreq.ifr_hwaddr.sa_data[1], (unsigned char) ifreq.ifr_hwaddr.sa_data[2], (unsigned char) ifreq.ifr_hwaddr.sa_data[3], (unsigned char) ifreq.ifr_hwaddr.sa_data[4], (unsigned char) ifreq.ifr_hwaddr.sa_data[5]);
#endif
}


inline int set_mac(char* interface_name, char* mac)
{
    struct ifreq temp;
    struct sockaddr* addr;
 
    int fd = 0;
    int ret = -1;
	
    if((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        return -1;
 
    strcpy(temp.ifr_name, interface_name);
    addr = (struct sockaddr*)&temp.ifr_hwaddr;
	
    addr->sa_family = ARPHRD_ETHER;
    memcpy(addr->sa_data, mac, 6);
	
    ret = ioctl(fd, SIOCSIFHWADDR, &temp);
    close(fd);
    return ret;
}

#if 0

inline std::string dumpMacAddr(char* addr)
{
    std::string result = "";
    if((addr != nullptr) && (strlen(addr) != 6))
        return result;

    char tmp[256];
    memset(tmp, 0, 256);
    sprintf(tmp, "%02x:%02x:%02x:%02x:%02x:%02x",  (unsigned char) addr[0], (unsigned char) addr[1],
              (unsigned char) addr[2], (unsigned char) addr[3],
               (unsigned char) addr[4], (unsigned char) addr[5]); 
    result.assign(tmp, 256);
    return result;
          
}
#endif

#if 0  
int main(int argc, char *argv[])
{
    int tun, ret;
    char tun_name[IFNAMSIZ];
    unsigned char buf[4096];

    tun_name[0] = '\0';
    tun = tun_create(tun_name, IFF_TAP | IFF_NO_PI);

    if (tun < 0) 
    {
        perror("tun_create");
        return 1;
    }
    printf("TUN name is %s\n", tun_name);

    set_if_flags(tun_name, IFF_UP);
    set_if_addr(tun_name, "10.9.8.4");

    while (1) 
    {
        int i = 0;

        ret = read(tun, buf, sizeof(buf));
        if (ret < 0)
            break;

        printf("receive a packet\n");

        for (i=0; i<ret; i++)
        {
            if ((i%8 == 0) && (i != 0))
            {
                printf("\n");
            }

            printf("0x%02x ", buf[i]);
        }

        printf("\n");
    }

    return 0;
}
#endif
