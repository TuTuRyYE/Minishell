#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h> /* wait */
#include <signal.h>
#include <errno.h>
#include <fcntl.h> 
#include "readcmd.h"

void printDir() { 
   char cwd[1024]; 
   getcwd(cwd, sizeof(cwd)); 
   printf("%s$ ", cwd); 
}

void cd(char ***input) { 
   if(input[0][1] == NULL){
      chdir("/home");
   }
   else {
      if (chdir(input[0][1]) == -1) {
         perror("Aucun répertoire de ce nom");
      }
   } 
}

void handler_chld(int signal_num) {
   int fils_termine, wstatus ;
   //printf("\nJ'ai reçu le signal %d\n",  signal_num) ;
   if (signal_num == SIGCHLD) {
      while ((fils_termine = (int) waitpid(-1, &wstatus, WNOHANG | WUNTRACED | WCONTINUED)) > 0) {
         if WIFEXITED(wstatus) {
            printf("\nMon fils de pid %d s'est arrete avec exit %d\n",  fils_termine, WEXITSTATUS(wstatus)) ;
         }
         else if WIFSIGNALED(wstatus) {
            printf("\nMon fils de pid %d a recu le signal %d\n",  fils_termine, WTERMSIG(wstatus)) ;
         }
            else if (WIFCONTINUED(wstatus)) {
            printf("\nMon fils de pid %d a ete relance \n",  fils_termine) ;
         }
         else if (WIFSTOPPED(wstatus)) {
            printf("\nMon fils de pid %d a ete suspendu \n", fils_termine) ;
         }
         sleep(1) ;
      }
   }   
}


void handler_sigpipe(int signal_num) {
   printf("\n     Processus de pid %d : J'ai reçu le signal %d\n", getpid(), signal_num) ;
   return ;
}

void close_pipes(int i, int nb_pipe, int pipe_cmd[nb_pipe][2]) {
   if(i > nb_pipe) {
      printf("Erreur index i, pour le nombre de commandes");
      return;
   }
   for(int j = 0; j < nb_pipe; j++) {
      if(i < 0) {
         close(pipe_cmd[j][0]);
         close(pipe_cmd[j][1]);
      } else if(i == 0) {
         if(j == 0) {
            close(pipe_cmd[j][0]);
            if(dup2(pipe_cmd[j][1], 1) < 0) {
               perror("");
               exit(EXIT_FAILURE);
            }
         } else {
            close(pipe_cmd[j][0]);
            close(pipe_cmd[j][1]);
         }
      } else if(i == nb_pipe) {
         if(j == nb_pipe-1) {
            close(pipe_cmd[j][1]);
            if(dup2(pipe_cmd[j][0], 0) < 0) {
               perror("");
               exit(EXIT_FAILURE);
            }
         } else {
            close(pipe_cmd[j][0]);
            close(pipe_cmd[j][1]);
         }
      } else {
         if(j == i-1) {
            close(pipe_cmd[j][1]);
            if(dup2(pipe_cmd[j][0], 0) < 0) {
               perror("");
               exit(EXIT_FAILURE);
            }
         } else if(j == i) {
            close(pipe_cmd[j][0]);
            if(dup2(pipe_cmd[j][1], 1) < 0) {
               perror("");
               exit(EXIT_FAILURE);
            }
         } else {
            close(pipe_cmd[j][0]);
            close(pipe_cmd[j][1]);
         }
      }
   }
}



