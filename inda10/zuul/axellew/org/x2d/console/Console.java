package org.x2d.console;
import javax.swing.*;
import java.util.*;
import java.awt.event.*;
import java.io.*;
/**
 * An textarea in a JScrollPane that displays all the text which is sent
 * to System.out
 */
public class Console extends JScrollPane implements Runnable {
    private final int maxLines;
    private PipedOutputStream pout;
    private final PipedInputStream pin = new PipedInputStream();
    private BufferedReader in;
    private Thread t;
    short[] newLinePos;
    private int newLineCounterPos = 0;
    private boolean linesFull = false;
    private JTextArea textArea = new JTextArea();
    boolean scrollNext = false;
    boolean firstLine = true;
    
    /**
     * Creates a new console with a maximum number of lines. When
     * created it will replace Syste.out so all it's output is
     * redirected to this console. If a second console is created the old
     * console will stop working.
     *
     * @param maxLines the maximum number of lines this console can show.
     */
    public Console(int maxLines) {
        super(VERTICAL_SCROLLBAR_ALWAYS, HORIZONTAL_SCROLLBAR_NEVER);
        if (maxLines<1) {
            throw new IllegalArgumentException("The number of lines must be greater than 0.");
        }
        getViewport().setView(textArea);
        this.maxLines = maxLines;
        newLinePos = new short[maxLines];
        textArea.setEditable(false);
        textArea.setLineWrap(true);
        textArea.setWrapStyleWord(true);
        final JScrollBar scroll = getVerticalScrollBar();
        scroll.addAdjustmentListener(new AdjustmentListener(){
        	public void adjustmentValueChanged(AdjustmentEvent e){
            //Scrolls to the bottom of the text area if needed.
		        if (scrollNext) {
		            scroll.setValue(scroll.getMaximum()-scroll.getVisibleAmount());
		            scrollNext=false;
		        }
        	}});
        /*
         * Redirects system.out
         */
        try {
            pout = new PipedOutputStream(pin);
            System.setOut(new PrintStream(pout,true));
            t = new Thread(this);
            t.setDaemon(true);
            t.start();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    
    /**
     * Used internally to read the System.out buffer for new text.
     */
    public void run() {
        try {
            while (true) {
                try {
                    t.sleep(100);
                }catch(InterruptedException ie) {}
                if (pin.available()!=0) {
                    addLine(readLine(pin));
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    
    /**
     * Adds a line to this console. Synchronized makes sure that
     * only one thread can use this object at any given time.
     *
     * @param line Appends line to this console.
     */
    public synchronized void addLine(String line) {
        /*
         * Makes sure that the textarea shows at maximum <i>maxLine</i> lines
         * of text and that the scroll stays at the bottom if
         * it was at the bottom before the new line was added.
         */
        
        JScrollBar scroll = getVerticalScrollBar();
        int max = scroll.getMaximum();
        int value = scroll.getValue();
        int visible = scroll.getVisibleAmount();
        if (max == value+visible) {
            scrollNext = true;
        }
        if (newLineCounterPos==maxLines-1) {
            linesFull = true;
        }
        newLineCounterPos = (newLineCounterPos+1)%maxLines;
        if (linesFull) {
            textArea.replaceRange(null, 0, (int)newLinePos[newLineCounterPos]);
        }
        int newPos=line.length();
        if (firstLine) {
            firstLine=false;
            newPos += 1;
        }
        newLinePos[newLineCounterPos] = (short)newPos;
        textArea.append(line);

    }
    
    /*
     * Reads one line from the pipe and returns it. Synchronized makes sure that
     * only one thread can use this object at any given time.
     */
    private synchronized String readLine(PipedInputStream in) throws IOException {
        StringBuilder input;
        if (firstLine) {
            input = new StringBuilder();
        } else {
            input = new StringBuilder("\n");
        }
        int end=0;
        //Reads bytes from the stream till it finds a '\n'
        do {
            int available=in.available();
            if (available==0) break;
            byte b[]=new byte[available];
            in.read(b);
            input.append(new String(b,0,b.length));
            end = input.length()-1;
        } while( input.charAt(end)!='\n' );
        if (input.charAt(end-1)=='\r') {
            end--;
        }
        return input.substring(0, end);
    } 
}
