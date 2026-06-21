#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

#define BUFFER_SIZE 65536

int main()
{
    int sock_raw;
    unsigned char *buffer = (unsigned char *)malloc(BUFFER_SIZE);
    struct sockaddr saddr;
    socklen_t saddr_len = sizeof(saddr);

    int packet_count = 0;

    sock_raw = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

    if (sock_raw < 0)
    {
        perror("Socket Error");
        return 1;
    }

    printf("ROLL_NO=CSB24051\n");
    printf("ASSIGNED_PROTOCOL=ICMP\n\n");

    while (packet_count < 20)
    {
        int data_size = recvfrom(sock_raw, buffer, BUFFER_SIZE, 0,
                                 &saddr, &saddr_len);

        if (data_size < 0)
        {
            perror("Recvfrom Error");
            break;
        }

        struct iphdr *ip_header = (struct iphdr *)buffer;

        if (ip_header->protocol == IPPROTO_ICMP)
        {
            struct sockaddr_in src, dst;
            src.sin_addr.s_addr = ip_header->saddr;
            dst.sin_addr.s_addr = ip_header->daddr;

            struct icmphdr *icmp_header =
                (struct icmphdr *)(buffer + (ip_header->ihl * 4));

            packet_count++;

            printf("PACKET_NO=%d\n", packet_count);

            printf("SRC_IP=%s\n",
                   inet_ntoa(src.sin_addr));

            printf("DST_IP=%s\n",
                   inet_ntoa(dst.sin_addr));

            printf("PROTOCOL=ICMP\n");

            printf("PROTOCOL_NO=%d\n",
                   ip_header->protocol);

            printf("TTL=%d\n",
                   ip_header->ttl);

            printf("PACKET_SIZE=%d\n",
                   data_size);

            printf("ICMP_TYPE=%d\n",
                   icmp_header->type);

            printf("ICMP_CODE=%d\n",
                   icmp_header->code);

            /* ---------- Task 5 Enhancement ---------- */
            printf("IP_VERSION=%d\n",
                   ip_header->version);
            /* --------------------------------------- */

            printf("\n");
        }
    }

    close(sock_raw);
    free(buffer);

    return 0;
}