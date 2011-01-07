import java.awt.Point;
/**
 * Beamer �r ett item som till�ter spelaren att f�rflytta
 * sig till positionen som spelaren valt att placera beamern
 * p�. Klassen fungerar f�r n�rvarande inte som den ska, och
 * jag f�rst�r inte varf�r. Positionen som den placerades p�
 * �ndras hela tiden n�r den inte ska det.
 * 
 * @author Jacob Norlin Andersson
 * @version 2010.12.02
 */
public class Beamer extends Item
{
    private int uses;
    private boolean isPlaced;
    private Point beamerPosition;

    /**
     * Skapar en beamer med angivet namn, beskrivning, �gare samt antal
     * g�nger �garen kan anv�nda beamern.
     * 
     * @param owner �garen till itemet.
     * @param name Itemets namn.
     * @param description Itemets beskrivning.
     * @param uses S� m�nga g�nger man f�r anv�nda itemet.
     */
    public Beamer(Actor owner, String name, String description, int uses){
        super(owner, name, description);
        this.uses = uses;
        isPlaced = false;
    }
    
    /**
     * Placerar beamern p� den angivna positionen.
     * 
     * @param position Positionen beamern placeras p�.
     */
    public void placeBeamer(Point position){
        beamerPosition = position;
        isPlaced = true;
        //System.out.println(isPlaced);
        
    }
    
    /**
     * Metoden som kallas n�r beamer anv�nds. Den kollar om beamern
     * �r placerad och om den �r det f�yttar den p� spelaren till
     * positionen d�r beamern �r placerad. Om den inte �r placerad
     * kommer den att placera beamern p� den position som anv�ndaren
     * st�r p�.
     */
    public void onUse(){
        
        if(isPlaced == false){
            System.out.println("You have placed your beamer! You can return to this position anytime you want");
            placeBeamer(owner.getPosition());
        }
        else{
            owner.moveActorPoint(beamerPosition);
            System.out.println("You have used your beamer!");
            isPlaced = false;
        }
    }

    

}
