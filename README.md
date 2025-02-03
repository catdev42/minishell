# minishell

How to complete this project quickly:

* Study XV6 shell (sh.c file in xv6)  
at first by watching the first 2 videos here: https://www.youtube.com/@hhp3/search?query=shell
* Find a version of and/or make a great tokenizer (do not make a perfect line, just make a great tokenizer)  
There are a lot of little tricks with "" '' and $vars: "     hello "cat'puppy " is a single token. Quotes go from one to the next, "   "''"" is "" '' then "".
* Basics of signals in forks: make signal handlers with sigaction  
  Disable control C before fork, enable it inside the fork so you can quit the current process if needed without quitting the entire program)  
* Make a function that can process the information you get with waitpid upon exiting a fork(then add to is)
* The heredocs are part of PARSING
