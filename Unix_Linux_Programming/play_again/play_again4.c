/* play_again4.c
 * purpose: ask if user wants another transaction
 * method: set tty into char-by-char, no-echo mode
 * 	   set tty into no-delay node
 * 	   read char, return result
 * 	   resets terminal modes on SIGINT, ignores SIGQUIT
 * returns: 0=>yes, 1=>no, 2=>timeout
 * better: reset terminal mode on Interrupt
 */
#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>

#define ASK "Do you want another transaction"
#define TRIES 3
#define SLEEPTIME 2
#define BEEP putchar('\a')

int get_response(char*, int);
void set_cr_noecho_mode();
int tty_mode(int);
int get_ok_char();
void set_nodelay_mode();

int main()
{
	int response;
	void ctrl_c_handler(int);
	tty_mode(0);
	set_cr_noecho_mode();
	set_nodelay_mode();
	signal(SIGINT, ctrl_c_handler);
	signal(SIGQUIT, SIG_IGN);
	response = get_response(ASK, TRIES);
	tty_mode(1);
	return response;
}

/*
 * purpose: ask a question and wait for a y/n answer
 * method: use getchar and ignore non y/n answers
 * returns: 0=>yes, 1=>no
 */
int get_response(char *question, int maxtries){
	printf("%s(y/n)?", question);
	int input;
	fflush(stdout);
	while(1){
		sleep(SLEEPTIME);
		input = tolower(get_ok_char());
		if(input == 'y')
			return 0;
		if(input == 'n')
			return 1;
		if(maxtries-- == 0)
			return 2;
		BEEP;
	}
}

/* skip over non - legal chars and return y,Y,n,N or EOF */
int get_ok_char()
{
	int c;
	while((c = getchar()) != EOF && strchr("yYnN", c) == NULL);
	return c;
}

/*
 * purpose: put file descriptor 0 (i.e. stdin) into chr-by-chr mode and noecho mode
 * method: use bit in termois
 */
void set_cr_noecho_mode(){
	struct termios ttystate;
	tcgetattr(0, &ttystate);
	ttystate.c_lflag &= ~ICANON;
	ttystate.c_lflag &= ~ECHO;
	ttystate.c_cc[VMIN] = 1;
	tcsetattr(0, TCSANOW, &ttystate);
}

/* purpose:put file descriptor 0 into no - delay mode
 * method:use fcntl to set bits
 * notes:tcsetattr() will do something similar, but it is complicated
 */
void set_nodelay_mode(){
	int termflags;
	termflags = fcntl(0, F_GETFL);
	termflags |= O_NONBLOCK;
	fcntl(0, F_SETFL, termflags);
}
/* how == 0 => save current mode, how == 1 => restore mode */
int tty_mode(int how)
{
	static struct termios original_mode;
	static int original_flags;
	if(how == 0){
		tcgetattr(0, &original_mode);
		original_flags = fcntl(0, F_GETFL);
	}
	else{
		tcsetattr(0, TCSANOW, &original_mode);
		fcntl(0, F_SETFL, original_flags);
	}
}

/*
 * purpose: called if SIGINT is detected
 * action: reset tty and scram
 */
void ctrl_c_handler(int sig){
	tty_mode(1);
	exit(1);
}
