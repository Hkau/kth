
/**
 * Klassen bestämmer vilket typ en tile är av. Den förlänger inte Tile
 * eftersom det inte fanns några fördelar med det. Type innehåller endast
 * namn samt om man kan gå på den typen.
 * 
 * @author Jacob Norlin Andersson
 * @version 2010.12.02
 */
public class Type
{
    // instance variables - replace the example below with your own
    private boolean walkable;
    private String name;

    /**
     * Skapar ett typobjekt, som innehåller information om man kan gå
     * på typen samt vad den heter.
     * 
     * @param walkable Om man kan gå på typen.
     * @param name Namnet på typen.
     */
    public Type(boolean walkable, String name){
        this.walkable = walkable;
        this.name = name;
    }
    
    /**
     * Metod för att komma åt typen namn.
     */
    public String getName(){
        return name;
    }
    
    /**
     * Metod för att veta om man kan gå på typen.
     */
    public boolean isTypeWalkable(){
        return walkable;
    }

}
