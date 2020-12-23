# Qt-Snake
**READ ME**

This is a Snake game created by Qt 5.15.1 MinGW 64-bit.

Before build and run the project, check if font "BankGothic Lt Bt" has been installed on your computer. If not please install it from the ttf file included in the folder.  Otherwise, the font will not rendered properly. You should also put the png files under the building directory to make sure the food icons will appear.

After run the program, a start window will show up. You can choose to start a new game or load a game from gameSave.txt in the building directory. You can also customize the map brefore you start.

To play the game you can hit the start game button and choose a play mode. Then you would see the game interface. Each time you enters the game interface, you will have 3 seconds to prepare. And the game would accept the keyboard input and change the direction of the snake. Since the snake is randomly generated on the map, you shall make full use of the prepare time in order not to hit the rock when the snake starts to run.

If you want to pause the game, you can press esc or space. When the game is paused, you can also choose to customize the map, or save the game into a file, or restart the game without changing the map. To customize the map, hitting the left mouse button is to add or remove blocks, hitting the right is to change or remove food. Use esc to quit customizing map.


**ABOUT CODES**

All the interfaces are classes inherited from QWidget, and are children of MainWindow. MainWindow handles all the keyboard input and distribute them to different windows. And it also build communications between the windows (QWidgets), and determines which window to show. The PlayWindow class have a Controller class as a member, which handles the collision of the objects (snakes, blocks, food), the special effect of snakes, the saving and loading of game saves and the communication with interface.

The comments in the codes are already detailed. If you want to know what the function does, you can refer to the header files and see the explanation above. In the cpp files, there are also necessary comments explaining what the code is doing.
