# 1-2-3

This program simulates a solitaire game, similar to _Frustration_, to calculate the probability of winning. The name "1-2-3" derives from the counting sequence used during gameplay. As for the actual name of this game, I'm not sure.

## How to play

This is a solitaire game played with a standard 52-card deck, where the objective is to avoid drawing cards that match the counting sequence. After shuffling the deck, you draw cards one at a time, counting aloud from "one" to "three" repetitively. For each number, the corresponding card (Ace for "one," 2 for "two," 3 for "three") must not be drawn. If a card matches the count, you lose; if you successfully cycle through the entire deck without a match, you win. It's a straightforward, luck-based game with no strategy involved, offering a quick and engaging way to test your fortune with a deck of cards.

## Background

Many years ago, someone introduced me to this game that has since been lingering in my mind. Every time I'm handed a deck of playing cards, I find myself drawn to playing this game, yet I invariably lose each round. This recurring pattern of losses sparked my curiosity: what are the chances of winning this game? As a developer with more of a knack for coding than mathematics, I was compelled to create a simulator to compute the odds.

## Compile and Run

This Git repo includes a GNU Make file. To download, build, and run it on a Unix system with Git, GNU Make, and GCC, simply run:
```bash
git clone https://github.com/DanielElgh/1-2-3.git
cd 1-2-3
make
./123
```
