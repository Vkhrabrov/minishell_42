#include "minishell.h"

static struct termios original_termios;

static void new_prompt_line()
{   
    printf("\n");
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void init_signals(void)
{
    struct sigaction    sigint_action;  // Replaces default 'ctrl + c'
    struct sigaction    sigquit_action; // Replaces default 'ctrl + \'

    // Setup the SIGINT ('ctrl + c') handler
    sigint_action.sa_handler = new_prompt_line;
    sigemptyset(&sigint_action.sa_mask);
    sigint_action.sa_flags = 0;
    sigaction(SIGINT, &sigint_action, NULL);

    // Setup the SIGQUIT ('ctrl + \') handler
    sigquit_action.sa_handler = SIG_IGN;
    sigemptyset(&sigquit_action.sa_mask);
    sigquit_action.sa_flags = 0;
    sigaction(SIGQUIT, &sigquit_action, NULL);
} 

void disable_control_chars_echo() 
{
    struct termios new_termios; 

    tcgetattr(0, &original_termios);
    new_termios = original_termios;
    new_termios.c_lflag &= ~ECHOCTL; // Disable echoing of control characters (^C, ^\)
    tcsetattr(0, TCSANOW, &new_termios);
}

void restore_terminal_settings() 
{
    tcsetattr(0, TCSANOW, &original_termios);
}
