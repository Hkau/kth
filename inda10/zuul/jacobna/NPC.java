import java.util.Random;
/**
 * NPC är en subklass till Actor och används för att simulera
 * datorstyrda spelare. Den innehåller några separata funktioner
 * som att skriva ut dialog och röra sig slumpmässigt som spelaren
 * inte har något behov att göra.
 * 
 * @author Jacob Norlin Andersson 
 * @version 2010.12.02
 */
public class NPC extends Actor
{

    private String dialog;
    private boolean met = false; // har den träffat någon?
    /**
     * Skapar en NPC på angiven plats med ett namn samt vad NPCn kan säga när den
     * träffar spelaren.
     * 
     * @param name Namnet på NPCn
     * @param startX Den x-koordinat som NPCn skapas på.
     * @param startY Den y-koordinat som NPCn skapas på.
     * @param dialog En sträng med vad NPCn säger till den den möter.
     */
    public NPC(String name, int startX, int startY, String dialog){
        super(name,startX,startY);
        this.dialog = dialog;
    }
    
    /**
     * Den simulerar att NPCn går i en slumpmässig riktning öst,väst,
     * norr eller syd. Om NPCn försöker gå utanför kartan kommer 
     * den inte att förflyttas.
     */
    public void moveNPC(int xMax,int yMax){
        Random rand = new Random();
        int x = rand.nextInt(3);
        int y = rand.nextInt(3);
        
        //jag hittade inget sätt att slumpa mellen -1 och 1 så det blev såhär.
        //Den slumpar mellan 0 och 2 och sätter värden utifrån dem.
        if(x==0){x = -1;}
        else if(x==1){x = 0;}
        else if(x==2){x = 1;}
        if(y==0){y = -1;}
        else if(y==1){y = 0;}
        else if(y==2){y = 1;}
        //System.out.println(x+" "+y);
        
        //sätt ny position
        int nextX = x+getX();
        int nextY = y+getY();
        //System.out.println(nextX+" "+nextY);
        if(nextX <= 0 || nextY <= 0 || nextX>=xMax || nextY>=yMax){
            return;
        }
        else{
            moveActor(nextX,nextY); //Flytta på NPCn.
        }
    }
    
    /**
     * Skriver ut dialogsträngen som anges i konstruktorn.
     */
    public void startDialog(){
        if(met == false){
            System.out.println(getName()+": "+dialog);
            met = true;
        }
    }
       
}


