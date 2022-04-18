# Flush

Group 199's implementation of __Famous little unix shell__. 
Flush is a basic shell written in C to be run on UNIX machines. 

## How-to

Build from source by running `make` (requires gcc).
Then run the binary `flush.o`.

## Functionality

### Running programs by absolute path or PATH search
```
These lines exhibit the same behaviour

$ /usr/bin/ls

$ ls 
```
### Change working directory
```
/usr/ $ cd bin

/usr/bin/ $
```
### Exit the shell
```
These lines exhibit the same behaviour

$ exit

$ ^d
```
### IO-redirection
```
Write output to foo

$ ls > foo

Read from foo and write result to bar

$ wc -l < foo > bar

!! Multiple input, or output redirects not supported !!

$ ls > foo > bar     <- Illegal syntax
```
### Background jobs
```
$ sleep 1 &        
> [sleep 1] - pid     -> sleep 1 now running in background with process id = pid

Run 'jobs' to see current background processes

$ jobs
> Background jobs:
> [sleep 1] - pid
```
### Piping
```
$ ls | grep .o | wc -l
> 1

Commands with pipes can be backgrounded
```
