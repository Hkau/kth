import java.util.ArrayList;
import java.util.HashMap;
import java.awt.Point;
/**
 * Actor.
 * 
 * Spelarklassen. Den är en del av "World of Zuul".
 * 
 * Klassen innehåller allt som tillhör spelaren. Just nu innehåller
 * den: Namn, position samt inventarielista. När klassen skapas kommer
 * dessa variabler att initieras.
 * 
 * @author Jacob Norlin Andersson 
 * @version 2010.12.02
 */
public class Actor
{
    // instance variables - replace the example below with your own
    private String name;
    private Point position;
    private ArrayList<Item> inventory;
    /**
     * Skapar en spelare på de angivna x och y kordinaterna. Spelaren får det
     * angivna namnet.
     */
    public Actor(String name, int startX,int startY)
    {
        position = new Point(startX,startY);
        this.name = name;
        inventory = new ArrayList<Item>();

    }
    

    
    /**
     * Flyttar på spelaren till angiven x och y koordinat.
     * @param x En integer som anger x värdet.
     * @param y En integer som anger y värdet.
     */
    public void moveActor(int x, int y){
        position.setLocation(x,y);
    }
    
    /**
     * Flyttar på spelaren till angiven Point.
     * @param newPos Den point som spelaren kommer flyttas till.
     */
    public void moveActorPoint(Point newPos){
        position.setLocation(newPos);
        //System.out.println(getX()+" "+getY());
    }
     /**
     * Metod för att komma åt name.
     * @return Objektets name-variabel.
     */
    public String getName(){
        return name;
    }
    
    /**
     * Metod för att komma åt spelarens x-koordinat.
     * @return Objektets x-koordinat.
     */
    public int getX(){
        return (int)position.getX();
    }
    
    /**
     * Metod för att komma åt spelarens y-koordinat.
     * @return Objektets y-koordinat
     */
    public int getY(){
        return (int)position.getY();
    }
    
    /**
     * Metod för att komma åt spelarens x,y koordinater i pointformat.
     * @return Objektets position i pointformat.
     */
    public Point getPosition(){
        return position;
    }
    
    /**
     * Metod för att komma åt vilken tile spelaren står på.
     * @param TileGrid En 2D-array innehållande alla tiles som finns i världen.
     */
    public Tile getCurrentTile(Tile[][] TileGrid){
        return TileGrid[getX()][getY()];
    }
    
    /**
     * Metod för att komma åt spelarens inventarielista.
     */
    public ArrayList getInventory(){
        return inventory;
    }
    
    /**
     * Metoden används för att spelaren ska kunna använda items.
     * Om spelaren inte har itemet i fråga kommer ett felmeddelande skrivas ut.
     * 
     * @param command Vilket item som ska användas.
     */
    public void useItem(Command command){
        String itemName = command.getSecondWord();
        Item item = null;
        
        
        for(Object i : inventory){
            Item tuffgrej = (Item)i;
            if(tuffgrej.getName().equals(itemName));
                item = tuffgrej;
        }
            
        if(!command.hasSecondWord() || !item.getName().equals(itemName) || item == null) {
            System.out.println("You can't use something you don't have..");
            return;
        }
        
        //gör krejjen
        else{
        item.onUse();
        }
    }
    
    /**
     * Skriver ut hela inventarielistan.
     * Om den är tom skrivs en tom rad ut.
     */
    public void showInventory(){
        System.out.println("This is you current inventory:");
        for(Object tempitem : inventory){
            Item item = (Item)tempitem;
            String itemName = item.getName();
            System.out.println(">"+itemName);
        }
    }

    /**
     * Metod för att lägga till items i inventarielistan.
     * 
     * @param whichItem Vilket item som ska läggas till.
     */
    public void addItemToInventory(Item whichItem){
        inventory.add(whichItem);
    }

}
