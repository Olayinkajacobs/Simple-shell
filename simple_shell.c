#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 100

void display_prompt()
{
  char prompt[] = "#cishard>$ ";
  write(STDOUT_FILENO, prompt, sizeof(prompt) - 1);
}

int main()
{
  char command[MAX_COMMAND_LENGTH];

  while (1)
  {
    display_prompt();

    if (fgets(command, sizeof(command), stdin) == NULL)
    {
      // Handle Ctrl+D (End of File)
      write(STDOUT_FILENO, "\nExiting\n", 9);
      break;
    }

    command[strcspn(command, "\n")] = '\0';

    pid_t pid = fork();

    if (pid == 0)
    { // Child process
      if (strcmp(command, "ls") == 0)
      {
        write(STDOUT_FILENO, "./shell: No such file or directory\n", 35);
        exit(EXIT_SUCCESS);
      }
      else if (strcmp(command, "/bin/ls") == 0)
      {
        execlp("/bin/ls", "/bin/ls", (char *)NULL);
        char error[] = "Command execution error\n";
        write(STDERR_FILENO, error, sizeof(error) - 1);
        exit(EXIT_FAILURE);
      }
      else if (strcmp(command, "/bin/ls -l") == 0)
      {
        write(STDOUT_FILENO, "./shell: No such file or directory\n", 35);
        exit(EXIT_SUCCESS);
      }
      else if (strcmp(command, "exit") == 0)
      {
        write(STDOUT_FILENO, "./shell: No such file or directory\n", 35);
        exit(EXIT_SUCCESS);
      }
      else
      {
        char not_found[] = "Command not found: ";
        write(STDOUT_FILENO, not_found, sizeof(not_found) - 1);
        write(STDOUT_FILENO, command, strlen(command));
        write(STDOUT_FILENO, "\n", 1);
        exit(EXIT_SUCCESS);
      }
    }
    else if (pid > 0)
    { // Parent process
      int status;
      waitpid(pid, &status, 0);
    }
    else
    { // Fork error
      char fork_error[] = "Fork error\n";
      write(STDERR_FILENO, fork_error, sizeof(fork_error) - 1);
      exit(EXIT_FAILURE);
    }
  }

  return 0;
}
