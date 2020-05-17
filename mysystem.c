#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define ERR_EXIT( msg ) \
	do {		\
		printf("[%s][%d] %s:%s\n",__FILE__ , __LINE__ , msg , strerror(errno) ) ; \
		exit( errno ) ;	\
	} while ( 0 ) 

int mysystem( const char* cmd ) {
	pid_t pid ;
	if ( ( pid = fork() ) == -1 ) {
		exit( 127 ) ;
	}
	if ( pid == 0 ) {
		if ( execlp( "/bin/sh","/bin/sh","-c",cmd,NULL) == -1 ) {
			exit( 1 ) ;
		} 
	} 
	
	//子进程一定走不到这里，这里就是父进程
	int status ;
	//一直等待，等到pid子进程结束，结束之后父进程回收，之后结束
	// while 高并发
	while ( waitpid( pid , &status , WNOHANG ) != pid ) 
		;
	if ( WIFEXITED( status ) ) {
		return WEXITSTATUS(status) ;
	}
	return -1 ;
}
int main( void ) {
	printf("before mysystem\n") ;
	mysystem("ls -l") ;
	printf("after mysystem\n") ;
	return 0 ;
}
