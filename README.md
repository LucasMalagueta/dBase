# dBase
This project aims to build a dBase like environment in C including its GUI. The main object in doing so is to implement concepts of Data Structure course, so all the operations will be done in memory using dynamic linked lists. 

## Scope
Simulate a system like classic dBase (80's / 90's) manipulating registers e data structures files ".DBF" with operations of Create, Read, Update, Delete and more

## List of Functionalities

- `SET DEFAULT TO`: Select a logic unity (`D:` or `C:`)
- `CREATE`: Create a `.DBF` file (in memory)
- `DIR`: Lists the files created in the current unity
- `USE`: Open a file
- `LIST STRUCTURE`: Show all fields of the file structure 
- `APPEND`: Append new records to the `.DBF` file 
- `LIST`: Lists the records with aditional filtersos opcionais
- `LOCATE`: Search for records based on conditions
- `GOTO`, `DISPLAY`, `EDIT`: Navigate and edit records
- `DELETE`, `RECALL`, `PACK`, `ZAP`: Delete methods
- `CLEAR`, `QUIT`, `SET DELETED ON/OFF`: Aux commands
- `MODIFY STRUCTURE` and `SORT`: Extra


## GUI

The interface is done with a modified version of TDM-GCC and [conio2.h](https://conio.sourceforge.net/) the dowload can be found [here](https://github.com/mateus-sm/Conio2-VsCode).

![GUI](assets/image.png)

## Credit

Made by [**Mateus SM**](https://github.com/mateus-sm) and [**Lucas Ataide Malagueta**](https://github.com/LucasMalagueta).
2025

---
