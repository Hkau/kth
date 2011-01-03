package org.x2d.zuul;
/**
 * A guard that will not the let the player enter the temple
 * till he sees a paper with a seal from the priest.
 */
public class TempleGuard extends Character {
    public TempleGuard() {
        super(Game.generateName(), "");
        setFirstTime(false);
    }
    
    public void talk(Game g, Player p) {
        //If the door is open.
        if (!g.getRoom(Game.Rooms.GARDEN).getDoor(Game.Directions.WEST.getValue()).isLocked()) {
            String[] texts = {
                "Det är bara att gå in i templet.",
            };
            System.out.println(texts[0]);
            return;
        }
        //If the door is locked and the player doesn't have the seal
        if (!p.hasItem(Items.SEAL)) {
            String[] texts = {
                "Här kommer du inte förbi!",
                "Jag släpper inte in något utan tillstånd."
            };
            System.out.println(texts[(int)(Math.random()*texts.length)]);
        //If the door is locked and the player has the seal 
        //the guard unlocks the 
        } else {
            System.out.println("Här kommer du...");
            System.out.println("Vad är det där för papper?");
            System.out.println("Jaha, ja då kommer du in, jag öppnar dörren.");
            p.removeItem(Items.SEAL);
            g.getRoom(Game.Rooms.GARDEN).getDoor("väster").setLocked(false);
            System.out.println("Den västra dörren är nu upplåst.");
        }
    }
}
