import java.util.Scanner;
import java.util.StringTokenizer;

/**
 * This class is part of the "World of Zuul" application. 
 * "World of Zuul" is a very simple, text based adventure game.  
 * 
 * This parser reads user input and tries to interpret it as an "Adventure"
 * command. Every time it is called it reads a line from the terminal and
 * tries to interpret the line as a two word command. It returns the command
 * as an object of class Command.
 *
 * The parser has a set of known command words. It checks user input against
 * the known commands, and if the input is not one of the known commands, it
 * returns a command object that is marked as an unknown command.
 * 
 * @author  Michael Kolling and David J. Barnes
 * @version 2008.03.30
 */
public class Parser 
{
    private static Scanner reader;         // source of command input

    /**
     * Create a parser to read from the terminal window.
     */
    public Parser() 
    {
        reader = new Scanner(System.in);
    }

    /**
     * @return The next command from the user.
     */
    public Command getCommand() 
    {
        String inputLine;   // will hold the full input line
        String word1 = null;
        String word2 = null;

        System.out.print("> ");     // print prompt

        inputLine = reader.nextLine();

        // Find commandword and subject of the command.
        Scanner tokenizer = new Scanner(inputLine);
        if(tokenizer.hasNext()) {
            word1 = tokenizer.next();      // get first word
            //Om man skrivit kommando "pick up":
            if(word1.equals("pick") && tokenizer.hasNext()) word1 += " " + tokenizer.next(); //In case the first word is pick, we get another word in case of "pick up".
            if(tokenizer.hasNext())
                word2 = inputLine.substring(word1.length() + 1);//Jag tar bort commandot från strängen och ytterligare ett tecken(mellanslaget efter) -> subjekt.
        
        }
        return new Command(word1,word2);
    }
    //Kan anropas var som helst ifrån för att få hela strängen. Jag används för att få subjekt till tool och key.
    public static String getWholeLine(){
        return reader.nextLine();
    }
}