int main(int argc, char *argv[]) {
   struct cmdline *cmd;
   pid_t pidFils, idFils;
   int codeTerm;



   typedef struct processus{
		int id_shell;
		pid_t pid;
		int bg_flag;
		char command[1024]; //fixe mais de grande taille
		int ended;
	}p;

   p jobs[1000];
	int nb_jobs = 1;

   void ajouter_job(pid_t pid, int bg_flag, char ***cmd) {
      char command[1024] = "";
      int i = 0;
      int j = 0;
      while(cmd[i] != NULL){
         if(i != 0) {
            strcat(command, "| ");
         }
         while(cmd[i][j] != NULL){
            strcat(command, cmd[i][j]);
            strcat(command, " ");
            j = j + 1;
         }
         i = i + 1;
      }
		jobs[nb_jobs].id_shell = nb_jobs;
		jobs[nb_jobs].pid = pid;
		jobs[nb_jobs].bg_flag = bg_flag;
		strcpy(jobs[nb_jobs].command, command);
		jobs[nb_jobs].ended = 0;
      nb_jobs = nb_jobs + 1;
	}

   void list(p jobs[], int nb_jobs) {
		printf("Liste des jobs :\n");
  		printf("%-10s%-10s%-10s%-10s\n","ID_SHELL", "PID", "BG(1/0)", "COMMANDE");
		for(int i = 1; i < nb_jobs; i = i+1 ){
      		if (jobs[i].ended){
      			continue;
      		}
      		else{
      			printf("%-10d%-10d%-10d%-10s\n", jobs[i].id_shell, jobs[i].pid, jobs[i].bg_flag, jobs[i].command);
     		} 			
    	}
	}

   void stop(int i) {
      if(i >= nb_jobs) {
         fprintf(stderr, "Mauvaise utilisation de l'id\n");
      } else {
         p job = jobs[i];
         if(job.ended) {
            fprintf(stderr, "Mauvaise utilisation de l'id\n");
         } else {
            if(kill(job.pid, 20) < 0){
               perror("bg");
}

void close_pipes(int i, int nb_pipe, int pipe_cmd[nb_pipe][2]) {
   if(i > nb_pipe) {
      printf("Erreur index i, pour le nombre de commandes");
      return;
   }
   for(int j = 0; j < nb_pipe; j++) {
      if(i < 0) {
         close(pipe_cmd[j][0]);
         close(pipe_cmd[j][1]);
      } else if(i == 0) {
         if(j == 0) {
            close(pipe_cmd[j][0]);
            if(dup2(pipe_cmd[j][1], 1) < 0) {
               perror("");
               exit(EXIT_FAILURE);
            }
         } else {
            close(pipe_cmd[j][0]);
            close(pipe_cmd[j][1]);
         }
      } else if(i == nb_pipe) {
         if(j == nb_pipe-1) {
            }
         }
      }
   }

   void bg(int i) {
      if(i >= nb_jobs) {
         fprintf(stderr, "Mauvaise utilisation de l'id\n");
      } else {
         p job = jobs[i];
         if(job.ended) {
            fprintf(stderr, "Mauvaise utilisation de l'id\n");
         } else {
            if(kill(job.pid, 18) < 0){
               perror("bg");
            }
         }
      }
   }

   void fg(int i) {
      if(i >= nb_jobs) {
         fprintf(stderr, "Mauvaise utilisation de l'id\n");
      } else {
         p job = jobs[i];

         if(job.ended) {
            fprintf(stderr, "Mauvaise utilisation de l'id\n");
         } else {
            if(kill(job.pid, 18) < 0){
               perror("bg");
            } else {
               idFils=wait(&codeTerm);
            }
         }
      }
   }

   signal(SIGCHLD, handler_chld);
   signal(SIGPIPE, handler_sigpipe);

   while(1) {
      sleep(1); //pour la mise en page
      printDir();
      printf("%d ",getpid());
      fflush(stdout);
      cmd = readcmd();

      if((cmd->err) != NULL) {
         printf("%s\n", cmd->err);
      } else {
         if(*(cmd->seq) != NULL) {
            //Commandes internes
            //exit
            if(strcmp("exit", cmd->seq[0][0]) == 0) {
               break;
            }
            //cd
            else if(strcmp("cd", cmd->seq[0][0]) == 0){
               cd(cmd->seq);
            }
            //list
            else if(strcmp("list", cmd->seq[0][0]) == 0){
               list(jobs, nb_jobs);
            }
            //stop
            else if(strcmp("stop", cmd->seq[0][0]) == 0){
               char *id_job = cmd->seq[0][1];
               int id_job_int;
               if(id_job == NULL || (id_job_int = atoi(id_job)) == 0) {
                  fprintf(stderr,"Erreur paramètre de stop\n");
               } else {
                  stop(id_job_int);
               }
            }
            //bg
            else if(strcmp("bg", cmd->seq[0][0]) == 0){
               char *id_job = cmd->seq[0][1];
               int id_job_int;
               if(id_job == NULL || (id_job_int = atoi(id_job)) == 0) {
                  fprintf(stderr,"Erreur paramètre de stop\n");
               } else {
                  bg(id_job_int);
               }
            }
            //fg
            else if(strcmp("fg", cmd->seq[0][0]) == 0){
               char *id_job = cmd->seq[0][1];
               int id_job_int;
               if(id_job == NULL || (id_job_int = atoi(id_job)) == 0) {
                  fprintf(stderr,"Erreur paramètre de stop\n");
               } else {
                  fg(id_job_int);
               }
            }
            //Commandes générales
            else {
               int nb_cmd = 0;
               while(cmd->seq[nb_cmd] != NULL) {
                  nb_cmd++; 
               }
               int nb_pipe = nb_cmd-1;
               int pipe_cmd[nb_pipe][2];
               for(int i = 0; i < nb_pipe; i++) {
                  if(pipe(pipe_cmd[i]) < 0) {
                     printf("Erreur pipe\n");
                     exit(EXIT_FAILURE);
                  }
               }
               for(int i = 0; i < nb_cmd; i++) {
                  pidFils = fork();
                  if(pidFils == -1) {
                     printf("Erreur fork\n");
                     exit(1);
                  } else if (pidFils == 0) { /*fils*/
                     //premier parametre la commande tandis que le deuxième c'est la commande complete
                     if((cmd->in) != NULL) {
                        //redirection de l'entrée standard, si erreur -> arrêt et message d'erreur
                        int fd_in;
                        if((fd_in = open(cmd->in, O_RDONLY)) < 0) {
                           perror("");
                           exit (EXIT_FAILURE);
                        //on associe fd_in à l'entrée standard
                        } else if(dup2(fd_in, 0) < 0) {
                           perror("");
                           exit(EXIT_FAILURE);
                        }
                     }
                     if((cmd->out) != NULL) {
                        //redirection de la sortie standard, si erreur -> arrêt et message d'erreur
                        int fd_out;
                        if((fd_out = open(cmd->out, O_WRONLY | O_CREAT | O_TRUNC, 0640)) < 0) {
                           perror("");
                           exit (EXIT_FAILURE);
                        //on associe fd_in à la sortie standard
                        } else if(dup2(fd_out, 1) < 0) {
                           perror("");
                           exit (EXIT_FAILURE);
                        }
                     }
                     close_pipes(i, nb_pipe, pipe_cmd);
                     if (execvp(cmd->seq[i][0], cmd->seq[i]) < 0) {
                        perror("La commande n'a pu être exécuté (execvp)"); 
                        exit(EXIT_FAILURE);
                     }
                     exit(EXIT_SUCCESS); //Au cas où
                  } else { /*pere*/
                     //printf("\n\n%d\n\n", nb_cmd);
                     //printf("\n\n%d\n\n", i);
                     if(i==nb_cmd-1) {
                        close_pipes(-1, nb_pipe, pipe_cmd);
                        int bg;
                        //printf("processus %d (pere), de pere %d\n", getpid(), getppid ());
                        //printf("processus %d (p`ere), de p`ere %d\n", getpid(), getppid ());
                        if((cmd->backgrounded) != NULL) {
                           bg = 1;
                           ajouter_job(pidFils, bg, cmd->seq);
                           //CREER UN DECALAGE A REGLER
                           continue;
                        } else {
                           bg = 0;
                           idFils=wait(&codeTerm);
                        }
                        if (idFils  ==  -1) {
                           perror("wait ");
                           exit (2);
                        }
                        if (WIFEXITED(codeTerm )) {
                           printf("[%d] fin fils %d par exit %d\n",codeTerm ,idFils ,WEXITSTATUS(codeTerm ));
                        } else {
                           printf("[%d] fin fils %d par signal %d\n",codeTerm ,idFils ,WTERMSIG(codeTerm ));
                        }
                     }
                  }
               }
            }
         }
      }
   }
   return EXIT_SUCCESS;
}