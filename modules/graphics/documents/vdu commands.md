34 VDU drivers
==============

The statement **VDU X** is equivalent to **PRINT CHR$(X);** and the statement **VDU X,Y,Z** is equivalent to

PRINT CHR$(X); CHR$(Y); CHR$(Z);.

However the **VDU** statement finds most common use when generating ASCII control codes and a detailed description of the effect of each control code is given in this chapter. The control codes are interpreted by part of the Machine Operating System called the VDU driver.

Those writing BASIC programs will need to refer to this summary of the VDU drivers if they wish to use some of the more advanced facilities such as definition of graphic and text windows. Those writing other high level languages or machine code programs will also need to refer to this section.

The VDU drivers are part of the "Machine Operating System" (MOS) software. All high level languages (including BASIC) use them to print and draw on the screen. Because they are so extensive and easily accessible to programmers it will be easy to ensure that all high level languages and smaller assembly language programs have access to the same graphics facilities. There is no need for the user to write special routines to handle the screen display.

The BBC Microcomputer is designed so that it can be expanded in many ways. All expansions will be compatible with the current Machine Operating System and it is _very_ important that those writing software use the facilities provided. In a "twin-processor" machine the only access to the screen memory is via the "Tube" and use of these VDU drivers and other Machine Operating System features will ensure that code will work correctly whether executed in the Input/Output processor or in the Language processor.

The VDU drivers interpret all 32 ASCII control character codes. Many of the ASCII control codes are followed by a number of bytes. The number of bytes which follow depends on the function to be performed. The table opposite summarises all the codes and gives the number of bytes which follow the ASCII control code.

VDU code summary
----------------

| **Decimal** | **Hex** | **CTRL** | **ASCII abbrev.** | **Bytes extra** | **Meaning**                                |
| :---------: | :-----: | :------: | :---------------: | :-------------: | ------------------------------------------ |
|      0      |    0    |    @     |        NUL        |        0        | does nothing                               |
|      1      |    1    |    A     |        SOH        |        1        | send next character to printer only        |
|      2      |    2    |    B     |        STX        |        0        | enable printer                             |
|      3      |    3    |    C     |        ETX        |        0        | disable printer                            |
|      4      |    4    |    D     |        EOT        |        0        | write text at text cursor                  |
|      5      |    5    |    E     |        ENQ        |        0        | write text at graphics cursor              |
|      6      |    6    |    F     |        ACK        |        0        | enable VDU drivers                         |
|      7      |    7    |    G     |        BEL        |        0        | make a short beep                          |
|      8      |    8    |    H     |        BS         |        0        | backspace cursor one character             |
|      9      |    9    |    I     |        HT         |        0        | forwardspace cursor one character          |
|     10      |    A    |    J     |        LF         |        0        | move cursor down one line                  |
|     11      |    B    |    K     |        VT         |        0        | move cursor up one line                    |
|     12      |    C    |    L     |        FF         |        0        | clear text area                            |
|     13      |    D    |    M     |        CR         |        0        | move cursor to start of current line       |
|     14      |    E    |    N     |        SO         |        0        | page mode on                               |
|     15      |    F    |    O     |        SI         |        0        | page mode off                              |
|     16      |   10    |    P     |        DLE        |        0        | clear graphics area                        |
|     17      |   11    |    Q     |        DC1        |        1        | define text colour                         |
|     18      |   12    |    R     |        DC2        |        2        | define graphics colour                     |
|     19      |   13    |    S     |        DC3        |        5        | define logical colour                      |
|     20      |   14    |    T     |        DC4        |        0        | restore default logical colours            |
|     21      |   15    |    U     |        NAK        |        0        | disable VDU drivers or delete current line |
|     22      |   16    |    V     |        SYN        |        1        | select screen mode                         |
|     23      |   17    |    W     |        ETB        |        9        | re-program display character               |
|     24      |   18    |    X     |        CAN        |        8        | define graphics window                     |
|     25      |   19    |    Y     |        EM         |        5        | **PLOT K,x,y**                             |
|     26      |   1A    |    Z     |        SUB        |        0        | restore default windows                    |
|     27      |   1B    |    \[    |        ESC        |        0        | does nothing                               |
|     28      |   1C    |    \     |        FS         |        4        | define text window                         |
|     29      |   1D    |    \]    |        GS         |        4        | define graphics origin                     |
|     30      |   1E    |    ^     |        RS         |        0        | home text cursor to top left               |
|     31      |   1F    |    _     |        US         |        2        | move text cursor to x,y                    |
|     127     |   7F    |  _none_  |        DEL        |        0        | Backspace and delete                       |

