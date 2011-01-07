import java.awt.Point;
/**
 * Beamer är ett item som tillåter spelaren att förflytta
 * sig till positionen som spelaren valt att placera beamern
 * på. Klassen fungerar för närvarande inte som den ska, och
 * jag förstår inte varför. Positionen som den placerades på
 * ändras hela tiden när den inte ska det.
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
     * Skapar en beamer med angivet namn, beskrivning, ägare samt antal
     * gånger ägaren kan använda beamern.
     * 
     * @param owner Ägaren till itemet.
     * @param name Itemets namn.
     * @param description Itemets beskrivning.
     * @param uses Så många gånger man får använda itemet.
     */
    public Beamer(Actor owner, String name, String description, int uses){
        super(owner, name, description);
        this.uses = uses;
        isPlaced = false;
    }
    
    /**
     * Placerar beamern på den angivna positionen.
     * 
     * @param position Positionen beamern placeras på.
     */
    public void placeBeamer(Point position){
        beamerPosition = position;
        isPlaced = true;
        //System.out.println(isPlaced);
        
    }
    
    /**
     * Metoden som kallas när beamer används. Den kollar om beamern
     * är placerad och om den är det föyttar den på spelaren till
     * positionen där beamern är placerad. Om den inte är placerad
     * kommer den att placera beamern på den position som användaren
     * står på.
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
