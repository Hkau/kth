package org.x2d.zuul;
import java.io.*;
/**
 * Command word save. Saves the current game.
 */
public class CommandSave extends CommandWord {
    private File saveGameDir;
    /**
     * Constructor.
     * @param saveDir Sets the dir where save games should be saved.
     */
    public CommandSave(String commandName, File saveDir) {
        super(commandName);
    	saveGameDir = saveDir;
    }
    public CommandSave(String commandName) {
        this(commandName, new File("saves/"));
    }
    
    @Override
    public String[] getTargets(Game game) {
        return new String[]{"Ett namn du vill använda för att spara ditt spel."};
    }
    
    /**
     * @param target The name of the file which the current game should be saved to.
     */
    @Override
    public void executeCommand(Game game, String target) {
        target = target.replaceAll("([^\\w])", ""); //Makes sure only [a-ö,0-9] are used.
        if (target==null || target == "") {
            System.out.println("Du måste välja ett namn på filen du vill spara.");
            return;
        }
        //If the save game folder does not exist create the needed folders.
        if (!saveGameDir.exists()) {
        	saveGameDir.mkdirs();
        }
        game.saveGame(new File(saveGameDir, target+SaveGameFilter.SAVE_GAME_EXTENSION));
        System.out.println(String.format("Ditt spel har blivit sparat som '%s'.", target));
    }
    @Override
    public boolean mustHaveTarget() {
        return true;
    }
}
