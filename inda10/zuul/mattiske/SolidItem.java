/**
 * Exakt kopia av item, förutom att den inte är ett item.
 * Varför gör jag detta istället för en boolean i item?
 * Jag vill hålla konstruktorn för item så clean som möjligt eftersom jag hårdkodar
 * alla föremål och det är kladdigt där ändå.
 * Dessutom så kan jag utan problem lägga till fler unika funktioner för Solida föremål.
 * 
 * När jag försöker plocka upp ett item så kollar jag alltså if(item instanceof SolidItem).
 */
public class SolidItem extends Item{
    public SolidItem(String name, String description){
        super(name, description);
    }
}