# pokemon-type-eval

Note (2024): This code was written in around 2018-2019 when I was far less skilled. As a result this code is very sloppy; it solves the problem in a brute force manner, and exhibits poor style. There are almost certainly more efficient methods. 
I would approach the problem differently if I were to do it again. This is not representative of my current skill level.
This code is, at worst, O(n^6) where n is the number of types; however, pokemon types are effectively a constant 18. 18 is a small enough number
That this is acceptably fast. Additionally, since this input is always constant, its realistically O(1) in practice (and if one or two more types were added, it would still be acceptably fast). Additionally, the speed is acceptable since this was only ever intended to run once.
There are multiple directions this could be expanded in the future if I return to it; the offensive type combo with fewest resistances, the defensive combo with the fewest super effectives, a system that accounts for 4x super effectives/0.25 resistances/immunities, and most immunities total, are all angles off the top of my head.
Additionally, this code doesn't handle ties in any way; the best defensive type combo (with 12 resistances + immunities) it outputs is normal/steel, but there are actually 3 that have 12: normal/steel, electric/steel, and ghost/steel. This code will end up just printing whichever it encounters first.

The purpose of this code was to allow me to evaluate the effectiveness of various pokemon type combinations.
In particular, the initial purpose was the combination of 4 types that would hit the most type combos super effectively.
I then also added a mode for the combo of 2 types that would hit the most type combos super effectively (the optimal offensive typing)
Finally, I decided to also add a mode that would output the type combo with the most defenses + immunities.

Input file with the name "typeChart.txt" is of this exact format; line 1 is the number of types, line 2 is the type names in a space seperated format
lines 3 through the end of file are the type matchups for a given type. Any specific line starts with the name of a [attacking] type, followed by a space,
followed by its matchup with every other type. In particular, these matchups are shown in a sequence of chars, with . representing neutral, * representing super effective,
\- representing not very effective, and 0 representing immunity. These character strings have no spaces between characters, and the assumption is that the order is the same as the order
the names were given on line 2; for example, if water is index 2 on line 2, then to see the effectiveness of electric on water, we would go to the line
beginning with electric, then go to the character with index 2 among the effectiveness string (in this case we would see a star, as electric is super effective against water)
