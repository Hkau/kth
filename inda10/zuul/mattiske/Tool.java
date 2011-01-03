/**
 * Subklass till Item som används med andra föremål. Metoden use() override:as alltså.
 */
public class Tool extends Item{
    public Tool(String name, String description){
        super(name, description);
    }
    /**
     * Använder ett verktyg med ett annat föremål.
     * Returnerar true om föremålet går sönder vid användning.
     */
    public boolean use(Game game){
        System.out.println("What do you want to use it with?");
        System.out.print(">> ");
        String itemName;
        while(true){
            //hämtar användarens svar.
            itemName = Parser.getWholeLine();
            if(itemName != null){
                Item subject = game.getInventory().get(itemName);
                //Om föremålet finns i inventory så använder jag det
                if(subject != null) return useItemWith(subject, game);
                else{
                    subject = game.getCurrentRoom().getItem(itemName);
                    //Om föremålet finns i rummet så använder jag det.
                    if(subject != null) return useItemWith(subject, game);
                }         
            }
            //annars klagar jag.
                System.out.println("Couldn't find such item...");
                return false;
        }        
    }
    private boolean useItemWith(Item subject, Game game){
        //plastskeden är fett bra.
        if(getName().equals("plastic spoon")){
            System.out.println("The plastic spoon breaks on " + subject.getName());
            return true;
        }
        //Cigarren tänder andra föremål.
        if(getName().equals("cigar")){
            if(subject.getName().equals("candle") || subject.getName().equals("fireplace")){
            System.out.println("You use the cigar to light " + subject.getName());
            game.getCurrentRoom().setDark(false);
            System.out.println(game.getCurrentRoom().getDescription(game.getTittriktning()));
            return false;
            }
        }
        //Skruvmejseln bara låtsas vara bra :P
        if(getName().equals("screwdriver")){
            if(subject.getName().equals("toilet")){
                System.out.println("You open the lid of the toilet. It contains nothing...");
            }
            return false;
        }
        if(getName().equals("batteries")){
            if(subject.getName().equals("flashlight")){
                System.out.println("You change the flashlight batteries.");
                //hittar du batterier behöver du nog inte bry dig mer om mörka rum...
                //eftersom jag vet att subject är en flashlight så är det lugnt att konvertera.
                ExpiringItem temp = (ExpiringItem) subject;
                temp.charge(100);
            }
            return false;
        }
        System.out.println("You can't combine thoose items");
        return true;
    }
}