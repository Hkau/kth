/**
 * Ett objekt med en variabel - låst eller ej. Objektet används som referens av bl.a. Room och Key.
 */
public class Door{
    private boolean locked;
    
    public Door(boolean locked){
        this.locked = locked;
    }
    
    public boolean getStatus(){
        return locked;
    }
    //Skriver ut när man låser/låser upp en dörr.
    public void setStatus(boolean newStatus){
        if(newStatus) System.out.println("The door locks with a snap.");
        else System.out.println("The door unlocks with a metallic sound.");
        locked = newStatus;
    }
}