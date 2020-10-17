/* play_again2.c
 * purpose: ask if user wants another transaction
 * method: set tty into char-by-char mode and no-echo mode, read char, return result
 * returns: 0=>yes, 1=>no
 * better: timeout if user walks away
 */
#include <stdio.h>
#include <termios.h>

#define QUESTION "Do you want another transaction"

int get_response(char*);
void set_cr_noecho_mode();
int tty_mode(int);

int main()
{
	int response;
	tty_mode(0);
	set_cr_noecho_mode();
	response = get_response(QUESTION);
	tty_mode(1);
	return response;
}

/*
 * purpose: ask a question and wait for a y/n answer
 * method: use getchar and ignore non y/n answers
 * returns: 0=>yes, 1=>no
 */
int get_response(char *question){
	printf("%s(y/n)?", question);
	int input;
	while(1){
		switch(input = getchar()){
			case 'y':
			case 'Y':return 0;
			case 'n':
			case 'N':
			case EOF: return 1;
		}
	}
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

/* how == 0 => save current mode, how == 1 => restore mode */
int tty_mode(int how)
{
	static struct termios original_mode;
	if(how == 0)
		tcgetattr(0, &original_mode);
	else
		return tcsetattr(0, TCSANOW, &original_mode);
}
