#ifndef _LWIPOPTS_H
#define _LWIPOPTS_H

// Configurações recomendadas para o Pico W
#define NO_SYS                      1
#define LWIP_SOCKET                 0
#define LWIP_NETCONN                0
#define LWIP_NETIF_HOSTNAME         1
#define LWIP_TCP                    1
#define LWIP_UDP                    1
#define LWIP_DHCP                   1
#define LWIP_DNS                    1
#define LWIP_TCP_KEEPALIVE          1
#define MEM_LIBC_MALLOC             0
#define MEM_ALIGNMENT               4
#define MEM_SIZE                    4000
#define MEMP_NUM_TCP_SEG            32
#define TCP_MSS                     1460
#define TCP_WND                     (8 * TCP_MSS)
#define TCP_SND_BUF                 (8 * TCP_MSS)
#define LWIP_CHKSUM_ALGORITHM       3
#define LWIP_CALLBACK_API           1
#define LWIP_STATS                  0
#define LWIP_DEBUG                  0

#endif