Detailed description
--------------------

The ASCII control codes work as follows:

0.  This code is ignored
1.  This code causes the next character to be sent to the printer only and not to the screen. The printer must already have been enabled with **VDU 2**. Many printers use special control characters to change, for example, the size of the printed output. For example the Epson MX-80 requires a code 14 to place it into double width print mode. This could be effected with the statement
    
    VDU1,14
    
    or by pressing **CTRL A** and then **CTRL N**. This code also enables the "printer ignore" character selected by ***FX 6** to be sent to the printer.
    
2.  This code "turns the printer on" by which is meant that all output to the screen will also be sent to the printer. In a program the statement **VDU 2** should be used, but the same effect can be obtained by typing **CTRL B**.
3.  This code "turns the printer off". No further output will be sent to the printer after the statement **VDU 3** or after typing **CTRL C**.
4.  This code causes text to be written at the text cursor ie in the normal fashion. A MODE change selects **VDU 4**, normal operation.
5.  This code causes text to be written where the graphics cursor is. The position of the text cursor is unaffected. Normally the text cursor is controlled with statements such as
    
    PRINT TAB(5,10);
    
    and the graphics cursor is controlled with statements like
    
    MOVE700,450
    
    Once the statement **VDU 5** has been given only one cursor is active (the graphics cursor). This enables text characters to be placed at any position on the screen. There are a number of other effects: Text characters overwrite what is already on the screen so that characters can be superimposed; text and graphics can only be written in the graphics window and the colours used for both text and graphics are the graphics colours. In addition the page no longer scrolls up when at the bottom of the page. Note however that **POS** and **VPOS** still give you the position of the text cursor. See page 172 for more information.
    
6.  **VDU 6** is a complementary code to **VDU 21**. **VDU 21** stops any further characters being printed on the screen and **VDU 6** re�enables screen output. A typical use for this facility would be to prevent a pass-word appearing on the screen as it is being typed in.
7.  This code, which can be entered in a program as **VDU 7** or directly from the keyboard as **CTRL G**, causes the computer to make a short "beep". This code is not normally passed to the printer.
8.  This code (**VDU 8** or **CTRL H**) moves the text cursor one space to the left. If the cursor was at the start of a line then it will be moved to the end of the previous line. It does not delete characters - unlike **VDU l27**.
9.  This code (**VDU 9** or **CTRL I** or **TAB**) moves the cursor forward one character position.
10. The statement (**VDU 10** or **CTRL J**) will move the cursor down one line. If the cursor is already on the bottom line then the whole display will normally be moved up one line.
11. This code (**VDU l1** or **CTRL K**) moves the text cursor up one line. If the cursor is at the top of the screen then the whole display will move down a line.
12. This code clears the screen - or at least the text area of the screen. The screen is cleared to the "Text background colour" which is normally black. The BASIC statement **CLS** has exactly the same effect as **VDU 12**, or **CTRL L**. This code also moves the text cursor to the top left of the text window.
13. This code is produced by the **RETURN** key. However its effect on the screen display if issued as a **VDU 13** or **PRINT CHR$(l3);** is to move the text cursor to the left hand edge of the current text line (but within the current text window, of course.)
14. This code makes the screen display wait at the bottom of each page. It is mainly used when listing long programs to prevent the listing going past so fast that it is impossible to read. The computer will wait until a **SHIFT** key is pressed before continuing. This mode is called "Paged mode". Paged mode is turned on with the **CTRL N** and off with **CTRL O**. When the computer is waiting at the bottom of a page both the _Shift lock_ and _Caps lock_ lights will be illuminated.
15. This code causes the computer to leave paged mode. See the previous entry (14) for more details.
16. This code (**VDU 16** or **CTRL P**) clears the graphics area of the screen to the graphics background colour and the BASIC statement **CLG** has exactly the same effect. The graphics background colour starts off as black but many have been changed with the **GCOL** statement. **VDU 16** does not move the graphics cursor - it just clears the graphics area of the screen.
17. **VDU 17** is used to change the text foreground and background colours. In BASIC the statement **COLOUR** is used for an identical purpose. **VDU 17** is followed by one number which determines the new colour. See the BASIC keyword **COLOUR** on page 222 for more details.
18. This code allows the definition of the graphics foreground and background colours. It also specifies how the colour is to be placed on the screen. The colour can be plotted directly, ANDed, ORed or Exclusive-ORed with the colour already there, or the colour there can be inverted. In BASIC this is called **GCOL**.
    
    The first byte specifies the mode of action as follows:
    
    | Code | Action                                                    |
    | :--: | --------------------------------------------------------- |
    |  0   | Plot the colour specified                                 |
    |  1   | OR the specified colour with that already there           |
    |  2   | AND the specified colour with that already there          |
    |  3   | Exclusive-OR the specified colour with that already there |
    |  4   | Invert the colour already there                           |
    
    The second byte defines the logical colour to be used in future. If the byte is greater than 127 then it defines the graphics background colour (modulo the number of colours available). If the byte is less than 128 then it defines the graphics foreground colour (modulo the number of colours available).
    
