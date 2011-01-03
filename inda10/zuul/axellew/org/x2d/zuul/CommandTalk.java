package org.x2d.zuul;
import java.util.*;
/**
 * Command word talk. This command is used when the player is trying to talk to a character.
 */
public class CommandTalk extends CommandWord {
    public CommandTalk(String commandName) {
        super(commandName);
    }
    
    @Override
    public String[] getTargets(Game game) {
        Collection<Character> rChars = game.getPlayer().getCurrentRoom().getCharacters();
        ArrayList<String> chars = new ArrayList<String>();
        for (Character cha : rChars) {
            chars.add(cha.getName());
        }
        return chars.toArray(new String[0]);
    }
    
    /**
     * @param target The name of the character which the player should try to talk to.
     */
    @Override
    public void executeCommand(Game game, String target) {
        Character cha = game.getPlayer().getCurrentRoom().getCharacter(target);
        if (cha == null) {
            System.out.println(String.format("Det finns ingen med namnet '%s' i det här rummet.", target));
            return;
        }
        System.out.println(String.format("Du börjar prata med %s.", cha.getName()));
        cha.talk(game, game.getPlayer());
    }
    @Override
    public boolean mustHaveTarget() {
        return true;
    }

}
