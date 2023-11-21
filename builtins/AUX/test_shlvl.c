#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[], char *envp[]) {
    // Check if SHLVL is already set
    char	*shlvl_str;
	int		shlvl;

	shlvl_str = get_env_var_value("SHLVL");
    if (shlvl_str != NULL) 
		shlvl = atoi(shlvl_str);
	else 
		shlvl = 0;

    // Increment SHLVL for the current shell
    update_env_var_value("SHLVL", ++shlvl, 1);

    // Your shell logic goes here

    // Example: if you spawn a new shell, increment SHLVL for the new shell
    if (fork() == 0) {
        setenv("SHLVL", ++shlvl, 1);
        // Code for the new shell process
        // ...

        // Exit the new shell
        exit(0);
    }

    // Rest of your shell logic

    return 0;
}


void	update_shlvl(void)
{
	char	*value;
	char	*new;
	int		num;

	value = env_node_value(&g_msh.env, "SHLVL");
	if (!value || !*value || !ft_isnum(value, 1))
		node_update("SHLVL", "1", EQUAL);
	else
	{
		num = ft_atoi(value);
		num++;
		if (num < 0)
			new = ft_strdup("0");
		else if (num > 999)
			new = ft_strdup("1");
		else
			new = ft_itoa(num);
		node_update("SHLVL", new, EQUAL);
		free(new);
	}
}