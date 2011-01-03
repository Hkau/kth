package org.x2d.zuul;
/**
 * A beggar NPC. He wants food and will give the player a key to the south door
 * when he gets the food.
 */
public class Beggar extends Character {
    private boolean gotFood = false;
    private int tCounter1 = 0;
    private int tCounter2 = 0;
    /**
     * Creates a new beggar NPC.
     */
    public Beggar() {
        super(Game.generateName(), "Kan du skänka mat till en hungrig stackare?");
    }
    
    @Override
    public void talk(Game g, Player p) {
        //When the player has given him food
        if (gotFood) {
            String[] texts = {
                "Underbart med mat!",
                "Nom, nom, nom!",
            };
            System.out.println(texts[tCounter1]);
            tCounter1 = (tCounter1 + 1) % texts.length;
            return;
        }
        //If the player has no food reply some text
        if (!p.hasItem(Items.FOOD)) {
            String[] texts = {
                "Kan du skänka mat till en hungrig stackare?",
                "Jag är så hungrig.",
                "Snälla kan jag få lite mat?",
                "Ingen tänker på stackars lilla mig."
            };
            System.out.println(texts[tCounter1]);
            tCounter1 = (tCounter1 + 1) % texts.length;
        //If the player has food give him the key and remove the food
        } else {
            System.out.println("Å, mat till mig? Jag har inte ätit på en vecka!");
            System.out.println("Jag ska hjälpa dig att öppna den södra dörren.");
            System.out.println("Här får du nyckeln, jag hitta den på marken här utanför.");
            p.removeItem(Items.FOOD);
            p.addItem(Items.STORE_ROOM_KEY);
            gotFood = true;
        }
    }
}
