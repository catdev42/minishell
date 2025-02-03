# minishell

How to complete this project quickly:

* Study XV6 shell (sh.c file in xv6)  
at first by watching the first 2 videos here: https://www.youtube.com/@hhp3/search?query=shell
(ast tree, recursive execution, etc)
* Find a version of and/or make a great tokenizer (do not make a perfect line, just make a great tokenizer)  
There are a lot of little tricks with "" '' and $vars: "     hello "cat'puppy " is a single token. Quotes go from one to the next, "   "''"" is "" '' then "".
* Basics of signals in forks: make signal handlers with sigaction  
  Disable control C before fork, enable it inside the fork so you can quit the current process if needed without quitting the entire program)
  Reenable after the fork exits.  
* Make a function that can process the information you get with waitpid upon exiting a fork(then add to it)
* The heredocs are part of PARSING - you can keed an fd open for the duration of a specific commans... and record it...
  if you think of this in the begining. Or incorporate it later somehow as an infile. Could use a union for example with fd OR filename.
* Store everythign in a struct that you pass around all the functions so you can free everything allocated if you receive an error.
* Use an AST Tree - don't waste time trying to decide whether to use an ast tree... they are fun and it doesn't matter if you have a fork launching a fork launching a fork, your check_exit function will do all the work for you on exit. This means the partner doing execution can know that you are using the ast tree... and its shape, and how to deal with it.

This is the basics, there are a lot of little details within, errors, checking permissions for files, looking for paths, etc.
