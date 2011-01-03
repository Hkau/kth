package org.x2d.zuul;
import java.util.*;
import java.io.*;
import java.net.*;
import java.util.regex.*;
/**
 * Command word help. This command word lists all possible command words which can be used at the current time.
 * If another command word is used as target then a list of possible targets for that command word will be displayed.
 */
public class CommandHelp extends CommandWord {
    public CommandHelp(String commandName) {
        super(commandName);
    }
    @Override
    public String[] getTargets(Game game) {
        Collection<CommandWord> commands = game.getParser().getCommandWords();
        String[] tmpArray = new String[commands.size()];
        int i=0;
        for (CommandWord cw : commands) {
            tmpArray[i++] = cw.getCommand();
        }
        return tmpArray;
    }
    
    /**
     * @param target If <code>null</code> then it lists all possible command words which can be used at the current time
     * else if another command word is used as target then a list of possible targets for that command word will be displayed.
     * If the there is a target but there is no command word with that name, then it will try to check wikipedia for more information.
     */
    @Override
    public void executeCommand(Game game, String target) {
        Parser parser = game.getParser();
        if (target==null) {
            System.out.println("Följande saker kan du göra: ");
            Collection<CommandWord> cWordsCollection = parser.getCommandWords();
            for (CommandWord cw : cWordsCollection) {
                if (!cw.mustHaveTarget()||cw.getTargets(game).length!=0) {
                    System.out.print(cw.getCommand()+" ");
                }
            }
        } else if (!parser.isCommand(target)) {
            //If the command word which help is requested for does not exist:
            //search for some information on wikipedia.
            printWikipedia(target);
        } else {
            String[] targets = parser.getCommand(target).getTargets(game);
            if (targets.length==0) {
                System.out.println("Det finns inga möjliga mål för: "+target);
            } else {
                System.out.println(String.format("Möjliga mål för kommandot %s är följande: ", target));
                for (String s : targets) {
                       System.out.print(s+" "); 
                }
            }
        }                  
    }
    
    //Tries to find some information from wikipedia from the search string.
    private void printWikipedia(String search) {
        try {
            URL wiki = new URL("http://sv.wikipedia.org/wiki/"+URLEncoder.encode(search, "UTF-8")); 
            Scanner reader = new Scanner(new InputStreamReader(wiki.openStream()));
            reader.findWithinHorizon(Pattern.compile("<p>(.{0,10})<b>"), 0);
            reader.useDelimiter("(</p>)");
            //Prints two paragraphs and removes html-tags and white-space chars.
            for (int i=0;i<2;i++) {
                System.out.println(reader.next().replaceAll(("<.*?>"), "").replaceAll("((?<=\\s)(\\s+))", "").trim());
            }
            reader.close();
        } catch (Exception e) {
            System.out.println("Inte ens wikipedia förstår vad du vill ha hjälp med.");
        }        
    }

}
