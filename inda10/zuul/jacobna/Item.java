
/**
 * Itemklassen anv�nds endast som pappaklass till andra klasser
 * och anv�nds aldrig sj�lv. Den inneh�ller alla parametrar och
 * metoder som items delar p�.
 * 
 * @author Jacob Norlin Andersson 
 * @version 2010.12.02
 */
public class Item
{
    protected Actor owner;
    private String name;
    private String description;
    
    /**
     * Skapar ett item med angiven �gare, namn och beskrivning,
     * 
     * @param owner Vem som �ger itemet.
     * @param name Itemets namn.
     * @param description Itemets beskrivning.
     */
    public Item(Actor owner,String name, String description){
        this.name = name;
        this.owner = owner;
        this.description = description;
    } 
    
    /**
     * Metod f�r att komma �t itemets namn.
     */
    public String getName(){
        return name;
    }
    
    /**
     * Metod f�r att komma �t beskrivningen.
     */
    public String getDescription(){
        return description;
    }
    
    public void onUse(){} // s� att jag kan kalla onUse p� alla Items.
    public void onPlace(){}
    public boolean isPlaced(){return false;}
}