19. This code is used to select the actual colour that is to be displayed for each logical colour. The statements **COLOUR** (and **GCOL**) are used to select the logical colour that is to be used for text (and graphics) in the immediate future. However the actual colour can be re-defined with **VDU 19**. For example
    
    MODE 5
    
    COLOUR 1
    
    will print all text in colour 1 which is red by default. However the addition of
    
    VDU 19,l,4,0,0,0 or VDU 19,l,4;0;
    
    will set logical colour 1 to actual colour 4 (blue). The 3 zeros after the actual colour in the **VDU 19** statement are for future expansion.
    
    In MODE 5 there are four colours (0,1,2 and 3). An attempt to set colour 4 will in fact set colour 0 so the statement
    
    VDU 19,4,4,0,0,0 or VDU 19,4,4;0;
    
    is equivalent to
    
    VDU 19,0,4,0,0,0 or VDU 19,0,4;0;
    
    we say that logical colours are reduced modulo the number of colours available in any particular mode.
    
    _Note:_ In the television series 'The Computer Programme', an attractive way of listing programs was produced by using **MODE 6** and **VDU 19,0,4,0,0,0**.
    
20. This code **VDU 20** or **CTRL T** sets default text and graphic foreground logical colours and also programs default logical to actual colour relationships. The default values are:
    
    **Two colour modes**
    
    0 =black
    
    1=white
    
    **Four colour modes**
    
    0=black
    
    1=red
    
    2= yellow
    
    3= white
    
    **Sixteen colour modes**
    
    0=black
    
    1=red
    
    2=green
    
    3=yellow
    
    4=blue
    
    5= magenta
    
    6=cyan
    
    7=white
    
    8=flashing black-white
    
    9=flashing red-cyan
    
    10=flashing green-magenta
    
    11 =flashing yellow-blue
    
    12= flashing blue-yellow
    
    13=flashing magenta-green
    
    14= flashing cyan-red
    
    15=flashing white-black
    
21. This code behaves in two different ways. If entered at the keyboard (as **CTRL U**) it can be used to delete the whole of the current line. It is used instead of pressing the **DELETE** key many times. If the code is generated from within a program by either **VDU21** or **PRINT CHR$(21);** it has the effect of stopping all further graphics or text output to the screen. The VDU is said to be disabled. It can be 'enabled' with **VDU 6**.
22. This VDU code is used to change MODE. It is followed by one number which is the new mode. Thus **VDU 22,7** is exactly equivalent to **MODE 7** (except that it does not change **HIMEM**, see pages 270 and 301).
23. This code is used to re-program displayed characters. The ASCII code assigns code numbers for each displayed letter and number. The normal range of displayed characters includes all upper and lower case letters, numbers and punctuation marks as well as some special symbols. These characters occupy ASCII codes 32 to 126. If the user wishes to define his or her own characters or shapes then ASCII codes 224 to 255 are left available for this purpose. In fact you can re-define _any_ character that is displayed, but extra memory must be set aside if this is done. See page 427.
    
    ASCII codes 0 to 31 are interpreted as VDU control codes - and this chapter is explaining the exact function of each. Thus the full ASCII set consists of all the VDU control codes, all the normal printable characters and a user defined set of characters.
    
24. This code enables the user to define the graphics window - that is, the area of the screen inside which graphics can be drawn with the **DRAW** and **PLOT** statements. The graphics screen is addressed with the following co-ordinates.
    
    Thus the co-ordinates of A would be approximately 1000,200.
    
    When defining a graphics window four co-ordinates must be given; the left, bottom, right and top edges of the graphics area. Suppose that we wish to confine all graphics to the area shown below.
    
    The left hand edge of the graphics area has an X value of (about) 150. The bottom of the area has a Y value of 300. The right hand side has X=1100 and the top has Y=700. The full statement to set this area is
    
    VDU 24,150;300;1100;700;
    
    Notice that the edges must be given in the order left X, bottom Y, right X, top Y and that when defining graphics windows the numbers must be followed by a semi-colon.
    
    For those who wish to know why trailing semi-colons are used the reason is as follows: X and Y graphic co-ordinates have to be sent to the VDU software as two bytes since the values may well by greater than 255. The semi-colon punctuation in the VDU statement sends the number as a two byte pair with low byte first followed by the high byte.
    
