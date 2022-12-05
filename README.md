# AdventOfCode2022

My solutions to this year's Advent of Code

## Day 1

Each elf's inventory is a group of lines. Each group of lines is separated by a blank line.  
On loading lines, add up numbers until you get to a blank line.  
Add this elf's total to a vector.

### Day 1 - Part 1

Find the elf with the highest sum:
While loading data, if this elf's total is greater than the max, replace the max.

### Day 1 - Part 2

Sort the vector of elves' totals, from highest to lowest.  
Add up the first three elements of the vector.

---

## Day 2

## Day 2 - Part 1

Create a map of strings and tuples. Each string represents an occurrance of input ("A X", "A Y", ... "C Y", "C Z").
Every possible occurrance is represented.  
Each tuple is a pair of them-you points, reflecting the outcome of each input.  
While loading input lines, add each point value to a them-total and a you-total.  
Report both totals at end of input.

## Day 2 - Part 2

Same as part 1, but the outcomes map is changed to reflect the different rules.

## Day 3

## Day 3 - Part 1

## Day 3 - Part 2
