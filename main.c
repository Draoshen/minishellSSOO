/*-
 * main.c
 * Minishell C source
 * Shows how to use "obtain_order" input interface function.
 *
 * Copyright (c) 1993-2002-2019, Francisco Rosales <frosal@fi.upm.es>
 * Todos los derechos reservados.
 *
 * Publicado bajo Licencia de Proyecto Educativo Práctico
 * <http://laurel.datsi.fi.upm.es/~ssoo/LICENCIA/LPEP>
 *
 * Queda prohibida la difusión total o parcial por cualquier
 * medio del material entregado al alumno para la realización
 * de este proyecto o de cualquier material derivado de este,
 * incluyendo la solución particular que desarrolle el alumno.
 *
 * DO NOT MODIFY ANYTHING OVER THIS LINE
 * THIS FILE IS TO BE MODIFIED
 */

#include <stddef.h>			/* NULL */
#include <stdio.h>			/* setbuf, printf */
#include <stdlib.h>

//includes adicionales
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>


extern int obtain_order();		/* See parser.y for description */

int main(void)
{
	char ***argvv = NULL;
	int argvc;
	char **argv = NULL;
	int argc;
	char *filev[3] = { NULL, NULL, NULL };
	int bg;
	int ret;
	//voy a guardar la cantidad de argumentos para cada mandato
	int numArgs[200]; //no creo que me vayan a pasar más de 200 mandatos de una tirada y de ser así GG!




	//Manejo de senales
	struct sigaction act;
	act.sa_handler=SIG_IGN;
	act.sa_flags=SA_RESTART;
	sigaction(SIGINT,&act,NULL);
	sigaction(SIGQUIT,&act,NULL);


	//Funcion que va a analizar las distintas tareas que tenga que hacer
/*	void functionName(ParameterList)
{
	//Statement 
	    
}
*/
	setbuf(stdout, NULL);			/* Unbuffered */
	setbuf(stdin, NULL);

	while (1) {
		fprintf(stderr, "%s", "msh> ");	/* Prompt */
		ret = obtain_order(&argvv, filev, &bg);
		if (ret == 0) break;		/* EOF */
		if (ret == -1) continue;	/* Syntax error */
		argvc = ret - 1;		/* Line */
		if (argvc == 0) continue;	/* Empty line */
#if 1
/*
 * LAS LINEAS QUE A CONTINUACION SE PRESENTAN SON SOLO
 * PARA DAR UNA IDEA DE COMO UTILIZAR LAS ESTRUCTURAS
 * argvv Y filev. ESTAS LINEAS DEBERAN SER ELIMINADAS.
 */
	for (argvc = 0; (argv = argvv[argvc]); argvc++) {
		for (argc = 0; argv[argc]; argc++){}
				//printf("El argumento %d , es : %s \n", argc,argv[argc]);
			numArgs[argvc]=numArgs[argvc]++;
			//printf("\n");
		}
	if (filev[0]) printf("< %s\n", filev[0]);/* IN */
	if (filev[1]) printf("> %s\n", filev[1]);/* OUT */
	if (filev[2]) printf(">& %s\n", filev[2]);/* ERR */
	if (bg) {
		printf("&\n");
		printf("El programa se ejecuta en background\n");
	}
	//printf("Los valores de argvc:%d,argc:%d\n",argvc,argc );

	if (argvc>1)
	{
		//printf("nos va a tocar pringar las manos\n");
	}
	else{
		//printf("nos ha tocado lo fácil\n");
		//printf("El comando a ejecutar es:%s\n",*argvv[0] );
		argv=argvv[0];
		//printf("su primer argumento es: %s\n",argv[1] );
		if (strstr(argv[0],"exit"))
		{
				exit(0);
		}
		if (strstr(argv[0],"cd"))
		{	
			char cwd[2048];
			if (argv[1]!=NULL)
			{	

				chdir(argv[1]);
				getcwd(cwd, sizeof(cwd));
				printf("%s\n",cwd );
			}
			else{
				chdir(getenv("HOME"));
				getcwd(cwd, sizeof(cwd));
				printf("%s\n",cwd );
			}
		}

		//Mandatos no reconocidos y que por tanto son externos
		else{
			int pid;
			int status;
			switch(pid=fork()){
				case -1:
				perror("Error en la llamada al fork");
				break;
				//código del hijo
				case 0:
					//printf("Hola soy el hijo\n");
					sleep(1);
					execvp(argv[0],&argv[0]);
					perror("Error en el exec");
				break;
				//código del padre
				default:
					//printf("Estamos esperando a que el hijo haga sus cositas\n");
					pid=wait(&status);
					if ( WIFEXITED(status) ){
						int exit_status = WEXITSTATUS(status);        
        				//printf("Exit status of the child was %d\n",exit_status);
					}
					//printf("He terminado con la tarea\n");
			}
		}
		

	}
	//printf("%s\n",*argvv[1] );
/*
 * FIN DE LA PARTE A ELIMINAR
 */
#endif
	}
	exit(0);
	return 0;
}
