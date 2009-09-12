#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define LISTENQ 10
#define MAXDATASIZE 100

int main (int argc, char **argv) {
   int    listenfd, connfd;
   struct sockaddr_in servaddr;
   char   buf[MAXDATASIZE];
   time_t ticks;

   /* socket: cria socket pai.
    * "listenfd" serah o file descriptor usado
    * para operar com o socket */
   if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
      perror("socket");
      exit(1);
   }

   /* Contrucao do endereco Internet do servidor */
   /* servaddr struct zerada */
   bzero(&servaddr, sizeof(servaddr));
   /* Este eh um endereco de Internet */
   servaddr.sin_family      = AF_INET;
   /* Deixe o sistema descobrir nosso IP */
   servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
   /* Esta serah a porta na qual escutaremos */
   servaddr.sin_port        = htons(13);

   /* bind: associa o socket pai com uma porta */
   if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
      perror("bind");
      exit(1);
   }

   /* listen: deixa esse socket preparado to aceitar pedidos de conexao */
   if (listen(listenfd, LISTENQ) == -1) {
      perror("listen");
      exit(1);
   }

   /*
    * main loop: espere por um pedido de conexao, devolva a hora,
    * e feche a conexao
    */
   for ( ; ; ) {
      /* accept: espera por um pedido de conexao */
      if ((connfd = accept(listenfd, (struct sockaddr *) NULL, NULL)) == -1 ) {
         perror("accept");
         exit(1);
      }

      ticks = time(NULL);
      snprintf(buf, sizeof(buf), "%.24s\r\n", ctime(&ticks));
      /* write: devolve a hora para o cliente */
      write(connfd, buf, strlen(buf));

      /* close: fecha a conexao */
      close(connfd);
   }
   return(0);
}
