# Qt-Snake

**README**

This is a Snake game created by Qt 5.15.1 MinGW 64-bit.

Before build and run the project, check if font "BankGothic Lt Bt" has been installed on your computer. If not, please install it with the ttf file included in the folder.  Otherwise, the font will not be rendered properly. You should also put the png files under the building directory to make sure the food icons will appear.

**To Play**
After run the program, a start window will show up. You can choose to start a new game or load a game from gameSave.txt in the building directory. You can also customize the map before you start.

To play the game you can hit the start game button and choose a play mode. You can play alone, or with another gamer or the computer. Then you would see the game interface. Use W A S D or UP DOWN LEFT RIGHT keys to move. Each time you enters the game interface, you will have 3 seconds to prepare. And during the preparation time, the game would accept the keyboard input and change the direction of the snake. Since the snake is randomly generated on the map, you shall make full use of the prepare time in order not to hit the rock when the snake starts to run.

**Food Types**
There are 5 types of food, including normal food, add-life food, accelerate food, decelerate food and frozen food, by eating which your snake would add length, or add lives, or speed up, or speed down, or be frozen.

**To Pause**
If you want to pause the game, you can press esc or space. When the game is paused, you can also choose to customize the map, or save the game into a file, or restart the game without changing the map. 

**To customize the map**
When customizing the map, hitting the left mouse key is to add or remove blocks, hitting the right key is to change or remove food. Use esc to quit customizing map.


**ABOUT CODES**

All the interfaces are classes inherited from QWidget, and are children of MainWindow. MainWindow handles all the keyboard input and distribute them to different windows. And it also build communications between the windows (QWidgets), and determines which window to show. The PlayWindow class have a Controller class as a member, which handles the collision of the objects (snakes, blocks, food), the special effect of snakes, the saving and loading of game saves and the communication with interface.

The comments in the codes are already detailed. If you want to know what the function does, you can refer to the header files and see the explanation above. In the cpp files, there are also necessary comments explaining what the code is doing.
