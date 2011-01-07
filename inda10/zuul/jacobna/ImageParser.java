import java.awt.image.*;
import java.io.File;
import javax.imageio.*;
import java.io.IOException;


/**
 * ImageParser tar en bild och l�ser av varje pixel. Klassen
 * returnerar en 2D array som inneh�ller varje pixels f�rg.
 * 
 * @author Jacob Norlin Andersson
 * @version 2010.12.02
 */
public class ImageParser
{
    private BufferedImage Image;
    private int xMax;
    private int yMax;
    private int area;
    
    /**
     * Skapar en ImageParser som l�ser in den angivna bilden
     * och anger vilka dimensioner den har samt dess area.
     * 
     * @param fileSource Bildens k�llplats(?).
     */
    public ImageParser(String fileSource){
        try{ //Vi l�ser in bilden.
            File source = new File(fileSource);
            Image = ImageIO.read(source);
        }
        catch (IOException e) {
        }
        
        //best�m bildens dimensioner.
        yMax = Image.getHeight();
        xMax = Image.getWidth();
        area = xMax*yMax;
        
    }
    
    /**
     * L�ser en sekvens av pixlar och returnerar en
     * 2D array inneh�llande varje pixels f�rg.
     * 
     * @return En 2D-array som inneh�ller varje pixels f�rg p� r�tt x,y position.
     */
    public int[][] readSequence(){
        int[][] imageArray = new int[xMax][yMax];
        int x=0;
        int y=0;
        
        for(int i = 0; i<area; i++){
            int color = Image.getRGB(x,y); //Get f�rgen p� pixeln.
            //System.out.println(color);
            imageArray[x][y] = color;
            x++;
            if(x>=xMax){//om vi kommer till slutet av bilden byter vi rad.
                y+=1;
                x=0;
            }
        }
        return imageArray;
    }
    
    /**
     * Metod f�r att skriva ut ett rutn�t av siffror
     * som anger varje pixels f�rg.
     */
    public void print2Darray(int[][] A1){
        int x = 0;
        int y = 0;
        int t = 0;
        for(int i=0; i<area;i++){
            if(x>=xMax){
                y+=1;
                x=0;
                System.out.println();//Om det blir ny rad g�r vi en ny rad..
            }
            System.out.print(A1[x][y]);
            x++;           
        }
    }
    
}
