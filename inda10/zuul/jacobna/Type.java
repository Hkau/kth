
/**
 * Klassen best�mmer vilket typ en tile �r av. Den f�rl�nger inte Tile
 * eftersom det inte fanns n�gra f�rdelar med det. Type inneh�ller endast
 * namn samt om man kan g� p� den typen.
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
     * Skapar ett typobjekt, som inneh�ller information om man kan g�
     * p� typen samt vad den heter.
     * 
     * @param walkable Om man kan g� p� typen.
     * @param name Namnet p� typen.
     */
    public Type(boolean walkable, String name){
        this.walkable = walkable;
        this.name = name;
    }
    
    /**
     * Metod f�r att komma �t typen namn.
     */
    public String getName(){
        return name;
    }
    
    /**
     * Metod f�r att veta om man kan g� p� typen.
     */
    public boolean isTypeWalkable(){
        return walkable;
    }

}
