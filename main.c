//text editor written in C
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>

//defines
#define CTRL_KEY(k) ((k) & 0x1f)

struct termios original_terminal;

void die(const char *s){
    perror(s);
    exit(1);
}

void disableRawMode(){

    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_terminal) == -1)
        die("tcsetattr");
}

void enableRawMode() {

    if (tcgetattr(STDIN_FILENO, &original_terminal) == -1) die("tcgetattr");
    atexit(disableRawMode);

    struct termios raw = original_terminal;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &=~(ECHO | ICANON | IEXTEN | ISIG);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    if (tcgetattr(STDIN_FILENO, &original_terminal) == -1) die("read");
}

int main() {

    enableRawMode();

    while(1) {
    char c = '\0';
    read(STDIN_FILENO, &c, 1);
        if(iscntrl(c)){
            printf("%d\r\n", c);
        } else {
            printf("%d ('%c')\r\n", c, c);
        }
        if(c == CTRL_KEY('q')) break;
    }
    return 0;

}