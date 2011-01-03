package org.x2d.zuul;
import java.io.*;
/**
 * Command word load. Loads a previously saved game. It tries to load the file from the 'saves' folder in the game folder.
 */
public class CommandLoad extends CommandWord {
    private File saveGameDir;
    /**
     * Constructor.
     * @param saveDir Sets the dir where save games should be saved.
     */
    public CommandLoad(String commandName, File saveDir) {
        super(commandName);
    	saveGameDir = saveDir;
    }
    public CommandLoad(String commandName) {
    	this(commandName, new File("saves/"));
    }

    @Override
    public String[] getTargets(Game game) {
        if (!saveGameDir.exists()) {
        	return null;
        }
        //Lists all files in 'saves/' ending with '.zul'
        File[] files = (new File("saves/")).listFiles(new SaveGameFilter());
        String[] fileNames = new String[files.length];
        for (int i=0;i<files.length;i++) {
            String fileName = files[i].getName();
            fileNames[i] = fileName.substring(0, fileName.length()-4);
        }
        return fileNames;
    }
    
    /**
     * @param target The name of the file which should be loaded.
     */
    @Override
    public void executeCommand(Game game, String target) {
        if (target==null || target == "") {
            System.out.println("Du måste skriva in namnet på filen du vill ladda.");
            return;
        }
        target = target.replaceAll("([^\\w])", ""); //Makes sure only [a-ö,0-9] are used.
        File f = new File("saves/"+target+SaveGameFilter.SAVE_GAME_EXTENSION);
        if (!f.exists()) {
            System.out.println("Filen du försöker ladda finns inte: "+ f);
        }
        game.loadGame(f);
        System.out.println("Ditt spel har blivit laddat.");
        System.out.println(game.getPlayer().getCurrentRoom().getLongDescription());
    }
    @Override
    public boolean mustHaveTarget() {
        return true;
    }

}
