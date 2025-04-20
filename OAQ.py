s = [10, 5, 5, 5, 5, 5, 10, 5, 5, 5, 5, 5]

playing = True
playerOne = True # always start with player one

messageCode = 0
message = ""

def Board(s):
    print (".---------------------------.")
    print ("|   |%3d|%3d|%3d|%3d|%3d|   |" %(s[1],s[2],s[3],s[4],s[5]))
    print ("|%3d|-------------------|%3d|" %(s[0],s[6]))
    print ("|   |%3d|%3d|%3d|%3d|%3d|   |" %(s[11],s[10],s[9],s[8],s[7]))
    print (".---------------------------.")


while playing:  
    if messageCode == 0: 
        message = "Player One's turn ..." if playerOne else "Player Two's turn ..."
    elif messageCode == -2:
        message = "Invalid input. Try again, Player One" if playerOne else "Invalid input. Try again, Player Two"
    elif messageCode == -1:
        message = "That square is empty, Player One." if playerOne else "That square is empty, Player Two."
    # elif messageCode == 2:
    #     message = "Thanks for playing"
    print()
    print(message)

# Show the game board 
    # print(" ")
    if not playerOne:
        print("       a   b   c   d   e")

    Board(s)

    if playerOne: 
        print("       a   b   c   d   e")

    print(" ")

    # # #
    userInput = input("Enter a letter (a -> e) or enter 'q' to QUIT game: ").lower()

    if userInput == "q":
        # messageCode = 2
        playing = False 
        continue

    # if not playing:
    #     restart = input("Start a new game? (y/n): ").lower()
    #     if restart == "y":
    #         s = [10, 5, 5, 5, 5, 5, 10, 5, 5, 5, 5, 5]
    #         playing = True
    #         playerOne = True 
    #         messageCode = 0 
    #         continue
    #     else:
    #         print("Thanks for playing!")
    #         break
    

    # # Main
    chosenOo = -1 # for checking invalid 

    if playerOne:
        if userInput == "a":
            chosenOo = 11
        elif userInput == "b":
            chosenOo = 10
        elif userInput == "c":
            chosenOo = 9
        elif userInput == "d":
            chosenOo = 8
        elif userInput == "e":
            chosenOo = 7
    else: 
        if userInput == "a":
            chosenOo = 1
        elif userInput == "b":
            chosenOo = 2
        elif userInput == "c":
            chosenOo = 3
        elif userInput == "d":
            chosenOo = 4
        elif userInput == "e":
            chosenOo = 5

    if 0 <= chosenOo < len(s):
        if s[chosenOo] == 0:
            print()
            messageCode = -1 # empty pit
        else:
            giveawayS = s[chosenOo]
            s[chosenOo] = 0 # clear the square into 0
            currentOo = chosenOo

            # # Stone distribution loop
            while True:
                while giveawayS > 0:
                    currentOo += 1
                    if currentOo > 11:
                        currentOo = 0

                    s[currentOo] += 1
                    giveawayS -= 1

                print("\nBoard show:") # board phasw 1
                Board(s)

                if s[currentOo] == 1:
                    print("\nLand on %d. Turn ends." % currentOo)
                    break

                print("\n Land on %d and has %d stone(s). Continue turn." % (currentOo, s[currentOo]))
                giveawayS = s[currentOo]
                s[currentOo] = 0

    # end and switch turn    
    playerOne = not playerOne

else:
    print()
    messageCode = -2 # invalid input
