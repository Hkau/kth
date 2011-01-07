import java.util.HashMap;
import java.awt.Point;

/**
 * En tile utgör en ruta av världen. Den innehåller all information
 * som en ruta av spelvärlden behöver. Den ersätter rum från det
 * ursprungliga systemet som använder i "World of Zuul". En tile 
 * känner dock inte till närliggande tiles och istället för utgångar
 * är en tile "walkable" eller ej.
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
     * @param description Tiles beskrivning. Används för närvarande inte direkt.
     */
    public Tile(Type type, int x, int y, String Description){
        this.type = type;    
        this.x = x;
        this.y = y;
        this.Description = Description;
        walkable = type.isTypeWalkable(); //Defaultvärde, ändras bara vid specialfall.
        setExits();
        
    }
    public Tile(){}
    
    /**
     * Sätter ett item på tilen.
     * 
     * @param item Vilket item som ska sättas.
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
     * Metod för att komma åt vilket item som ligger på tilen.
     */
    public Item getItem(){
        return item;
    }
    
    /**
     * Metod för att kolla om tilen innehåller ett item.
     */
    public boolean hasItem(){
        return hasItem;
    }
    
    /**
     * Metod för att ange tilens beskrivning.
     */
    public void setDescription(String text){
        Description = text;
    }
    
    /**
     * Metod för att komma åt vilket typ tilen är.
     */
    public Type getType(){
        return type;
    }
    
    /**
     * Metod för att komma åt namnet på tilen typ.
     */
    public String getTypeName(){
        return getType().getName();
    }
    
    /**
     * Metod för att ta reda på om man kan gå på tilen.
     */
    public boolean getWalkable(){
        return walkable;
    }
    
    /**
     * Metod för att komma åt vilka kordinater riktningen man vill gå i
     * korresponderar mot.
     * 
     * @param direction Riktingen som angetts. T ex: "east"
     */
    public Point getExitDirection(String direction){
        direction = direction.toLowerCase();
        
        return exits.get(direction);
    }

    /**
     * Sätter de kordinater som varje riktning korresponerar mot.
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
     * Metod för att komma åt vilka riktningar som man kan gå i.
     */
    public HashMap getExitMap(){
        return exits;
    }
    
    
}
