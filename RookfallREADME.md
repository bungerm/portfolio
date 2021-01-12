<p align="center">
  <img src="https://www.rookfall.com/uploads/4/7/3/7/47375157/1478154041.png" width="800">
</p>

## The Story Of Rookfall

Rookfall takes place on an empty chess board in rook purgatory. To start, tap a tile to place your rook, then swipe in a direction to move. Be mindful that your rook travels as far as it can. Traversing a tile will have it fall into the abyss. Drop all tiles to complete the level. Check it out at:
www.rookfall.com

Rookfall began as a prototype during the senior year of my undergraduate degree. I worked with a fellow student, Ronald M. Burgess, to come up with a puzzle game. The goal was a game that would not require many art assets (neither of us were artists), with a truly unique core mechanic, where levels could be generated easily.

### Machinarium's greenhouse puzzles

<p align="center">
  <img src="https://github.com/bungerm/portfolio/blob/main/_images/machinariumPuzzle.jpg" width="800">
</p>

Also during my senior year, I was enjoying a game called Machinarium by Amanita Design. There is a section of the game where you are made to solve puzzles in a greenhouse (seen above). In attempting to solve these puzzles, I felt a vestigial part of my brain quiver under the strain. Ron and I decided this would be an excellent starting point.

### SLIDE, The Prototype

<p align="center">
  <img src="https://github.com/bungerm/portfolio/blob/main/_images/slide0.png" width="800">
  <img src="https://github.com/bungerm/portfolio/blob/main/_images/slide2.png" width="800">
</p>


We tested numerous added mechanics. Among them were bumpers, teleport pads, wormholes, bridges, bombs, double squares, traveling over edges, and NPCs. Ron and I were considering the puzzles be placed on cubes. The player would rotate the cube for one very large and hard to conceive puzzle. After much deliberation, the humble wall stood as an additional mechanic that dynamically added to the gameplay. There are spaces you cannot go, and walls between spaces to prevent that path.

### Puzzle Generator Psuedocode

Now to make hundreds of puzzles. Instead of creating each level by hand, we needed a level generator. First task was to create a level randomizer. Take a blank puzzle board, select a random number of spaces and walls, turn them impassable. To prevent unsolvable levels, check if any open spaces are surrounded by closed spaces/walls on all sides. If so, clear it out. Check if the level already exists. If it doesn't, run it through the solving algorithm.

Perform a recursive postorder traversal on every root (open space) in the puzzle. If the traversal completes the puzzle, increment a "number of solutions" variable, and return the number when finished. If the number of solutions is greater than zero, write it to an XML and you've got yourself a puzzle. We would try to use that number to determine how difficult a puzzle is, but it turns out the number of solutions isn't always indicative of difficulty, and several factors make the level generator less time saving than we thought.

### The Puzzle Editor

<p align="center">
  <img src="https://github.com/bungerm/portfolio/blob/main/_images/Editor2.png" width="800">
</p>

With the ability to crank out puzzles, a puzzle editor was needed. This was by and large my favorite part of this project. With it, we could generate, review, save, delete, play, randomize, modify, solve, and create custom levels. The puzzle generator did not consistently create quality, interesting levels that would make it into the game. Lots of the puzzles had solutions that were too obvious, too numerous, too similar to other puzzles, or with uninteresting layouts. I would create and solve around 1500 levels before the final 300 were chosen. Around 30 of those were entirely custom made.

### What Could Have Been

<p align="center">
  <img src="https://github.com/bungerm/portfolio/blob/main/_images/lightning.png" width="800">
</p>

On to aesthetics. A rook in purgatory did not come until about 3 months after development started as I was lying in bed with my face in the pillow. We went through many (too many) different aesthetics over the course of the project, and began asset creation for several. The most notable was what we called the lightning rod aesthetic. The idea was a top down shot of a mad scientist's rooftop filled with lightning rods. The player would begin the puzzle by striking a rod with lightning, which would crackle and glow with ambient light. Swipe in a direction and the lightning would channel to other nodes.

<p align="center">
  <img src="https://github.com/bungerm/portfolio/blob/main/_images/zomb.png">
  <img src="https://github.com/bungerm/portfolio/blob/main/_images/rod_side.png">
</p>

When all available lightning rods were lit and the puzzle completed, the rooftop would open up to reveal a monster strapped to a table. The monster would be random, mixed and matched. The head of a werewolf and body of a zombie etc. A fellow student Txema Palacio created this lovely art, but had to leave the project due to time constraints. We decided the monster mash was pointless, and the lightning rods were not discernible from a top down angle. Other aesthetics included a Japanese rock garden, a mine field, and a dog scooting its butt on the carpet.

### Beta Versions

<p align="center">
  <img src="https://github.com/bungerm/portfolio/blob/main/_images/OldTitle.png" width="800">
</p>

The original title was Rook's Gambit. Back then the tiles were thicker and the UI was 3D. There were several optimization problems, especially with the level select scene. Loading in the specific XML when the player pressed the puzzle dimension (5x5, 6x6, 7x7) would cause a 1.4 second delay, worse in older phones. To avoid this, I preloaded assets before the title screen, where all 3 XMLs and the music would load unnoticed.

### The Final Product

<p align="center">
  <img src="https://github.com/bungerm/portfolio/blob/main/_images/newTitle.png" width="800">
</p>

Here we have the final product. 2D black and white UI. Tutorial. Credits. Mute music button. Seamless scene transitions. A new progression system. Every 5 completed levels, the appearance of the rook or one of it's tiles changes. We began to sell the paid version in November 2016 at $3, then provided an update and price drop to $1 along with a free version in December 2016.

### Post Mortem

This project cost Ron and I a total of $225 and over 6 months of work. I'll tell you we haven't made that money back... yet. Without money to advertise, an appealing aesthetic, or immediately recognizable gameplay we knew we didn't stand much of a chance in the sea of apps. We relied on review mining, word of mouth, and getting picked up by aggregation sites. We knew going in we had little chance of breaking even, but that wasn't the reason why I did it. I wanted to challenge myself and learn.

Working with one other designer meant we had disagreements. Learning how to compromise was invaluable. Making student projects was one thing, but the act of finalizing all aspects of a game, every small decision, and driving it to completion was a much different task. Having the core gameplay functional is less than 10% of the total work required. Motivation on a large project is critical, and different for every individual. Aligning those motivations may be the key to creating a great game.

## Thank you for reading
