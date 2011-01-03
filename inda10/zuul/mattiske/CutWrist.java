/**
 * Om du skadar dig har du ett begränsat antal rundor att åtgärda såret. Annars tar spelet slut.
 */
public class CutWrist{
    int bleed = 0;
    public boolean turn(){
        //Om den var ett, så tar spelet slut.
        if(--bleed == 0){
            System.out.println("Your massive bloodloss makes you unconcious...");
            return true;
        }
        //Om den var 0, så kommer den bli negativ i ovanstådende if-sats: alltså var du inte skadad.
        if(bleed < 0) bleed = 0;
        //Det tredje tänkbara fallet är att du är skadad, men ännu inte död. Då skriver den ut att du är skadad.
        else{
            if(bleed > 10) System.out.println("You are bleeding heavily, you need to stop it before you bleed to death.");
            else if(bleed > 7) System.out.println("Your bleeding starts to make you dizzy...");
            else if(bleed > 3) System.out.println("You are having trouble to stay awake. You still bleed alot.");
            else System.out.println("You are barely awake. If you dont stop the bleeding now you will pass out.");
        }
        return false;
    }
    /**
     * Starts the countdown.
     */
    public void get(){
        if(bleed == 0) bleed = 15;
    }
    /**
     * Stops the countdown.
     */
    public void heal(){
        bleed = 0;
    }
}