# ASH (Ares SHell) - Unix Shell in C

OS Project for Operating System Course at Cluster Innovation Centre, University of Delhi:

## Language

 We are going with "C" as other familiar language like python has most of the low-level stuff already done and gives less opportunity to learn more about really making a unix shell. Another language option was Nim but it required us to learn Nim and then work on it, which was too much for us.

## Implementation

- Built-in functions:
  - cd
  - help
  - exit
- Check for dotfile
- Run normal application example `>> subl text.txt`

## Organisation

 - ash: shell
 - src/main.c
 - src/builtin.c: declaration of self made functions

## Run
 
```
 $ gcc builtin.c main.c -o ash  #compile code
 $ ./ash  #run the shell
```

## ToDo

- [ ] Using dotfile
- [ ] Add more built-in functions
- [ ] Make better ui. Colors? New Window?
- [x] Add current location to prompt

## Screenshot

![ash (Ares SHell)](https://github.com/duskybomb/Project-shell/blob/master/images/screenshot_11-11.png?raw=true "ash help")


## Members

- Dhairya Kathpalia
- Eklavya Chopra
- Gaurav
- Hardik Kapoor
- Harshit Joshi
