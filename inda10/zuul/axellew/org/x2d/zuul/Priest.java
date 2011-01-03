package org.x2d.zuul;
/**
 * The priest NPC that has lost his prayer beads
 * and when the player gives them to the priest
 * he gives player a seal which will let him get into
 * the temple.
 */
public class Priest extends Character {
    private Item seal;
    boolean gotPrayerBeads = false;
    private int tCounter1 = 0;
    private int tCounter2 = 0;
    public Priest() {
        super(Game.generateName(), "Argh!");
    }
    
    @Override public void talk(Game g, Player p) {
        //Replies when the player has given the Priest the prayer beads
        if (gotPrayerBeads) {
            String[] texts = {
                "Gud är stor!",
                "Jag hoppas att du får all välgång!",
            };
            System.out.println(texts[tCounter1]);
            tCounter1 = (tCounter1 + 1) % texts.length;
            return;
        }
        //Some replies to give the player a clue of what the priest want for an item
        if (!p.hasItem(Items.PRAYER_BEADS)) {
            String[] texts = {
                "Jag förstår inte vart mitt böneband har tagit vägen!",
                "Du kanske kan hjälpa mig att hitta den?",
                "Jag vet att jag hade den när jag var och åt.",
                "Jag var inne i lagerrummet sen."
            };
            System.out.println(texts[tCounter1]);
            tCounter1 = (tCounter1 + 1) % texts.length;
        //If the player has the prayer beads give it to the priest and the player gets
        //a seal which will help him get into the temple.
        } else {
            System.out.println("Ah du hittade den!");
            p.removeItem(Items.PRAYER_BEADS);
            System.out.println("Aha, du vill komma ut ur borgen.");
            System.out.println("Jag tror du kan hitta det du letar efter i templet.");
            System.out.println("Här får du ett mitt sigill som du kan visa för vakten så att du kommer in.");
            p.addItem(Items.SEAL);
            gotPrayerBeads = true;
        }
    }
}