25. This VDU code is identical to the BASIC **PLOT** statement. Only those writing machine code graphics will need to use it. **VDU 25** is followed by 5 bytes. The first gives the value of K referred to on page 319 of the explanation of **PLOT** in the BASIC keywords chapter. The next two bytes give the X co-ordinate and the last two bytes give the Y co-ordinate. Refer to the entry for **VDU 24** for an explanation of the semi-colon syntax used. Thus
    
    VDU 25,4,100;500;
    
    would move to absolute position 100,500.
    
    The above is completely equivalent to
    
    VDU 25,4,100,0,244,1
    
26. The code **VDU 26** (**CTRL Z**) returns both the graphics and text windows to their initial values where they occupy the whole screen. This code re-positions the text cursor at the top left of the screen, the graphics cursor at the bottom left and sets the graphics origin to the bottom left of the screen. In this state it is possible to write text and to draw graphics anywhere on the screen.
27. This code does nothing.
28. This code (**VDU 28**) is used to set a text window. Initially it is possible to write text anywhere on the screen but establishing a text window enables the user to restrict all future text to a specific area of the screen. The format of the statement is
    
    VDU 28,leftX,bottomY,rightX,topY
    
    | Parameter | Meaning                               |
    | :-------: | ------------------------------------- |
    |   leftX   | sets the left hand edge of the window |
    |  bottomY  | sets the bottom edge                  |
    |  rightX   | sets the right hand edge              |
    |   topY    | sets the top edge                     |
    
    For the example shown the statement would be
    
    VDU 28,5,20,30,12
    
    Note that the units are character positions and the maximum
    
    values will depend on the mode in use. The example above refers to **MODE 1** and **MODE 4**. In MODES 2 and 5 the maximum values would be 19 for X and 31 for Y.
    
    since these modes have only 20 characters per line
    
29. This code is used to move the graphics origin. The statement **VDU 29** is followed by two numbers giving the X and Y co-ordinates of the new origin.
    
    To move the origin to the centre of the screen the statement
    
    VDU 29,640;512;
    
    should be executed. Note that the X and Y values should be followed by semi-colons. See the entry for **VDU 24** if you require an explanation of the trailing semi-colons. Note also that the graphics cursor is not affected by **VDU 29**.
    
30. This code (**VDU 30** or **CTRL ^**) moves the text cursor to the top left of the text area.
31. The code **VDU 31** enables the text cursor to be moved to any character position on the screen. The statement **VDU 31** is followed by two numbers which give the X and Y co-ordinates of the desired position.
    
    Thus to move the text cursor to the centre of the screen in MODE 7 one would execute the statement
    
    VDU 31,20,12
    
    Note that the maximum values of X and Y depend on the mode selected and that both X and Y are measured from the edges of the current text window not the edges of the screen.
    

The remaining ASCII characters work as follows:

**32-126** These codes generate the full set of letters and numbers in the ASCII set. See the ASCII codes on pages 486 to 492. **127** This code moves the text cursor back one character and deletes the character at that position. **VDU 127** has exactly the same effect as the **DELETE** key.

**128-223** These characters are normally undefined and will produce random shapes (see pages 384 and 427).

**224-255** These characters may be defined by the user using the statement **VDU 23**. It is thus possible to have 32 user defined shapes such as

- VDU 23,224,8,28,28,107,127,107,8,28

- VDU 23,225,8,28,62,127,62,28,8,0 0

- VDU 23,226,54,127,127,127,62,28,8,0

- VDU 23,227,8,28,62,127,127,127,28,62


* * *

[The BBC Microcomputer User Guide](https://central.kaserver5.org/Kasoft/Typeset/BBC/Intro.html) was written by John Coll and edited by David Allen for the British Broadcasting Corporation.

Optical character recognition and original formatting effort by Mark Usher.

HTML version maintained by: Kade "Archer" Hansson; [e-mail: archer@dialix.com.au](mailto:archer@dialix.com.au)

Last updated: Monday 12th February 2001