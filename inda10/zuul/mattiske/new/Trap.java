/**
 * Exakt kopia av item, förutom att den inte är ett item.
 * Varför gör jag detta istället för en boolean i item?
 * Jag vill hålla konstruktorn för item så clean som möjligt eftersom jag hårdkodar
 * alla föremål och det är kladdigt där ändå.
 * Dessutom så kan jag utan problem lägga till fler unika funktioner för fällor föremål.
 * 
 * När jag letar igenom mörka rum kollar jag alltså if(item instanceof Trap).
 */
public class Trap extends Item{
    public Trap(String name, String description){
        super(name, description);
    }
}