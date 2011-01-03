package org.x2d.zuul;
/**
 * A cat NPC that walks around randomly in the world and
 * really does nothing useful.
 */
public class Cat extends Character {
    private int tCounter1 = 0;
    /**
     * Creates a new cat NPC.
     */
    public Cat() {
        super("Katt", "Mjauu!");
        setWalkRandomly(true);
    }
    
    @Override
    public void talk(Game g, Player p) {
        String[] texts = {
            "Mjau",
            "Nom, nom, nomä!",
            "Mjaaaaaaaau!"
        };
        System.out.println(texts[tCounter1]);
        tCounter1 = (tCounter1 + 1) % texts.length;
        return;

    }
}
