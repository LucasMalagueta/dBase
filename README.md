# dBase
This project aims to build a dBase like environment in C including its GUI. The main object in doing so is to implement concepts of Data Structure course, so all the operations will be done in memory using dynamic linked lists. 

## Scope
Simulate a system like classic dBase (80's / 90's) manipulating fields, records and data structures files ".DBF" with operations of Create, Read, Update, Delete and more

![SCOPE](assets/Scope.png)

## List of Functionalities

- `SET DEFAULT TO`: Select a logic Drive (`D:` or `C:`)
- `CREATE`: Create a `.DBF` file (in memory)
- `DIR`: Lists the files created in the current unity
- `USE`: Open a file
- `LIST STRUCTURE`: Show all fields of the file structure 
- `APPEND`: Append new records to the `.DBF` file 
- `LIST`: Lists the records with aditional filters
- `LOCATE`: Search for records based on conditions
- `GOTO`, `DISPLAY`, `EDIT`: Navigate and edit records
- `DELETE`, `RECALL`, `PACK`, `ZAP`: Delete methods
- `CLEAR`, `QUIT`, `SET DELETED ON/OFF`: Aux commands
- `MODIFY STRUCTURE` and `SORT`: Extra


## GUI

The interface is done with a modified version of TDM-GCC and [conio2.h](https://conio.sourceforge.net/) the dowload can be found [here](https://github.com/mateus-sm/Conio2-VsCode).

![GUI](assets/image.png)

## Preview

### SET DEFAULT TO
![SET DEFAULT TO](assets/SET.gif)

### CREATE
![CREATE](assets/CREATE.gif)

### DIR
![DIR](assets/DIR.gif)

### USE and LIST STRUCTURE
![USE](assets/USE.gif)

### APPEND
![APPEND](assets/APPEND.gif)

### LIST and CLEAR
![LIST](assets/LIST.gif)

### LOCATE, GOTO and DISPLAY
![LOCATEGOTODISPLAY](assets/LOCATEGOTODISPLAY.gif)

## Credit

Made by [**Mateus SM**](https://github.com/mateus-sm) and [**Lucas Ataide Malagueta**](https://github.com/LucasMalagueta).
2025

---
