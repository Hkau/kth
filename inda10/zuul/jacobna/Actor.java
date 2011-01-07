import java.util.ArrayList;
import java.util.HashMap;
import java.awt.Point;
/**
 * Actor.
 * 
 * Spelarklassen. Den �r en del av "World of Zuul".
 * 
 * Klassen inneh�ller allt som tillh�r spelaren. Just nu inneh�ller
 * den: Namn, position samt inventarielista. N�r klassen skapas kommer
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
     * Skapar en spelare p� de angivna x och y kordinaterna. Spelaren f�r det
     * angivna namnet.
     */
    public Actor(String name, int startX,int startY)
    {
        position = new Point(startX,startY);
        this.name = name;
        inventory = new ArrayList<Item>();

    }
    

    
    /**
     * Flyttar p� spelaren till angiven x och y koordinat.
     * @param x En integer som anger x v�rdet.
     * @param y En integer som anger y v�rdet.
     */
    public void moveActor(int x, int y){
        position.setLocation(x,y);
    }
    
    /**
     * Flyttar p� spelaren till angiven Point.
     * @param newPos Den point som spelaren kommer flyttas till.
     */
    public void moveActorPoint(Point newPos){
        position.setLocation(newPos);
        //System.out.println(getX()+" "+getY());
    }
     /**
     * Metod f�r att komma �t name.
     * @return Objektets name-variabel.
     */
    public String getName(){
        return name;
    }
    
    /**
     * Metod f�r att komma �t spelarens x-koordinat.
     * @return Objektets x-koordinat.
     */
    public int getX(){
        return (int)position.getX();
    }
    
    /**
     * Metod f�r att komma �t spelarens y-koordinat.
     * @return Objektets y-koordinat
     */
    public int getY(){
        return (int)position.getY();
    }
    
    /**
     * Metod f�r att komma �t spelarens x,y koordinater i pointformat.
     * @return Objektets position i pointformat.
     */
    public Point getPosition(){
        return position;
    }
    
    /**
     * Metod f�r att komma �t vilken tile spelaren st�r p�.
     * @param TileGrid En 2D-array inneh�llande alla tiles som finns i v�rlden.
     */
    public Tile getCurrentTile(Tile[][] TileGrid){
        return TileGrid[getX()][getY()];
    }
    
    /**
     * Metod f�r att komma �t spelarens inventarielista.
     */
    public ArrayList getInventory(){
        return inventory;
    }
    
    /**
     * Metoden anv�nds f�r att spelaren ska kunna anv�nda items.
     * Om spelaren inte har itemet i fr�ga kommer ett felmeddelande skrivas ut.
     * 
     * @param command Vilket item som ska anv�ndas.
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
        
        //g�r krejjen
        else{
        item.onUse();
        }
    }
    
    /**
     * Skriver ut hela inventarielistan.
     * Om den �r tom skrivs en tom rad ut.
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
     * Metod f�r att l�gga till items i inventarielistan.
     * 
     * @param whichItem Vilket item som ska l�ggas till.
     */
    public void addItemToInventory(Item whichItem){
        inventory.add(whichItem);
    }

}
