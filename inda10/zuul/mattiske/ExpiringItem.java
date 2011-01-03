/**
 * Subklass till item, som har begränsat antal användningar.
 */
public class ExpiringItem extends Item{
    private int uses;
    public ExpiringItem(String name, String description, int uses){
        super(name, description);
        this.uses = uses;
    }
    /**
     * Utökar antalet användningar.
     */
    public void charge(int amount){
         uses += amount;
    }
    /**
     * Använder föremålet och minskar antalet uses om uses > 0, annars säger den "nu är det slut".
     */
    public boolean use(Game game){
        //Om uses var noll kommer --uses bli negativt (innan kollen).
        if(--uses < 0){
            uses = 0;
            System.out.println("It's used up. Maybe you can refill it?");
        }
        else{
            //Ficklampan tänder rummet. När det finns tre användningar kvar säger den till subtilt.
            if(getName().equals("flash light")){
                game.getCurrentRoom().setDark(true);
                if(uses <= 3) System.out.println("The light starts to decay...");
                System.out.println(game.getCurrentRoom().getDescription(game.getTittriktning()));
                return false;
            }
            //Första hjälpen helar alla sår.
            if(getName().equals("first aid kit")){
                game.wound(false);
                System.out.println("The first aid kit heals your wounds");
                if(uses <= 3) System.out.println("The supplies dwindling...");
                return false;
            }
        }
        return false;
    }
}
