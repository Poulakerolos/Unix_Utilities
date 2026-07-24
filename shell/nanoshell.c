#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#define yellow_color "\033[33m"
#define reset_color  "\033[0m"
//block1:take user input {each word is separated form the other by one space}
char *
readshell ()
{
  int arraysize = 8;
  int length = 0;
  char *buf = malloc (arraysize);
  int c;
  while ((c = getchar ()) != '\n')
    {
      if (c == EOF)
	{
	  free (buf);
	  return 0;
	}
      if (arraysize < length + 1)
	{
	  arraysize *= 2;
	  char *temp = realloc (buf, arraysize);
	  if (temp == NULL)
	    {
	      printf ("error\n");
	      return NULL;
	    }
	  buf = temp;
	}
      if (c == ' ')
	{
	  if (length > 0 && buf[length - 1] != ' ')
	    buf[length++] = ' ';
	}
      else
	buf[length++] = (char) c;

    }
  buf[length] = 0;
  return buf;
}

//block2:returns pointer to character {will be used int block 3}
char *
findword (char *buf)
{
  int commandsize = 4;
  int length = 0;
  char *bufc = malloc (commandsize);
  while (buf[length] != ' ' && buf[length] != '\0')
    {
      if (length + 1 >= commandsize)
	{
	  commandsize *= 2;
	  char *temp = realloc (bufc, commandsize);
	  if (temp == NULL)
	    {
	      printf ("error\n");
	      return NULL;
	    }
	  bufc = temp;
	}
      bufc[length] = buf[length];
      length++;
    }
  bufc[length++] = '\0';
  return bufc;
}

//block3:uses block2 to divide the user input into tokens,returns char** 
//char**-----------------------------
//      |         |        |        |
//    token1    token2   token3   NULL
char **
tokenize (char *input)
{
  int maxTokens = 100;
  char **tokens = malloc (maxTokens * sizeof (char *));
  if (tokens == NULL)
    {
      printf ("error in tokenize\n");
      fflush (stdout);
      return NULL;
    }
  int argc = 0, pos = 0;
  while (input[pos] != '\0')
    {
      if (argc > maxTokens - 1)
	{
	  maxTokens *= 2;
	  char **temp = realloc (tokens, sizeof (char *) * maxTokens);
	  if (temp == NULL)
	    {
	      printf ("error:reallocating tokens\n");
	      return NULL;
	    }
	  tokens = temp;
	}
      while (input[pos] == ' ')
	pos++;
      char *word = findword (&input[pos]);
      tokens[argc++] = word;
      pos += strlen (word);
    }
  tokens[argc] = NULL;
  return tokens;
}

//deallocation
void
freetokens (char **tokens)
{
  for (int i = 0; tokens[i]; i++)
    free (tokens[i]);
  free (tokens);
}

int envsize = 100;
int locsize = 100;
char **envar = NULL;
char **enval = NULL;
char **locvar = NULL;
char **locval = NULL;
int lenenv = 0, lenloc = 0;

void
store_variable (char *argv0, char *argv1, char *argv2, char *argv3)
{
  if (lenenv + 1 >= envsize)
    {
      envsize *= 2;
      envar = realloc (envar, envsize * sizeof (char *));
      enval = realloc (enval, envsize * sizeof (char *));
    }
  if (lenloc + 1 >= locsize)
    {
      locsize *= 2;
      locvar = realloc (locvar, locsize * sizeof (char *));
      locval = realloc (locval, locsize * sizeof (char *));
    }
  if (strcmp (argv0, "export") == 0 && argv1 && argv2 && argv3)
    {
      //strdup(char*s)creates a duplicate of what is in s and returns a pointer to where that duplicate is stored
      envar[lenenv] = strdup (argv1);
      enval[lenenv] = strdup (argv3);
      lenenv++;
      envar[lenenv] = NULL;
      enval[lenenv] = NULL;
      return;
    }
  else if (argv0 && argv1 && argv2)
    {
      locvar[lenloc] = strdup (argv0);
      locval[lenloc] = strdup (argv2);
      lenloc++;
      locvar[lenloc] = NULL;
      locval[lenloc] = NULL;
      return;
    }
  else
    {
      printf ("local variables should take the form:variable = value\n");
      printf
	("environment variables should take the form:export varable = value\n");
      return;
    }

}

