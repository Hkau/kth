
/**
 * Itemklassen används endast som pappaklass till andra klasser
 * och används aldrig själv. Den innehåller alla parametrar och
 * metoder som items delar på.
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
     * Skapar ett item med angiven ägare, namn och beskrivning,
     * 
     * @param owner Vem som äger itemet.
     * @param name Itemets namn.
     * @param description Itemets beskrivning.
     */
    public Item(Actor owner,String name, String description){
        this.name = name;
        this.owner = owner;
        this.description = description;
    } 
    
    /**
     * Metod för att komma åt itemets namn.
     */
    public String getName(){
        return name;
    }
    
    /**
     * Metod för att komma åt beskrivningen.
     */
    public String getDescription(){
        return description;
    }
    
    public void onUse(){} // så att jag kan kalla onUse på alla Items.
    public void onPlace(){}
    public boolean isPlaced(){return false;}
}
