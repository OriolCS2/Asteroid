Alien Earth is a 2D Platform Game for PC made by Oriol Capdevila and Víctor Segura for the subject of VideoGame Development at CITM University, Terrassa.

It consist of 3 little aliens, every one of them with different powers and strengths that explore the world they are on.

The Team is a young Developer pair formed by Oriol Capdevila and Víctor Segura.

___________________________________________________
TEAM PHOTO

![alt text](https://raw.githubusercontent.com/VictorSegura99/Alien_Earth/master/PlatformGame/Docs/20181212_114736.jpg)

_____________
AUTHORS
- [Víctor Segura Blanco](https://github.com/VictorSegura99)
- [Oriol Capdevila Salinas](https://github.com/OriolCS2)


ACHIVEMENTS by each member:

First Assigment:

- Víctor Segura:
    - Maps Creation in Tiled
    - Maps 1 and 2 Levels Desing
    - Colliders 
    - Art: All Animations and Spritesheets
    - ReadMe and Management 
    - Godmode

- Oriol Capdevila:
    - Colliders Logic
    - XML Logic and Creation
    - Levels Change Logic
    - Parallax
    - Save&Load

Second Assigment:

- Víctor Segura:
    - Camera
    - Bat
    - Dash
    - Menu Sprites and Animations
    - Tutorials
    - Particles
    - Art: All Animations and Spritesheets
    - Framerate Cap
    - Brofile
    - ReadMe and Management 

- Oriol Capdevila:
    - Entity System
    - UML
    - Spider
    - Moving Platform
    - Double Jump
    - Laser
    - Pathfinding
    - Menu Logic
    - FPS & dt Logic

Third Assignemnt:

- Oriol Capdevila:
    - UI Manager and Elements Logic
    - Main Menu
    - Settings Menu
    - Hacks
    - Pause Menu
    - Player UI: Coins Count and Time

- Víctor Segura:
    - Intro Menu
    - Credits Menu
    - All Player UI (Lifes, Coins Count and Time)
    - Coins
    - Animated Intro
    - All Art, Sfx And Music related
    - Website
    - Trailer
    
_____________
SYSTEMS

The Main Core Systems of our Game are:
- Entity Manager: A module that organizes every entity and uses a factory method to create new ones. Every entity inherates from a parent class named Entity. The entities managed managed are by this system are:
	- Bat
	- Spider
	- Player
	- Coins
	- Moving Platform
- UI Manager: A module that organizes every UI element and uses a factory method to create new ones. Every element inherates from a parent class named Ui_Element. The UI elements managed are by this system are:
	- Button
	- Slider
	- Image
	- Label
	- CheckBox

INNOVATIONS:

First Assigment:

- Climb ladders and ropes.
- Swim through water.

Second Assigment:

- Initial Menu+Choose Menu.
- 3 Different Characters with unique habilities.
    - Yellow Alien(Jeff): Double Jump+Bomb Jump(only when previous doble jump).
    - Pink Alien(Jane): Shoots Laser.
    - Blue Alien(Jerry): Dash
- Movement Platforms.

Third Assigment:

- Animation Cinematic Intro
- Menu Hacks and All Hacks Logic
- Animated Transition
- Countdown After Pause
- Direct Download Link on Website
- Enemies Loaded on TMX
- Coins Loaded on TMX

_____________
GAME

- [Repository ALIEN EARTH](https://github.com/VictorSegura99/Alien-Earth)
- [Last Release ALIEN EARTH](https://github.com/VictorSegura99/Alien-Earth/releases)

<a href="https://github.com/VictorSegura99/Alien_Earth/releases/download/1.0/Alien.Earth.1.0.zip" download>
DOWNLOAD THE GAME</a>

_____________
ALIEN EARTH TRAILER
<iframe width="1024" height="768" src="https://www.youtube.com/embed/G1NoxKnEdlY" frameborder="0" allow="autoplay; encrypted-media" 
allowfullscreen> </iframe>

_____________

If you get 15 Coins you get 1 live!

CONTROLS:
- D: Move to the right.
- A: Move to the left.
- SPACE: Jump.
- W: Go Up while Climbing/Swimming.
- S: Go Down while Climbing/Swimming.
- J: Special Character Hability.
- +: Level audio up.
- -: Level audio down.

GODMODE CONTROLS:
- D: Move to the right.
- A: Move to the left.
- W: Fly Up.
- S: Fly Down.

DEBUG KEYS:
- F1: Return to the begining of the first level.
- F2: Start from the begining of the current level.
- F3: Begining of the second level.
- F4: Return to Choose Menu.
- F5: Save.
- F6: Load.
- F9: Show Colliders.
- F10: Godmode.
- F11: 30FPS Cap Activate/Desactivate.
- 1: Change to Jeff.
- 2: Change to Jane.
- 3: Change to Jerry.

_____________
CREDITS

[SPRITES](https://opengameart.org/content/platformer-art-complete-pack-often-updated)

[UI SPRITES](https://opengameart.org/content/pixel-ui-pack-750-assets)

[SPACEBAR SPRITE](https://www.facebook.com/suepeisueba/)

[FONT](https://www.dafont.com/es/sunday-afternoon.font)

SFX:
- [INTRO](https://freesound.org/people/GameAudio/sounds/220210)

- [DEATH](http://soundbible.com/1454-Pain.html)

- [LOSE](https://www.zapsplat.com/music/multimedia-processed-male-voice-says-you-lose)

- [JUMP](http://soundbible.com/1601-Mario-Jumping.html)

- [CHOOSE](https://freesound.org/people/GameAudio/sounds/220182/)

- [OVNI](http://soundbible.com/147-UFO-Landing.html)

- [COIN](https://www.zapsplat.com/music/game-tone-collect-coin-or-treasure-1/)

- [WATER](http://soundbible.com/1148-Bubbling.html)

- [LASER](https://www.zapsplat.com/music/laser-noise-synth-vintage-12/)

- [ENEMY DEATH](https://freesound.org/people/columbia23/sounds/395395/)

- [BOMB JUMP](http://soundbible.com/1414-Batman-Punch.html)

MUSIC:

- [BACKGROUND](https://www.bensound.com/royalty-free-music/track/retro-soul)

- [MENU](https://www.youtube.com/watch?v=XVHVFwwuOa0&ab_channel=RossBugden-Music)

- [TRAILER](https://www.youtube.com/watch?v=eThl2vUnx3g&ab_channel=1-Minute-Music)

____________
LICENSE:

MIT License

Copyright (c) [2018] [Víctor Segura Blanco; Oriol Capdevila Salinas]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
