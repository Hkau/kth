import java.util.HashMap;
import java.awt.Point;

/**
 * En tile utg�r en ruta av v�rlden. Den inneh�ller all information
 * som en ruta av spelv�rlden beh�ver. Den ers�tter rum fr�n det
 * ursprungliga systemet som anv�nder i "World of Zuul". En tile 
 * k�nner dock inte till n�rliggande tiles och ist�llet f�r utg�ngar
 * �r en tile "walkable" eller ej.
 * 
 * @author Jacob Norlin Andersson
 * @version 2010.12.02
 */
public class Tile
{
    // instance variables - replace the example below with your own
    private Type type;
    private int x;
    private int y;
    private String Description;
    private boolean walkable;
    private HashMap<String,Point> exits;
    private boolean hasUnitOnIt;
    private boolean hasItem = false;
    private Item item;

    /**
     * Skapa en tile med position typ och beskrivning, samt anger dess
     * position.
     * 
     * @param type Tilens typ
     * @param x Tilens x-koordinat
     * @param y Tiles y-koordinat.
     * @param description Tiles beskrivning. Anv�nds f�r n�rvarande inte direkt.
     */
    public Tile(Type type, int x, int y, String Description){
        this.type = type;    
        this.x = x;
        this.y = y;
        this.Description = Description;
        walkable = type.isTypeWalkable(); //Defaultv�rde, �ndras bara vid specialfall.
        setExits();
        
    }
    public Tile(){}
    
    /**
     * S�tter ett item p� tilen.
     * 
     * @param item Vilket item som ska s�ttas.
     */
    public void setItem(Item item){
        this.item = item;
        hasItem = true;
    }
    
    /**
     * Tar bort tilens item.
     */
    public void removeItem(){
        item = null;
        hasItem = false;
    }
    
    /**
     * Metod f�r att komma �t vilket item som ligger p� tilen.
     */
    public Item getItem(){
        return item;
    }
    
    /**
     * Metod f�r att kolla om tilen inneh�ller ett item.
     */
    public boolean hasItem(){
        return hasItem;
    }
    
    /**
     * Metod f�r att ange tilens beskrivning.
     */
    public void setDescription(String text){
        Description = text;
    }
    
    /**
     * Metod f�r att komma �t vilket typ tilen �r.
     */
    public Type getType(){
        return type;
    }
    
    /**
     * Metod f�r att komma �t namnet p� tilen typ.
     */
    public String getTypeName(){
        return getType().getName();
    }
    
    /**
     * Metod f�r att ta reda p� om man kan g� p� tilen.
     */
    public boolean getWalkable(){
        return walkable;
    }
    
    /**
     * Metod f�r att komma �t vilka kordinater riktningen man vill g� i
     * korresponderar mot.
     * 
     * @param direction Riktingen som angetts. T ex: "east"
     */
    public Point getExitDirection(String direction){
        direction = direction.toLowerCase();
        
        return exits.get(direction);
    }

    /**
     * S�tter de kordinater som varje riktning korresponerar mot.
     */
    public void setExits(){
        Point east = new Point(1,0);
        Point west = new Point(-1,0);
        Point north = new Point(0,-1);
        Point south = new Point(0,1);
        
        exits = new HashMap<String, Point>();
        exits.put("east",east);
        exits.put("north",north);
        exits.put("west",west);
        exits.put("south",south);
    }
    
    /**
     * Metod f�r att komma �t vilka riktningar som man kan g� i.
     */
    public HashMap getExitMap(){
        return exits;
    }
    
    
}