int
searchdollar (char **argv)
{
  int i = 0;
  for (; argv[i]; i++)
    if (strcmp (argv[i], "$") == 0)
      {
	i++;
	return i;
      }
  return 0;
}

int
nanoshell_main ()
{
  printf (yellow_color "NanoShell_Pversion>>" reset_color);
  fflush (stdout);
  int status;
  char *input = readshell ();
  if (input == NULL)
    {
      free (input);
      return 0;
    }
  char **argv = tokenize (input);
  int variableindex = searchdollar (argv);
  if (argv == NULL || argv[0] == NULL)
    {
      freetokens (argv);
      free (input);
      return 1;
    }
  else if (strcmp (argv[0], "exit") == 0)
    {
      goto terminate;
    }
  else if (strcmp (argv[0], "cd") == 0)
    {
      int value = chdir (argv[1]);
      if (value == 0)
	{
	  return 1;
	}
      else
	return 0;
    }
  else if ((argv[0] && strchr (argv[0], '=') != 0)
	   || (argv[1] && strchr (argv[1], '=') != 0))
    {
	    char*loc=NULL,*env=NULL;
	    if(!argv[1]){
      loc = strchr (argv[0], '=');}
	    else if(argv[1]){
      env = strchr (argv[1], '=');}
      if (loc != NULL)
	{
	  *loc = ' ';
	  char **newtoken = tokenize (argv[0]);
	  store_variable (newtoken[0], "=", newtoken[1], NULL);
	  return 1;
	}
      else if (env != NULL)
	{
	  *env = ' ';
	  char **newtoken = tokenize (argv[1]);

	  store_variable (argv[0], newtoken[0], "=", newtoken[1]);
	  return 1;
	}
      else
	{
	  printf ("local variables should take the form:variable=value\n");
	  return 1;
	}
    }
  else if (variableindex)
    {
      int found = 0;
      for (int i = 0; envar[i]; i++)
	{
	  if (strcmp (envar[i], argv[variableindex]) == 0)
	    {
	      printf ("%s\n", enval[i]);
	      argv[variableindex] = enval[i];
	      found = 1;
	      return 1;
	    }
	}
      if (!found)
	{
	  for (int j = 0; locvar[j]; j++)
	    {
	      if (strcmp (locvar[j], argv[variableindex]) == 0)
		{
		  printf ("%s\n", locval[j]);
		  argv[variableindex] = locval[j];
		  found = 1;
		  return 1;
		}
	    }
	}
      if (!found)
	{
	  printf ("your variable is not found\n");
	  return 1;
	}
    }
  pid_t pid = fork ();
  if (pid > 0)
    {
      free (input);
      freetokens (argv);
      wait (&status);
      return 1;
    }
  else if (pid == 0)
    {
      execvp (argv[0], argv);
      printf ("error: undefined input \n");
      free (input);
      freetokens (argv);
      exit (1);
      return 2;
    }
  else
    {
      printf ("error\n");
      free (input);
      freetokens (argv);
      return 0;
    }

terminate:{
    freetokens (argv);
    freetokens (locval);
    freetokens (locvar);
    freetokens (enval);
    freetokens (envar);
    free (input);
    printf ("Good bye\n");
    return 0;
  }
}

int
main ()
{
  envar = malloc (envsize * sizeof (char *));
  enval = malloc (envsize * sizeof (char *));
  locvar = malloc (locsize * sizeof (char *));
  locval = malloc (locsize * sizeof (char *));
  enval[0] = NULL;
  envar[0] = NULL;
  locvar[0] = NULL;
  locval[0] = NULL;
  int x = 1;
  while (x)
    {
      x = nanoshell_main ();
    }
  printf ("poula kerolos \n");
  return 0;
}
