# Simple ascii rain demo

# Demo
![rain demo](https://github.com/x2DA/rainc/blob/main/assets/demo.gif)

# Usage
## To run
Simply run the executable:
```sh
./rain
```

## To stop
Press the `space` key (defined by HALTKEY after inludes).

# How to build
## Dependencies
`curses`

On debian / ubuntu / etc: `libncurses-dev` \
On alpine: `ncurses-dev` \
On arch: `ncurses` \

## Compiling
```sh
gcc -o rain rain.c -lncurses
```

